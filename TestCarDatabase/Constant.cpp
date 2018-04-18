// Constant.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "Constant.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "FaceDetect2.h"
#include "FaceAlignment.h"
#include "Retina_Model.h"
#include "FaceRec.h"

#include "fstream"
#include <memory>
#include "tiny_cnn.h"
#include "MSLDE.h"

#include "facedetect-dll.h"
#pragma comment(lib,"libfacedetect.lib")

using namespace tiny_cnn;
using namespace tiny_cnn::activation;


// CConstant dialog

IMPLEMENT_DYNAMIC(CConstant, CDialogEx)

CConstant::CConstant(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConstant::IDD, pParent)
{

}

CConstant::~CConstant()
{
}

void CConstant::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	CRect temprect(0,0,640,420); 
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	//��Ӳ˵�
	m_Menu.LoadMenu(IDR_CONSTANT_MENU);  //  IDR_MENU1Ϊ�����Ĳ˵���ID����Resource��ͼ��Menu�ļ����¿����ҵ�
	SetMenu(&m_Menu);


	//���þ�̬�ı��������С
	CFont * f; 
	f = new CFont; 
	f->CreateFont(20,            // nHeight 
		0,           // nWidth 
		0,           // nEscapement 
		0,           // nOrientation 
		FW_SEMIBOLD,     // nWeight 
		FALSE,        // bItalic 
		FALSE,       // bUnderline 
		0,           // cStrikeOut 
		ANSI_CHARSET,              // nCharSet 
		OUT_DEFAULT_PRECIS,        // nOutPrecision 
		CLIP_DEFAULT_PRECIS,       // nClipPrecision 
		DEFAULT_QUALITY,           // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("����"));              // lpszFac

	GetDlgItem(IDC_C_STATIC1)->SetFont(f);
	GetDlgItem(IDC_C_STATIC2)->SetFont(f);
}


BEGIN_MESSAGE_MAP(CConstant, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_32802, &CConstant::OnReadImage)
	ON_COMMAND(ID_32803, &CConstant::OnFaceRec)
END_MESSAGE_MAP()

void CConstant::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		//�Ի���ı�ʱ����picture�ؼ��ϵ�ͼƬ�����ػ�
		if(oriImage.data)
		{
			IplImage img=oriImage;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
			DrawPicToHDC(img, IDC_C_ORI_IMG); 
		}
		
		if(resImage.data)
		{
			IplImage img=resImage;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
			DrawPicToHDC(img, IDC_C_RES_IMG); 
		}
	}
}

void CConstant::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//�ؼ���Ի����С���ı�
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
	{  
		ReSize();  
	}  
}


//ʹ�ؼ���Ի����С�ĸı���ı�
void CConstant::ReSize()  
{  
	float fsp[2];  
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;      
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x=recta.right-recta.left;  
	Newp.y=recta.bottom-recta.top;  
	fsp[0]=(float)Newp.x/old.x;  
	fsp[1]=(float)Newp.y/old.y;  
	CRect Rect;  
	int woc;  
	CPoint OldTLPoint,TLPoint; //���Ͻ�  
	CPoint OldBRPoint,BRPoint; //���½�  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�    
	while(hwndChild)      
	{      
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID  
		GetDlgItem(woc)->GetWindowRect(Rect);    
		ScreenToClient(Rect);    
		OldTLPoint = Rect.TopLeft();    
		TLPoint.x = long(OldTLPoint.x*fsp[0]);    
		TLPoint.y = long(OldTLPoint.y*fsp[1]);    
		OldBRPoint = Rect.BottomRight();    
		BRPoint.x = long(OldBRPoint.x *fsp[0]);    
		BRPoint.y = long(OldBRPoint.y *fsp[1]);    
		Rect.SetRect(TLPoint,BRPoint);    
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}  
	old=Newp;  
}  


//��ͼƬ��ʾ��picture�ؼ���
void CConstant::DrawPicToHDC(IplImage img, UINT ID)
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

//����ͼ��
void CConstant::OnReadImage()
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

	oriImage=cv::imread(mPath.GetBuffer(0),1);
	if( !oriImage.data )   // �ж��Ƿ�ɹ�����ͼƬ
	{
		MessageBox("����ͼƬʧ��","������ʾ��");
		return;
	}

	IplImage img=oriImage;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
	DrawPicToHDC(img, IDC_C_ORI_IMG); 
}



