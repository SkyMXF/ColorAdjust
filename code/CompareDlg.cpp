
// CompareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Compare.h"
#include "CompareDlg.h"
#include "afxdialogex.h"
#include "labRGBconvert.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCompareDlg 对话框



CCompareDlg::CCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_COMPARE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CCompareDlg::~CCompareDlg()
{
	if (m_inputBmp.m_hObject != NULL)
		m_inputBmp.DeleteObject();
	if (m_standardBmp.m_hObject != NULL)
		m_standardBmp.DeleteObject();

	CDialogEx::~CDialogEx();
}

void CCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_LSlider);
	DDX_Control(pDX, IDC_EDIT10, LValueEdit);
	DDX_Control(pDX, IDC_EDIT1, colorMatrix[0][0]);
	DDX_Control(pDX, IDC_EDIT2, colorMatrix[0][1]);
	DDX_Control(pDX, IDC_EDIT3, colorMatrix[0][2]);
	DDX_Control(pDX, IDC_EDIT4, colorMatrix[1][0]);
	DDX_Control(pDX, IDC_EDIT6, colorMatrix[1][1]);
	DDX_Control(pDX, IDC_EDIT8, colorMatrix[1][2]);
	DDX_Control(pDX, IDC_EDIT5, colorMatrix[2][0]);
	DDX_Control(pDX, IDC_EDIT7, colorMatrix[2][1]);
	DDX_Control(pDX, IDC_EDIT9, colorMatrix[2][2]);
}

