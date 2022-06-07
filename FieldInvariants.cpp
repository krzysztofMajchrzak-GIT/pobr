#include "FielndInvariants.h"

using namespace std;
FieldInvariants::FieldInvariants(vector<pair<int, int> >  ff) :field(ff)
{
    
    /*** Invariants calculation ***/
    c11 = centralM(1,1);
    c02 = centralM(0,2);
    c20 = centralM(2,0);
    c12 = centralM(1,2);
    c21 = centralM(2,1);
    c30 = centralM(3,0);
    c03 = centralM(0,3);
    M00 = M(0, 0);
    M10 = M(1, 0);
    M01 = M(0, 1);

    I1 = c20 + c02; I1 /= pow(M00, 2);
	I2 = pow(c20 - c02, 2) + 4 * pow(c11, 2); I2 /= pow(M00, 4);
	I3 = pow(c30 - 3 * c12, 2) + pow(3 * c21 - c03, 2); I3 /= pow(M00, 5);
	I4 = pow(c30 + c12, 2) + pow(c21 + c03, 2); I4 /= pow(M00, 5);
	I5 = (c30 - 3 * c12) * (c30 + c12) *
        (pow(c30 + c12, 2) - 3 * pow(c21 + c03, 2)) + 
        (3 * c21 - c03) * (c21 + c03) * (3 * pow(c30 + c12, 2) - 
        pow(c21 + c03, 2)); I5 /= pow(M00, 10);
	I6 = (c20 - c02) * (pow(c30 + c12, 2) - pow(c21 + c03, 2)) + 
    4 * c11 * (c30 + c12) * (c21 + c03); I6 /= pow(M00, 7);

    /*** gravity center ***/
    gc = std::pair<double, double>(M01 / M00, M10 / M00);
    
}
double FieldInvariants::M(int x, int y)
{
    double ret = 0;
	for (auto i = 0; i < field.size(); i++) {
		ret += pow(field[i].second, y) * pow(field[i].first, y);
	}
	return ret;
}
double FieldInvariants::centralM(int x, int y)
{
    double ret = 0.0, xcenter = M10 / M00, ycenter = M01 / M00;
	for (auto i = 0; i < field.size(); i++) {
		ret += pow(field[i].second - xcenter, x) * pow(field[i].first - ycenter, y);
	}
	return ret;
}
bool FieldInvariants::findCircle() // Pablo check
{
    if (  !(I1 < 0.210226 * 0.8 || I1 > 0.252874 * 1.2) 
       && !(I2 < 0.00012 * 0.8 || I2 >  0.021667 * 1.2)
       && !(I3 < -0.000001 * 1.2 || I3 > 0.000058 * 1.2)
       && !(I4 < -0.000001 * 1.2 || I4 > 0.000017 * 1.2)
       && !(I5 < -0.000001 * 1.2 || I5 > 0.000001 * 1.2)
       && !(I6 < -0.000001 * 1.2 || I6 > 0.000001 * 1.2)
    )
        return true;
    else
		return false;
}

bool FieldInvariants::findD()
{
    if (  !(I1 < 0.197702 * 0.8 || I1 > 0.227674 * 1.2) 
       && !(I2 < 0.000273 * 0.8 || I2 > 0.007914 * 1.2)
       && !(I3 < 0.00028 * 0.8 || I3 > 0.000478 * 1.2)
       && !(I4 < -0.000001 * 1.2 || I4 > 0.000012 * 1.2)
       && !(I5 < -0.000001 * 1.2 || I5 > 0.003797 * 1.2)
       && !(I6 < -0.000001 * 1.2 || I6 > 0.000001 * 1.2)
    )
        return true;
    else
		return false;
}

bool FieldInvariants::findL()
{
    if (  !(I1 < 0.232904 * 0.7 || I1 > 0.402047 * 1.2) 
       && !(I2 < 0.001229 * 0.7 || I2 > 0.109387 * 1.2)
       && !(I3 < 0.002978 * 0.7 || I3 > 0.01445 * 1.2)
       && !(I4 < 0.000188 * 0.7 || I4 >0.006359 * 1.2)
       && !(I5 < -0.000001 * 1.3 || I5 >0.00006 * 1.2)
       && !(I6 < -0.000068 * 1.3 || I6 >  0.002069 * 1.2)
    )
        return true;
    else
		return false;
}

bool FieldInvariants::findI()
{
    if (  !(I1 < 0.166381 * 0.8 || I1 > 0.19249 * 1.2) 
       && !(I2 < 0.000004 * 0.8 || I2 > 0.003012 * 1.2)
       && !(I3 < 0.00001 * 0.8 || I3 > 0.000852 * 1.2)
       && !(I4 < -0.000001 * 1.2 || I4 >  0.000053 * 1.2)
       && !(I5 < -0.000001 * 1.2 || I5 > 0.000001 * 1.2)
       && !(I6 < -0.000001 * 1.2 || I6 >  0.000001 * 1.2)
    )
        return true;
    else
		return false;
}

bool FieldInvariants::findDot()
{
    if (  !(I1 < 0.159161 * 0.8 || I1 > 0.195348 * 1.2) 
       && !(I2 < -0.000001 * 1.2 || I2 > 0.012819 * 1.2)
       && !(I3 < -0.000001 * 1.2 || I3 > 0.000105 * 1.2)
       && !(I4 < -0.000001 * 1.2 || I4 >  0.00001 * 1.2)
       && !(I5 < -0.000001 * 1.2 || I5 > 0.000001 * 1.2)
       && !(I6 < -0.000001 * 1.2 || I6 > 0.000001 * 1.2)
    )
        return true;
    else
		return false;
}

pair<double, double> FieldInvariants::getgc()
{
	return gc;
}

bool FieldInvariants::isgcInBB(FieldInvariants field_)
{
	auto g = field_.getgc();
	return boundigBox.isIn(g);
}
// TODO !!!!!!!!!!!!!

double FieldInvariants::rToCenter(FieldInvariants field_)
{
	auto fgc = field_.getgc();
	return sqrt(pow(gc.second - fgc.second, 2) + pow(gc.first - fgc.first, 2));
}

pair<double, double> FieldInvariants::calculategc()
{
	int sumx = 0;
    int sumy = 0;
	for (auto x = 0; x < field.size(); x++) {
		sumx = field[x].second; sumy = field[x].first;
	}
	pair<double, double> result(sumy / field.size(), sumx / field.size());
	return result;
}






