#pragma once
#include <opencv.hpp>

// CStep dialog

class CStep : public CDialogEx
{
	DECLARE_DYNAMIC(CStep)

public:
	CStep(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStep();

// Dialog Data
	enum { IDD = IDD_STEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_Menu;   //���ò˵���
	POINT old;    //��������Ի����ԭʼ����㣨�ı�Ի����Сʱ�ã�
	cv::Mat oriImage,carImage,faceImage,feaImage,aliImage,illImage,resImage; //ԭͼ�����������������롢�����㡢���ա�ʶ����
	CvRect face_region;  //����ͼ������
	
	CvPoint left_eye, right_eye;  //��������

public:
	afx_msg void OnReadImg();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();  //ʹ�ؼ���Ի����С�ĸı���ı�
	void DrawPicToHDC(IplImage img, UINT ID);  //ͨ���ú�����ͼƬ��ʾ��picture�ؼ���
	afx_msg void OnFaceDetect();
	afx_msg void OnFaceAli();
	afx_msg void OnIllumination();
	afx_msg void OnFaceRec();
	afx_msg void OnFea();
	afx_msg void OnCarDetect();
};
