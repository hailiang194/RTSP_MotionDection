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
	const bool& MotionDetected() const;
	const bool& MotionDetected();

private:
	cv::Mat m_currentImage;
	cv::Mat m_preprocessImage;
	cv::Ptr<cv::BackgroundSubtractor> m_bgSub;
	bool m_motionDetected;

	void PreprocessImage();
	bool HasDifferent();
};

#endif //!_RTSP_MOTION_DETECTION_MOTION_DETECTION_H_

