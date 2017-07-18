#pragma once
/******************************************************
一个bmp文件由4部分组成：
1.位图文件头BITMAPFILEHEADER
2.位图信息段BITMAPINFOHEADER
3.调色板信息PALETTEENTRY(可能没有)
4.位图数据(通过指针查询索引)
******************************************************/

#pragma pack(2)

typedef unsigned char BYTE;		//1字节
typedef unsigned short WORD;	//2字节
typedef unsigned long DWORD;	//4字节
typedef long LONG;				//4字节，有符号

typedef struct {
	WORD	bfType;				//文件类型标记，该值必须为0x4D42(即字符"BM")
	DWORD	bfSize;				//文件大小，以字节为单位
	WORD	bfReserved1;		//保留位，必须设置为0
	WORD	bfReserved2;		//保留位，必须设置为0
	DWORD	bfOffBits;			//声明从文件头开始到实际图像数据之间的字节的偏移量(文件头长度+信息长度+调色板长度)
} MYBITMAPFILEHEADER;

typedef struct {
	DWORD	biSize;			//声明BITMAPINFOHEADER所需的字节数
	LONG	biWidth;		//图像宽度
	LONG	biHeight;		//图像高度
	WORD	biPlanes;		//bmp图片的所属平面，一般为1
	WORD	biBitCount;		//每像素位数，值为1,4,8,16,24,32
	DWORD	biCompression;		//图像数据压缩类型
								//BI_RGB：没有压缩
								//BI_RLE8：每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；
								//BI_RLE4：每个象素4比特的RLE压缩编码，压缩格式由2字节组成
								//BI_BITFIELDS：每个象素的比特由指定的掩码决定。
								//BI_JPEG：JPEG格式

	DWORD	biSizeImage;		//图像大小，格式为BI_RGB时，可以设为0
	LONG	biXPelsPerMeter;	//水平分辨率，单位像素/米
	LONG	biYPelsPerMeter;	//垂直分辨率，单位像素/米
	DWORD	biClrUsed;			//实际使用的彩色表中的颜色索引数(设为0则说明使用所有调色板项)
	DWORD	biClrImportant;		//说明对图像显示有重要影响的颜色索引数目，如果为0，表示都重要
} MYBITMAPINFOHEADER;

typedef struct {				//调色板信息
	BYTE peRed;
	BYTE peGreen;
	BYTE peBlue;
	BYTE peFlags;
} MYPALETTEENTRY;

class Bitmap {
public:
	MYBITMAPFILEHEADER	fileHeader;
	MYBITMAPINFOHEADER	infoHeader;
	MYPALETTEENTRY		*paletteentry;
	BYTE				*buffer;

	Bitmap();
	Bitmap(Bitmap&);
	~Bitmap();

	bool readFile(char*);
	bool writeFile(char*);
};