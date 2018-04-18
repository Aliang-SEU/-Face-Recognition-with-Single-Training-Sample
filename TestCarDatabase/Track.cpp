// Track.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "Track.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "FaceDetect2.h"

// CTrack dialog

IMPLEMENT_DYNAMIC(CTrack, CDialogEx)

CTrack::CTrack(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrack::IDD, pParent)
{
	matchPar=0;
}

CTrack::~CTrack()
{
}

void CTrack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrack, CDialogEx)
	ON_BN_CLICKED(IDC_TR_BUTTON1, &CTrack::OnBnClickedTrButton1)
	ON_BN_CLICKED(IDC_TR_BUTTON3, &CTrack::OnOpenFace)
	ON_BN_CLICKED(IDC_TR_BUTTON2, &CTrack::OnOpenCar)
END_MESSAGE_MAP()


// CTrack message handlers

//ȷ����ť
void CTrack::OnBnClickedTrButton1()
{	
	if (face_Img.data)
	{
		matchPar=GetDlgItemInt(IDC_TR_EDIT7);

		if (matchPar>100||matchPar<0)
		{
			MessageBox("������׼ȷ��ƥ��ȣ�");
			return;
		} 
	} 
	GetDlgItemText(IDC_TR_EDIT1,car_plate);
	GetDlgItemText(IDC_TR_EDIT2,car_logo);
	GetDlgItemText(IDC_TR_EDIT3,car_type);
	GetDlgItemText(IDC_TR_EDIT4,car_colour);
	GetDlgItemText(IDC_TR_EDIT5,car_time);
	GetDlgItemText(IDC_TR_EDIT6,car_locate);

	if (car_plate==""&&car_logo==""&&car_type==""&&car_colour==""&&car_time==""&&car_locate=="")
	{
		MessageBox("����������һ������������");
	} 
	else
	{
		EndDialog(1);
	}
}


//��ͼƬ��ʾ��picture�ؼ���
void CTrack::DrawPicToHDC(IplImage img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf( &img ); // ����ͼƬ
	cimg.DrawToHDC( hDC, &rect ); // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC( pDC );
}


//�򿪼�ʻԱͼƬ
void CTrack::OnOpenFace()
{
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files(*.bmp;*.jpg)|*.bmp;*.jpg| All Files (*.*) |*.*||"), NULL
		);                                        // ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("ѡ��Ҫ����ͼƬ");    // ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )                    // �ж��Ƿ���ͼƬ
	{
		return;
	}

	CString mPath = dlg.GetPathName();            // ��ȡͼƬ·��

	face_Img=cv::imread(mPath.GetBuffer(0),1);
	if( !face_Img.data )   // �ж��Ƿ�ɹ�����ͼƬ
	{
		MessageBox("����ͼƬʧ��","������ʾ��");
		return;
	}

	IplImage img=face_Img;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
	DrawPicToHDC(img, IDC_TR_FACE_IMG); 
}


//�򿪳���ͼƬ�����
void CTrack::OnOpenCar()
{
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files(*.bmp;*.jpg)|*.bmp;*.jpg| All Files (*.*) |*.*||"), NULL
		);                                        // ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("ѡ��Ҫ����ͼƬ");    // ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )                    // �ж��Ƿ���ͼƬ
	{
		return;
	}

	CString mPath = dlg.GetPathName();            // ��ȡͼƬ·��

	car_Img=cv::imread(mPath.GetBuffer(0),1);
	if( !car_Img.data )   // �ж��Ƿ�ɹ�����ͼƬ
	{
		MessageBox("����ͼƬʧ��","������ʾ��");
		return;
	}


	FaceDetect2 a;
	a.draw(car_Img);
	face_Img=a.face_Image;

	if (face_Img.data)
	{
		IplImage img=face_Img;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
		DrawPicToHDC(img, IDC_TR_FACE_IMG); 
		IplImage img1=car_Img;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
		DrawPicToHDC(img1, IDC_TR_CAR_IMG); 
	} 
	else
	{
		MessageBox("û�м�⵽����","������ʾ��");
	}
}
