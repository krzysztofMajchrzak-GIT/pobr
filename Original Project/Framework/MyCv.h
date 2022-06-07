#include "opencv2/core/core.hpp"
#include <optional>
#include <vector>
#include "BoundingBox.h"
#include "SegmentInfo.h"
#include "Range.h"
namespace MyCV
{
	typedef std::vector<Pixel> Segment;

	cv::Mat drawBoundingBox(cv::Mat &image, BoundingBox BoundingBox);
	cv::Mat convertBGR2HSV(cv::Mat &image);
	cv::Mat getMaskFrom3D(cv::Mat &image, std::pair<int, int> &r1, std::pair<int, int> &r2, std::pair<int, int> &r3);
	std::vector<Segment> getSegemntsFromMask(cv::Mat &mask);
	cv::Mat bitwiseOr(cv::Mat &image, cv::Mat &image2);
	cv::Mat dilationFilter(cv::Mat &image, int N);
	cv::Mat erosionFilter(cv::Mat &image, int N);
	cv::Mat blurFilter(cv::Mat &image, int N);
	cv::Mat equalizeHistogramHSV(cv::Mat &image);
	cv::Mat unsharpMask(cv::Mat &image);
}
