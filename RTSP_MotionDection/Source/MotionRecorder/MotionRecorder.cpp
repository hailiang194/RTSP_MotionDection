#include "MotionRecorder.h"

MotionRecorder::MotionRecorder()
	:m_isRecording(false), m_writer()
{
}

MotionRecorder::~MotionRecorder()
{
	Stop();
}

bool MotionRecorder::Start(const cv::String& filename, const cv::Size& frameSize)
{
	if (m_isRecording)
		return false;

	m_writer.open(filename, 0, 30, frameSize);
	m_isRecording = m_writer.isOpened();
	return m_isRecording;
}

void MotionRecorder::Stop()
{
	if (!m_isRecording)
		return;

	if(m_writer.isOpened())
		m_writer.release();
	m_isRecording = false;
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
	if (!m_isRecording)
		return false;

	if (!m_writer.isOpened())
	{
		m_isRecording = false;
		return false;
	}

	m_writer << frame;
	return true;
}
