#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <utility>

//typedef std::pair<int, int> singlePixel;

cv::Mat unsharp(cv::Mat& photo);
cv::Mat bgr2hsv(cv::Mat& photo);
cv::Mat equlize(cv::Mat& photo);
