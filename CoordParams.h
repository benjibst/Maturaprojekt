#pragma once
#include "opencv2/core.hpp"
#include <string>
#include <vector>
class CoordParams
{
public:
	static std::vector<unsigned char> ParamDataFromCoordinates(std::vector<cv::Point2f>& points);
};