BEGIN_MESSAGE_MAP(CCompareDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, &CCompareDlg::OnBnClickedButton1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CCompareDlg 消息处理程序

BOOL CCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//各变量初始化
	m_LValue = 50;
	m_inputChosen = false;
	m_standardRePaint = true;
	m_inputRectPaint = false;
	m_standardRectPaint = false;
	m_standardCirclePaint = false;
	m_LeftPicLButtonDown = false;
	m_RightPicLButtonDown = false;
	m_choosingCircleRow = -1;
	m_choosingCircleCol = -1;
	m_convertMat.Create(3, 3);
	
	//初始化m_convertMat为单位矩阵
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			m_convertMat.data[i * 3 + j] = (i == j ? 1 : 0);
		}
	}

	//显示单位矩阵
	CString cstr;						
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			cstr.Format(_T("%.4f"), m_convertMat.data[i * 3 + j]);
			colorMatrix[i][j].SetWindowTextA(cstr);
		}
	}

	int standardValue[72] =
	{//24个标准颜色RGB值
		116,81,39,
		197,152,95,
		96,124,193,
		87,109,52,
		133,130,204,
		101,191,211,
		218,126,0,
		75,91,219,
		197,88,46,
		94,58,117,
		160,190,4,
		228,164,0,
		48,59,208,
		68,150,74,
		179,47,0,
		235,201,0,
		190,84,129,
		0,136,255,
		243,244,243,
		202,202,202,
		162,162,162,
		123,123,123,
		85,85,85,
		49,50,49
	};

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			m_standardRGB[i][j].R = standardValue[6 * i * 3 + j * 3 + 0];
			m_standardRGB[i][j].G = standardValue[6 * i * 3 + j * 3 + 1];
			m_standardRGB[i][j].B = standardValue[6 * i * 3 + j * 3 + 2];
		}
	}

	// L分量Slider初始化
	m_LSlider.SetRange(0, 100);		// 滑动范围[0,100]
	m_LSlider.SetTicFreq(10);		// 每10个单位画一个刻度
	m_LSlider.SetPos(50);
	LValueEdit.SetWindowTextA(_T("50"));

	//获取控件信息
	CRect rect;
	GetDlgItem(IDC_STATIC2)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	//根据L分量值,将标准图像写入图像文件
	
	int height = rect.Height();								//标记为-60,100
	int width = rect.Width();								//标记为-60,80
	double dA = (13.1989 + 13.1977) / width;		//横轴变化量
	double dB = (12.4584 + 12.471) / height;		//纵轴变化量

	RGBColor rgb;
	LabColor lab;
	int size = height * width * 3;

	//设置图像基本信息
	Bitmap pic;
	pic.fileHeader.bfSize = sizeof(MYBITMAPFILEHEADER) + sizeof(MYBITMAPINFOHEADER) + size;
	pic.infoHeader.biHeight = height;
	pic.infoHeader.biWidth = width;
	if (pic.buffer != NULL) delete[]pic.buffer;
	pic.buffer = new BYTE[size];
	//导入图像数据
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			lab.L = m_LValue / 100.0 * 8.99144;
			lab.a = -13.1977 + j * dA;
			lab.b = -12.4710 + i * dB;
			lab2RGB(lab, rgb);
			pic.buffer[i * width * 3 + j * 3 + 0] = rgb.B;
			pic.buffer[i * width * 3 + j * 3 + 1] = rgb.G;
			pic.buffer[i * width * 3 + j * 3 + 2] = rgb.R;
		}
	}
	//写入文件
	FILE *output = fopen("standard.bmp", "wb");
	if (!output)
	{
		return false;
	}
	fwrite(&pic.fileHeader, sizeof(MYBITMAPFILEHEADER), 1, output);
	fwrite(&pic.infoHeader, sizeof(MYBITMAPINFOHEADER), 1, output);
	fwrite(pic.buffer, size, 1, output);
	fclose(output);

	//加载图像信息至内存
	if (m_standardBmp.m_hObject != NULL)//判断
		m_standardBmp.DeleteObject();
	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		"standard.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hbmp == NULL)
		return FALSE;
	m_standardBmp.Attach(hbmp);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCompareDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	/*
	CDC *pDC = (CDC*)this;
	CRect dlgRect;
	GetWindowRect(dlgRect);

	CDC memDC;				//显示设备对象
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(NULL);
	memBitmap.CreateCompatibleBitmap(pDC, dlgRect.Width(), dlgRect.Height());
	CBitmap *pOldBit = memDC.SelectObject(&memBitmap);		//保存原图
	memDC.FillSolidRect(0, 0, dlgRect.Width(), dlgRect.Height(), RGB(255, 255, 255));*/
	if (m_standardRePaint)				//重画右边图像
	{
		m_standardRePaint = false;
		
		//获取控件信息
		CRect rcclient;
		GetDlgItem(IDC_STATIC2)->GetClientRect(&rcclient);
		int height = rcclient.Height();
		int width = rcclient.Width();
		int size = height * width * 3;
		BYTE *dataPointer = new BYTE[size];
		int i, j;
		double dA = (13.1989 + 13.1977) / width;		//横轴变化量
		double dB = (12.4584 + 12.471) / height;		//纵轴变化量
		RGBColor rgb;
		LabColor lab;
		
		//根据当前L的值，重写图像数据
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				lab.L = m_LValue / 100.0 * 8.99144;
				lab.a = -13.1977 + j * dA;
				lab.b = 12.4584 - i * dB;
				lab2RGB(lab, rgb);
				dataPointer[i * width * 3 + j * 3 + 0] = rgb.B;
				dataPointer[i * width * 3 + j * 3 + 1] = rgb.G;
				dataPointer[i * width * 3 + j * 3 + 2] = rgb.R;
			}
		}
		m_standardBmp.SetBitmapBits(size, dataPointer);
		//m_standardBmp.CreateBitmap(width, height, 1, 24, dataPointer);
		delete[]dataPointer;
		
		CDC *pDC = GetDlgItem(IDC_STATIC2)->GetDC();

		CDC memDC;				//显示设备对象
		CBitmap memBitmap;
		memDC.CreateCompatibleDC(pDC);
		memBitmap.CreateCompatibleBitmap(pDC, width, height);
		memDC.SelectObject(&memBitmap);
		//memDC.FillSolidRect(0, 0, width, height, RGB(255, 255, 255));

		CPen pen;

		CBrush brush;
		brush.CreatePatternBrush(&m_standardBmp);
		memDC.SelectObject(&brush);
		memDC.Rectangle(rcclient);
		//memDC.FillRect(rcclient, &brush);
		//memDC.BitBlt(0, 0, width, height,
		//&memDC, rcclient.left, rcclient.top, SRCPAINT);
		brush.DeleteObject();

		//绘制方块(标准色位置)
		if (m_standardRectPaint)
		{
			lab.L = 100 - m_LValue;
			lab.a = 0, lab.b = 0;
			lab2RGB(lab, rgb);

			pen.CreatePen(PS_SOLID, 1, RGB(rgb.R, rgb.G, rgb.B));				//1像素宽实线
			brush.CreateStockObject(NULL_BRUSH);
			memDC.SelectObject(&pen);
			memDC.SelectObject(&brush);
			int xPointer;		//在右边图片中绘制方块的目标位置x坐标
			int yPointer;		//在右边图片中绘制方块的目标位置y坐标
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 6; j++)
				{
					rgb2lab(m_standardRGB[i][j], lab);
					xPointer = (lab.a + 13.1977) / (13.1989 + 13.1977) * width + 0.5;			//计算x坐标(相对控件)
					yPointer = (1 - (lab.b + 12.471) / (12.4584 + 12.471)) * height + 0.5;		//计算y坐标(相对控件)
					memDC.Rectangle(xPointer - 7, yPointer - 7, xPointer + 7, yPointer + 7);
				}
			}
			pen.DeleteObject();
			brush.DeleteObject();
		}

		//绘制圆(当前颜色位置)
		if (m_standardCirclePaint)
		{
			lab.L = 100 - m_LValue;
			lab.a = 0, lab.b = 0;
			lab2RGB(lab, rgb);

			pen.CreatePen(PS_SOLID, 1, RGB(rgb.R, rgb.G, rgb.B));				//1像素宽实线
			brush.CreateStockObject(NULL_BRUSH);
			memDC.SelectObject(&pen);
			memDC.SelectObject(&brush);
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 6; j++)
				{
					memDC.Ellipse(m_rightCircle[i][j]);
				}
			}
			pen.DeleteObject();
			brush.DeleteObject();
		}

		//从内存中复制
		pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
		memBitmap.DeleteObject();
		memDC.DeleteDC();
	}

	if (m_inputChosen)
	{
		//获取控件信息
		CRect rectLeft;
		GetDlgItem(IDC_STATIC1)->GetClientRect(&rectLeft);
		int leftHeight = rectLeft.Height();
		int leftWidth = rectLeft.Width();

		CDC *pDC = GetDlgItem(IDC_STATIC1)->GetDC();

		CDC memDC;				//显示设备对象
		CBitmap memBitmap;
		memDC.CreateCompatibleDC(pDC);
		memBitmap.CreateCompatibleBitmap(pDC, leftWidth, leftHeight);
		memDC.SelectObject(&memBitmap);
		memDC.FillSolidRect(0, 0, leftWidth, leftHeight, RGB(255, 255, 255));

		//加载图片
		CDC originBmpDC;		//用以加载图片，之后缩放到memDC
		originBmpDC.CreateCompatibleDC(pDC);
		BITMAP bmInfo;
		m_inputBmp.GetObject(sizeof(bmInfo), &bmInfo);
		originBmpDC.SelectObject(m_inputBmp);
		memDC.StretchBlt(0, 0, leftWidth, leftHeight, &originBmpDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);

		//绘制矩形选择框
		if (m_inputRectPaint)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));			//白色实线，1像素宽
			CBrush brush;
			brush.CreateStockObject(NULL_BRUSH);
			memDC.SelectObject(&pen);
			memDC.SelectObject(&brush);
			memDC.Rectangle(&m_leftRect);

			//绘制24个小矩形，每行6个，共4行
			int i, j;
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 6; j++)
				{
					memDC.Rectangle(&m_leftSmallRect[i][j]);
				}
			}
			if (m_choosingCircleRow != -1)
			{
				CRect choosingCircle = m_leftSmallRect[m_choosingCircleRow][m_choosingCircleCol];
				brush.DeleteObject();
				brush.CreateSolidBrush(RGB(255, 255, 255));
				memDC.SelectObject(&brush);
				memDC.Rectangle(&choosingCircle);
			}

			pen.DeleteObject();
			brush.DeleteObject();
		}

		//从内存中复制
		pDC->BitBlt(0, 0, leftWidth, leftHeight, &memDC, 0, 0, SRCCOPY);
		memBitmap.DeleteObject();
		memDC.DeleteDC();
	}

	/*
	pDC->BitBlt(0, 0, dlgRect.Width(), dlgRect.Height(), &memDC, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
	memDC.DeleteDC();*/
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCompareDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	m_LValue = pSlidCtrl->GetPos();			//取得滑块当前位置值
	m_standardRePaint = true;				//右边标准图需要重绘
	char temp[5];
	sprintf(temp, "%d", m_LValue);
	GetDlgItem(IDC_EDIT10)->SetWindowTextA(temp);	//显示L分量值

	CRect picRect;									//使右边图片区域无效
	GetDlgItem(IDC_STATIC2)->GetClientRect(picRect);
	GetDlgItem(IDC_STATIC2)->ClientToScreen(picRect);
	ScreenToClient(picRect);
	InvalidateRect(picRect);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCompareDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;
}


void CCompareDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//打开文件
	CString filter;
	filter = "*.bmp|*.bmp| BMP(*.bmp)|*.bmp|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//按下确定按钮 dlg.DoModal() 函数显示对话框  
	if (dlg.DoModal() == IDOK)
	{
		//打开对话框获取图像信息  
		CString BmpName = dlg.GetPathName();     //获取文件路径名
		CString EntName = dlg.GetFileExt();      //获取文件扩展名
		EntName.MakeLower();                     //将文件扩展名转换为一个小写字符
		if (EntName.Compare("bmp") != 0) return;
		if (m_inputBmp.m_hObject != NULL)//判断 
			m_inputBmp.DeleteObject();
		HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
			BmpName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		if (hbmp == NULL)
			return;
		m_inputBmp.Attach(hbmp);
		if (m_inputChosen)		//左边已有图片
		{
			m_inputRectPaint = false;			//不再绘制矩形
			m_standardCirclePaint = false;		//右边图片不用绘制圆

			//使右边图像区无效
			CRect rightRect;
			GetDlgItem(IDC_STATIC2)->GetClientRect(rightRect);
			GetDlgItem(IDC_STATIC2)->ClientToScreen(rightRect);
			ScreenToClient(rightRect);
			InvalidateRect(rightRect);
		}
		m_standardRePaint = true;			//右边图片需要重绘(不绘制圆形)
		m_inputChosen = true;				//已选择输入图像
		m_standardRectPaint = true;			//右边标准位置(方形)需要绘制
		m_standardCirclePaint = false;

		//使左边图像区无效
		CRect picRect;
		GetDlgItem(IDC_STATIC1)->GetClientRect(picRect);
		GetDlgItem(IDC_STATIC1)->ClientToScreen(picRect);
		ScreenToClient(picRect);
		InvalidateRect(picRect);
	}
}


void CCompareDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//左边图片区域
	CRect rectLeft;
	POINT ptPut = point;
	GetDlgItem(IDC_STATIC1)->GetWindowRect(&rectLeft);		//控件相对屏幕的坐标
	ClientToScreen(&ptPut);
	if (rectLeft.PtInRect(ptPut) && m_inputChosen)
	{
		m_LeftPicLButtonDown = true;
		m_ptOld.x = ptPut.x - rectLeft.left;		//m_ptOld为相对控件的坐标
		m_ptOld.y = ptPut.y - rectLeft.top;
		m_standardCirclePaint = false;
	}

	//右边图片区域
	CRect rectRight;
	GetDlgItem(IDC_STATIC2)->GetWindowRect(rectRight);
	if (rectRight.PtInRect(ptPut) && m_inputChosen)
	{ 
		m_ptOld.x = ptPut.x - rectRight.left;
		m_ptOld.y = ptPut.y - rectRight.top;
		int i, j;
		m_choosingCircleRow = -1;
		m_choosingCircleCol = -1;
		for (i = 0; i < 4; i++)				//检测鼠标点击区域是否为圆所在区域
		{
			for (j = 0; j < 6; j++)
			{
				if (m_rightCircle[i][j].PtInRect(m_ptOld))
				{
					m_choosingCircleRow = i;
					m_choosingCircleCol = j;
					break;
				}
			}
		}
		if (m_choosingCircleRow != -1)
			m_RightPicLButtonDown = true;
		//使左边图像区无效
		CRect picRect;
		GetDlgItem(IDC_STATIC1)->GetClientRect(picRect);
		GetDlgItem(IDC_STATIC1)->ClientToScreen(picRect);
		ScreenToClient(picRect);
		InvalidateRect(picRect);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CCompareDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	POINT ptPut = point;
	ClientToScreen(&ptPut);
	if (m_LeftPicLButtonDown)
	{
		GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);
		m_LeftPicLButtonDown = false;
		
		//记录左边图片中各个小矩形中的RGB平均值
		int i, j;

		/*
		FILE *dataOut = fopen("standardValue.txt", "wt");
		if (!dataOut)
		{
			return;
		}*/

		CDC *pDC = GetDlgItem(IDC_STATIC1)->GetDC();
		
		CRect rightPicRect;
		GetDlgItem(IDC_STATIC2)->GetWindowRect(&rightPicRect);
		int xPointer;		//在右边图片中绘制圆的目标位置x坐标
		int yPointer;		//在右边图片中绘制圆的目标位置y坐标
		LabColor lab;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 6; j++)
			{
				getRectColor(pDC, m_leftSmallRect[i][j], m_originRGB[i][j], 0.4 * m_leftSmallRect[i][j].Width());
				m_moveRGB[i][j] = m_originRGB[i][j];
				rgb2lab(m_moveRGB[i][j], lab);
				xPointer = (lab.a + 13.1977) / (13.1989 + 13.1977) * rightPicRect.Width() + 0.5;			//计算x坐标(相对控件)
				yPointer = (1 - (lab.b + 12.471) / (12.4584 + 12.471)) * rightPicRect.Height() + 0.5;		//计算y坐标(相对控件)
				m_rightCircle[i][j].left = xPointer - 10;
				m_rightCircle[i][j].right = xPointer + 10;
				m_rightCircle[i][j].top = yPointer - 10;
				m_rightCircle[i][j].bottom = yPointer + 10;
			}
		}

		calConvertMat();
		m_standardRePaint = true;
		m_standardCirclePaint = true;

		//使右边图像区无效
		CRect rightRect;
		GetDlgItem(IDC_STATIC2)->GetClientRect(rightRect);
		GetDlgItem(IDC_STATIC2)->ClientToScreen(rightRect);
		ScreenToClient(rightRect);
		InvalidateRect(rightRect);

		/*
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 6; j++)
			{
				getRectColor(pDC, m_leftSmallRect[i][j], m_originRGB[i][j], 3);
				
				fprintf(dataOut, "%u\t%u\t%u\n", m_originRGB[i][j].R, m_originRGB[i][j].G, m_originRGB[i][j].B);
			}
		}*/
		
		//fclose(dataOut);
	}

	if (m_RightPicLButtonDown)
	{
		m_RightPicLButtonDown = false;
		m_choosingCircleRow = -1;
		m_choosingCircleCol = -1;

		//使左边图像区无效
		CRect picRect;
		GetDlgItem(IDC_STATIC1)->GetClientRect(picRect);
		GetDlgItem(IDC_STATIC1)->ClientToScreen(picRect);
		ScreenToClient(picRect);
		InvalidateRect(picRect);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CCompareDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	POINT ptPut = point;
	ClientToScreen(&ptPut);
	if (m_LeftPicLButtonDown)
	{
		GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);
		m_inputRectPaint = true;
		m_ptNow.x = ptPut.x - rect.left;			//鼠标当前位置相对客户区坐标
		m_ptNow.y = ptPut.y - rect.top;
		
		if (m_ptNow.x > m_ptOld.x)					//鼠标当前位置在原位置右侧
		{
			if (ptPut.x <= rect.right)				//鼠标水平位置在控件内
			{
				m_leftRect.left = m_ptOld.x;		//设置所应绘制矩阵的范围
				m_leftRect.right = m_ptNow.x;
			}
			else									//鼠标水平位置在控件右侧
			{
				m_leftRect.left = m_ptOld.x;		//设置所应绘制矩阵的范围
				m_leftRect.right = rect.Width();
			}
		}
		else										//鼠标当前位置在原位置左侧
		{
			if (ptPut.x >= rect.left)				//鼠标水平位置在控件内
			{
				m_leftRect.left = m_ptNow.x;		//设置所应绘制矩阵的范围
				m_leftRect.right = m_ptOld.x;
			}
			else									//鼠标水平位置在控件左侧
			{
				m_leftRect.left = 0;				//设置所应绘制矩阵的范围
				m_leftRect.right = m_ptOld.x;
			}
		}

		if (m_ptNow.y > m_ptOld.y)					//鼠标当前位置在原位置之下
		{
			if (ptPut.y <= rect.bottom)				//鼠标竖直位置在控件内
			{
				m_leftRect.top = m_ptOld.y;			//设置所应绘制矩阵的范围
				m_leftRect.bottom = m_ptNow.y;
			}
			else									//鼠标竖直位置在控件下方
			{
				m_leftRect.top = m_ptOld.y;			//设置所应绘制矩阵的范围
				m_leftRect.bottom = rect.Height();
			}
		}
		else										//鼠标当前位置在原位置之上
		{
			if (ptPut.y >= rect.top)				//鼠标竖直位置在控件内
			{
				m_leftRect.top = m_ptNow.y;			//设置所应绘制矩阵的范围
				m_leftRect.bottom = m_ptOld.y;
			}
			else									//鼠标竖直位置在控件上方
			{
				m_leftRect.top = 0;					//设置所应绘制矩阵的范围
				m_leftRect.bottom = m_ptOld.y;
			}
		}

		//计算小矩形位置
		int rectHeight = m_leftRect.Height() / 10.0 + 0.5;		//小矩形的高
		int rectWidth = m_leftRect.Width() / 15.0 + 0.5;		//小矩形的宽
		int i, j;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 6; j++)
			{
				m_leftSmallRect[i][j].left = m_leftRect.left + (0.75 + 2.5 * j) * rectWidth;
				m_leftSmallRect[i][j].right = m_leftRect.left + (1.75 + 2.5 * j) * rectWidth;
				m_leftSmallRect[i][j].top = m_leftRect.top + (0.75 + 2.5 * i) * rectHeight;
				m_leftSmallRect[i][j].bottom = m_leftRect.top + (1.75 + 2.5 * i) * rectHeight;
			}
		}

		//使左边图像区无效
		CRect picRect;
		GetDlgItem(IDC_STATIC1)->GetClientRect(picRect);
		GetDlgItem(IDC_STATIC1)->ClientToScreen(picRect);
		ScreenToClient(picRect);
		InvalidateRect(picRect);
	}

	if (m_RightPicLButtonDown)
	{
		m_standardRePaint = true;
		GetDlgItem(IDC_STATIC2)->GetWindowRect(&rect);
		
		m_ptNow.x = ptPut.x - rect.left;		//鼠标当前位置相对控件坐标
		m_ptNow.y = ptPut.y - rect.top;

		if (ptPut.x < rect.left)				//鼠标水平位置在控件内
		{
			m_ptNow.x = 0;
		}
		if (ptPut.x > rect.right)				//鼠标水平位置在控件内
		{
			m_ptNow.x = rect.Width();
		}
		if (ptPut.y < rect.top)					//鼠标水平位置在控件内
		{
			m_ptNow.y = 0;
		}
		if (ptPut.y > rect.bottom)				//鼠标水平位置在控件内
		{
			m_ptNow.y = rect.Height();
		}

		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].left = m_ptNow.x - 10;
		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].right = m_ptNow.x + 10;
		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].top = m_ptNow.y - 10;
		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].bottom = m_ptNow.y + 10;

		CRect rightPicRect;					//获取右边图片区域矩形
		GetDlgItem(IDC_STATIC2)->GetWindowRect(&rightPicRect);

		LabColor lab2;
		rgb2lab(m_originRGB[m_choosingCircleRow][m_choosingCircleCol], lab2);					//获取原颜色L值
		int xPoint = m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].CenterPoint().x;
		int yPoint = m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].CenterPoint().y;
		lab2.a = xPoint / (double)rightPicRect.Width() * (13.1989 + 13.1977) - 13.1977;			//计算新位置a值
		lab2.b = yPoint / (double)rightPicRect.Height() * (12.4584 + 12.471) - 12.471;			//计算新位置b值
		lab2RGB(lab2, m_moveRGB[m_choosingCircleRow][m_choosingCircleCol]);

		calConvertMat();			//计算转换矩阵

		//使右边图像区无效
		CRect rightRect;
		GetDlgItem(IDC_STATIC2)->GetClientRect(rightRect);
		GetDlgItem(IDC_STATIC2)->ClientToScreen(rightRect);
		ScreenToClient(rightRect);
		InvalidateRect(rightRect);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCompareDlg::getRectColor(CDC * pDC, CRect & rect, RGBColor & rgb, int l)
