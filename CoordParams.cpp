#include "CoordParams.h"

std::vector<unsigned char> CoordParams::ParamDataFromCoordinates(std::vector<cv::Point2f>& points)
{
	std::vector<unsigned char> coordParams;
	coordParams.push_back((unsigned char)points.size());	
	for (auto i : points)
	{
		coordParams.push_back((unsigned char)i.x);
		coordParams.push_back((unsigned char)i.y);
	}
	return coordParams;
}
