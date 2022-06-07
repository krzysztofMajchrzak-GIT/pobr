#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include "MySimpleLogger.h"
using namespace sl;
using namespace cv;
using namespace std;

void getSegments(Mat& img, vector<vector<Point>>& segments) {

	Mat processImage;
	img.convertTo(processImage, CV_32FC1);
	float label = 2;

	for (int x = 0; x < processImage.cols; x++) {
		for (int y = 0; y < processImage.rows; y++) {

			if (processImage.at<float>(y, x) != 255.0)
				continue;

			Rect rect;
			floodFill(processImage, Point(x, y), Scalar(label), &rect, Scalar(0), Scalar(0));
			vector<Point> seg;
			for (int i = rect.x; i < rect.x + rect.width; i++) {
				for (int j = rect.y; j < rect.y + rect.height; j++) {
					if (processImage.at<float>(y, x) != label)
						continue;
					seg.push_back(Point(i, j));
				}
			}

			segments.push_back(seg);
			label++;
		}
	}
}


bool isYellowCircle(double m[7])
{
	double _I1 = m[0];
	double _I2 = m[1];
	double _I3 = m[2];
	double _I4 = m[3];
	double _I5 = m[4];
	double _I6 = m[5];
	if (_I1 < 0.159391 * 0.8 || _I1 > 0.232382 * 1.2)
		return false;
	if (_I2 < 7.15338E-05


		* 0.8 || _I2 >  0.0175107

		* 1.2)
		return false;
	if (_I3 < 3.42125E-09

		* 0.8 || _I3 > 0.000133239

		* 1.2)
		return false;
	if (_I4 < -0.000001
		* 1.2 || _I4 > 2.96307E-05
		* 1.2)
		return false;
	if (_I5 < -0.000001
		* 1.2 || _I5 > 0.000001
		* 1.2)
		return false;
	if (_I6 < -1.49807E-06

		* 1.2 || _I6 > 0.000001
		* 1.2)
		return false;
	return true;
}

bool isLetterI(double m[7])
{
	double _I1 = m[0];
	double _I2 = m[1];
	double _I3 = m[2];
	double _I4 = m[3];
	double _I5 = m[4];
	double _I6 = m[5];
	if (_I1 < 0.166057


		* 0.8 || _I1 > 0.190622

		* 1.2)
		return false;
	if (_I2 < 1.34E-05


		* 0.8 || _I2 > 0.00242965

		* 1.2)
		return false;
	if (_I3 < 4.56e-06


		* 0.8 || _I3 >0.000868728

		* 1.2)
		return false;
	if (_I4 < 1.87E-07


		* 0.8 || _I4 > 5.57461E-05

		* 1.2)
		return false;
	if (_I5 < -2.73774E-09

		* 1.2 || _I5 > 1.82001E-11

		* 1.2)
		return false;
	if (_I6 < -2.19381E-08

		* 1.2 || _I6 >  6.95565E-07

		* 1.2)
		return false;
	return true;
}

bool isRedDot(double m[7])
{
	double _I1 = m[0];
	double _I2 = m[1];
	double _I3 = m[2];
	double _I4 = m[3];
	double _I5 = m[4];
	double _I6 = m[5];

	if (_I1 < 0.162164

		* 0.8 || _I1 > 0.21061

		* 1.2)
		return false;
	if (_I2 < 0.000422695
		* 0.8 || _I2 > 0.0181908

		* 1.2)
		return false;
	if (_I3 < 5.16E-08



		* 0.8 || _I3 > 8.01172E-05

		* 1.2)
		return false;
	if (_I4 < 5.75E-10


		* 0.8 || _I4 >  1.89015E-05

		* 1.2)
		return false;
	if (_I5 < -1.35493E-12

		* 1.2 || _I5 > 7.28444E-10

		* 1.2)
		return false;
	if (_I6 < -6.13072E-09

		* 1.2 || _I6 > 2.50681E-06

		* 1.2)
		return false;
	return true;
}

bool isLetterL(double m[7])
{
	double _I1 = m[0];
	double _I2 = m[1];
	double _I3 = m[2];
	double _I4 = m[3];
	double _I5 = m[4];
	double _I6 = m[5];

	if (_I1 < 0.249054

		* 0.7 || _I1 >0.491307

		* 1.2)
		return false;
	if (_I2 < 0.00164057

		* 0.7 || _I2 > 0.175954

		* 1.2)
		return false;
	if (_I3 < 0.00588011

		* 0.7 || _I3 > 0.0322566

		* 1.2)
		return false;
	if (_I4 < 0.000421377

		* 0.7 || _I4 >0.0166033

		* 1.2)
		return false;
	if (_I5 < -1.05789E-06

		* 1.2 || _I5 >0.000381264

		* 1.2)
		return false;
	if (_I6 < -0.000106396

		* 1.2 || _I6 >  0.00687371

		* 1.2)
		return false;
	return true;
}

