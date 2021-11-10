#pragma once
#include "Helpers.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core.hpp"
#include "opencv2/calib3d.hpp"
#include <chrono>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class OCVProc
{
private:
	HDC drawingDC = 0;
	wxPanel* streamCanvas = nullptr;
	bool stream = false;
	bool mirror = false;
	int rotation = 4;
	cv::Mat framePreProc,afterTransform, framePostProc;
	cv::Size cameraRes;
	cv::VideoCapture camera;
	std::thread cameraStream;
	void InitCameraRes();
	void previewLoop();
	double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	bool isQuad(std::vector<cv::Point> contours);
	void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);
	void setLabelAtPoint(cv::Mat& im, const std::string label, cv::Point point);
	void drawMatToDC(cv::Mat& mat);
public:
	void SetStreamCanvas(wxPanel* canvas);
	bool OpenCamera(int selectedCameraIndex);
	bool OpenCamera(std::string cameraIP);
	bool IsStreaming();
	wxSize GetCameraResolution();
	void rotateStream();
	void mirrorStream();
	void StartCameraStream();
	void StopCameraStream();
	void ProcessImage();
	OCVProc();
	~OCVProc();
};

