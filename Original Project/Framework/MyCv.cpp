#include "MyCv.h"
#include <iostream>
#include <stack>

float crop(float val)
{
	if (val > 255.0)
		val = 255.0;
	if (val < 0.0)
		val = 0.0;
	return val;
}

cv::Mat MyCV::drawBoundingBox(cv::Mat &image, BoundingBox boundingBox)
{
	int borderThick = 2;
	int b = borderThick / 2;
	auto imgBB = image.clone();
	cv::Mat_<cv::Vec3b> _R = imgBB;

	for (int i = boundingBox.xmin; i < boundingBox.xmax; i++)
	{

		for (int j = -b; j <= b; j++)
		{
			int ymin = boundingBox.ymin + j;
			int ymax = boundingBox.ymax + j;

			if (ymin >= 0 && ymin < imgBB.rows)
			{
				_R(ymin, i)
				[0] = 0;
				_R(ymin, i)
				[1] = 255;
				_R(ymin, i)
				[2] = 0;
			}
			if (ymax >= 0 && ymax < imgBB.rows)
			{
				_R(ymax, i)
				[0] = 0;
				_R(ymax, i)
				[1] = 255;
				_R(ymax, i)
				[2] = 0;
			}
		}
	}

	for (int i = boundingBox.ymin; i < boundingBox.ymax; i++)
	{
		for (int j = -b; j <= b; j++)
		{
			int xmin = boundingBox.xmin + j;
			int xmax = boundingBox.xmax + j;

			if (xmin >= 0 && xmin < imgBB.cols)
			{
				_R(i, xmin)
				[0] = 0;
				_R(i, xmin)
				[1] = 255;
				_R(i, xmin)
				[2] = 0;
			}
			if (xmax >= 0 && xmax < imgBB.cols)
			{
				_R(i, xmax)
				[0] = 0;
				_R(i, xmax)
				[1] = 255;
				_R(i, xmax)
				[2] = 0;
			}
		}
	}
	return imgBB;
}

cv::Mat MyCV::convertBGR2HSV(cv::Mat &image)
{
	cv::Mat HSVimage(image.rows, image.cols, CV_8UC3);

	cv::Mat_<cv::Vec3b> _I = image;
	cv::Mat_<cv::Vec3b> _R = HSVimage;

	for (auto x = 0; x < image.cols; x++)
	{
		for (auto y = 0; y < image.rows; y++)
		{

			auto R = _I(y, x)[2] / 255.0;
			auto G = _I(y, x)[1] / 255.0;
			auto B = _I(y, x)[0] / 255.0;

			auto Value = std::max(std::max(R, G), B);
			auto Cmin = std::min(std::min(R, G), B);
			auto delta = Value - Cmin;

			auto Saturation = Value == 0 ? 0.0 : delta / Value;

			float Hue = 0.0;

			if (Value == R)
				Hue = 60.0 * ((G - B) / delta);
			else if (Value == G)
				Hue = 60.0 * (B - R) / delta + 120;
			else if (Value == B)
				Hue = 60.0 * (R - G) / delta + 240;
			else if (R == G && G == B)
				Hue = 0.0;

			if (Hue < 0)
				Hue += 360;

			_R(y, x)
			[0] = Hue / 2;
			_R(y, x)
			[1] = Saturation * 255;
			_R(y, x)
			[2] = Value * 255;
		}
	}
	return HSVimage;
}

cv::Mat MyCV::getMaskFrom3D(cv::Mat &image, Range &r1, Range &r2, Range &r3)
{
	cv::Mat_<uchar> _R(image.rows, image.cols);
	cv::Mat_<cv::Vec3b> _I = image;

	for (auto x = 0; x < image.cols; x++)
	{
		for (auto y = 0; y < image.rows; y++)
		{
			_R(y, x) = (r1.start <= _I(y, x)[0] && r1.end > _I(y, x)[0]) && (r2.start <= _I(y, x)[1] && r2.end > _I(y, x)[1]) && (r3.start <= _I(y, x)[2] && r3.end > _I(y, x)[2]) ? 255 : 0;
		}
	}
	return _R;
}

std::vector<MyCV::Segment> MyCV::getSegemntsFromMask(cv::Mat &mask)
{
	cv::Mat VisitedPixels(mask.rows, mask.cols, CV_8U);
	cv::Mat_<uchar> _VP = VisitedPixels;
	cv::Mat_<uchar> _I = mask;
	std::vector<MyCV::Segment> segmentsList;

	for (auto x = 0; x < mask.cols; x++)
	{
		for (auto y = 0; y < mask.rows; y++)
		{
			_VP(y, x) = 0;
		}
	}

	for (auto x = 0; x < mask.cols; x++)
	{
		for (auto y = 0; y < mask.rows; y++)
		{

			if (_VP(y, x) == 1 || _I(y, x) == 0)
				continue;

			MyCV::Segment segment;
			std::stack<MyCV::Pixel> stack;
			stack.push(MyCV::Pixel(y, x));
			while (!stack.empty())
			{

				auto pixel = stack.top();
				stack.pop();

				if (_I(pixel.first, pixel.second) == 255 && _VP(pixel.first, pixel.second) != 1)
				{
					segment.push_back(pixel);
					_VP(pixel.first, pixel.second) = 1;

					if (pixel.first - 1 >= 0)
						stack.push(MyCV::Pixel(pixel.first - 1, pixel.second));

					if (pixel.first + 1 < mask.rows)
						stack.push(MyCV::Pixel(pixel.first + 1, pixel.second));

					if (pixel.second - 1 >= 0)
						stack.push(MyCV::Pixel(pixel.first, pixel.second - 1));

					if (pixel.second + 1 < mask.cols)
						stack.push(MyCV::Pixel(pixel.first, pixel.second + 1));
				}
			}
			segmentsList.push_back(segment);
		}
	}

	return segmentsList;
}

