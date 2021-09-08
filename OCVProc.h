#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "Helpers.h"
#include "opencv2/videoio.hpp"
#include <chrono>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

const int fpsIntervalMS = 30;

class OCVProc
{
private:
	wxPanel* streamPanel;
	cv::Mat framePreProc, frameCanny, framePostProc;
	cv::Size cameraRes;
	wxBitmap displayImage;
	cv::VideoCapture camera;
	std::thread cameraStream;
public:
	OCVProc(int selectedCameraIndex, wxPanel* surface);
	void StartCameraStream();
	void StopCameraStream();
	void previewLoop();
};

