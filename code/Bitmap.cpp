#include <iostream>
#include <fstream>
using namespace std;

#include "stdafx.h"
#include "Bitmap.h"

Bitmap::Bitmap()
{
	//文件头
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfSize = sizeof(MYBITMAPFILEHEADER) + sizeof(MYBITMAPINFOHEADER);
	fileHeader.bfOffBits = sizeof(MYBITMAPFILEHEADER) + sizeof(MYBITMAPINFOHEADER);

	//bmp文件信息
	infoHeader.biSize = sizeof(MYBITMAPINFOHEADER);
	infoHeader.biHeight = 1;
	infoHeader.biWidth = 1;
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 24;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = 0;
	infoHeader.biXPelsPerMeter = 0;
	infoHeader.biYPelsPerMeter = 0;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;

	//调色板(默认不使用)
	paletteentry = 0;

	//图像数据(默认为空)
	buffer = NULL;
}

Bitmap::Bitmap(Bitmap &copy)
{
	//文件头
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = copy.fileHeader.bfReserved1;
	fileHeader.bfReserved2 = copy.fileHeader.bfReserved2;
	fileHeader.bfSize = copy.fileHeader.bfSize;
	fileHeader.bfOffBits = copy.fileHeader.bfOffBits;

	//bmp文件信息
	infoHeader.biSize = copy.infoHeader.biSize;
	infoHeader.biHeight = copy.infoHeader.biHeight;
	infoHeader.biWidth = copy.infoHeader.biWidth;
	infoHeader.biPlanes = copy.infoHeader.biPlanes;
	infoHeader.biBitCount = copy.infoHeader.biBitCount;
	infoHeader.biCompression = copy.infoHeader.biCompression;
	infoHeader.biSizeImage = copy.infoHeader.biSizeImage;
	infoHeader.biXPelsPerMeter = copy.infoHeader.biXPelsPerMeter;
	infoHeader.biYPelsPerMeter = copy.infoHeader.biYPelsPerMeter;
	infoHeader.biClrUsed = copy.infoHeader.biClrUsed;
	infoHeader.biClrImportant = copy.infoHeader.biClrImportant;

	//调色板
	int paletteentrySize = copy.fileHeader.bfOffBits - sizeof(MYBITMAPFILEHEADER) - sizeof(MYBITMAPINFOHEADER);
	paletteentry = new MYPALETTEENTRY[paletteentrySize / 4];		//一个调色板变量占用4个字节
	for (int i = 0; i < paletteentrySize / 4; i++)
	{
		paletteentry[i] = copy.paletteentry[i];
	}

	//图像数据
	if (buffer) delete[]buffer;
	int bufferSize = copy.fileHeader.bfSize - copy.fileHeader.bfOffBits;
	buffer = new BYTE[bufferSize];
	for (int j = 0; j < bufferSize; j++)
	{
		buffer[j] = copy.buffer[j];
	}
}

Bitmap::~Bitmap()
{
	if (paletteentry)
	{
		delete[]paletteentry;
		paletteentry = NULL;
	}

	if (buffer) {
		delete[]buffer;
		buffer = NULL;
	}
}

bool Bitmap::readFile(char *fileName)
{
	FILE *input = fopen(fileName, "rb");
	if (!input)
	{
		return false;			//返回false：未成功打开文件
	}

	fread(&fileHeader, 1, sizeof(MYBITMAPFILEHEADER), input);			//文件头
	fread(&infoHeader, 1, sizeof(MYBITMAPINFOHEADER), input);			//文件信息
	if (fileHeader.bfOffBits != sizeof(MYBITMAPFILEHEADER) + sizeof(MYBITMAPINFOHEADER))		//可能包含的调色板信息
	{
		int palSize = fileHeader.bfOffBits - sizeof(MYBITMAPFILEHEADER) - sizeof(MYBITMAPINFOHEADER);
		paletteentry = new MYPALETTEENTRY[palSize];
		fread(paletteentry, 1, palSize, input);
	}

	//读图像数据
	int height = infoHeader.biHeight;			//图片高度
	int width = infoHeader.biWidth;				//图片长度
	int size = height * width * 3;				//图像数据字节数

	if (buffer) delete[]buffer;
	buffer = new BYTE[size];
	fread(buffer, 1, size, input);
	fclose(input);
	return true;
}

bool Bitmap::writeFile(char *fileName)
{
	FILE *output = fopen(fileName, "wb");
	if (!output)
	{
		return false;
	}

	fwrite(&fileHeader, 1, sizeof(MYBITMAPFILEHEADER), output);
	fwrite(&infoHeader, 1, sizeof(MYBITMAPINFOHEADER), output);
	fwrite(buffer, 1, infoHeader.biHeight*infoHeader.biWidth * 3, output);
	fclose(output);

	return true;
}