cv::Mat MyCV::bitwiseOr(cv::Mat &image, cv::Mat &image2)
{
	cv::Mat_<uchar> _R(image.rows, image.cols);
	cv::Mat_<uchar> _I1 = image;
	cv::Mat_<uchar> _I2 = image2;

	for (auto x = 0; x < image.cols; x++)
	{
		for (auto y = 0; y < image.rows; y++)
		{
			_R(y, x) = _I1(y, x) | _I2(y, x);
		}
	}

	return _R;
}

cv::Mat MyCV::dilationFilter(cv::Mat &image, int N)
{
	int p = N / 2;
	cv::Mat imageResult = image.clone();
	for (int x = p; x < image.cols - p; x++)
	{
		for (int y = p; y < image.rows - p; y++)
		{
			uchar newVal = image.at<uchar>(y, x);
			for (int i = -p; i <= p; i++)
			{
				for (int j = -p; j <= p; j++)
				{
					newVal = std::max(newVal, image.at<uchar>(y - i, x - j));
				}
			}
			imageResult.at<uchar>(y, x) = newVal;
		}
	}
	return imageResult;
}

cv::Mat MyCV::erosionFilter(cv::Mat &image, int N)
{
	int p = N / 2;
	cv::Mat imageResult = image.clone();
	for (int x = p; x < image.cols - p; x++)
	{
		for (int y = p; y < image.rows - p; y++)
		{
			uchar newVal = image.at<uchar>(y, x);
			for (int i = -p; i <= p; i++)
			{
				for (int j = -p; j <= p; j++)
				{
					newVal = std::min(newVal, image.at<uchar>(y - i, x - j));
				}
			}
			imageResult.at<uchar>(y, x) = newVal;
		}
	}
	return imageResult;
}

cv::Mat MyCV::blurFilter(cv::Mat &image, int N)
{
	cv::Mat BuluredImage(image.rows, image.cols, CV_8UC3);
	cv::Mat_<cv::Vec3b> _BI = BuluredImage;

	int p = N / 2;
	int mult = N * N;
	for (int x = p; x < image.cols - p; x++)
	{
		for (int y = p; y < image.rows - p; y++)
		{
			float newVal1 = 0, newVal2 = 0, newVal3 = 0;
			for (int i = -p; i <= p; i++)
			{
				for (int j = -p; j <= p; j++)
				{
					newVal1 += image.at<cv::Vec3b>(y - i, x - j)[0];
					newVal2 += image.at<cv::Vec3b>(y - i, x - j)[1];
					newVal3 += image.at<cv::Vec3b>(y - i, x - j)[2];
				}
			}
			_BI(y, x)
			[0] = newVal1 / mult;
			_BI(y, x)
			[1] = newVal2 / mult;
			_BI(y, x)
			[2] = newVal3 / mult;
		}
	}
	return BuluredImage;
}

cv::Mat MyCV::equalizeHistogramHSV(cv::Mat &image)
{ // Wyr�wnanie tylko sk�adowej V
	std::array<int, 256> histogram;
	for (int x = 0; x < 256; x++)
		histogram[x] = 0;

	for (int x = 0; x < image.cols; x++)
	{
		for (int y = 0; y < image.rows; y++)
		{

			int value = image.at<cv::Vec3b>(y, x)[2];
			histogram[value]++;
		}
	}
	std::array<int, 256> LUT;

	int cumSum = 0;
	int pixelsNumber = image.rows * image.cols;
	for (auto i = 0; i < 256; i++)
	{
		cumSum += histogram[i];
		LUT[i] = cumSum * 255 / pixelsNumber;
	}

	for (int x = 0; x < image.cols; x++)
	{
		for (int y = 0; y < image.rows; y++)
		{
			image.at<cv::Vec3b>(y, x)[2] = LUT[image.at<cv::Vec3b>(y, x)[2]];
		}
	}
	return image;
}

cv::Mat MyCV::unsharpMask(cv::Mat &image)
{
	int kernel[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}};

	cv::Mat imageResult = image.clone();
	for (int x = 1; x < image.cols - 1; x++)
	{
		for (int y = 1; y < image.rows - 1; y++)
		{
			int newR = 0, newB = 0, newG = 0;
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					newB += kernel[i + 1][j + 1] * image.at<cv::Vec3b>(y + j, x + i)[0];
					newG += kernel[i + 1][j + 1] * image.at<cv::Vec3b>(y + j, x + i)[1];
					newR += kernel[i + 1][j + 1] * image.at<cv::Vec3b>(y + j, x + i)[2];
				}
			}
			imageResult.at<cv::Vec3b>(y, x)[0] = crop(newB);
			imageResult.at<cv::Vec3b>(y, x)[1] = crop(newG);
			imageResult.at<cv::Vec3b>(y, x)[2] = crop(newR);
		}
	}
	return imageResult;
}
