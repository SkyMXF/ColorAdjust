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
	//��������r������c������ռ䴴������
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
	//����˷�,result���ڵ���ǰʹ�ú��ʵ���������Create
	void matTrans(Mat &result, Mat &matA);
	//����ת��,result���ڵ���ǰʹ�ú��ʵ���������Create
	
	bool invGauss(Mat &result, Mat &matA);
	//���ò�����Ԫ�ĸ�˹��ȥ������A�������
};