//����ʶ��
void CConstant::OnFaceRec()
{
	//if (!oriImage.data)   // �ж��Ƿ�ɹ�����ͼƬ
	//{
	//	MessageBox("���ȶ���ԭͼ��", "������ʾ��");
	//	return;
	//}

//	For TEST
	

	if (AllocConsole())
	{
		freopen("CONOUT$", "w", stdout);
		std::cout << "First picture read��" << std::endl;
		CString path = "C:\\Users\\hzl\\Desktop\\���Լ�\\";	//���ĵ�ַ
		CString outpath1 = "C:\\Users\\hzl\\Desktop\\faceimage1\\";
		CString outpath2 = "C:\\Users\\hzl\\Desktop\\alimentimage1\\";
		CString outpath3 = "C:\\Users\\hzl\\Desktop\\resultimage1\\";
		int num = 1;
		std::ofstream out2txt("1.txt", std::ios::out);
		for (num = 1; num <= 174; num++)
		{
			CString mpath, temp, _outpath1, _outpath2, _outpath3;
			temp.Format("%d", num);
			mpath = path + temp + ".jpg";
			_outpath1 = outpath1 + temp + ".jpg";
			_outpath2 = outpath2 + temp + ".jpg";
			_outpath3 = outpath3 + temp + ".jpg";
			oriImage = cv::imread(mpath.GetBuffer(0), 1);

			if (!oriImage.data)
			{
				std::cout << "δ�ܶ�ȡ" << mpath << std::endl;
				continue;
			}
			std::cout << "��ȡ" << mpath << "�ɹ�!,���м��:" << std::endl;

			int * pResults = NULL;
			cv::Mat temp_img;
			cv::cvtColor(oriImage, temp_img, cv::COLOR_RGB2GRAY);
			pResults = facedetect_frontal_tmp((unsigned char*)(temp_img.ptr(0)), temp_img.cols, temp_img.rows, temp_img.step,
				1.14f, 3, 24);	// ���޲��� 1.7f��2��24  ��Ѳ���1.6f��2��24 
			printf("%d frontal faces detected.\n", (pResults ? *pResults : 0));
			//print the detection results

			//�������
			FaceDetect2 fd;
			cv::Rect face_range = fd.draw(oriImage);
			cv::Mat faceImage = fd.face_Image;
			CvRect face_region = cvRect(0, 0, 0, 0);
			face_region.x = face_range.x;
			face_region.y = face_range.y;
			face_region.width = face_range.width;
			face_region.height = face_range.height;

			//���
			for (int i = 0; i < (pResults ? *pResults : 0); i++)
			{
				short * p = ((short*)(pResults + 1)) + 6 * 0;		//������һ������
				face_region.x = p[0];
				face_region.y = p[1];
				face_region.width = p[2];
				face_region.height = p[3];
				int neighbors = p[4];
			}
			faceImage = oriImage(cv::Rect(face_region.x, face_region.y, face_region.width, face_region.height));


			if (!face_region.width)
			{
				std::cout << "δ���ҵ�����" << std::endl;
				//cv::imwrite(outpath.GetBuffer(0), oriImage);
				faceImage = oriImage;
				cv::imwrite(_outpath1.GetBuffer(0), faceImage);
				continue;
			}
			//cv::resize(faceImage, faceImage, cv::Size(100, 100));

			cv::imwrite(_outpath1.GetBuffer(0), faceImage);

			cv::imwrite("1.jpg", faceImage);



			// ������궨
			network<mse, gradient_descent_levenberg_marquardt> nn;

			// connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
			static const bool connection[] = {
				O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
				O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
				O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
				X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
				X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
				X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
			};
#undef O
#undef X

			nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6) // 32x32 in, 5x5 kernel, 1-6 fmaps conv
				<< average_pooling_layer<tan_h>(28, 28, 6, 2) // 28x28 in, 6 fmaps, 2x2 subsampling
				<< convolutional_layer<tan_h>(14, 14, 5, 6, 16,
				connection_table(connection, 6, 16)) // with connection-table
				<< average_pooling_layer<tan_h>(10, 10, 16, 2)
				<< convolutional_layer<tan_h>(5, 5, 5, 16, 120)
				<< fully_connected_layer<tan_h>(120, 10);

			//std::cout << "load models..." << std::endl;

			std::string testpath = "1.jpg";  //  ����ͼƬλ��

			// load MNIST dataset
			std::vector<vec_t> train_labels, test_labels;
			std::vector<vec_t> train_images, test_images;
			std::vector<vec_t> train_faceAreas, test_faceAreas;
			std::vector<std::string> train_imgNames, test_imgNames;


			getTestImage(testpath, &test_images, &test_faceAreas);


			std::ifstream iws("Weights.txt");
			iws >> nn;
			// test and show results

			double eyes[4] = { 0, 0, 0, 0 }; //  ��������������������

			nn.test(test_images, test_faceAreas, eyes);

			cv::Mat face_show = faceImage.clone();

			CvPoint left_eye = cvPoint(eyes[0], eyes[1]);
			CvPoint right_eye = cvPoint(eyes[2], eyes[3]);
			//cv::circle(face_show, left_eye, 1, cvScalar(0, 0, 255), 1, 8);
			//cv::circle(face_show, right_eye, 1, cvScalar(0, 0, 255), 1, 8);

			left_eye.x += face_region.x;
			left_eye.y += face_region.y;
			right_eye.x += face_region.x;
			right_eye.y += face_region.y;

			//��������
			cv::Mat src_points = cv::Mat::zeros(2, 2, CV_64FC1);
			cv::Mat dst_points = cv::Mat::zeros(2, 2, CV_64FC1);


			src_points.at<double>(0, 0) = left_eye.x;		//��������
			src_points.at<double>(0, 1) = right_eye.x;		//��������
			src_points.at<double>(1, 0) = left_eye.y;
			src_points.at<double>(1, 1) = right_eye.y;

			dst_points.at<double>(0, 0) = 35;		//��������ʵ�ʹ̶���λ�� 2016-09-10 
			dst_points.at<double>(0, 1) = 65;
			dst_points.at<double>(1, 0) = 42;
			dst_points.at<double>(1, 1) = 42;

			CFaceAlignment fa;
			cv::Mat aliImage = fa.Alignment(oriImage, src_points, dst_points);

			//test:HZL
			cv::imwrite(_outpath2.GetBuffer(0), aliImage);
			//test end

			//���մ���

			//aliImage = faceImage;
			cv::Mat aliImage_gray;
			//�ҶȻ�
			cv::cvtColor(aliImage, aliImage_gray, cv::COLOR_RGB2GRAY);
			//cv::imshow("1",aliImage_gray);
			//С�����մ���
			Retina_Model rw;
			cv::Mat illImage;// = rw.Retina_Model_Normalization(aliImage_gray);
			normalize(aliImage_gray, illImage, 0, 1, cv::NORM_MINMAX);

			//����ʶ��
			cv::Mat_<double> train_Img, assist_Img, test_Img; //ѵ�������������Ͳ��Լ�����
			std::string face_Path;   //����ͼƬ��·��
			cv::Mat_<double> img, imgData;

			Retina_Model rw2;

			for (int i = 1; i <= 15; i++)
			{
				std::ostringstream oss;
				oss << i;
				face_Path = "Demo2//" + oss.str() + ".jpg";
				cv::Mat_<double> src = cv::imread(face_Path, 0);	//����Ϊ�Ҷ�ͼ��
				//С�����մ���
				//img = rw2.Retina_Model_Normalization(src);
				normalize(src, img, 0, 1, cv::NORM_MINMAX);
				/*cv::imshow("", img);
				cv::waitKey();*/
				if (img.data)
				{
					cv::resize(img, imgData, cv::Size(20, 20));
					imgData = imgData.t();
					cv::Mat_<double> colImgs = imgData.reshape(imgData.rows*imgData.cols);
					train_Img.push_back(colImgs);
				}
			}
			train_Img = train_Img.t();  //ת�ú�õ����յ�ѵ��������ͼƬ���д�ţ�
			cv::FileStorage oos(".\\Assist.xml", cv::FileStorage::READ);
			oos["Assist"] >> assist_Img;     //�õ�����������
			oos.release();

			cv::Mat test = illImage.clone();  //����һ������ͼ��
			cv::Mat test_resize;
			cv::resize(test, test_resize, cv::Size(20, 20));
			cv::Mat gray;
			// �ҶȻ�
			if (test_resize.channels() == 3)
				cv::cvtColor(test_resize, gray, CV_BGR2GRAY);
			else
				gray = test_resize;

			cv::Mat_<double> testData = gray;  //��ͼ������תΪdouble�ͣ����ں��������ʶ��
			testData = testData.t();
			test_Img = testData.reshape((testData.rows)*(testData.cols));   //�õ�����ͼƬ�ľ���
			test_Img = test_Img.t();
			//��������ʶ����
			FaceRec a;

			int result = a.recognition2(train_Img, assist_Img, test_Img);

			std::ostringstream oss;
			oss << result;
			out2txt << result << std::endl;
			face_Path = "Demo2//" + oss.str() + ".jpg";
			resImage = cv::imread(face_Path);
			IplImage img_show = resImage;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
			DrawPicToHDC(img_show, IDC_C_RES_IMG);

			//test:HZL
			cv::imwrite(_outpath3.GetBuffer(0), resImage);
		}
		out2txt.close();
	//TEST end
	}
}
