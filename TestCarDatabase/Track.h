#pragma once
#include <opencv.hpp>

// CTrack dialog

class CTrack : public CDialogEx
{
	DECLARE_DYNAMIC(CTrack)

public:
	CTrack(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrack();

// Dialog Data
	enum { IDD = IDD_TRACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	 CString car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate;  //��ţ����ơ����ꡢ���͡���ɫ��ʱ�䡢�ص�(�����Ի������õ���Ϊpublic)
	 cv::Mat car_Img,face_Img;  //��������ʻԱͼƬ
	 int matchPar;   //����ƥ�����

	afx_msg void OnBnClickedTrButton1();  //ȷ����ť
	void DrawPicToHDC(IplImage img, UINT ID);  //ͨ���ú�����ͼƬ��ʾ��picture�ؼ���   
	afx_msg void OnOpenFace();     //�򿪼�ʻԱͼƬ
	afx_msg void OnOpenCar();   //�򿪳���ͼƬ�����
};
