#include "SegmentInfo.h"

MyCV::SegmentInfo::SegmentInfo(Segment s) : _s(s)
{
	auto xmin = *std::min_element(_s.begin(), _s.end(), [](Pixel &p1, Pixel &p2)
								  { return p1.second < p2.second; });
	auto xmax = *std::max_element(_s.begin(), _s.end(), [](Pixel &p1, Pixel &p2)
								  { return p1.second < p2.second; });
	auto ymin = *std::min_element(_s.begin(), _s.end(), [](Pixel &p1, Pixel &p2)
								  { return p1.first < p2.first; });
	auto ymax = *std::max_element(_s.begin(), _s.end(), [](Pixel &p1, Pixel &p2)
								  { return p1.first < p2.first; });
	boundigBox = BoundingBox(xmin.second, xmax.second, ymin.first, ymax.first);

	_M00 = computeGM(0, 0);
	_M10 = computeGM(1, 0);
	_M01 = computeGM(0, 1);

	double _c11, _c02, _c20, _c12, _c21, _c30, _c03;

	_c11 = computeCGM(1, 1);
	_c02 = computeCGM(0, 2);
	_c20 = computeCGM(2, 0);
	_c12 = computeCGM(1, 2);
	_c21 = computeCGM(2, 1);
	_c30 = computeCGM(3, 0);
	_c03 = computeCGM(0, 3);

	_I1 = _c20 + _c02;
	_I2 = std::pow(_c20 - _c02, 2) + 4 * std::pow(_c11, 2);
	_I3 = std::pow(_c30 - 3 * _c12, 2) + std::pow(3 * _c21 - _c03, 2);
	_I4 = std::pow(_c30 + _c12, 2) + std::pow(_c21 + _c03, 2);
	_I5 = (_c30 - 3 * _c12) * (_c30 + _c12) * (std::pow(_c30 + _c12, 2) - 3 * std::pow(_c21 + _c03, 2)) + (3 * _c21 - _c03) * (_c21 + _c03) * (3 * std::pow(_c30 + _c12, 2) - std::pow(_c21 + _c03, 2));
	_I6 = (_c20 - _c02) * (std::pow(_c30 + _c12, 2) - std::pow(_c21 + _c03, 2)) + 4 * _c11 * (_c30 + _c12) * (_c21 + _c03);

	_I1 /= std::pow(_M00, 2);
	_I2 /= std::pow(_M00, 4);
	_I3 /= std::pow(_M00, 5);
	_I4 /= std::pow(_M00, 5);
	_I5 /= std::pow(_M00, 10);
	_I6 /= std::pow(_M00, 7);

	/*std::cout << std::to_string(_I1) << std::endl;
	std::cout << std::to_string(_I2) << std::endl;
	std::cout << std::to_string(_I3) << std::endl;
	std::cout << std::to_string(_I4) << std::endl;
	std::cout << std::to_string(_I5) << std::endl;
	std::cout << std::to_string(_I6) << std::endl;*/

	_gravityCenter = std::pair<float, float>(_M01 / _M00, _M10 / _M00);

	/*std::cout << _gravityCenter.first << std::endl;*/
}

std::pair<float, float> MyCV::SegmentInfo::getGravityCenter()
{
	return _gravityCenter;
}

bool MyCV::SegmentInfo::isLetterL()
{
	if (_I1 < 0.232904 * 0.7 || _I1 > 0.402047 * 1.2)
		return false;
	if (_I2 < 0.001229 * 0.7 || _I2 > 0.109387 * 1.2)
		return false;
	if (_I3 < 0.002978 * 0.7 || _I3 > 0.01445 * 1.2)
		return false;
	if (_I4 < 0.000188 * 0.7 || _I4 > 0.006359 * 1.2)
		return false;
	if (_I5 < -0.000001 * 1.3 || _I5 > 0.00006 * 1.2)
		return false;
	if (_I6 < -0.000068 * 1.3 || _I6 > 0.002069 * 1.2)
		return false;
	return true;
}

