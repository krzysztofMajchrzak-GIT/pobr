#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>
#include <utility>

//typedef std::pair<int, int> singlePixel;

cv::Mat unsharp(cv::Mat& photo);
cv::Mat bgr2hsv(cv::Mat& photo);
cv::Mat bitwiseOr(cv::Mat& image, cv::Mat& image2);
cv::Mat equlize(cv::Mat& photo);
cv::Mat masking(cv::Mat& photo, std::pair<int,int>& c1, std::pair<int,int>& c2, std::pair<int,int>& c3);
cv::Mat erosion(cv::Mat& photo);
cv::Mat dilation(cv::Mat& photo);

//std::vector<std::vector<pair<int,int> > > field(cv::Mat& photo);
