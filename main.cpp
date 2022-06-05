#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <ctime>
#include "utils.h"

using namespace std;

/*** Color ranges ***/
pair<int,int> yellow1, yellow2, yellow3;
pair<int,int> blue1, blue2, blue3;
pair<int,int> red_i1, red_i2, red_i3;
pair<int,int> red_c1, red_c2, red_c3;
/////////////////////////////////////
yellow1.first = 17;
yellow1.second = 45;

yellow2.first = 90;
yellow2.second = 256;

yellow3.first = 90;
yellow3.second = 256;
/////////////////////////////////////
blue1.first = 95;
blue1.second = 145;

blue2.first = 45;
blue2.second = 256;

blue3.first = 0;
blue3.second = 256;
/////////////////////////////////////
red_i1.first = 0;
red_i1.second = 14;

red_i2.first = 0;
red_i2.second = 256;

red_i3.first = 0;
red_i3.second = 256;
/////////////////////////////////////
red_c1.first = 165;
red_c1.second = 181;

red_c2.first = 0;
red_c2.second = 256;

red_c3.first = 0;
red_c3.second = 256;




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

        cv::waitKey(0);




    }   
}

