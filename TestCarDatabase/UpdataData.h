#pragma once


// CUpdataData dialog

class CUpdataData : public CDialogEx
{
	DECLARE_DYNAMIC(CUpdataData)

public:
	CUpdataData(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUpdataData();

// Dialog Data
	enum { IDD = IDD_UPDATADATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate;  //��ţ����ơ����ꡢ���͡���ɫ��ʱ�䡢�ص�(�����Ի������õ���Ϊpublic)
	afx_msg void OnBnClickedUButton1();
};
