
// TestCarDatabaseDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <opencv.hpp>

// CTestCarDatabaseDlg dialog
class CTestCarDatabaseDlg : public CDialogEx
{
// Construction
public:
	CTestCarDatabaseDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTCARDATABASE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString DataSourse,UseName,PassWord;  //�������ݿ������Դ���û��������루�ڱ�������õ�������Ϊpublic��

private:
	_ConnectionPtr m_pConn;//���ݿ�����ָ��
	_RecordsetPtr m_pRs,m_pRs1;//��¼��ָ��
	 CString car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate;  //��ţ����ơ����ꡢ���͡���ɫ��ʱ�䡢�ص�


private:
	CMenu m_Menu;   //���ò˵���
	POINT old;    //��������Ի����ԭʼ����㣨�ı�Ի����Сʱ�ã�
	int nItem;   //�б�ؼ����к�
	cv::Mat car_Img,face_Img,plate_Img,logo_Img;   //��������ʻԱ�����ơ�����ͼƬ
	CString picturePath,txtPath;  //��������ʱ��ͼƬ�ļ���·����txt�ļ�·��
	int matchPar;   //����ƥ�����
	

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();  //ʹ�ؼ���Ի����С�ĸı���ı�
	CListCtrl my_ctllist1;  //�б��
	afx_msg void OnBnConDB();  //�������ݿ�
	afx_msg void OnShowDatabase();  //��ʾ���ݿ��еļ�¼��
	afx_msg void OnComQuery();  //��ϲ�ѯ
	afx_msg void OnAcQuery();  //��ȷ���Ʋ�ѯ
	afx_msg void OnFuQuery();   //ģ�����Ʋ�ѯ
	afx_msg void OnTrack();  //��������׷��
	afx_msg void OnInputOne();  //����¼��
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);   //ѡ���б��е�ĳһ��
	afx_msg void OnShowImg();   //��ʾͼƬ
	afx_msg void OnHideImg();   //����ͼƬ
	void DrawPicToHDC(IplImage img, UINT ID);  //ͨ���ú�����ͼƬ��ʾ��picture�ؼ���   
	afx_msg void OnDeleteData();  //ɾ��ָ����¼
	afx_msg void OnUpdataData();  //��������
	afx_msg void OnInputMulti();  //����¼��
	afx_msg void OnStep();
	afx_msg void OnConstant();
};   
