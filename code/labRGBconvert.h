#pragma once

#include "Bitmap.h"

typedef struct LabColor {
	double L;
	double a;
	double b;
};

typedef struct RGBColor {
	BYTE R;
	BYTE G;
	BYTE B;
};

typedef struct dRGBColor {
	double R;
	double G;
	double B;
};

void lab2RGB(LabColor &lab, RGBColor &rgb);
//lab2RGB
//L = [0,8.99144]
//a = [-13.1977,13.1989]
//b = [-12.471,12.4584]

void rgb2lab(RGBColor &rgb, LabColor &lab);
//RGB2Lab

void lab2dRGB(LabColor &lab, dRGBColor &rgb);
//lab转RGB，其中R,G,B的值为double类型