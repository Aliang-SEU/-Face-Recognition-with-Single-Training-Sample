#pragma once


// CDBConDlg dialog

class CDBConDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDBConDlg)

public:
	CDBConDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDBConDlg();

// Dialog Data
	enum { IDD = IDD_DBCON };

public:
	CString DataSourse,UseName,PassWord;   //�������ݿ������Դ���û���������(��TestCar�����õ�������Ϊpublic)

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCon();  //ȷ�����Ӱ�ť
};
