#pragma once
#include "Helpers.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core.hpp"
#include <chrono>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class OCVProc
{
private:
	HDC drawingDC = 0;
	wxPanel* panel;
	bool stream = false;
	bool mirror = false;
	cv::RotateFlags rotation = (cv::RotateFlags)4;
	cv::Mat framePreProc, frameCanny, framePostProc;
	cv::Size cameraRes;
	cv::VideoCapture camera;
	std::thread cameraStream;
	void GetSizeFromCamera();
	void previewLoop();
	void fillHDCBackground(cv::Scalar colour);
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
	void drawMatToDC(cv::Mat& mat);
public:
	bool Init(int selectedCameraIndex, wxPanel* panel);
	bool Init(std::string cameraIP, wxPanel* panel);
	void rotateStream();
	void mirrorStream();
	void StartCameraStream();
	void StopCameraStream();
	void ProcessImage();
	OCVProc();
	~OCVProc();
};

