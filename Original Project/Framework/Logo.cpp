#include "Logo.h"

bool MyCV::Logo::isValidLogo()
{
	return Circle.has_value() && L.size() == 2 && I.has_value() && Dot.has_value() && D.has_value();
}

MyCV::Logo::Logo()
{
	Circle = std::nullopt;
	I = std::nullopt;
	Dot = std::nullopt;
	D = std::nullopt;
}
