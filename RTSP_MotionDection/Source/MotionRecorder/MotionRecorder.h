#ifndef _RTSP_MOTION_DETECTION_MOTION_RECORDER_H_
#define _RTSP_MOTION_DETECTION_MOTION_RECORDER_H_

#include <opencv2/opencv.hpp>

class MotionRecorder
{
public:
	MotionRecorder();
	MotionRecorder(const MotionRecorder& recorder) = delete;
	MotionRecorder& operator=(const MotionRecorder& recorder) = delete;
	~MotionRecorder();

	bool Start(const cv::String& filename, const cv::Size& frameSize);
	void Stop();
	const bool& IsRecording() const;
	const bool& IsRecording();
	bool Write(const cv::Mat& frame);
private:
	bool m_isRecording;
	cv::VideoWriter m_writer;
};

#endif //!_RTSP_MOTION_DETECTION_MOTION_RECORDER_H_

