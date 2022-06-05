#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <ctime>
#include "utils.h"

using namespace std;

/*** Color ranges ***/
pair yellow1(17,45), yellow2(90,256), yellow3(90,256);
pair blue1(95,145), blue2(45,256), blue3(0,256);
pair red_i1(0,14), red_i2(0,256), red_i3(0,256);
pair red_c1(165,181), red_c2(0,256), red_c3(0,256);
//yellow1.first = 1

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
        //cv::waitKey(0);

        /*** Convertion from BGR to HSV ***/

        cout << "Starting BGR to HSV convertion" << endl;

        cv::Mat hsv = bgr2hsv(unsharpedImage);

        cv::imshow("hsv", hsv);

        /*** Histogram equlization on HSV image for contrast enhancement ***/

        cout << "Starting equlization on HSV image" << endl;

        cv::Mat equlized = equlize(hsv); 

        cv::imshow("equlized", equlized);

        cv::Mat equlized2 = equalizeHistogramHSV(hsv);

        cv::imshow("equlized2", equlized2);

        cv::waitKey(0);




    }   
}

