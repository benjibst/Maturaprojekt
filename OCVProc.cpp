#include "OCVProc.h"

void OCVProc::InitCameraRes()
{
	cameraRes.width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
	cameraRes.height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);
}

void OCVProc::SetStreamCanvas(wxPanel* canvas)
{
	this->drawingDC = GetDC(canvas->GetHWND());
	this->streamCanvas = canvas;
}

bool OCVProc::OpenCamera(int selectedCameraIndex)
{
	if (camera.open(selectedCameraIndex, cv::CAP_MSMF))
	{
		InitCameraRes();
		return true;
	}
	else
		return false;
}

bool OCVProc::OpenCamera(std::string cameraIP)
{
	if (camera.open(cv::String(cameraIP)))
	{
		InitCameraRes();
		return true;
	}
	else
		return false;
}

bool OCVProc::IsStreaming()
{
	return stream;
}


wxSize OCVProc::GetCameraResolution()
{
	return wxSize(cameraRes.width,cameraRes.height);
}

void OCVProc::StartCameraStream()
{
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
	cv::Mat rotated;
	stream = true;
	while (stream)
	{
		camera >> framePreProc;
		
		if (rotation != 4)
			cv::rotate(framePreProc, rotated, rotation);
		else
			rotated = framePreProc;
		if (mirror)
			cv::flip(rotated, afterTransform, 1);
		else
			afterTransform = rotated;
		drawMatToDC(afterTransform);
	}
}

void OCVProc::ProcessImage()
{
	cv::Mat gray, gauss, canny;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<std::vector<cv::Point>> contoursExternal;
	cv::cvtColor(afterTransform, gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(gray, gauss, cv::Size(3, 3), 0);
	// Use Canny instead of threshold to catch squares with gradient shading
	cv::Canny(gauss, canny, 100, 200);
	cv::findContours(canny, contours, cv::RETR_LIST, cv::CHAIN_APPROX_TC89_KCOS);
	

	std::vector<cv::Point> approx;
	framePostProc = afterTransform.clone();

	cv::findContours(canny, contoursExternal, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);
	std::vector<cv::Point> approxExternal;
	cv::approxPolyDP(contoursExternal[0], approxExternal, cv::arcLength(cv::Mat(contoursExternal[0]), true) * 0.12, true);
	if (approxExternal.size() == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			setLabelAtPoint(framePostProc, std::to_string(i), approxExternal[i]);
		}
	}


	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.12, true);

		// Skip small or non-convex objects
		if (cv::contourArea(contours[i]) < 200 || !cv::isContourConvex(approx))
			continue;
		else if (approx.size() == 4)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc + 1; j++)
				cos.push_back(angle(approx[j % vtc], approx[j - 2], approx[j - 1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 && mincos >= -0.3 && maxcos <= 0.3)
			{
				setLabel(framePostProc, "RECT", contours[i]);
				cv::polylines(framePostProc, approx, true, cv::Scalar(0, 0, 255), 2);
			}
		}
	}
	drawMatToDC(framePostProc);
}

void OCVProc::mirrorStream() { mirror = !mirror; }
void OCVProc::rotateStream()
{
	streamCanvas->ClearBackground();
	rotation++;
	rotation %= 4;
	
}

double OCVProc::angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = (double)pt1.x - (double)pt0.x;
	double dy2 = (double)pt2.y - (double)pt0.y;
	double dy1 = (double)pt1.y - (double)pt0.y;
	double dx2 = (double)pt2.x - (double)pt0.x;
	return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

void OCVProc::setLabel(cv::Mat &im, const std::string label, std::vector<cv::Point> &contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), cv::FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}

void OCVProc::setLabelAtPoint(cv::Mat& im, const std::string label, cv::Point point)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.8;
	int thickness = 2;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);

	cv::putText(im, label, point, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}

void OCVProc::drawMatToDC(cv::Mat &mat)
{
	BITMAPINFO info;
	memset(&info, 0, sizeof(BITMAPINFO));
	info.bmiHeader.biBitCount = 24;
	info.bmiHeader.biWidth = mat.cols;
	info.bmiHeader.biHeight = mat.rows;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biSizeImage = mat.cols * mat.rows * 3;
	info.bmiHeader.biCompression = BI_RGB;
	StretchDIBits(drawingDC, 0, 0, mat.cols, mat.rows, 0, 0, mat.cols, mat.rows, mat.data, &info, DIB_RGB_COLORS, SRCCOPY);
}

OCVProc::OCVProc()
{
}

OCVProc::~OCVProc()
{
	if (camera.isOpened())
		camera.release();
}
