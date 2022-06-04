#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <ctime>
#include "utils.h"

using namespace std;



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

