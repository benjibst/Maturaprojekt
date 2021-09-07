#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class OCVProc
{
private:
	cv::VideoCapture camera;
	cv::Mat framePreProc;
	cv::Mat frameCanny;
public:
	OCVProc(int selectedCameraIndex);
	StartCameraStream(wxImagePanel);
	wxImage
		
};

