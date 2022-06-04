#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <ctime>

using namespace std;


float crop(float val) {
	if (val > 255.0)  val = 255.0;
	if (val < 0.0) val = 0.0;
	return val;
}

cv::Mat unsharp(cv::Mat& photo)
{
	int sharpenFilter[][3] = {
		{0,-1,0},
		{-1,5,-1},
		{0,-1,0}
        };

	cv::Mat result = photo.clone();

    for (int x = 1; x < photo.cols - 1; x++) { // For every column...
		for (int y = 1; y < photo.rows - 1; y++) { // ...iterate through every row

			int red = 0;
            int green = 0;
            int blue = 0;
			
            for (int i = 0; i <= 2; i++) { // filter colums
				for (int j = 0; j <= 2; j++) { // filter rows
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

int main(int, char* []) {
    while(true) // Main loop
    {
        /*** Image loading ***/

        cv::Mat photo;
        string photo_path;

        while(!photo.data) // Data loading loop
        {
            cout << "Image name:";
            cin >> photo_path;
            photo =  cv::imread(photo_path);
            if(photo.data)
            {
                cout << "Image" + photo_path + "read, starting detection..." << endl;
                break;
            }
                
        }

        /*** Image enhancement ***/
        cout << "Starting image correction" << endl;
        /** Image sharpening using unsharp mask F(x,y) = f(x,y)+k*( f(x,y)-Fblur(x,y) ) **/

        cv::Mat unsharpedImage = unsharp(photo);
        cv::imshow("original", photo);
        cv::imshow("bluredImage", unsharpedImage);
        cv::waitKey(0);
    }   
}

