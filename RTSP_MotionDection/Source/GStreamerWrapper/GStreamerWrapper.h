#ifndef _RTSP_MOTION_DETECTION_GSTREAMER_WRAPPER_H_
#define _RTSP_MOTION_DETECTION_GSTREAMER_WRAPPER_H_

#include <gst/gst.h>
#include <string>
#include <mutex>
#include <functional>

#include "MotionDetection/MotionDetection.h"
#include "MotionRecorder/MotionRecorder.h"

class GStreamerWrapper
{
public:
	GStreamerWrapper(int argc, char** argv);
	GStreamerWrapper(const GStreamerWrapper& wrapper) = delete;
	GStreamerWrapper& operator=(const GStreamerWrapper& wrapper) = delete;
	~GStreamerWrapper();

	static GStreamerWrapper* Instance();
	void Setup(const std::string& pipeline);
	void Run();

private:
	GstElement* m_pipeline;
	GstElement* m_sinkVideo;

	static std::mutex m_muInstance;
	static GStreamerWrapper* g_instance;

	void HandleMessageLoop(const std::string& prefix);
	bool MessageHandler(GstElement* pipeline, GstMessage* msg, const std::string& prefix);

	void AppsinkHandler();
};

#endif //!_RTSP_MOTION_DETECTION_GSTREAMER_WRAPPER_H_