//取rect中的所有像素点rgb值的平均值，舍弃rect边缘宽度为l的范围
{
	int x, y;
	COLORREF rgbREF;
	int R = 0;			//R分量累计值
	int G = 0;			//G分量累计值
	int B = 0;			//B分量累计值
	int counter = 0;	//记数计算的像素点数
	for (x = l; x < rect.Width() - l; x++)
	{
		for (y = l; y < rect.Height() - l; y++)
		{
			rgbREF = pDC->GetPixel(x + rect.left, y + rect.top);
			R += GetRValue(rgbREF);
			G += GetGValue(rgbREF);
			B += GetBValue(rgbREF);
			counter++;
		}
	}
	rgb.R = R / (double)counter;
	rgb.G = G / (double)counter;
	rgb.B = B / (double)counter;
}

void CCompareDlg::calConvertMat()
//根据moveRGB中当前的值，设定显示的矩阵中的值
{
	int i, j;
	
	/*
	//精确计算
	CRect rightPicRect;					//获取右边图片区域矩形
	GetDlgItem(IDC_STATIC2)->GetWindowRect(&rightPicRect);
	int xPoint, yPoint;
	
	LabColor lab;
	dRGBColor dRGB;
	Mat oriMat(3, 24);				//初始位置(左边图片中实际颜色)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			rgb2lab(m_originRGB[i][j], lab);						//获取原颜色L值
			lab2dRGB(lab, dRGB);
			oriMat.data[i * 6 + j] = dRGB.R;
			oriMat.data[24 + i * 6 + j] = dRGB.G;
			oriMat.data[48 + i * 6 + j] = dRGB.B;
		}
	}

	Mat aimMat(3, 24);				//目标位置(右边图片中圆圈位置)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			rgb2lab(m_originRGB[i][j], lab);						//获取原颜色L值
			xPoint = m_rightCircle[i][j].CenterPoint().x;
			yPoint = m_rightCircle[i][j].CenterPoint().y;
			lab.a = xPoint / (double)rightPicRect.Width() * (13.1989 + 13.1977) - 13.1977;			//计算新位置a值
			lab.b = (1 - yPoint / (double)rightPicRect.Height()) * (12.4584 + 12.471) - 12.471;		//计算新位置b值
			lab2dRGB(lab, dRGB);
			aimMat.data[i * 6 + j] = dRGB.R;
			aimMat.data[24 + i * 6 + j] = dRGB.G;
			aimMat.data[48 + i * 6 + j] = dRGB.B;
		}
	}*/

	//使用整数计算
	Mat oriMat(3, 24);				//初始位置(左边图片中实际颜色)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			oriMat.data[i * 6 + j] = m_originRGB[i][j].R;
			oriMat.data[24 + i * 6 + j] = m_originRGB[i][j].G;
			oriMat.data[48 + i * 6 + j] = m_originRGB[i][j].B;
		}
	}

	Mat aimMat(3, 24);				//目标位置(右边图片中圆圈位置)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			aimMat.data[i * 6 + j] = m_moveRGB[i][j].R;
			aimMat.data[24 + i * 6 + j] = m_moveRGB[i][j].G;
			aimMat.data[48 + i * 6 + j] = m_moveRGB[i][j].B;
		}
	}

	//Mat convertMat(3, 3);			//转换矩阵
	MatrixTool tools;
	//转换矩阵C，原矩阵O，目标矩阵A
	//C = A * (O转置) * (O*(O转置))逆
	Mat oriTrans(24, 3);					//O转置
	tools.matTrans(oriTrans, oriMat);
	Mat oriMultiTrans(3, 3);				//O*(O转置)
	tools.matMulti(oriMultiTrans, oriMat, oriTrans);
	Mat oriMultiTrans_inv(3, 3);			//(O*(O转置))逆
	tools.invGauss(oriMultiTrans_inv, oriMultiTrans);
	Mat aimMultiOriTrans(3, 3);				//A * (O转置)
	tools.matMulti(aimMultiOriTrans, aimMat, oriTrans);
	tools.matMulti(m_convertMat, aimMultiOriTrans, oriMultiTrans_inv);

	//显示矩阵
	CString cstr;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			cstr.Format(_T("%.4f"), m_convertMat.data[i * 3 + j]);
			colorMatrix[i][j].SetWindowTextA(cstr);
		}
	}
}
