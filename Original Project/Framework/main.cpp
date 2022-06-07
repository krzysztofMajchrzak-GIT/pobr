#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>
#include "MyCv.h"
#include "MySimpleLogger.h"
#include "BoundingBox.h"
#include "Logo.h"
#include <ctime>

using namespace sl;

int main(int, char *[])
{
	std::string yesNo = "";
	do
	{
		std::string image_path = "";
		cv::Mat image;

		do
		{
			println("Provide image name...");
			std::cin >> image_path;
			image = cv::imread(image_path);
			if (image.data)
			{
				break;
			}
			println("Could not open or find the image. Try again...");
		} while (true);

		println("Image " + image_path + " successful read!");
		println("Start proccesing...");

		clock_t begin = clock();
		println("Image enchancement...");

		// Blurr nd Unsharp
		cv::Mat bluerdImage = MyCV::blurFilter(image, 3);
		cv::Mat unsharp = MyCV::unsharpMask(bluerdImage);

		cv::Mat image2 = MyCV::convertBGR2HSV(unsharp);
		MyCV::equalizeHistogramHSV(image2);

		// cv::imshow("un", unsharp);
		cv::imshow("bluredImage", bluerdImage);

		// Yellow
		MyCV::Range y1(17, 45);
		MyCV::Range y2(90, 256);
		MyCV::Range y3(90, 256);

		// Blue
		MyCV::Range b1(95, 145);
		MyCV::Range b2(45, 256);
		MyCV::Range b3(0, 256);

		// Red low
		MyCV::Range r1(0, 14);
		MyCV::Range r2(0, 256);
		MyCV::Range r3(0, 256);
		// Red up
		MyCV::Range r4(165, 181);
		MyCV::Range r5(0, 256);
		MyCV::Range r6(0, 256);

		println("Segmentation....");
		// cv::Mat mask = MyCV::getMaskFrom3D(image2, b1, b2, b3);

		cv::Mat blueMask = MyCV::getMaskFrom3D(image2, b1, b2, b3);
		blueMask = MyCV::erosionFilter(blueMask, 3);
		blueMask = MyCV::dilationFilter(blueMask, 3);
		cv::imshow("BLuemask", blueMask);

		cv::Mat yellowMask = MyCV::getMaskFrom3D(image2, y1, y2, y3);
		// yellowMask = MyCV::erosionFilter(yellowMask, 3);
		// yellowMask = MyCV::dilationFilter(yellowMask, 3);
		// cv::imshow("yelmask", yellowMask);
		cv::Mat redLowMask = MyCV::getMaskFrom3D(image2, r1, r2, r3);
		cv::Mat redUpMask = MyCV::getMaskFrom3D(image2, r4, r5, r6);
		cv::Mat redMask = MyCV::bitwiseOr(redLowMask, redUpMask);
		redMask = MyCV::erosionFilter(redMask, 3);
		redMask = MyCV::dilationFilter(redMask, 3);

		// cv::imshow("redmask", redMask);
		auto blueSegments = MyCV::getSegemntsFromMask(blueMask);
		auto yellowSegments = MyCV::getSegemntsFromMask(yellowMask);
		auto redSegments = MyCV::getSegemntsFromMask(redMask);

		std::sort(blueSegments.begin(), blueSegments.end(), [](MyCV::Segment &s1, MyCV::Segment &s2) -> bool
				  { return s1.size() < s2.size(); });

		std::sort(yellowSegments.begin(), yellowSegments.end(), [](MyCV::Segment &s1, MyCV::Segment &s2) -> bool
				  { return s1.size() < s2.size(); });

		std::sort(redSegments.begin(), redSegments.end(), [](MyCV::Segment &s1, MyCV::Segment &s2) -> bool
				  { return s1.size() < s2.size(); });

		// Finding logos

		println("Recognation....");
		int logosCount = 0;
		std::vector<MyCV::Logo> logos;

		// Finding yellow circle
		std::for_each(yellowSegments.begin(), yellowSegments.end(), [&image, &logos](MyCV::Segment &s1)
					  {
			if (s1.size() < 50)
				return;
			//std::cout << "Yellow" << std::endl;
			MyCV::SegmentInfo inf(s1);
			if (inf.isYellowCircle()) {

				MyCV::Logo newLogo;
				newLogo.Circle = inf;
				logos.push_back(newLogo);
				//image = MyCV::drawBoundingBox(image, inf.boundigBox);
			} });

		// std::for_each(redSegments.begin(), redSegments.end(), [](MyCV::Segment& s1) {
		//	if (s1.size() < 50)
		//		return;
		//	//std::cout << "Red" << std::endl;
		//	MyCV::SegmentInfo inf(s1);
		//	if (inf.isLetterI()) {
		//		int i = 0;
		//	}
		//	else if (inf.isRedDot()) {
		//		int i = 0;
		//	}
		//	});

		for (int i = 0; i < logos.size(); i++)
		{
			auto bg = logos[i];
			std::vector<MyCV::SegmentInfo> letterL;
			std::vector<MyCV::SegmentInfo> letterD;

			std::for_each(blueSegments.begin(), blueSegments.end(), [&letterD, &letterL, &bg](MyCV::Segment &s1)
						  {
				if (s1.size() < 50)
					return;
				//std::cout << "Blue" << std::endl;
				MyCV::SegmentInfo inf(s1);
				if (!bg.Circle.value().isGravityCenterInsideBB(inf))
					return;

				if (inf.isLetterD()) {
					letterD.push_back(inf);
				}
				if (inf.isLetterL()) {
					letterL.push_back(inf);
				} });

			if (letterL.size() != 2)
				break;

			MyCV::Segment segment;
			std::copy(letterL[0]._s.begin(), letterL[0]._s.end(), std::back_inserter(segment));
			std::copy(letterL[1]._s.begin(), letterL[1]._s.end(), std::back_inserter(segment));

			MyCV::SegmentInfo twoLInfo(segment);
			auto dist = letterL[0].getDistanceToCenter(letterL[1]);
			auto dwRatio = dist / twoLInfo.boundigBox.getWidth();

			if (dwRatio >= 0.7 && dwRatio <= 0.85)
			{
				std::vector<MyCV::SegmentInfo> cloneVec;
				std::copy(letterL.begin(), letterL.end(), std::back_inserter(cloneVec));
				bg.L = cloneVec;
			}
			// std::cout << "L d/w ratio " << dwRatio << std::endl;
			if (letterD.size() != 1)
				break;
			if (bg.Circle.value().isGravityCenterInsideBB(letterD[0]))
			{
				MyCV::SegmentInfo temp(letterD[0]._s);
				bg.D = temp;
			}

			std::vector<MyCV::SegmentInfo> letterDot;
			std::vector<MyCV::SegmentInfo> letterI;

			std::for_each(redSegments.begin(), redSegments.end(), [&letterDot, &letterI, &bg, &twoLInfo](MyCV::Segment &s1)
						  {
				if (s1.size() < 50)
					return;
				//std::cout << "Red" << std::endl;
				MyCV::SegmentInfo inf(s1);
				if (!bg.Circle.value().isGravityCenterInsideBB(inf))
					return;

				if (inf.isLetterI() && twoLInfo.isGravityCenterInsideBB(inf)) {
					letterI.push_back(inf);
				}
				if (inf.isRedDot()) {
					letterDot.push_back(inf);
				} });
			MyCV::Segment segment2;
			std::copy(letterI[0]._s.begin(), letterI[0]._s.end(), std::back_inserter(segment2));
			std::copy(letterDot[0]._s.begin(), letterDot[0]._s.end(), std::back_inserter(segment2));
			MyCV::SegmentInfo fullI(segment2);

			dist = letterI[0].getDistanceToCenter(letterDot[0]);
			auto dhRatio = dist / fullI.boundigBox.getHeight();
			if (dhRatio >= 0.43 && dhRatio <= 0.6)
			{
				bg.Dot = letterDot[0];
				bg.I = letterI[0];
			}
			// std::cout << "I d/h ratio " << dhRatio << std::endl;
			logos[i] = bg;
		}

		std::for_each(logos.begin(), logos.end(), [&image, &logosCount](MyCV::Logo &s1)
					  {
			if (s1.isValidLogo()) {
				image = MyCV::drawBoundingBox(image, s1.Circle.value().boundigBox);
				logosCount++;
			} });
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		println("[DONE] Processing time: " + std::to_string(elapsed_secs) + "s");
		println("Logos found: " + std::to_string(logosCount));
		cv::imshow("Result Image", image);
		cv::imwrite("resultImage.png", image);
		cv::waitKey(-1);
		println("");
		println("Another image? y/n");
		std::cin >> yesNo;
		if (yesNo != "y")
			break;

		println("");
		println("");
	} while (true);
	return 0;
}
