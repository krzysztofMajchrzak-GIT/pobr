#include "BoundingBox.h"


/** Bounding Box **/
BoundingBox::BoundingBox()
{
}

bool BoundingBox::isIn(std::pair<int,int> p)
{
	if (x_m <= p.second && x_mx >= p.second && y_m <= p.first && y_mx >= p.first)
		return true;
	return false;
}

int BoundingBox::height()
{
	return y_mx - y_m + 1;
}

int BoundingBox::width()
{
	return x_mx - x_m + 1;
}


