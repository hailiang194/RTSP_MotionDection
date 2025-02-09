#include "MotionDetection.h"

MotionDetection::MotionDetection()
	:m_currentImage(), m_preprocessImage(), m_previousImage(), m_motionDetected(false)
{
}

MotionDetection::MotionDetection(const MotionDetection& detection)
	:m_currentImage(detection.m_currentImage), m_preprocessImage(detection.m_preprocessImage), m_previousImage(detection.m_previousImage), m_motionDetected(detection.m_motionDetected)
{
}

MotionDetection& MotionDetection::operator=(const MotionDetection& detection)
{
	m_currentImage = detection.m_currentImage;
	m_preprocessImage = detection.m_preprocessImage;
	m_previousImage = detection.m_previousImage;
	m_motionDetected = detection.m_motionDetected;
	return (*this);
}

MotionDetection::~MotionDetection()
{
}

void MotionDetection::UpdateFrame(const cv::Mat& currentImage)
{

}

const bool& MotionDetection::MotionDetected()
{
	return m_motionDetected;
}

void MotionDetection::PreprocessImage()
{
}
