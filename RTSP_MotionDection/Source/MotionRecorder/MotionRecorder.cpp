#include "MotionRecorder.h"

MotionRecorder::MotionRecorder()
	:m_isRecording(false), m_writer()
{
}

MotionRecorder::~MotionRecorder()
{
}

bool MotionRecorder::Start()
{
	return false;
}

void MotionRecorder::Stop()
{
}

const bool& MotionRecorder::IsRecording() const
{
	return m_isRecording;
}

const bool& MotionRecorder::IsRecording()
{
	return static_cast<const MotionRecorder&>(*this).IsRecording();
}

bool MotionRecorder::Write(const cv::Mat& frame)
{
	return false;
}
