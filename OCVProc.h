#pragma once
#include "Helpers.h"
#include "opencv2/imgproc/imgproc.hpp"
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
	void previewLoop();
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
	void drawMatToDC(cv::Mat& mat);
public:
	bool Init(int selectedCameraIndex, HDC drawingDC);
	bool Init(std::string cameraIP, HDC drawingDC);
	void StartCameraStream();
	void StopCameraStream();
	void ProcessImage();
	OCVProc();
	~OCVProc();
};

