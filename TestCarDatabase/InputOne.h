#pragma once
#include <opencv.hpp>

// CInputOne dialog

class CInputOne : public CDialogEx
{
	DECLARE_DYNAMIC(CInputOne)

public:
	CInputOne(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputOne();

// Dialog Data
	enum { IDD = IDD_INPUTONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString DataSourse,UseName,PassWord;  //�������ݿ������Դ���û��������루�����Ի������õ�������Ϊpublic��

private:
	_ConnectionPtr m_pConn;//���ݿ�����ָ��
	_RecordsetPtr m_pRs,m_pRs1;//��¼��ָ��

	CString car_plate,car_logo,car_type,car_colour,car_locate,car_time;;  //���Ƶ���Ϣ

	cv::Mat carImage;//ԭͼ  ע�⣺cv::Mat_<float> carImage�����������ʱ����imread����

public:
	afx_msg void OnOpenImage();  //��ͼƬ
	afx_msg void OnInputData();   //¼������
	void DrawPicToHDC(IplImage img, UINT ID);  //ͨ���ú�����ͼƬ��ʾ��picture�ؼ���
};