bool isLetterD(double m[7])
{
	double _I1 = m[0];
	double _I2 = m[1];
	double _I3 = m[2];
	double _I4 = m[3];
	double _I5 = m[4];
	double _I6 = m[5];

	if (_I1 < 0.167023



		* 0.7 || _I1 >0.181604


		* 1.2)
		return false;
	if (_I2 < 0.000213468


		* 0.7 || _I2 > 0.00552385


		* 1.2)
		return false;
	if (_I3 < 0.000136146

		* 0.7 || _I3 > 0.000284067




		* 1.2)
		return false;
	if (_I4 < 6.54E-06


		* 0.7 || _I4 > 2.33E-05




		* 1.2)
		return false;
	if (_I5 < -7.78E-10


		* 1.2 || _I5 >-1.60E-10



		* 0.8)
		return false;
	if (_I6 < -6.69E-07


		* 1.2 || _I6 > 4.23416E-07


		* 1.2)
		return false;
	return true;
}

int main(int, char* []) {
	std::string yesNo = "";
	do {
		std::string image_path = "";
		cv::Mat image;

		do {
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
		Mat blurredImage, sharpmask, hsv;


		blur(image, blurredImage, Size(3, 3));
		Mat_<double> mat(3, 3);
		mat << 0, -1, 0, -1, 5, -1, 0, -1, 0;
		filter2D(blurredImage, sharpmask, -1, mat);
		cvtColor(sharpmask, hsv, COLOR_BGR2HSV);
		//imshow("blured", blurredImage);
		//imshow("sharp", sharpmask);
		vector<Mat> hsvSplited(3);
		split(hsv, hsvSplited);
		equalizeHist(hsvSplited[2], hsvSplited[2]);
		merge(hsvSplited, hsv);

		println("Segmentation....");
		auto upYellow = Scalar(45, 256, 256);
		auto lowYellow = Scalar(17, 90, 90);

		auto upRed1 = Scalar(14, 256, 256);
		auto lowRed1 = Scalar(0, 0, 0);

		auto upRed2 = Scalar(181, 256, 256);
		auto lowRed2 = Scalar(165, 0, 0);

		auto upBlue = Scalar(145, 256, 256);
		auto lowBlue = Scalar(95, 45, 0);

		Mat yellowMask, blueMask, redMask1, redMask2, redMask;
		inRange(hsv, lowYellow, upYellow, yellowMask);
		//imshow("Y", yellowMask);

		inRange(hsv, lowBlue, upBlue, blueMask);
		//imshow("B", blueMask);
		erode(blueMask, blueMask, getStructuringElement(MORPH_RECT, Size(3, 3)));
		dilate(blueMask, blueMask, getStructuringElement(MORPH_RECT, Size(3, 3)));

		inRange(hsv, lowRed1, upRed1, redMask1);
		inRange(hsv, lowRed2, upRed2, redMask2);
		bitwise_or(redMask1, redMask2, redMask);
		erode(redMask, redMask, getStructuringElement(MORPH_RECT, Size(3, 3)));
		dilate(redMask, redMask, getStructuringElement(MORPH_RECT, Size(3, 3)));
		//imshow("R", redMask);
		//imshow("R1", redMask1);
		//imshow("R2", redMask2);
		vector<vector<Point>> contoursYellow;
		vector<vector<Point>> contoursBlue;
		vector<vector<Point>> contoursRed;

		findContours(yellowMask, contoursYellow, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		findContours(blueMask, contoursBlue, RETR_TREE, CHAIN_APPROX_SIMPLE);
		findContours(redMask, contoursRed, RETR_TREE, CHAIN_APPROX_SIMPLE);

		vector<pair<double*, int>> yellowCircles;
		println("Recognation....");
		for (int i = 0; i < contoursYellow.size(); i++) {
			double hu[7];
			HuMoments(moments(contoursYellow[i]), hu);
			//cout << "YEllow" << endl;
			//cout << "Size = " << contoursYellow[i].size() << endl;
			//for each (auto p in  hu) {
			//	cout << p << endl;
			//}
			if (isYellowCircle(hu)) {
				//auto rect = boundingRect(contoursYellow[i]);
				//rectangle(image, rect, Scalar(0, 255, 0));
				yellowCircles.push_back(pair(hu, i));
			}

		}


		//for (int i = 0; i < contoursRed.size(); i++) {
		//	double hu[7];
		//	HuMoments(moments(contoursRed[i]), hu);
		//	cout << "Red" << endl;
		//	cout << "Size = " << contoursRed[i].size() << endl;
		//	for each (auto p in  hu) {
		//		cout << p << endl;
		//	}
		//	/*if (isLetterI(hu)) {
		//		auto rect = boundingRect(contoursRed[i]);
		//		rectangle(image, rect, Scalar(0, 255, 0));		
		//	}*/

		//	if (isRedDot(hu)) {
		//		auto rect = boundingRect(contoursRed[i]);
		//		rectangle(image, rect, Scalar(0, 255, 0));
		//	}

		//}
		int logosCount=0;
		for (int i = 0; i < yellowCircles.size(); i++) {
			auto rectYellowCircle = boundingRect(contoursYellow[yellowCircles[i].second]);
			bool blueGood = false, redGood = false;
			vector<pair<double*, int>> LetterD;
			vector<pair<double*, int>> LetterL;

			for (int i = 0; i < contoursBlue.size(); i++) {
				double hu[7];
				if (contoursBlue[i].size() < 15)continue;
				auto m = moments(contoursBlue[i]);
				int x = m.m10 / m.m00, y = m.m01 / m.m00;
				HuMoments(m, hu);

				if (rectYellowCircle.contains(Point2i(x, y))) {
					if (isLetterD(hu)) {
						LetterD.push_back(pair(hu, i));
					}
					if (isLetterL(hu)) {
						LetterL.push_back(pair(hu, i));
					}
				}

			}
			if (LetterL.size() != 2 && LetterD.size() != 1)continue;

			vector<Point> sum;
			auto m = moments(contoursBlue[LetterL[0].second]);
			auto m2 = moments(contoursBlue[LetterL[1].second]);
			Point p1(m.m10 / m.m00, m.m01 / m.m00);
			Point p2(m2.m10 / m2.m00, m2.m01 / m2.m00);
			sum.insert(sum.end(), contoursBlue[LetterL[0].second].begin(), contoursBlue[LetterL[0].second].end());
			sum.insert(sum.end(), contoursBlue[LetterL[1].second].begin(), contoursBlue[LetterL[1].second].end());
			auto r = boundingRect(sum);
			auto dist = norm(p1 - p2);
			auto dwRatio = dist / r.width;
			if (dwRatio >= 0.7 && dwRatio <= 0.85) {
				blueGood = true;
			}
			vector<pair<double*, int>> LetterI;
			vector<pair<double*, int>> RedDot;

			for (int i = 0; i < contoursRed.size(); i++) {
				double hu[7];
				if (contoursRed[i].size() < 5)continue;
				auto m = moments(contoursRed[i]);
				int x = m.m10 / m.m00, y = m.m01 / m.m00;
				HuMoments(m, hu);

				if (rectYellowCircle.contains(Point2i(x, y))) {

					//cout << "Red" << endl;
					//cout << "Size = " << contoursRed[i].size() << endl;

					for each (auto p in  hu) {
						cout << p << endl;
					}
					if (isLetterI(hu)) {
						LetterI.push_back(pair(hu, i));
					}
					if (isRedDot(hu)) {
						RedDot.push_back(pair(hu, i));
					}


				}
			}
			//if (RedDot.size() != 1 && LetterI.size() != 1)continue;
			for each (auto dot in RedDot)
			{
				for each (auto iletter in LetterI)
				{
					vector<Point> sum;
					auto m = moments(contoursRed[dot.second]);
					auto m2 = moments(contoursRed[iletter.second]);
					Point p1(m.m10 / m.m00, m.m01 / m.m00);
					Point p2(m2.m10 / m2.m00, m2.m01 / m2.m00);
					sum.insert(sum.end(), contoursRed[dot.second].begin(), contoursRed[dot.second].end());
					sum.insert(sum.end(), contoursRed[iletter.second].begin(), contoursRed[iletter.second].end());
					auto r = boundingRect(sum);
					auto dist = norm(p1 - p2);
					auto dhRatio = dist / r.height;
					if (dhRatio >= 0.43 && dhRatio <= 0.6) {
						redGood = true;
					}
				}
			}
			if (redGood && blueGood) {
				rectangle(image, rectYellowCircle, Scalar(0, 255, 0),2);
				logosCount++;
			}

		}
		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		println("[DONE] Processing time: " + std::to_string(elapsed_secs) + "s");
		println("Logos found: " + std::to_string(logosCount));
		imshow("Result Image", image);
		imwrite("output.png", image);
		cv::waitKey(-1);

		println("");
		println("Another image? y/n");
		std::cin >> yesNo;
		if (yesNo != "y") break;

		println("");
		println("");
	}while (true);
	return 0;
}
