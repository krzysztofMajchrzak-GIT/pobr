#include "PotentialHit.h"


bool PotentialHit::checkHit()
{
	return background.has_value() && outlyingLetters.size() == 2 && secondLetterBottom.has_value() && secondLetterTop.has_value() && thirdLetter.has_value();
	// return Circle.has_value() && L.size() == 2 && I.has_value() && Dot.has_value() && D.has_value();
}

PotentialHit::PotentialHit()
{
	// Circle = std::nullopt;
	// I = std::nullopt;
	// Dot = std::nullopt;
	// D = std::nullopt;
}
