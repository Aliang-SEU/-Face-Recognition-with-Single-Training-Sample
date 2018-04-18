#pragma once
#include <opencv.hpp>

// CConstant dialog

class CConstant : public CDialogEx
{
	DECLARE_DYNAMIC(CConstant)

public:
	CConstant(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConstant();

// Dialog Data
	enum { IDD = IDD_CONSTANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_Menu;   //���ò˵���
	POINT old;    //��������Ի����ԭʼ����㣨�ı�Ի����Сʱ�ã�
	cv::Mat oriImage,resImage; //ԭͼ��ʶ����

public:
	void DrawPicToHDC(IplImage img, UINT ID);  //ͨ���ú�����ͼƬ��ʾ��picture�ؼ���
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();  //ʹ�ؼ���Ի����С�ĸı���ı�
	afx_msg void OnReadImage();
	afx_msg void OnFaceRec();
};
