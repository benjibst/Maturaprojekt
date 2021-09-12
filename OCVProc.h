#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "Helpers.h"
#include "opencv2/videoio.hpp"
#include <chrono>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class OCVProc
{
private:
	HDC drawingDC = 0;
	bool stream = false;
	cv::Mat framePreProc, frameCanny, framePostProc;
	cv::Size cameraRes;
	cv::VideoCapture camera;
	std::thread cameraStream;
	void GetSizeFromCamera();
public:
	bool Init(int selectedCameraIndex, HDC drawingDC);
	bool Init(std::string cameraIP, HDC drawingDC);
	void StartCameraStream();
	void StopCameraStream();
	void previewLoop();
	OCVProc();
	~OCVProc();
};

