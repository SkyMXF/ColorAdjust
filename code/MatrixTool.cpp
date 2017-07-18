#include <iostream>
#include <cmath>
#include "stdafx.h"
#include "MatrixTool.h"

void MatrixTool::matMulti(Mat & result, Mat & matA, Mat & matB)
//矩阵乘法,result需在调用前使用合适的行数列数Create
{
	int i, j, k;
	for (i = 0; i < result.row; i++)
	{
		for (j = 0; j < result.col; j++)
		{
			result.data[i * result.col + j] = 0.0;		//因为需要累加,因此将result初始化为0矩阵
		}
	}

	for (int i = 0; i < matA.row; i++)
	{
		for (int j = 0; j < matB.col; j++)
		{
			for (int k = 0; k < matA.col; k++)
			{
				result.data[i * result.col + j] += matA.data[i * matA.col + k] * matB.data[k * matB.col + j];
				//result第i行j列 = A中第i行k列 * B中第k行j列
			}
		}
	}
}

void MatrixTool::matTrans(Mat & result, Mat & matA)
//矩阵转置,result需在调用前使用合适的行数列数Create
{
	for (int i = 0; i < matA.row; i++)
	{
		for (int j = 0; j < matA.col; j++)
		{
			result.data[j * result.col + i] = matA.data[i * matA.col + j];
			//result第j行i列 = A中第i行j列
		}
	}
}

bool MatrixTool::invGauss(Mat & result, Mat & matA)
//采用部分主元的高斯消去法求方阵A的逆矩阵
{
	if (matA.row != matA.col)
		return false;				//不是方阵

	int i, j, k;
	double max, temp;
	int n = matA.row;
	Mat matT(n, n);					//临时矩阵

	for (i = 0; i < n; i++)			//将A矩阵存放在临时矩阵t[n][n]中
	{
		for (j = 0; j < n; j++)
		{
			matT.data[i * n + j] = matA.data[i * n + j];
		}
	}

	//初始化result矩阵为单位阵
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			result.data[i * n + j] = (i == j) ? 1.0 : 0;
		}
	}

	for (i = 0; i < n; i++)
	{
		//寻找主元  
		max = matT.data[i * n + i];
		k = i;
		for (j = i + 1; j < n; j++)
		{
			if (fabs(matT.data[j * n + i]) > fabs(max))
			{
				max = matT.data[j * n + i];
				k = j;
			}
		}
		//如果主元所在行不是第i行，进行行交换  
		if (k != i)
		{
			for (j = 0; j < n; j++)
			{
				temp = matT.data[i * n + j];
				matT.data[i * n + j] = matT.data[k * n + j];
				matT.data[k * n + j] = temp;
				//B伴随交换  
				temp = result.data[i * n + j];
				result.data[i * n + j] = result.data[k * n + j];
				result.data[k * n + j] = temp;
			}
		}
		//判断主元是否为0, 若是, 则矩阵A不是满秩矩阵,不存在逆矩阵  
		if (matT.data[i * n + i] == 0)
		{
			return false;				//no inverse matrix
		}
		//消去A的第i列除去i行以外的各行元素  
		temp = matT.data[i * n + i];
		for (j = 0; j < n; j++)
		{
			matT.data[i * n + j] = matT.data[i * n + j] / temp;				//主对角线上的元素变为1  
			result.data[i * n + j] = result.data[i * n + j] / temp;			//伴随计算  
		}
		for (j = 0; j < n; j++)        //第0行->第n行  
		{
			if (j != i)                //不是第i行  
			{
				temp = matT.data[j * n + i];
				for (k = 0; k < n; k++)        //第j行元素 - i行元素*j列i行元素  
				{
					matT.data[j * n + k] = matT.data[j * n + k] - matT.data[i * n + k] * temp;
					result.data[j * n + k] = result.data[j * n + k] - result.data[i * n + k] * temp;
				}
			}
		}
	}
	return true;
}
