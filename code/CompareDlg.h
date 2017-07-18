
// CompareDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "labRGBconvert.h"
#include "MatrixTool.h"

// CCompareDlg �Ի���
class CCompareDlg : public CDialogEx
{
// ����
public:
	CCompareDlg(CWnd* pParent = NULL);	// ��׼���캯��

	virtual ~CCompareDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_LSlider;
	int m_LValue;						//Lֵ,��slider��,��Χ[0,100]
	bool m_inputChosen;					//���ͼƬ�Ƿ�������
	bool m_standardRePaint;				//�ұ�ͼƬ�Ƿ���Ҫ�ػ�
	bool m_inputRectPaint;				//���ͼƬ���Ƿ���Ҫ���ƾ��ο�
	bool m_standardRectPaint;			//�ұ�ͼƬ���Ƿ���Ҫ���Ʒ��ε�(��׼ɫ��)
	bool m_standardCirclePaint;			//�ұ�ͼƬ���Ƿ���Ҫ����Բ�ε�(ʵ����ɫ��)
	bool m_LeftPicLButtonDown;			//����Ƿ������ͼƬ������
	bool m_RightPicLButtonDown;			//����Ƿ����ұ�ͼƬ������
	int m_choosingCircleRow;			//������ұ�ͼƬ������ʱ��ȷ����ѡ���Բ�б��
	int m_choosingCircleCol;			//������ұ�ͼƬ������ʱ��ȷ����ѡ���Բ�б��

	CPoint m_ptOld;						//����϶�ʱ����¼��ʼλ��
	CPoint m_ptNow;						//����϶�ʱ����¼��ǰλ��
	
	CRect m_leftRect;					//���ͼƬ��ǰ������Ƶľ���(��Կؼ�)
	CRect m_leftSmallRect[4][6];		//���ͼƬ��ǰ������Ƶ�24��С����
	CRect m_rightCircle[4][6];			//�ұ�ͼƬ��ǰ������Ƶ�24��Բλ��
	RGBColor m_originRGB[4][6];			//������ͼ�е�ͼ�񱣴��24��RGB��ֵ
	RGBColor m_moveRGB[4][6];			//������ͼ�е�Բ�㵱ǰλ�ñ����24��RGB��ֵ
	RGBColor m_standardRGB[4][6];		//24����׼RGBֵ
	//Mat m_oriRGB;						//������ͼ�е�ͼ�񱣴��24��RGB��ֵ
	//Mat m_moveRGB;					//������ͼ�е�Բ�㵱ǰλ�ñ����24��RGB��ֵ
	//Mat m_standardRGB;				//24����׼RGBֵ
	Mat m_convertMat;					//3x3ת������
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit LValueEdit;					//��ʾLֵ�ı༭��
	CEdit colorMatrix[3][3];			//3x3����༭��
	CBitmap m_inputBmp;					//�洢���ͼƬ
	CBitmap m_standardBmp;				//�洢�ұ�ͼƬ
	void getRectColor(CDC* pDC, CRect &rect, RGBColor &rgb, int l);		//ȡrect�е��������ص�rgbֵ��ƽ��ֵ������rect��Ե���Ϊl�ķ�Χ
	void calConvertMat();				//����moveRGB�е�ǰ��ֵ���趨��ʾ�ľ����е�ֵ
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
