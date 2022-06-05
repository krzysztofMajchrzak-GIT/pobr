#include "utils.h"
#include <iostream>
using namespace std;

cv::Mat unsharp(cv::Mat& photo)
{
	int sharpenFilter[3][3] = {
		{0,-1,0},
		{-1,5,-1},
		{0,-1,0}
        };

	cv::Mat result = photo.clone();

    for (int x = 1; x < photo.cols - 1; x++) 
    { // For every column...
		for (int y = 1; y < photo.rows - 1; y++) 
        { // ...iterate through every row

			int red = 0;
            int green = 0;
            int blue = 0;
			
            for (int i = -1; i <= 1; i++) 
            { // filter colums
				for (int j = -1; j <= 1; j++) 
                { // filter rows
					blue += sharpenFilter[i + 1][j + 1] * photo.at<cv::Vec3b>(y + j, x + i)[0];
					green += sharpenFilter[i + 1][j + 1] * photo.at<cv::Vec3b>(y + j, x + i)[1];
					red += sharpenFilter[i + 1][j + 1] * photo.at<cv::Vec3b>(y + j, x + i)[2];
				}
			}

            if (red > 255.0) 
                red = 255.0;
	        else if (red < 0.0) 
                red = 0.0;
            
            if (green > 255.0) 
                green = 255.0;
	        else if (green < 0.0) 
                green = 0.0;
            
            if (blue > 255.0) 
                blue = 255.0;
	        else if (blue < 0.0) 
                blue = 0.0;
            

			result.at<cv::Vec3b>(y, x)[0] = blue;
			result.at<cv::Vec3b>(y, x)[1] = green;
			result.at<cv::Vec3b>(y, x)[2] = red;

		}
	}
	return result;
}

cv::Mat bgr2hsv(cv::Mat& photo)
{
    cv::Mat result(photo.rows, photo.cols, CV_8UC3);

    //cv::Mat_<cv::Vec3b> _I = photo;
    cv::Mat_<cv::Vec3b> _R = result;
    
    
    //float b = 0.0;
    //float g = 0.0;
    //float r = 0.0;
    


    for (int x = 1; x < photo.cols - 1; x++) 
    { // For every column...
		for (int y = 1; y < photo.rows - 1; y++) 
        { // ...iterate through every row
            // R, G, B values are divided by 255
            // to change the range from 0..255 to 0..1:
            auto b = photo.at<cv::Vec3b>(y, x)[0]/ 255.0;
            auto g = photo.at<cv::Vec3b>(y, x)[1]/ 255.0;
            auto r = photo.at<cv::Vec3b>(y, x)[2]/ 255.0;

            // h, s, v = hue, saturation, value
            float h = 0.0;
            float s = 0.0;
            float v = 0.0;

            auto cmax = max(max(r, g), b);
            auto cmin = min(min(r, g), b);
            auto difference = cmax-cmin;

            // compute h

            if(cmax == cmin)
                h = 0;
            
            else if(cmax == r)
                h = 60.0 * ((g - b) / difference);

            else if(cmax == g)
                h = (60.0 * ((b - r) / difference) + 120);

            else if(cmax == b)
                h = (60 * ((r - g) / difference) + 240);

            if (h < 0)
                h += 360; 

            // compute s

            if (cmax == 0)
                s = 0;
            else
                s = (difference / cmax);
            
            // compute v

            v = cmax;

            _R(y, x)[0] = h / 2;
			_R(y, x)[1] = s * 255;
			_R(y, x)[2] = v * 255;        
        }
    }
    return result;
}

cv::Mat equlize(cv::Mat& photo)
{
    int resolution = photo.rows * photo.cols;
    int sum = 0;

    int his[256] = {0};
    float lut[256] = {0};

    cv::Mat result = photo.clone();

    for (int x = 1; x < photo.cols - 1; x++) 
    { // For every column...
		for (int y = 1; y < photo.rows - 1; y++) 
        { // ...iterate through every row

            int v = photo.at<cv::Vec3b>(y, x)[2];
            his[v] = his[v] + 1;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        sum = sum + his[i];
        lut[i] = sum * 255 / resolution;
    }

    for (int x = 1; x < photo.cols - 1; x++) 
    { // For every column...
		for (int y = 1; y < photo.rows - 1; y++) 
        { // ...iterate through every row
            //_R(y,x)[0] = photo.at<cv::Vec3b>(y, x)[0]
            //_R(y,x)[1] = photo.at<cv::Vec3b>(y, x)[1]
            result.at<cv::Vec3b>(y, x)[2] = lut[photo.at<cv::Vec3b>(y, x)[2]];
        }
    }

    return result;
    
}

cv::Mat masking(cv::Mat& photo, std::pair<int,int>& c1, std::pair<int,int>& c2, std::pair<int,int>& c3)
{
    cv::Mat_<uchar> _R(photo.rows, photo.cols);
    //cv::Mat_<cv::Vec3b> _I = photo;

    for (int x = 1; x < photo.cols - 1; x++) 
    { // For every column...
		for (int y = 1; y < photo.rows - 1; y++) 
        { // ...iterate through every row
            if(c1.first <= photo.at<cv::Vec3b>(y, x)[0] && c1.second > photo.at<cv::Vec3b>(y, x)[0]
            && c2.first <= photo.at<cv::Vec3b>(y, x)[1] && c2.second > photo.at<cv::Vec3b>(y, x)[1] 
            && c3.first <= photo.at<cv::Vec3b>(y, x)[2] && c3.second > photo.at<cv::Vec3b>(y, x)[2])
                _R(y,x) = 255;

            else
                _R(y,x) = 0;
        }
    }

    return _R;
}

cv::Mat dilatationFilter(cv::Mat& image, int N)
{
	int p = N / 2;
	cv::Mat imageResult = image.clone();
	for (int x = p; x < image.cols - p; x++) {
		for (int y = p; y < image.rows - p; y++) {
			uchar newVal = image.at<uchar>(y, x);
			for (int i = -p; i <= p; i++) {
				for (int j = -p; j <= p; j++) {
					newVal = std::max(newVal, image.at<uchar>(y - i, x - j));
				}
			}
			imageResult.at<uchar>(y, x) = newVal;
		}
	}
	return imageResult;
}

cv::Mat erosionFilter(cv::Mat& image, int N)
{
	int p = N / 2;
	cv::Mat imageResult = image.clone();
	for (int x = p; x < image.cols - p; x++) {
		for (int y = p; y < image.rows - p; y++) {
			uchar newVal = image.at<uchar>(y, x);
			for (int i = -p; i <= p; i++) {
				for (int j = -p; j <= p; j++) {
					newVal = std::min(newVal, image.at<uchar>(y - i, x - j));
				}
			}
			imageResult.at<uchar>(y, x) = newVal;
		}
	}
	return imageResult;
}