// InputOne.cpp : implementation file
//
#include "stdafx.h"
#include "TestCarDatabase.h"
#include "InputOne.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "FaceDetect.h"
#include "FaceDetect2.h"
#include "Wavelet.h"
#include "Retina_Model.h"

// CInputOne dialog

IMPLEMENT_DYNAMIC(CInputOne, CDialogEx)

CInputOne::CInputOne(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputOne::IDD, pParent)
{

}

CInputOne::~CInputOne()
{
}

void CInputOne::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputOne, CDialogEx)
	ON_BN_CLICKED(IDC_IN_BUTTON1, &CInputOne::OnOpenImage)
	ON_BN_CLICKED(IDC_IN_BUTTON2, &CInputOne::OnInputData)
END_MESSAGE_MAP()


// CInputOne message handlers

//��ͼƬ
void CInputOne::OnOpenImage()
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

	carImage=cv::imread(mPath.GetBuffer(0),1);
	if( !carImage.data )   // �ж��Ƿ�ɹ�����ͼƬ
	{
		MessageBox("����ͼƬʧ��","������ʾ��");
		return;
	}

	IplImage img=carImage;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
	DrawPicToHDC(img, IDC_CAR_PIC); 
}


//��ͼƬ��ʾ��picture�ؼ���
void CInputOne::DrawPicToHDC(IplImage img, UINT ID)
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


//¼������
void CInputOne::OnInputData()
{

	GetDlgItemText(IDC_IN_EDIT1,car_plate);
	GetDlgItemText(IDC_IN_EDIT2,car_logo);
	GetDlgItemText(IDC_IN_EDIT3,car_type);
	GetDlgItemText(IDC_IN_EDIT4,car_colour);
	GetDlgItemText(IDC_IN_EDIT5,car_time);
	GetDlgItemText(IDC_IN_EDIT6,car_locate);

	if (car_plate!=""&&car_logo!=""&&car_type!=""&&car_colour!=""&&car_time!=""&&car_locate!="")
	{
		//����ʵ��
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   
		m_pRs1.CreateInstance(__uuidof(Recordset));

		//����ָ�����ݿ�
		if(!m_pConn->State)    
		{    
			if (DataSourse!=""&&UseName!=""&&PassWord!="")
			{
				CString sql;
				sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

				CString sInfo;  
				try    
				{    
					m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				}catch (_com_error e)    
				{  
					sInfo.Format(_T("���ݿ�����ʧ��"));  
					MessageBox(sInfo);  
				}    
			} 
			else
			{
				MessageBox("�������ݿ��������ӣ�");
				return;
			}
		}   

		//��ȡ��һ��car_ID
		CString sql_next="SELECT MAX(CAR_ID)  MAXID FROM hr.CARCHRCT";
		m_pRs1->Open((_bstr_t)sql_next,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);
		int nextID = 0;
		try
		{
			nextID = m_pRs1->GetCollect("MAXID");
			nextID++;
		}
		catch (_com_error e)
		{
			nextID = 1;
		}
	//����ͼƬ�����������ơ�����ȣ�
		cv::Mat faceImageData; 

    //��ȡ����
		  //��ȡ��������
		if(carImage.data)
		{
			FaceDetect2 fd;
			fd.draw(carImage);
			cv::Mat faceImage = fd.face_Image;  //����FaceDetect���������⺯��
			if(faceImage.data&&faceImage.cols!=0)
			{
			    cv::resize(faceImage,faceImageData,cv::Size(150,150));
				//�ҶȻ�
				//cv::cvtColor(faceImageData,faceImageData,cv::COLOR_RGB2GRAY);
				//С�����մ���,�任֮ǰtypeΪ0��CV_8U���任֮��Ϊ6�� CV_64F��Ԫ�ط�Χ��0---1.0,��Ҫת��
				//Retina_Model rw;
				//faceImageData = rw.Retina_Model_Normalization(faceImageData);
				//normalize(faceImageData, faceImageData,0,255, cv::NORM_MINMAX);
			}
			else
			{
				MessageBox("û����ȡ����������","������ʾ��");
				return;
			}
		}
		else
		{
			MessageBox("����ѡ��ҪͼƬ","������ʾ��");
			return;
		}  

       //���滹����ȡ���ơ����������


		/*cv::imshow("", ill_faceImageData);
		cv::waitKey();*/

	// ���泵�������������ơ�����ͼƬ��ָ���ļ���
		std::string car_path,face_path;  //·��
		std::ostringstream oss;
		oss<<nextID;
		car_path="CarImage\\"+oss.str()+".jpg";
		face_path="FaceImage\\"+oss.str()+".jpg";
		cv::imwrite(car_path, carImage);
		cv::imwrite(face_path, faceImageData);


		//����ָ����
		CString sql="SELECT * FROM hr.CARCHRCT";
		m_pRs->Open((_bstr_t)sql,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);

		try  
		{   
			CString sql1;
			sql1.Format("insert into hr.CARCHRCT (car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate) values ('%d','%s','%s','%s','%s','%s','%s')",nextID, car_plate,car_logo,car_type,car_colour,car_time,car_locate);
			m_pConn->Execute((_bstr_t)sql1,NULL,adExecuteNoRecords);  
			MessageBox("��ӳɹ���","��ʾ");  
		}  
		catch(_com_error e)  
		{  
			MessageBox("���ʧ�ܣ�","��ʾ"); 
		}  
	} 
	else
	{
		MessageBox("������������Ϊ�գ�");
	}
	
}
