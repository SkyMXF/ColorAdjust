#pragma once
#include <iostream>
#include <iomanip>
using namespace std;

typedef struct Mat {
	int row;
	int col;
	double *data;
	Mat() {
		data = NULL;
	}

	Mat(int r, int c) {
		Create(r, c);
	}
	
	~Mat() {
		if (data) delete[]data; 
		data = NULL;
	}

	void Create(int r, int c) {
	//根据行数r，列数c，申请空间创建矩阵
		row = r;
		col = c;
		data = new double[row * col];
		for (int i = 0; i < row * col; i++)
			data[i] = 0.0;
	}
};

class MatrixTool
{
public:
	MatrixTool() {}
	~MatrixTool() {}

	void matMulti(Mat &result, Mat &matA, Mat &matB);
	//矩阵乘法,result需在调用前使用合适的行数列数Create
	void matTrans(Mat &result, Mat &matA);
	//矩阵转置,result需在调用前使用合适的行数列数Create
	
	bool invGauss(Mat &result, Mat &matA);
	//采用部分主元的高斯消去法求方阵A的逆矩阵
};

