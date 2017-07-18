#pragma once

#include <cmath>
#include "stdafx.h"
#include "labRGBconvert.h"

void lab2RGB(LabColor &lab, RGBColor &rgb)
//LabToRGB
//L = [0,8.99144]
//a = [-13.1977,13.1989]
//b = [-12.471,12.4584]
{
	double fY, fX, fZ;
	fY = (lab.L + 16) / 116.0;
	fX = lab.a / 500.0 + fY;
	fZ = fY - lab.b / 200.0;

	double Xn = 95.047, Yn = 100.0, Zn = 108.883;
	double X, Y, Z;
	if (fX > 6 / 29.0)
		X = Xn * fX * fX * fX;
	else
		X = Xn * (fX - 4 / 29.0) * 3 * 6 * 6 / 29.0 / 29.0;
	if (fY > 6 / 29.0)
		Y = Yn * fY * fY * fY;
	else
		Y = Yn * (fY - 4 / 29.0) * 3 * 6 * 6 / 29.0 / 29.0;
	if (fZ > 6 / 29.0)
		Z = Zn * fZ * fZ * fZ;
	else
		Z = Zn * (fZ - 4 / 29.0) * 3 * 6 * 6 / 29.0 / 29.0;

	double r = 3.2406 * X - 1.5372 * Y - 0.4986 * Z;
	double g = -0.9689 * X + 1.8758 * Y + 0.0415 * Z;
	double b = 0.0557 * X - 0.2040 * Y + 1.0570 * Z;

	if (r > 1) r = 1.0;
	if (r < 0) r = 0.0;
	if (g > 1) g = 1.0;
	if (g < 0) g = 0.0;
	if (b > 1) b = 1.0;
	if (b < 0) b = 0.0;

	rgb.R = (int)(r * 255 + 0.5);
	rgb.G = (int)(g * 255 + 0.5);
	rgb.B = (int)(b * 255 + 0.5);
}

void rgb2lab(RGBColor &rgb, LabColor &lab)
//RGBtoLab
{
	double r = rgb.R / 255.0;
	double g = rgb.G / 255.0;
	double b = rgb.B / 255.0;

	double X = r * 0.4124 + g * 0.3576 + b * 0.1805;
	double Y = r * 0.2126 + g * 0.7152 + b * 0.0722;
	double Z = r * 0.0193 + g * 0.1192 + b * 0.9505;

	double Xn = 95.047, Yn = 100.0, Zn = 108.883;

	double fX, fY, fZ;

	if (X / Xn > pow(6 / 29.0, 3))
		fX = pow(X / Xn, 1 / 3.0);
	else
		fX = 1 / 3.0 * 29 * 29 / 6.0 / 6.0*X / Xn + 4 / 29.0;
	if (Y / Yn > pow(6 / 29.0, 3))
		fY = pow(Y / Yn, 1 / 3.0);
	else
		fY = 1 / 3.0 * 29 * 29 / 6.0 / 6.0*Y / Yn + 4 / 29.0;
	if (Z / Zn > pow(6 / 29.0, 3))
		fZ = pow(Z / Zn, 1 / 3.0);
	else
		fZ = 1 / 3.0 * 29 * 29 / 6.0 / 6.0*Z / Zn + 4 / 29.0;

	lab.L = 116 * (fY) - 16;
	lab.a = 500 * (fX - fY);
	lab.b = 200 * (fY - fZ);
}

void lab2dRGB(LabColor & lab, dRGBColor & rgb)
{
	double fY, fX, fZ;
	fY = (lab.L + 16) / 116.0;
	fX = lab.a / 500.0 + fY;
	fZ = fY - lab.b / 200.0;

	double Xn = 95.047, Yn = 100.0, Zn = 108.883;
	double X, Y, Z;
	if (fX > 6 / 29.0)
		X = Xn * fX * fX * fX;
	else
		X = Xn * (fX - 4 / 29.0) * 3 * 6 * 6 / 29.0 / 29.0;
	if (fY > 6 / 29.0)
		Y = Yn * fY * fY * fY;
	else
		Y = Yn * (fY - 4 / 29.0) * 3 * 6 * 6 / 29.0 / 29.0;
	if (fZ > 6 / 29.0)
		Z = Zn * fZ * fZ * fZ;
	else
		Z = Zn * (fZ - 4 / 29.0) * 3 * 6 * 6 / 29.0 / 29.0;

	double r = 3.2406 * X - 1.5372 * Y - 0.4986 * Z;
	double g = -0.9689 * X + 1.8758 * Y + 0.0415 * Z;
	double b = 0.0557 * X - 0.2040 * Y + 1.0570 * Z;

	if (r > 1) r = 1.0;
	if (r < 0) r = 0.0;
	if (g > 1) g = 1.0;
	if (g < 0) g = 0.0;
	if (b > 1) b = 1.0;
	if (b < 0) b = 0.0;

	rgb.R = r * 255;
	rgb.G = g * 255;
	rgb.B = b * 255;
}
