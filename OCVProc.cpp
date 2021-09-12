#include "OCVProc.h"

void OCVProc::GetSizeFromCamera()
{
	cameraRes.width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
	cameraRes.height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);
}

bool OCVProc::Init(int selectedCameraIndex, HDC drawingDC)
{
	this->drawingDC = drawingDC;
	if (camera.open(selectedCameraIndex, cv::CAP_MSMF))
	{
		GetSizeFromCamera();
		StartCameraStream();
		return true;
	}
	else
		return false;
}

bool OCVProc::Init(std::string cameraIP, HDC drawingDC)
{
	this->drawingDC = drawingDC;
	if (camera.open(cv::String(cameraIP)))
	{
		GetSizeFromCamera();
		StartCameraStream();
		return true;
	}
	else
		return false;
}

void OCVProc::StartCameraStream()
{
	stream = true;
	cameraStream = std::thread(&OCVProc::previewLoop, this);
}

void OCVProc::StopCameraStream()
{
	if (stream && camera.isOpened())
	{
		stream = false;
		cameraStream.join();
	}
}

void OCVProc::previewLoop()
{
	if (!camera.isOpened())
		return;

	BITMAPINFO info;
	memset(&info, 0, sizeof(BITMAPINFO));
	info.bmiHeader.biBitCount = 24;
	info.bmiHeader.biWidth = cameraRes.width;
	info.bmiHeader.biHeight = cameraRes.height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = cameraRes.width * cameraRes.height * 3;
	info.bmiHeader.biCompression = BI_RGB;

	cv::Mat gray, canny24, gauss;
	while (stream)
	{
		camera >> framePreProc;
		// Convert to grayscale
		cv::cvtColor(framePreProc, gray, cv::COLOR_BGR2GRAY);
		cv::GaussianBlur(gray, gauss, cv::Size(3, 3), 0);
		// Use Canny instead of threshold to catch squares with gradient shading
		cv::Canny(gauss, frameCanny, 100, 200);

		cv::cvtColor(frameCanny, canny24, cv::COLOR_GRAY2BGR, 3);

		StretchDIBits(drawingDC, 0, 0, framePreProc.cols, framePreProc.rows, 0, 0, framePreProc.cols, framePreProc.rows, canny24.data, &info, DIB_RGB_COLORS, SRCCOPY);
	}
}

OCVProc::OCVProc()
{
}

OCVProc::~OCVProc()
{
	if (camera.isOpened())
		camera.release();
}