bool MyCV::SegmentInfo::isLetterI()
{
	if (_I1 < 0.166381 * 0.8 || _I1 > 0.19249 * 1.2)
		return false;
	if (_I2 < 0.000004 * 0.8 || _I2 > 0.003012 * 1.2)
		return false;
	if (_I3 < 0.00001 * 0.8 || _I3 > 0.000852 * 1.2)
		return false;
	if (_I4 < -0.000001 * 1.2 || _I4 > 0.000053 * 1.2)
		return false;
	if (_I5 < -0.000001 * 1.2 || _I5 > 0.000001 * 1.2)
		return false;
	if (_I6 < -0.000001 * 1.2 || _I6 > 0.000001 * 1.2)
		return false;
	return true;
}

bool MyCV::SegmentInfo::isLetterD()
{
	if (_I1 < 0.197702

				  * 0.8 ||
		_I1 > 0.227674

				  * 1.2)
		return false;
	if (_I2 < 0.000273

				  * 0.8 ||
		_I2 > 0.007914

				  * 1.2)
		return false;
	if (_I3 < 0.00028

				  * 0.8 ||
		_I3 > 0.000478

				  * 1.2)
		return false;
	if (_I4 < -0.000001 * 1.2 || _I4 > 0.000012

										   * 1.2)
		return false;
	if (_I5 < -0.000001 * 1.2 || _I5 > 0.003797

										   * 1.2)
		return false;
	if (_I6 < -0.000001 * 1.2 || _I6 > 0.000001

										   * 1.2)
		return false;
	return true;
}

bool MyCV::SegmentInfo::isYellowCircle()
{
	if (_I1 < 0.210226 * 0.8 || _I1 > 0.252874 * 1.2)
		return false;
	if (_I2 < 0.00012

				  * 0.8 ||
		_I2 > 0.021667 * 1.2)
		return false;
	if (_I3 < -0.000001 * 1.2 || _I3 > 0.000058 * 1.2)
		return false;
	if (_I4 < -0.000001 * 1.2 || _I4 > 0.000017 * 1.2)
		return false;
	if (_I5 < -0.000001 * 1.2 || _I5 > 0.000001 * 1.2)
		return false;
	if (_I6 < -0.000001 * 1.2 || _I6 > 0.000001 * 1.2)
		return false;
	return true;
}

bool MyCV::SegmentInfo::isRedDot()
{
	if (_I1 < 0.159161 * 0.8 || _I1 > 0.195348 * 1.2)
		return false;
	if (_I2 < -0.000001 * 1.2 || _I2 > 0.012819 * 1.2)
		return false;
	if (_I3 < -0.000001 * 1.2 || _I3 > 0.000105 * 1.2)
		return false;
	if (_I4 < -0.000001 * 1.2 || _I4 > 0.00001 * 1.2)
		return false;
	if (_I5 < -0.000001 * 1.2 || _I5 > 0.000001 * 1.2)
		return false;
	if (_I6 < -0.000001 * 1.2 || _I6 > 0.000001 * 1.2)
		return false;
	return true;
}

bool MyCV::SegmentInfo::isGravityCenterInsideBB(SegmentInfo seg)
{
	auto gc = seg.getGravityCenter();
	return boundigBox.isInside(gc);
}

double MyCV::SegmentInfo::getDistanceToCenter(SegmentInfo seg)
{
	auto segCog = seg.getGravityCenter();
	return std::sqrt(std::pow(_gravityCenter.second - segCog.second, 2) + std::pow(_gravityCenter.first - segCog.first, 2));
}

std::pair<float, float> MyCV::SegmentInfo::computeGravityCenter()
{
	int sumX = 0, sumY = 0;
	for (auto i = 0; i < _s.size(); i++)
	{
		sumX = _s[i].second;
		sumY = _s[i].first;
	}
	std::pair<float, float> result(sumY / _s.size(), sumX / _s.size());
	return result;
}

double MyCV::SegmentInfo::computeGM(int p, int q)
{
	double sum = 0.0;
	for (auto i = 0; i < _s.size(); i++)
	{
		sum += std::pow(_s[i].second, p) * std::pow(_s[i].first, q);
	}
	return sum;
}

double MyCV::SegmentInfo::computeCGM(int p, int q)
{
	double sum = 0.0, _xCentroid = _M10 / _M00, _yCentroid = _M01 / _M00;
	for (auto i = 0; i < _s.size(); i++)
	{
		sum += std::pow(_s[i].second - _xCentroid, p) * std::pow(_s[i].first - _yCentroid, q);
	}
	return sum;
}