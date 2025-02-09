#ifndef _RTSP_MOTION_DETECTION_MOTION_DETECTION_H_
#define _RTSP_MOTION_DETECTION_MOTION_DETECTION_H_

#include <opencv2/opencv.hpp>

class MotionDetection
{
public:
	MotionDetection();
	MotionDetection(const MotionDetection& detection);
	MotionDetection& operator=(const MotionDetection& detection);
	~MotionDetection();

	void UpdateFrame(const cv::Mat& currentImage);
	const bool& MotionDetected();

private:
	cv::Mat m_currentImage;
	cv::Mat m_previousImage;
	cv::Mat m_preprocessImage;
	bool m_motionDetected;

	void PreprocessImage();
};

#endif //!_RTSP_MOTION_DETECTION_MOTION_DETECTION_H_

