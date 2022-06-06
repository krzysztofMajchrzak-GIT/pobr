#pragma once

#include <iostream>
#include <vector>
#include <math.h>
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
class FieldInvariants
{
    
    public:
        FieldInvariants(std::vector<std::pair<int, int> > ff); //done
        double M(int x, int y); //done
        double centralM(int x, int y); //done
        bool findCircle(); //done
        bool findD(); //done
        bool findL(); //done
        bool findI(); //done
        bool findDot(); //done
        BoundingBox boundigBox;// done

        std::vector<std::pair<int, int> >  field; // done
        std::pair<double, double> calculategc();
        std::pair<double, double> getgc(); //done
        bool isgcInBB(FieldInvariants f); //done
		double rToCenter(FieldInvariants f);
    
    private:
        double M00;
        double M01;
        double M10;
        double I1;
        double I2;
        double I3;
        double I4;
        double I5;
        double I6;
        double c11;
        double c02;
        double c20;
        double c12;
        double c21;
        double c30;
        double c03;
        std::pair<double, double> gc;


};


class PotentialHit
{
	std::vector<FieldInvariants> outlyingLetters;
	FieldInvariants secondLetterTop;
	FieldInvariants secondLetterBottom;
	FieldInvariants thirdLetter;
	FieldInvariants background;
	bool checkHit();
};
