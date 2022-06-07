#pragma once
#include <optional>
#include <algorithm>
#include "BoundingBox.h"
#include "MyCv.h"
namespace MyCV
{

	class SegmentInfo
	{
	public:
		SegmentInfo(Segment s);
		std::pair<float, float> getGravityCenter();
		bool isLetterL();
		bool isLetterI();
		bool isLetterD();
		bool isYellowCircle();
		bool isRedDot();
		BoundingBox boundigBox;
		bool isGravityCenterInsideBB(SegmentInfo seg);
		double getDistanceToCenter(SegmentInfo seg);
		Segment _s;

	private:
		std::pair<float, float> _gravityCenter;
		double _M00, _M01, _M10;
		double _I1, _I2, _I3, _I4, _I5, _I6;
		std::pair<float, float> computeGravityCenter();
		double computeGM(int p, int q);
		double computeCGM(int p, int q);
	};
}
