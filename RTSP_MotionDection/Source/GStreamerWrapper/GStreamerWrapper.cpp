#include "GStreamerWrapper.h"
#include <iostream>
#include <thread>
#include <strstream>

#include <gst/app/gstappsink.h>

#include <opencv2/opencv.hpp>

#define GSTREAMER_WRAPPER_ASSERT(b, post_notify) \
if(!(b)) \
{ \
	std::cerr << "GSTREAMER_WRAPPER_ASSERT FAILED: " << #b << std::endl; \
	post_notify; \
}



GStreamerWrapper* GStreamerWrapper::g_instance = nullptr;
std::mutex GStreamerWrapper::m_muInstance;

GStreamerWrapper::GStreamerWrapper(int argc, char** argv)
	:m_pipeline(nullptr), m_sinkVideo(nullptr)
{
	GStreamerWrapper::g_instance = this;
	gst_init(&argc, &argv);
}

GStreamerWrapper::~GStreamerWrapper()
{
    cv::destroyAllWindows();
	gst_element_set_state(m_pipeline, GST_STATE_NULL);
	gst_object_unref(m_pipeline);
}

GStreamerWrapper* GStreamerWrapper::Instance()
{
	std::lock_guard<std::mutex> lock(m_muInstance);
	return g_instance;
}

void GStreamerWrapper::Setup(const std::string& pipeline)
{
	std::string gstPipeline = pipeline + " ! appsink name=mysink max-buffers=2 sync=1 caps=video/x-raw,format=BGR";
	GError* err = nullptr;
	m_pipeline = gst_parse_launch(gstPipeline.c_str(), &err);
	GSTREAMER_WRAPPER_ASSERT(m_pipeline, return);
	m_sinkVideo = gst_bin_get_by_name(GST_BIN(m_pipeline), "mysink");
	GSTREAMER_WRAPPER_ASSERT(m_sinkVideo, return);
}

void GStreamerWrapper::Run()
{
	GSTREAMER_WRAPPER_ASSERT((m_sinkVideo) && (m_pipeline), return);
	// Play the pipeline
	GSTREAMER_WRAPPER_ASSERT(gst_element_set_state(m_pipeline, GST_STATE_PLAYING), return);

	std::thread threadMessage(&GStreamerWrapper::HandleMessageLoop, this, "GOBLIN");
    std::thread threadAppsink(&GStreamerWrapper::AppsinkHandler, this);

	threadMessage.join();
    threadAppsink.join();
}

void GStreamerWrapper::HandleMessageLoop(const std::string& prefix)
{
	GstBus* bus = gst_element_get_bus(m_pipeline);
	bool res;
	while (true)
	{
		GstMessage* msg = gst_bus_timed_pop(bus, GST_CLOCK_TIME_NONE);
		GSTREAMER_WRAPPER_ASSERT(msg, break);
		gst_message_unref(msg);
        res = MessageHandler(m_pipeline, msg, prefix);
		if (!res)
			break;
	}

	gst_object_unref(bus);
}

bool GStreamerWrapper::MessageHandler(GstElement* pipeline, GstMessage* msg, const std::string& prefix) 
{
    GstMessageType mType = GST_MESSAGE_TYPE(msg);
    std::cout << "[" << prefix << "] : mType = " << mType << " ";
    switch (mType) 
    {
    case (GST_MESSAGE_ERROR):
        // Parse error and exit program, hard exit
        GError* err;
        gchar* dbg;
        gst_message_parse_error(msg, &err, &dbg);
        std::cout << "ERR = " << err->message << " FROM " << GST_OBJECT_NAME(msg->src) << std::endl;
        std::cout << "DBG = " << dbg << std::endl;
        g_clear_error(&err);
        g_free(dbg);
        exit(1);
    case (GST_MESSAGE_EOS):
        // Soft exit on EOS
        std::cout << " EOS !" << std::endl;
        return false;
    case (GST_MESSAGE_STATE_CHANGED):
        // Parse state change, print extra info for pipeline only
        std::cout << "State changed !" << std::endl;
        if (GST_MESSAGE_SRC(msg) == GST_OBJECT(pipeline)) 
        {
            GstState sOld, sNew, sPenging;
            gst_message_parse_state_changed(msg, &sOld, &sNew, &sPenging);
            std::cout << "Pipeline changed from " << gst_element_state_get_name(sOld) << " to " <<
                gst_element_state_get_name(sNew) << std::endl;
        }
        break;
    case (GST_MESSAGE_STEP_START):
        std::cout << "STEP START !" << std::endl;
        break;
    case (GST_MESSAGE_STREAM_STATUS):
        std::cout << "STREAM STATUS !" << std::endl;
        break;
    case (GST_MESSAGE_ELEMENT):
        std::cout << "MESSAGE ELEMENT !" << std::endl;
        break;

        // You can add more stuff here if you want

    default:
        std::cout << std::endl;
    }
    return true;
}

void GStreamerWrapper::AppsinkHandler()
{
    MotionDetection detection;
    MotionRecorder recorder;
    int i = 0;
    while (true)
    {
        if (gst_app_sink_is_eos(GST_APP_SINK(m_sinkVideo))) 
        {
            std::cout << "EOS !" << std::endl;
            break;
        }

        GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(m_sinkVideo));
        if (sample == nullptr) 
        {
            std::cout << "NO sample !" << std::endl;
            break;
        }

        // Get width and height from sample caps (NOT element caps)
        GstCaps* caps = gst_sample_get_caps(sample);
        GSTREAMER_WRAPPER_ASSERT(caps != nullptr, break);
        GstStructure* s = gst_caps_get_structure(caps, 0);
        int imW, imH;
        GSTREAMER_WRAPPER_ASSERT(gst_structure_get_int(s, "width", &imW), break);
        GSTREAMER_WRAPPER_ASSERT(gst_structure_get_int(s, "height", &imH), break);
        std::cout << "Sample: W = " << imW << ", H = " << imH << std::endl;
        GstBuffer* buffer = gst_sample_get_buffer(sample);
        GstMapInfo m;
        GSTREAMER_WRAPPER_ASSERT(gst_buffer_map(buffer, &m, GST_MAP_READ), break);
        GSTREAMER_WRAPPER_ASSERT(m.size == imW * imH * 3, break);

        //notify here
        cv::Mat frame(imH, imW, CV_8UC3, (void*)m.data);
        cv::imshow("frame", frame);

        if (!frame.empty())
        {

            detection.UpdateFrame(frame);
            if (detection.MotionDetected())
            {
                std::cout << "Motion detected" << std::endl;
                if (!recorder.IsRecording())
                {
                    std::strstream filenameStream;
                    filenameStream << "Output/MotionDetection" << (i++) << ".mp4";
                    if (!recorder.Start(filenameStream.str(), frame.size()))
                        std::cout << "Failed to start recording motion" << std::endl;
                }

                if (!recorder.Write(frame))
                {
                    std::cout << "Failed to write the frame" << std::endl;
                }
            }
            else
            {
                recorder.Stop();
            }
        }

        int key = cv::waitKey(1);

        gst_buffer_unmap(buffer, &m);
        gst_sample_unref(sample);

        if (27 == key)
            break;
    }
}
