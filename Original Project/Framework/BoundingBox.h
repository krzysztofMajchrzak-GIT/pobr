#pragma once
#include <optional>
namespace MyCV
{
	typedef std::pair<int, int> Pixel;
	class BoundingBox
	{
	public:
		int xmin, ymin, xmax, ymax;
		BoundingBox(int xmin, int xmax, int ymin, int ymax) : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax){};
		BoundingBox();
		bool isInside(Pixel p);
		int getHeight();
		int getWidth();
	};
}