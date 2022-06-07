#include "BoundingBox.h"

MyCV::BoundingBox::BoundingBox()
{
}

bool MyCV::BoundingBox::isInside(Pixel p)
{
	if (xmin <= p.second && xmax >= p.second && ymin <= p.first && ymax >= p.first)
		return true;
	return false;
}

int MyCV::BoundingBox::getHeight()
{
	return ymax - ymin + 1;
}

int MyCV::BoundingBox::getWidth()
{
	return xmax - xmin + 1;
}