#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include <stack>

#include "PotentialHit.h"
#include "utils.h"

using namespace std;

std::vector<std::vector<pair<int, int>>> getField(cv::Mat &photo);
void sortField(std::vector<std::vector<pair<int, int>>> &field);
cv::Mat drawBoundingBox(cv::Mat &matrix, BoundingBox box);

/*** Color ranges ***/
pair yellow1(17, 45), yellow2(90, 256), yellow3(90, 256);
pair blue1(95, 145), blue2(45, 256), blue3(0, 256);
pair red_i1(0, 14), red_i2(0, 256), red_i3(0, 256);
pair red_c1(165, 181), red_c2(0, 256), red_c3(0, 256);
// yellow1.first = 1

int main(int, char *[])
{
    const int minValidFieldSize = 64;
    while (true) // Main loop
    {
        /*** Image loading ***/

        cv::Mat photo;
        string photo_path;

        while (!photo.data) // Data loading loop
        {
            cout << "Image name:";
            cin >> photo_path;
            photo = cv::imread(photo_path);
            if (photo.data)
            {
                cout << "Image" + photo_path + "read, starting detection..." << endl;
                break;
            }
        }

        /*** Image correction ***/

        cout << "Starting image correction" << endl;

        /** Image sharpening using unsharp mask F(x,y) = f(x,y)+k*( f(x,y)-Fblur(x,y) ) **/

        cv::Mat unsharpedImage = unsharp(photo);

        /** Convertion from BGR to HSV **/

        cout << "Starting BGR to HSV convertion" << endl;

        cv::Mat hsv = bgr2hsv(unsharpedImage);

        /** Histogram equlization on HSV image for contrast enhancement **/

        cout << "Starting equlization on HSV image" << endl;

        cv::Mat equlized = equlize(hsv);

        /*** Logo segmentation ***/

        cout << "Starting logo segmentation" << endl;

        /** Erosion and dilation filters **/

        cv::Mat ym = masking(photo, yellow1, yellow2, yellow3);
        ym = erosion(ym);
        ym = dilation(ym);

        cv::Mat bm = masking(photo, blue1, blue2, blue3);
        bm = erosion(bm);
        bm = dilation(bm);

        cv::Mat r1m = masking(photo, red_c1, red_c2, red_c3);
        cv::Mat r2m = masking(photo, red_i1, red_i2, red_i3);
        cv::Mat rm = bitwiseOr(r1m,r2m);

        rm = erosion(rm);
        rm = dilation(rm);

        cv::imshow("blurredImage", unsharpedImage);
        cv::imshow("convertedImage", hsv);
        cv::imshow("ymask", ym);
        cv::imshow("rmask", rm);
        cv::imshow("bmask", bm);
        cv::waitKey(0);

        /** Mask fields **/

        auto bfield = getField(bm);
        auto yfield = getField(ym);
        auto rfield = getField(rm);
        // auto r2field = getField(r2m);

        /** Sort the segments **/

        sortField(bfield);
        sortField(yfield);
        sortField(rfield);
        // sortField(r2field);

        /** Searching for letters **/

        std::vector<PotentialHit> hits;

        for (int i = 0; i < yfield.size(); i++) // Iterate through all yellow fields
        {
            if (yfield[i].size() > minValidFieldSize) // Minimum threshold
            {
                FieldInvariants fi(yfield[i]); // Calculating Invariants
                if (fi.findCircle())
                {
                    PotentialHit ph;
                    ph.background = fi;
                    hits.push_back(ph);
                }
            }
        }

        for (int i = 0; i < hits.size(); i++)
        {
            PotentialHit hit = hits[i];
            std::vector<FieldInvariants> fieldsWithOutlying;
            std::vector<FieldInvariants> fieldsWithThird;

            for (int j = 0; j < bfield.size(); j++)
            {
                if (bfield[j].size() > minValidFieldSize)
                {
                    FieldInvariants fi(bfield[j]);
                    if (hit.background.value().isgcInBB(fi))
                    {
                        if (fi.findD())
                            fieldsWithThird.push_back(fi);
                        if (fi.findL())
                            fieldsWithOutlying.push_back(fi);
                    }
                }
            }

            if (fieldsWithOutlying.size() != 2)
                break;
            std::vector<pair<int, int>> outlyingField;
            for (int j = 0; j < 2; j++)
                std::copy(fieldsWithOutlying[j].field.begin(), fieldsWithOutlying[j].field.end(), std::back_inserter(outlyingField));
            FieldInvariants outlyingInvariants(outlyingField);
            auto separation = fieldsWithOutlying[0].rToCenter(fieldsWithOutlying[1]);
            auto widthRatio = separation / outlyingInvariants.boundigBox.width();
            if (0.7 <= widthRatio && widthRatio <= 0.85)
            {
                std::vector<FieldInvariants> copy;
                std::copy(fieldsWithOutlying.begin(), fieldsWithOutlying.end(), std::back_inserter(copy));
                hit.outlyingLetters = copy;
            }

            if (fieldsWithThird.size() != 1)
                break;
            if (hit.background.value().isgcInBB(fieldsWithThird[0]))
            {
                FieldInvariants thirdLetter(fieldsWithThird[0].field);
                hit.thirdLetter = thirdLetter;
            }

            std::vector<FieldInvariants> secondLetterBottom;
            std::vector<FieldInvariants> secondLetterTop;
            for (int j = 0; j < rfield.size(); j++)
            {
                if (rfield[j].size() > minValidFieldSize)
                {
                    FieldInvariants fi(rfield[j]);
                    if (hit.background.value().isgcInBB(fi))
                    {
                        if (fi.findI() && outlyingInvariants.isgcInBB(fi))
                            secondLetterBottom.push_back(fi);
                        if (fi.findDot())
                            secondLetterTop.push_back(fi);
                    }
                }
            }

            std::vector<pair<int, int>> secondLetterField;
            std::copy(secondLetterBottom[0].field.begin(), secondLetterBottom[0].field.end(), std::back_inserter(secondLetterField));
            std::copy(secondLetterTop[0].field.begin(), secondLetterTop[0].field.end(), std::back_inserter(secondLetterField));
            FieldInvariants secondLetterFull(secondLetterField);
            separation = secondLetterBottom[0].rToCenter(secondLetterTop[0]);
            auto heightRatio = separation / secondLetterFull.boundigBox.height();
            if (heightRatio >= 0.43 && heightRatio <= 0.6)
            {
                hit.secondLetterBottom = secondLetterBottom[0];
                hit.secondLetterTop = secondLetterTop[0];
            }
            hits[i] = hit;
        }

        int hitCount = 0;
        for (int j = 0; j < hits.size(); j++)
        {
            if (hits[j].checkHit())
            {
                photo = drawBoundingBox(photo, hits[j].background.value().boundigBox);
                hitCount++;
            }
        }
        cv::imshow("Result Image", photo);
        cv::imwrite("resultImage.png", photo);
        cv::waitKey(-1);
    }
}

