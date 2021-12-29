#include "OCVProc.h"

void OCVProc::InitCameraRes()
{
	cameraRes.width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
	cameraRes.height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);
}

cv::Point2f OCVProc::quadCenter(std::vector<cv::Point> corners)
{
	assert(corners.size() == 4);
	int avgX = 0, avgY = 0;
	for (int i = 0; i < 4; i++)
	{
		avgX += corners[i].x;
		avgY += corners[i].y;
	}
	return cv::Point2f(avgX / 4.0F, avgY / 4.0F);
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
	return wxSize(cameraRes.width, cameraRes.height);
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
		if (rotation != 3)
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

std::vector<std::vector<cv::Point>> OCVProc::FindQuadsInMat(cv::Mat& mat)
{
	cv::Mat gray, gauss, canny;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Point> currentPoly;
	std::vector<std::vector<cv::Point>> allQuad;		//angenäherte polygone

	cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(gray, gauss, cv::Size(3, 3), 0);
	// Use Canny instead of threshold to catch squares with gradient shading
	cv::Canny(gauss, canny, 100, 200);
	cv::findContours(canny, contours, cv::RETR_LIST, cv::CHAIN_APPROX_TC89_KCOS);

	for (int i = 0; i < contours.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contours[i]), currentPoly, cv::arcLength(cv::Mat(contours[i]), true) * 0.12, true);
		if (isQuad(currentPoly))
			allQuad.push_back(currentPoly);
	}
	return allQuad;
}

std::vector<cv::Point2f> OCVProc::ProcessImage()
{
	std::vector<std::vector<cv::Point>> allQuad;
	std::vector<cv::Point> outerQuad;
	std::vector<cv::Point2f> outerQuadf;
	std::vector<cv::Point2f> QuadCenters;
	std::vector<cv::Point2f> transformedCentersMCU; //punkte werden auf 1 byte skaliert für UART Übertragung
	std::vector<cv::Point2f> transformedCentersIMG;
	cv::Mat transformationIMG;
	cv::Mat transformationMCU;
	cv::Mat transformedImg;

	framePostProc = afterTransform.clone();
	allQuad = FindQuadsInMat(afterTransform);

	if (allQuad.size() < 2)
		goto end;

	outerQuad = removeBiggestQuad(allQuad);
	removeDoubleQuads(allQuad);
	for (int i = 0; i < allQuad.size(); i++)
		QuadCenters.push_back(quadCenter(allQuad[i]));

	sortCorners(outerQuad);
	cv::Mat(outerQuad).convertTo(outerQuadf, CV_32F);
	transformationIMG = cv::getPerspectiveTransform(outerQuadf, transformPointsIMG);
	transformationMCU = cv::getPerspectiveTransform(outerQuadf, transformPointsMCU);

	cv::warpPerspective(framePostProc, transformedImg, transformationIMG, cv::Size(480, 480));
	
	cv::perspectiveTransform(QuadCenters, transformedCentersIMG, transformationIMG);
	cv::perspectiveTransform(QuadCenters, transformedCentersMCU, transformationMCU);

	for (int i = 0; i < QuadCenters.size(); i++)
		cv::circle(transformedImg, transformedCentersIMG[i], 5, cv::Scalar(255, 0, 0), 4);
end:
	streamCanvas->ClearBackground();
	drawMatToDC(transformedImg);
	return transformedCentersMCU;
}

void OCVProc::sortCorners(std::vector<cv::Point>& corners)
{
	if (corners.size() != 4)
		return;
	int cornerIndices[4]{ 0 };
	double distances[4]{ 0 };
	cv::Rect bounding = cv::boundingRect(corners);
	cv::Point cornerPoints[4] = {
		cv::Point(bounding.x,bounding.y),
		cv::Point(bounding.x + bounding.width,bounding.y),
		cv::Point(bounding.x + bounding.width,bounding.y + bounding.height),
		cv::Point(bounding.x,bounding.y + bounding.height)
	}; //corners of boundingrect
	for (int i = 0; i < _countof(cornerPoints); i++)
	{
		for (int j = 0; j < corners.size(); j++)
			distances[j] = cv::norm(corners[j] - cornerPoints[i]);
		cornerIndices[i] = std::distance(
			distances,
			std::min_element(distances,
				distances + _countof(distances)));
	}
	std::vector<cv::Point> cornerstemp(corners);
	corners.clear();
	for (int i = 0; i < 4; i++)
		corners.push_back(cornerstemp[cornerIndices[i]]);
}

std::vector<cv::Point> OCVProc::removeBiggestQuad(std::vector<std::vector<cv::Point>>& quads)
{
	if (quads.size() == 0)
		return std::vector<cv::Point>();
	std::vector<cv::Point> biggestQuad = quads[0];
	int index = 0;
	for (size_t i = 0; i < quads.size(); i++)
	{
		if (cv::contourArea(quads[i]) > cv::contourArea(biggestQuad))
		{
			biggestQuad = quads[i];
			index = i;
		}
	}
	quads.erase(quads.begin() + index);
	return biggestQuad;
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

bool OCVProc::isQuad(std::vector<cv::Point> contours)
{
	if (contours.size() != 4 || cv::contourArea(contours) < 100 || !cv::isContourConvex(contours))
		return false;
	// Number of vertices of polygonal curve
	int vtc = 4;
	// Get the cosines of all corners
	std::vector<double> cos;
	for (int j = 2; j < vtc + 1; j++)
		cos.push_back(angle(contours[j % vtc], contours[j - 2], contours[j - 1]));

	// Sort ascending the cosine values
	std::sort(cos.begin(), cos.end());

	// Get the lowest and the highest cosine
	double mincos = cos.front();
	double maxcos = cos.back();

	// Use the degrees obtained above and the number of vertices
	// to determine the shape of the contour
	if (mincos >= -0.4 && maxcos <= 0.4)
		return true;
	return false;
}



void OCVProc::removeDoubleQuads(std::vector<std::vector<cv::Point>>& quads)
{
	double currentQuadSize = 0;
	double compareQuadSize = 0;
	cv::Point currentQuadCenter;
	cv::Point compareQuadCenter;
	for (int i = 0; i < quads.size(); i++)
	{
		currentQuadSize = cv::contourArea(quads[i]);		//information of current quad to find similar quads
		cv::Point currentQuadCenter = quadCenter(quads[i]);
		for (int j = i + 1; j < quads.size(); j++)
		{
			compareQuadSize = cv::contourArea(quads[j]);	//compare with current quad
			compareQuadCenter = quadCenter(quads[j]);		//if similar,delete comparequad
			double maxSize = std::max(compareQuadSize, currentQuadSize);
			double minSize = std::min(compareQuadSize, currentQuadSize);
			double hypothenuse = cv::norm(currentQuadCenter - compareQuadCenter);
			if ((maxSize / minSize) < 1.4 && hypothenuse < 60)
			{
				quads.erase(quads.begin() + j);
				j--;
			}
		}
	}
}

void OCVProc::setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
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

void OCVProc::drawMatToDC(cv::Mat& mat)
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
	StretchDIBits(drawingDC, 0, mat.rows, mat.cols, -mat.rows, 0, 0, mat.cols, mat.rows, mat.data, &info, DIB_RGB_COLORS, SRCCOPY);
}

OCVProc::OCVProc()
{
}

OCVProc::~OCVProc()
{
	if (camera.isOpened())
		camera.release();
}
