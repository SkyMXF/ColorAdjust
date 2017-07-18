
// CompareDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Compare.h"
#include "CompareDlg.h"
#include "afxdialogex.h"
#include "labRGBconvert.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCompareDlg �Ի���



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


// CCompareDlg ��Ϣ�������

BOOL CCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��������ʼ��
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
	
	//��ʼ��m_convertMatΪ��λ����
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			m_convertMat.data[i * 3 + j] = (i == j ? 1 : 0);
		}
	}

	//��ʾ��λ����
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
	{//24����׼��ɫRGBֵ
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

	// L����Slider��ʼ��
	m_LSlider.SetRange(0, 100);		// ������Χ[0,100]
	m_LSlider.SetTicFreq(10);		// ÿ10����λ��һ���̶�
	m_LSlider.SetPos(50);
	LValueEdit.SetWindowTextA(_T("50"));

	//��ȡ�ؼ���Ϣ
	CRect rect;
	GetDlgItem(IDC_STATIC2)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	//����L����ֵ,����׼ͼ��д��ͼ���ļ�
	
	int height = rect.Height();								//���Ϊ-60,100
	int width = rect.Width();								//���Ϊ-60,80
	double dA = (13.1989 + 13.1977) / width;		//����仯��
	double dB = (12.4584 + 12.471) / height;		//����仯��

	RGBColor rgb;
	LabColor lab;
	int size = height * width * 3;

	//����ͼ�������Ϣ
	Bitmap pic;
	pic.fileHeader.bfSize = sizeof(MYBITMAPFILEHEADER) + sizeof(MYBITMAPINFOHEADER) + size;
	pic.infoHeader.biHeight = height;
	pic.infoHeader.biWidth = width;
	if (pic.buffer != NULL) delete[]pic.buffer;
	pic.buffer = new BYTE[size];
	//����ͼ������
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
	//д���ļ�
	FILE *output = fopen("standard.bmp", "wb");
	if (!output)
	{
		return false;
	}
	fwrite(&pic.fileHeader, sizeof(MYBITMAPFILEHEADER), 1, output);
	fwrite(&pic.infoHeader, sizeof(MYBITMAPINFOHEADER), 1, output);
	fwrite(pic.buffer, size, 1, output);
	fclose(output);

	//����ͼ����Ϣ���ڴ�
	if (m_standardBmp.m_hObject != NULL)//�ж�
		m_standardBmp.DeleteObject();
	HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		"standard.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (hbmp == NULL)
		return FALSE;
	m_standardBmp.Attach(hbmp);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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

	CDC memDC;				//��ʾ�豸����
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(NULL);
	memBitmap.CreateCompatibleBitmap(pDC, dlgRect.Width(), dlgRect.Height());
	CBitmap *pOldBit = memDC.SelectObject(&memBitmap);		//����ԭͼ
	memDC.FillSolidRect(0, 0, dlgRect.Width(), dlgRect.Height(), RGB(255, 255, 255));*/
	if (m_standardRePaint)				//�ػ��ұ�ͼ��
	{
		m_standardRePaint = false;
		
		//��ȡ�ؼ���Ϣ
		CRect rcclient;
		GetDlgItem(IDC_STATIC2)->GetClientRect(&rcclient);
		int height = rcclient.Height();
		int width = rcclient.Width();
		int size = height * width * 3;
		BYTE *dataPointer = new BYTE[size];
		int i, j;
		double dA = (13.1989 + 13.1977) / width;		//����仯��
		double dB = (12.4584 + 12.471) / height;		//����仯��
		RGBColor rgb;
		LabColor lab;
		
		//���ݵ�ǰL��ֵ����дͼ������
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

		CDC memDC;				//��ʾ�豸����
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

		//���Ʒ���(��׼ɫλ��)
		if (m_standardRectPaint)
		{
			lab.L = 100 - m_LValue;
			lab.a = 0, lab.b = 0;
			lab2RGB(lab, rgb);

			pen.CreatePen(PS_SOLID, 1, RGB(rgb.R, rgb.G, rgb.B));				//1���ؿ�ʵ��
			brush.CreateStockObject(NULL_BRUSH);
			memDC.SelectObject(&pen);
			memDC.SelectObject(&brush);
			int xPointer;		//���ұ�ͼƬ�л��Ʒ����Ŀ��λ��x����
			int yPointer;		//���ұ�ͼƬ�л��Ʒ����Ŀ��λ��y����
			for (i = 0; i < 4; i++)
			{
				for (j = 0; j < 6; j++)
				{
					rgb2lab(m_standardRGB[i][j], lab);
					xPointer = (lab.a + 13.1977) / (13.1989 + 13.1977) * width + 0.5;			//����x����(��Կؼ�)
					yPointer = (1 - (lab.b + 12.471) / (12.4584 + 12.471)) * height + 0.5;		//����y����(��Կؼ�)
					memDC.Rectangle(xPointer - 7, yPointer - 7, xPointer + 7, yPointer + 7);
				}
			}
			pen.DeleteObject();
			brush.DeleteObject();
		}

		//����Բ(��ǰ��ɫλ��)
		if (m_standardCirclePaint)
		{
			lab.L = 100 - m_LValue;
			lab.a = 0, lab.b = 0;
			lab2RGB(lab, rgb);

			pen.CreatePen(PS_SOLID, 1, RGB(rgb.R, rgb.G, rgb.B));				//1���ؿ�ʵ��
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

		//���ڴ��и���
		pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);
		memBitmap.DeleteObject();
		memDC.DeleteDC();
	}

	if (m_inputChosen)
	{
		//��ȡ�ؼ���Ϣ
		CRect rectLeft;
		GetDlgItem(IDC_STATIC1)->GetClientRect(&rectLeft);
		int leftHeight = rectLeft.Height();
		int leftWidth = rectLeft.Width();

		CDC *pDC = GetDlgItem(IDC_STATIC1)->GetDC();

		CDC memDC;				//��ʾ�豸����
		CBitmap memBitmap;
		memDC.CreateCompatibleDC(pDC);
		memBitmap.CreateCompatibleBitmap(pDC, leftWidth, leftHeight);
		memDC.SelectObject(&memBitmap);
		memDC.FillSolidRect(0, 0, leftWidth, leftHeight, RGB(255, 255, 255));

		//����ͼƬ
		CDC originBmpDC;		//���Լ���ͼƬ��֮�����ŵ�memDC
		originBmpDC.CreateCompatibleDC(pDC);
		BITMAP bmInfo;
		m_inputBmp.GetObject(sizeof(bmInfo), &bmInfo);
		originBmpDC.SelectObject(m_inputBmp);
		memDC.StretchBlt(0, 0, leftWidth, leftHeight, &originBmpDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);

		//���ƾ���ѡ���
		if (m_inputRectPaint)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));			//��ɫʵ�ߣ�1���ؿ�
			CBrush brush;
			brush.CreateStockObject(NULL_BRUSH);
			memDC.SelectObject(&pen);
			memDC.SelectObject(&brush);
			memDC.Rectangle(&m_leftRect);

			//����24��С���Σ�ÿ��6������4��
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

		//���ڴ��и���
		pDC->BitBlt(0, 0, leftWidth, leftHeight, &memDC, 0, 0, SRCCOPY);
		memBitmap.DeleteObject();
		memDC.DeleteDC();
	}

	/*
	pDC->BitBlt(0, 0, dlgRect.Width(), dlgRect.Height(), &memDC, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
	memDC.DeleteDC();*/
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCompareDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CSliderCtrl *pSlidCtrl = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	m_LValue = pSlidCtrl->GetPos();			//ȡ�û��鵱ǰλ��ֵ
	m_standardRePaint = true;				//�ұ߱�׼ͼ��Ҫ�ػ�
	char temp[5];
	sprintf(temp, "%d", m_LValue);
	GetDlgItem(IDC_EDIT10)->SetWindowTextA(temp);	//��ʾL����ֵ

	CRect picRect;									//ʹ�ұ�ͼƬ������Ч
	GetDlgItem(IDC_STATIC2)->GetClientRect(picRect);
	GetDlgItem(IDC_STATIC2)->ClientToScreen(picRect);
	ScreenToClient(picRect);
	InvalidateRect(picRect);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCompareDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return true;
}


void CCompareDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//���ļ�
	CString filter;
	filter = "*.bmp|*.bmp| BMP(*.bmp)|*.bmp|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);

	//����ȷ����ť dlg.DoModal() ������ʾ�Ի���  
	if (dlg.DoModal() == IDOK)
	{
		//�򿪶Ի����ȡͼ����Ϣ  
		CString BmpName = dlg.GetPathName();     //��ȡ�ļ�·����
		CString EntName = dlg.GetFileExt();      //��ȡ�ļ���չ��
		EntName.MakeLower();                     //���ļ���չ��ת��Ϊһ��Сд�ַ�
		if (EntName.Compare("bmp") != 0) return;
		if (m_inputBmp.m_hObject != NULL)//�ж� 
			m_inputBmp.DeleteObject();
		HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
			BmpName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		if (hbmp == NULL)
			return;
		m_inputBmp.Attach(hbmp);
		if (m_inputChosen)		//�������ͼƬ
		{
			m_inputRectPaint = false;			//���ٻ��ƾ���
			m_standardCirclePaint = false;		//�ұ�ͼƬ���û���Բ

			//ʹ�ұ�ͼ������Ч
			CRect rightRect;
			GetDlgItem(IDC_STATIC2)->GetClientRect(rightRect);
			GetDlgItem(IDC_STATIC2)->ClientToScreen(rightRect);
			ScreenToClient(rightRect);
			InvalidateRect(rightRect);
		}
		m_standardRePaint = true;			//�ұ�ͼƬ��Ҫ�ػ�(������Բ��)
		m_inputChosen = true;				//��ѡ������ͼ��
		m_standardRectPaint = true;			//�ұ߱�׼λ��(����)��Ҫ����
		m_standardCirclePaint = false;

		//ʹ���ͼ������Ч
		CRect picRect;
		GetDlgItem(IDC_STATIC1)->GetClientRect(picRect);
		GetDlgItem(IDC_STATIC1)->ClientToScreen(picRect);
		ScreenToClient(picRect);
		InvalidateRect(picRect);
	}
}


void CCompareDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//���ͼƬ����
	CRect rectLeft;
	POINT ptPut = point;
	GetDlgItem(IDC_STATIC1)->GetWindowRect(&rectLeft);		//�ؼ������Ļ������
	ClientToScreen(&ptPut);
	if (rectLeft.PtInRect(ptPut) && m_inputChosen)
	{
		m_LeftPicLButtonDown = true;
		m_ptOld.x = ptPut.x - rectLeft.left;		//m_ptOldΪ��Կؼ�������
		m_ptOld.y = ptPut.y - rectLeft.top;
		m_standardCirclePaint = false;
	}

	//�ұ�ͼƬ����
	CRect rectRight;
	GetDlgItem(IDC_STATIC2)->GetWindowRect(rectRight);
	if (rectRight.PtInRect(ptPut) && m_inputChosen)
	{ 
		m_ptOld.x = ptPut.x - rectRight.left;
		m_ptOld.y = ptPut.y - rectRight.top;
		int i, j;
		m_choosingCircleRow = -1;
		m_choosingCircleCol = -1;
		for (i = 0; i < 4; i++)				//�������������Ƿ�ΪԲ��������
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
		//ʹ���ͼ������Ч
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	POINT ptPut = point;
	ClientToScreen(&ptPut);
	if (m_LeftPicLButtonDown)
	{
		GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);
		m_LeftPicLButtonDown = false;
		
		//��¼���ͼƬ�и���С�����е�RGBƽ��ֵ
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
		int xPointer;		//���ұ�ͼƬ�л���Բ��Ŀ��λ��x����
		int yPointer;		//���ұ�ͼƬ�л���Բ��Ŀ��λ��y����
		LabColor lab;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 6; j++)
			{
				getRectColor(pDC, m_leftSmallRect[i][j], m_originRGB[i][j], 0.4 * m_leftSmallRect[i][j].Width());
				m_moveRGB[i][j] = m_originRGB[i][j];
				rgb2lab(m_moveRGB[i][j], lab);
				xPointer = (lab.a + 13.1977) / (13.1989 + 13.1977) * rightPicRect.Width() + 0.5;			//����x����(��Կؼ�)
				yPointer = (1 - (lab.b + 12.471) / (12.4584 + 12.471)) * rightPicRect.Height() + 0.5;		//����y����(��Կؼ�)
				m_rightCircle[i][j].left = xPointer - 10;
				m_rightCircle[i][j].right = xPointer + 10;
				m_rightCircle[i][j].top = yPointer - 10;
				m_rightCircle[i][j].bottom = yPointer + 10;
			}
		}

		calConvertMat();
		m_standardRePaint = true;
		m_standardCirclePaint = true;

		//ʹ�ұ�ͼ������Ч
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

		//ʹ���ͼ������Ч
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	POINT ptPut = point;
	ClientToScreen(&ptPut);
	if (m_LeftPicLButtonDown)
	{
		GetDlgItem(IDC_STATIC1)->GetWindowRect(&rect);
		m_inputRectPaint = true;
		m_ptNow.x = ptPut.x - rect.left;			//��굱ǰλ����Կͻ�������
		m_ptNow.y = ptPut.y - rect.top;
		
		if (m_ptNow.x > m_ptOld.x)					//��굱ǰλ����ԭλ���Ҳ�
		{
			if (ptPut.x <= rect.right)				//���ˮƽλ���ڿؼ���
			{
				m_leftRect.left = m_ptOld.x;		//������Ӧ���ƾ���ķ�Χ
				m_leftRect.right = m_ptNow.x;
			}
			else									//���ˮƽλ���ڿؼ��Ҳ�
			{
				m_leftRect.left = m_ptOld.x;		//������Ӧ���ƾ���ķ�Χ
				m_leftRect.right = rect.Width();
			}
		}
		else										//��굱ǰλ����ԭλ�����
		{
			if (ptPut.x >= rect.left)				//���ˮƽλ���ڿؼ���
			{
				m_leftRect.left = m_ptNow.x;		//������Ӧ���ƾ���ķ�Χ
				m_leftRect.right = m_ptOld.x;
			}
			else									//���ˮƽλ���ڿؼ����
			{
				m_leftRect.left = 0;				//������Ӧ���ƾ���ķ�Χ
				m_leftRect.right = m_ptOld.x;
			}
		}

		if (m_ptNow.y > m_ptOld.y)					//��굱ǰλ����ԭλ��֮��
		{
			if (ptPut.y <= rect.bottom)				//�����ֱλ���ڿؼ���
			{
				m_leftRect.top = m_ptOld.y;			//������Ӧ���ƾ���ķ�Χ
				m_leftRect.bottom = m_ptNow.y;
			}
			else									//�����ֱλ���ڿؼ��·�
			{
				m_leftRect.top = m_ptOld.y;			//������Ӧ���ƾ���ķ�Χ
				m_leftRect.bottom = rect.Height();
			}
		}
		else										//��굱ǰλ����ԭλ��֮��
		{
			if (ptPut.y >= rect.top)				//�����ֱλ���ڿؼ���
			{
				m_leftRect.top = m_ptNow.y;			//������Ӧ���ƾ���ķ�Χ
				m_leftRect.bottom = m_ptOld.y;
			}
			else									//�����ֱλ���ڿؼ��Ϸ�
			{
				m_leftRect.top = 0;					//������Ӧ���ƾ���ķ�Χ
				m_leftRect.bottom = m_ptOld.y;
			}
		}

		//����С����λ��
		int rectHeight = m_leftRect.Height() / 10.0 + 0.5;		//С���εĸ�
		int rectWidth = m_leftRect.Width() / 15.0 + 0.5;		//С���εĿ�
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

		//ʹ���ͼ������Ч
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
		
		m_ptNow.x = ptPut.x - rect.left;		//��굱ǰλ����Կؼ�����
		m_ptNow.y = ptPut.y - rect.top;

		if (ptPut.x < rect.left)				//���ˮƽλ���ڿؼ���
		{
			m_ptNow.x = 0;
		}
		if (ptPut.x > rect.right)				//���ˮƽλ���ڿؼ���
		{
			m_ptNow.x = rect.Width();
		}
		if (ptPut.y < rect.top)					//���ˮƽλ���ڿؼ���
		{
			m_ptNow.y = 0;
		}
		if (ptPut.y > rect.bottom)				//���ˮƽλ���ڿؼ���
		{
			m_ptNow.y = rect.Height();
		}

		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].left = m_ptNow.x - 10;
		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].right = m_ptNow.x + 10;
		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].top = m_ptNow.y - 10;
		m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].bottom = m_ptNow.y + 10;

		CRect rightPicRect;					//��ȡ�ұ�ͼƬ�������
		GetDlgItem(IDC_STATIC2)->GetWindowRect(&rightPicRect);

		LabColor lab2;
		rgb2lab(m_originRGB[m_choosingCircleRow][m_choosingCircleCol], lab2);					//��ȡԭ��ɫLֵ
		int xPoint = m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].CenterPoint().x;
		int yPoint = m_rightCircle[m_choosingCircleRow][m_choosingCircleCol].CenterPoint().y;
		lab2.a = xPoint / (double)rightPicRect.Width() * (13.1989 + 13.1977) - 13.1977;			//������λ��aֵ
		lab2.b = yPoint / (double)rightPicRect.Height() * (12.4584 + 12.471) - 12.471;			//������λ��bֵ
		lab2RGB(lab2, m_moveRGB[m_choosingCircleRow][m_choosingCircleCol]);

		calConvertMat();			//����ת������

		//ʹ�ұ�ͼ������Ч
		CRect rightRect;
		GetDlgItem(IDC_STATIC2)->GetClientRect(rightRect);
		GetDlgItem(IDC_STATIC2)->ClientToScreen(rightRect);
		ScreenToClient(rightRect);
		InvalidateRect(rightRect);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCompareDlg::getRectColor(CDC * pDC, CRect & rect, RGBColor & rgb, int l)