std::vector<std::vector<pair<int, int>>> getField(cv::Mat &photo)
{
    // We need a list of fields which is a list of single pixels

    cv::Mat seen(photo.rows, photo.cols, CV_8U); // 8 bits per pixel - 255 values
    cv::Mat_<uchar> _S = seen;
    cv::Mat_<uchar> _P = photo;

    int height = photo.rows;
    int width = photo.cols;

    for (int x = 0; x < width; x++)
    { // For every column...
        for (int y = 0; y < height; y++)
        { // ...iterate through every row
            _S(y, x) = 0;
        }
    }

    std::vector<std::vector<pair<int, int>>> result;

    for (int x = 0; x < photo.cols; x++)
    { // For every column...
        for (int y = 0; y < photo.rows; y++)
        {                      // ...iterate through every row
            if (_P(y, x) == 0) // It is not a part of mask
                continue;

            std::vector<pair<int, int>> field;
            std::stack<pair<int, int>> fieldIterator;
            fieldIterator.push(pair(y, x));
            pair<int, int> singlePixel;

            while (true)
            {
                singlePixel = fieldIterator.top();
                fieldIterator.pop();

                if (_P(singlePixel.first, singlePixel.second) == 255 &&
                    _S(singlePixel.first, singlePixel.second) != 1)
                {
                    field.push_back(singlePixel);
                    _S(singlePixel.first, singlePixel.second) = 1;

                    // Check every single pixel around

                    if (singlePixel.first + 1 < height)
                        fieldIterator.push(pair(singlePixel.first + 1, singlePixel.second));
                    if (singlePixel.first - 1 >= 0)
                        fieldIterator.push(pair(singlePixel.first - 1, singlePixel.second));
                    if (singlePixel.second + 1 < width)
                        fieldIterator.push(pair(singlePixel.first, singlePixel.second + 1));
                    if (singlePixel.second - 1 >= 0)
                        fieldIterator.push(pair(singlePixel.first, singlePixel.second - 1));
                }
            }
            result.push_back(field);
        }

        return result;
    }
}

void sortField(std::vector<std::vector<pair<int, int>>> &field)
{
    std::sort(field.begin(), field.end(), [](std::vector<pair<int, int>> &v1, std::vector<pair<int, int>> &v2) -> bool
              { return v1.size() < v2.size(); });
}

cv::Mat drawBoundingBox(cv::Mat &matrix, BoundingBox box)
{
    const int borderThickness = 1;
    auto matrixClone = matrix.clone();
    cv::Mat_<cv::Vec3b> _R = matrixClone;

    for (int i = box.x_m; i < box.x_mx; i++)
    {

        for (int j = -borderThickness; j <= borderThickness; j++)
        {
            int y_m = box.y_m + j;
            int y_mx = box.y_mx + j;

            if (y_m >= 0 && y_m < matrixClone.rows)
            {
                _R(y_m, i)
                [0] = 0;
                _R(y_m, i)
                [1] = 255;
                _R(y_m, i)
                [2] = 0;
            }
            if (y_mx >= 0 && y_mx < matrixClone.rows)
            {
                _R(y_mx, i)
                [0] = 0;
                _R(y_mx, i)
                [1] = 255;
                _R(y_mx, i)
                [2] = 0;
            }
        }
    }

    for (int i = box.y_m; i < box.y_mx; i++)
    {
        for (int j = -borderThickness; j <= borderThickness; j++)
        {
            int x_m = box.x_m + j;
            int x_mx = box.x_mx + j;

            if (x_m >= 0 && x_m < matrixClone.cols)
            {
                _R(i, x_m)
                [0] = 0;
                _R(i, x_m)
                [1] = 255;
                _R(i, x_m)
                [2] = 0;
            }
            if (x_mx >= 0 && x_mx < matrixClone.cols)
            {
                _R(i, x_mx)
                [0] = 0;
                _R(i, x_mx)
                [1] = 255;
                _R(i, x_mx)
                [2] = 0;
            }
        }
    }
    return matrixClone;
}
