
// CompareDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "labRGBconvert.h"
#include "MatrixTool.h"

// CCompareDlg 对话框
class CCompareDlg : public CDialogEx
{
// 构造
public:
	CCompareDlg(CWnd* pParent = NULL);	// 标准构造函数

	virtual ~CCompareDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_LSlider;
	int m_LValue;						//L值,与slider绑定,范围[0,100]
	bool m_inputChosen;					//左边图片是否已载入
	bool m_standardRePaint;				//右边图片是否需要重绘
	bool m_inputRectPaint;				//左边图片中是否需要绘制矩形框
	bool m_standardRectPaint;			//右边图片中是否需要绘制方形点(标准色点)
	bool m_standardCirclePaint;			//右边图片中是否需要绘制圆形点(实际颜色点)
	bool m_LeftPicLButtonDown;			//左键是否在左边图片区域按下
	bool m_RightPicLButtonDown;			//左键是否在右边图片区域按下
	int m_choosingCircleRow;			//左键在右边图片区域按下时，确定所选择的圆行编号
	int m_choosingCircleCol;			//左键在右边图片区域按下时，确定所选择的圆列编号

	CPoint m_ptOld;						//鼠标拖动时，记录初始位置
	CPoint m_ptNow;						//鼠标拖动时，记录当前位置
	
	CRect m_leftRect;					//左边图片当前所需绘制的矩形(相对控件)
	CRect m_leftSmallRect[4][6];		//左边图片当前所需绘制的24个小矩形
	CRect m_rightCircle[4][6];			//右边图片当前所需绘制的24个圆位置
	RGBColor m_originRGB[4][6];			//根据左图中的图像保存的24个RGB的值
	RGBColor m_moveRGB[4][6];			//根据左图中的圆点当前位置保存的24个RGB的值
	RGBColor m_standardRGB[4][6];		//24个标准RGB值
	//Mat m_oriRGB;						//根据左图中的图像保存的24个RGB的值
	//Mat m_moveRGB;					//根据左图中的圆点当前位置保存的24个RGB的值
	//Mat m_standardRGB;				//24个标准RGB值
	Mat m_convertMat;					//3x3转换矩阵
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit LValueEdit;					//显示L值的编辑框
	CEdit colorMatrix[3][3];			//3x3矩阵编辑框
	CBitmap m_inputBmp;					//存储左边图片
	CBitmap m_standardBmp;				//存储右边图片
	void getRectColor(CDC* pDC, CRect &rect, RGBColor &rgb, int l);		//取rect中的所有像素点rgb值的平均值，舍弃rect边缘宽度为l的范围
	void calConvertMat();				//根据moveRGB中当前的值，设定显示的矩阵中的值
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
