#include <vector>

class BoundingBox
{
	public:
		int x_m, y_m, x_mx, y_mx;
		BoundingBox(int x_m, int x_mx, int y_m, int y_mx) :x_m(x_m), x_mx(x_mx), y_m(y_m), y_mx(y_mx) {};
		BoundingBox();
		bool isIn(std::pair<int,int> p);
		int height();
		int width();
};
