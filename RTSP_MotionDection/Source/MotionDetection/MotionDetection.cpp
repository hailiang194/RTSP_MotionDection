#include <vector>
#include "MotionDetection.h"

MotionDetection::MotionDetection()
	:m_currentImage(), m_preprocessImage(),m_motionDetected(false), m_bgSub(cv::createBackgroundSubtractorMOG2())
{
}

MotionDetection::MotionDetection(const MotionDetection& detection)
	:m_currentImage(detection.m_currentImage), m_preprocessImage(detection.m_preprocessImage), m_motionDetected(detection.m_motionDetected), m_bgSub(detection.m_bgSub)
{
}

MotionDetection& MotionDetection::operator=(const MotionDetection& detection)
{
	m_currentImage = detection.m_currentImage;
	m_preprocessImage = detection.m_preprocessImage;
	m_motionDetected = detection.m_motionDetected;
	m_bgSub = detection.m_bgSub;
	return (*this);
}

MotionDetection::~MotionDetection()
{
}

void MotionDetection::UpdateFrame(const cv::Mat& currentImage)
{
	m_currentImage = currentImage;
	PreprocessImage();
	m_motionDetected = HasDifferent();
}

const bool& MotionDetection::MotionDetected() const
{
	return m_motionDetected;
}

const bool& MotionDetection::MotionDetected()
{
	return static_cast<const MotionDetection&>(*this).MotionDetected();
}

void MotionDetection::PreprocessImage()
{
	const int width = 500;
	const int height = width * m_currentImage.size().height / m_currentImage.size().width;
	cv::Mat resizedImage;
	cv::resize(m_currentImage, resizedImage, cv::Size(width, height));
	cv::Mat gray;
	cv::cvtColor(resizedImage, gray, cv::COLOR_BGR2GRAY);
	cv::Mat blur;
	cv::GaussianBlur(gray, blur, cv::Size(21, 21), 0);
	m_preprocessImage = blur;
}

bool MotionDetection::HasDifferent()
{
	cv::Mat diff;
	m_bgSub->apply(m_preprocessImage, diff);
	cv::imshow("diff", diff);
	cv::Mat threadhold;
	cv::threshold(diff, threadhold, 25, 255, cv::THRESH_BINARY);
	cv::Mat dilation;
	cv::dilate(threadhold, dilation, cv::Mat(), cv::Point(-1, -1), 2);
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(dilation, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	return !contours.empty();
}