//ȡrect�е��������ص�rgbֵ��ƽ��ֵ������rect��Ե���Ϊl�ķ�Χ
{
	int x, y;
	COLORREF rgbREF;
	int R = 0;			//R�����ۼ�ֵ
	int G = 0;			//G�����ۼ�ֵ
	int B = 0;			//B�����ۼ�ֵ
	int counter = 0;	//������������ص���
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
//����moveRGB�е�ǰ��ֵ���趨��ʾ�ľ����е�ֵ
{
	int i, j;
	
	/*
	//��ȷ����
	CRect rightPicRect;					//��ȡ�ұ�ͼƬ�������
	GetDlgItem(IDC_STATIC2)->GetWindowRect(&rightPicRect);
	int xPoint, yPoint;
	
	LabColor lab;
	dRGBColor dRGB;
	Mat oriMat(3, 24);				//��ʼλ��(���ͼƬ��ʵ����ɫ)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			rgb2lab(m_originRGB[i][j], lab);						//��ȡԭ��ɫLֵ
			lab2dRGB(lab, dRGB);
			oriMat.data[i * 6 + j] = dRGB.R;
			oriMat.data[24 + i * 6 + j] = dRGB.G;
			oriMat.data[48 + i * 6 + j] = dRGB.B;
		}
	}

	Mat aimMat(3, 24);				//Ŀ��λ��(�ұ�ͼƬ��ԲȦλ��)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			rgb2lab(m_originRGB[i][j], lab);						//��ȡԭ��ɫLֵ
			xPoint = m_rightCircle[i][j].CenterPoint().x;
			yPoint = m_rightCircle[i][j].CenterPoint().y;
			lab.a = xPoint / (double)rightPicRect.Width() * (13.1989 + 13.1977) - 13.1977;			//������λ��aֵ
			lab.b = (1 - yPoint / (double)rightPicRect.Height()) * (12.4584 + 12.471) - 12.471;		//������λ��bֵ
			lab2dRGB(lab, dRGB);
			aimMat.data[i * 6 + j] = dRGB.R;
			aimMat.data[24 + i * 6 + j] = dRGB.G;
			aimMat.data[48 + i * 6 + j] = dRGB.B;
		}
	}*/

	//ʹ����������
	Mat oriMat(3, 24);				//��ʼλ��(���ͼƬ��ʵ����ɫ)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			oriMat.data[i * 6 + j] = m_originRGB[i][j].R;
			oriMat.data[24 + i * 6 + j] = m_originRGB[i][j].G;
			oriMat.data[48 + i * 6 + j] = m_originRGB[i][j].B;
		}
	}

	Mat aimMat(3, 24);				//Ŀ��λ��(�ұ�ͼƬ��ԲȦλ��)
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 6; j++)
		{
			aimMat.data[i * 6 + j] = m_moveRGB[i][j].R;
			aimMat.data[24 + i * 6 + j] = m_moveRGB[i][j].G;
			aimMat.data[48 + i * 6 + j] = m_moveRGB[i][j].B;
		}
	}

	//Mat convertMat(3, 3);			//ת������
	MatrixTool tools;
	//ת������C��ԭ����O��Ŀ�����A
	//C = A * (Oת��) * (O*(Oת��))��
	Mat oriTrans(24, 3);					//Oת��
	tools.matTrans(oriTrans, oriMat);
	Mat oriMultiTrans(3, 3);				//O*(Oת��)
	tools.matMulti(oriMultiTrans, oriMat, oriTrans);
	Mat oriMultiTrans_inv(3, 3);			//(O*(Oת��))��
	tools.invGauss(oriMultiTrans_inv, oriMultiTrans);
	Mat aimMultiOriTrans(3, 3);				//A * (Oת��)
	tools.matMulti(aimMultiOriTrans, aimMat, oriTrans);
	tools.matMulti(m_convertMat, aimMultiOriTrans, oriMultiTrans_inv);

	//��ʾ����
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
