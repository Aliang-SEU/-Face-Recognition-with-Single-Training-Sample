
// TestCarDatabaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "TestCarDatabaseDlg.h"
#include "afxdialogex.h"
#include "DBConDlg.h"
#include "ComQuery.h"
#include "FaQuery.h"
#include "FuQuery.h"
#include "Track.h"
#include "InputOne.h"
#include "CvvImage.h"
#include "UpdataData.h"
#include "FaceDetect2.h"
#include<fstream>
#include<sstream>
#include<string>
#include "FaceRec.h"
#include "Step.h"
#include "Constant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestCarDatabaseDlg dialog




CTestCarDatabaseDlg::CTestCarDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestCarDatabaseDlg::IDD, pParent)
{
	nItem=-1;  //��ʼ���б����б��
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CTestCarDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, my_ctllist1);
}

BEGIN_MESSAGE_MAP(CTestCarDatabaseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_32771, &CTestCarDatabaseDlg::OnBnConDB)
	ON_COMMAND(ID_32789, &CTestCarDatabaseDlg::OnShowDatabase)
//	ON_BN_CLICKED(IDC_QUERY, &CTestCarDatabaseDlg::OnBnClickedQuery)
ON_COMMAND(ID_32783, &CTestCarDatabaseDlg::OnComQuery)
ON_COMMAND(ID_32787, &CTestCarDatabaseDlg::OnAcQuery)
ON_COMMAND(ID_32788, &CTestCarDatabaseDlg::OnFuQuery)
ON_COMMAND(ID_32790, &CTestCarDatabaseDlg::OnTrack)
ON_COMMAND(ID_32779, &CTestCarDatabaseDlg::OnInputOne)
ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTestCarDatabaseDlg::OnNMClickList1)
ON_BN_CLICKED(IDC_T_BUTTON1, &CTestCarDatabaseDlg::OnShowImg)
//ON_BN_CLICKED(IDC_T_BUTTON2, &CTestCarDatabaseDlg::OnDeleteData)
ON_COMMAND(ID_32791, &CTestCarDatabaseDlg::OnDeleteData)
ON_COMMAND(ID_32781, &CTestCarDatabaseDlg::OnUpdataData)
ON_COMMAND(ID_32792, &CTestCarDatabaseDlg::OnInputMulti)
ON_BN_CLICKED(IDC_T_BUTTON2, &CTestCarDatabaseDlg::OnHideImg)
ON_COMMAND(ID_32793, &CTestCarDatabaseDlg::OnStep)
ON_COMMAND(ID_32794, &CTestCarDatabaseDlg::OnConstant)
END_MESSAGE_MAP()


// CTestCarDatabaseDlg message handlers

BOOL CTestCarDatabaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//����
	SetWindowText("�����������ݿ����ϵͳ");

	//��Ӳ˵�
	m_Menu.LoadMenu(IDR_MAIN_MENU);  //  IDR_MENU1Ϊ�����Ĳ˵���ID����Resource��ͼ��Menu�ļ����¿����ҵ�
	SetMenu(&m_Menu);

	CRect temprect(0,0,960,640); 
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	//�õ�ԭʼ�ͻ���������
	CRect rect;      
	GetClientRect(&rect);     //ȡ�ͻ�����С    
	old.x=rect.right-rect.left;  
	old.y=rect.bottom-rect.top;


	//��ʼ��com��
	::CoInitialize(NULL);     

	//����б��������ߣ�����
	DWORD dwStyle = my_ctllist1.GetExtendedStyle();                   
	dwStyle |= LVS_EX_FULLROWSELECT;            
	dwStyle |= LVS_EX_GRIDLINES;                
	my_ctllist1.SetExtendedStyle(dwStyle);


	//����б���
	my_ctllist1.InsertColumn(0,"���",LVCFMT_CENTER,100);
	my_ctllist1.InsertColumn(1,"����",LVCFMT_CENTER,150);	
	my_ctllist1.InsertColumn(2,"����",LVCFMT_CENTER,150);
	my_ctllist1.InsertColumn(3,"����",LVCFMT_CENTER,150);
	my_ctllist1.InsertColumn(4,"��ɫ",LVCFMT_CENTER,150);
	my_ctllist1.InsertColumn(5,"ʱ��",LVCFMT_CENTER,200);
	my_ctllist1.InsertColumn(6,"λ��",LVCFMT_CENTER,200);


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

	GetDlgItem(IDC_T_STATIC1)->SetFont(f);
	GetDlgItem(IDC_T_STATIC2)->SetFont(f);
	GetDlgItem(IDC_T_STATIC3)->SetFont(f);
	GetDlgItem(IDC_T_STATIC4)->SetFont(f);
	GetDlgItem(IDC_T_STATIC5)->SetFont(f);
/*	GetDlgItem(IDC_T_STATIC6)->SetFont(f);
	GetDlgItem(IDC_T_STATIC7)->SetFont(f);*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestCarDatabaseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestCarDatabaseDlg::OnPaint()
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
		if(car_Img.data)
		{
			IplImage img=car_Img;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
			DrawPicToHDC(img, IDC_T_CAR_IMG); 
		}
		if(face_Img.data)
		{
			IplImage img=face_Img;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
			DrawPicToHDC(img, IDC_T_FACE_IMG); 
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestCarDatabaseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestCarDatabaseDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//�ؼ���Ի����С���ı�
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
	{  
		ReSize();  
	}  
}


//ʹ�ؼ���Ի����С�ĸı���ı�
void CTestCarDatabaseDlg::ReSize()  
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


//�������ݿ�
void CTestCarDatabaseDlg::OnBnConDB()
{
	CDBConDlg a;
	int i=a.DoModal();

	if (i==1)
	{
		DataSourse=a.DataSourse;
		UseName=a.UseName;
		PassWord=a.PassWord;

		//����ʵ��
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

		//����ָ�����ݿ�
		if(!m_pConn->State)    
		{    
			CString sql;
			sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);
			//_bstr_t sql=(_bstr_t)sql1; 

			CString sInfo;  
			try    
			{    
				m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				sInfo.Format(_T("���ӳɹ�"));  
			}catch (_com_error e)    
			{  
				sInfo.Format(_T("����ʧ��"));  
			}    

			MessageBox(sInfo);  
		}
	}

	
}


//��ʾ���ݿ��еļ�¼��
void CTestCarDatabaseDlg::OnShowDatabase()
{
	//����ʵ��
	m_pConn.CreateInstance(__uuidof(Connection));    
	m_pRs.CreateInstance(__uuidof(Recordset));   

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

	//����ָ����
	CString biao;  
	biao.Format("select * from hr.carchrct order by car_ID");  
	m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

	my_ctllist1.DeleteAllItems();  //  ����б��  
	int nItem;  
	_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

	// �жϼ�¼��ָ���־���Ƿ��ȡ���  
	while(!m_pRs->adoEOF)  
	{  
		// ��ȡ��¼��  
		vCarNum=m_pRs->GetCollect("car_ID");
		vCarPlate=m_pRs->GetCollect("car_plate");
		vCarLogo=m_pRs->GetCollect("car_logo");
		vCarSize=m_pRs->GetCollect("car_type");
		vCarColor=m_pRs->GetCollect("car_colour");
		vCarTime=m_pRs->GetCollect("car_time");
		vCarLocate=m_pRs->GetCollect("car_locate");

		nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
		my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
		// �ƶ���¼��ָ�뵽��һ��  
		m_pRs->MoveNext();  
	}  
}


//��ϲ�ѯ
void CTestCarDatabaseDlg::OnComQuery()
{
	CComQuery a;
	int i=a.DoModal();
	
	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;
		car_time=a.car_time;
		car_locate=a.car_locate;

		//����ʵ��
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

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

		//����ָ����
		CString biao;  
		biao.Format("select * from hr.carchrct where car_plate like '%s%s%s' and car_logo like '%s%s%s' and car_type like '%s%s%s' and car_colour like '%s%s%s' and car_time like '%s%s%s'and car_locate like '%s%s%s' order by car_ID","%",car_plate,"%","%",car_logo,"%","%",car_type,"%","%",car_colour,"%","%",car_time,"%","%",car_locate,"%");  
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

		my_ctllist1.DeleteAllItems();  //  ����б��  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

		// �жϼ�¼��ָ���־���Ƿ��ȡ���  
		while(!m_pRs->adoEOF)  
		{  
			// ��ȡ��¼��  
			vCarNum=m_pRs->GetCollect("car_ID");
			vCarPlate=m_pRs->GetCollect("car_plate");
			vCarLogo=m_pRs->GetCollect("car_logo");
			vCarSize=m_pRs->GetCollect("car_type");
			vCarColor=m_pRs->GetCollect("car_colour");
			vCarTime=m_pRs->GetCollect("car_time");
			vCarLocate=m_pRs->GetCollect("car_locate");

			nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
			my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
			// �ƶ���¼��ָ�뵽��һ��  
			m_pRs->MoveNext();  
		}  
	}
	
}


//��ȷ���Ʋ�ѯ
void CTestCarDatabaseDlg::OnAcQuery()
{
	CFaQuery a;
	int i=a.DoModal();

	if (i==1)
	{
		car_plate=a.car_plate;

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

		//����ָ����
		CString biao;  
		biao.Format("select distinct car_plate,car_logo,car_type,car_colour from hr.carchrct where car_plate like '%s%s%s'","%",car_plate,"%");  //ȥ��
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

		int nTotals=0;  //��¼����

		while(!m_pRs->adoEOF)  
		{  
			nTotals++;
			m_pRs->MoveNext();  
		}  

		//����������
		if (nTotals<=1)
		{
			my_ctllist1.DeleteAllItems();  //  ����б��  
			MessageBox("�˳������������ɣ�","��ѯ���");
		} 
		else  //����������
		{
			biao.Format("select * from hr.carchrct where car_plate like '%s%s%s'","%",car_plate,"%");  
			m_pRs1->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 

			my_ctllist1.DeleteAllItems();  //  ����б��  
			int nItem;  
			_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

			// �жϼ�¼��ָ���־���Ƿ��ȡ���  
			while(!m_pRs1->adoEOF)  
			{  
				// ��ȡ��¼��  
				vCarNum=m_pRs1->GetCollect("car_ID");
				vCarPlate=m_pRs1->GetCollect("car_plate");
				vCarLogo=m_pRs1->GetCollect("car_logo");
				vCarSize=m_pRs1->GetCollect("car_type");
				vCarColor=m_pRs1->GetCollect("car_colour");
				vCarTime=m_pRs1->GetCollect("car_time");
				vCarLocate=m_pRs1->GetCollect("car_locate");

				nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
				my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				// �ƶ���¼��ָ�뵽��һ��  
				m_pRs1->MoveNext();  
			}  

			MessageBox("�˳������������ɣ���鿴�����¼��","��ѯ���");
		}
	}

	
}


//ģ�����Ʋ�ѯ
void CTestCarDatabaseDlg::OnFuQuery()
{
	CFuQuery a;
	int i=a.DoModal();

	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;

		//����ʵ��
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

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

		//����ָ����
		CString Query;

		Query.Format("CREATE TABLE hr.TEMP AS SELECT DISTINCT CAR_PLATE,CAR_LOGO,CAR_TYPE,CAR_COLOUR FROM hr.CARCHRCT WHERE car_plate like'%s%s%s' and car_logo like '%s%s%s' and car_type like '%s%s%s' and car_colour like '%s%s%s'","%",car_plate,"%","%",car_logo,"%","%",car_type,"%","%",car_colour,"%");
		_variant_t RecordsAffected;
		m_pConn->Execute((_bstr_t)Query,&RecordsAffected,adCmdText);
		CString Scrn;
		Scrn="SELECT * FROM hr.CARCHRCT WHERE CAR_PLATE IN(SELECT CAR_PLATE FROM hr.TEMP GROUP BY CAR_PLATE HAVING COUNT(*)>=2) ORDER BY CAR_PLATE";
		m_pRs->Open((_bstr_t)Scrn,(IDispatch*)m_pConn,adOpenStatic,adLockOptimistic,adCmdText);

		my_ctllist1.DeleteAllItems();  //  ����б��  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

		if (!m_pRs->adoEOF)
		{
			// �жϼ�¼��ָ���־���Ƿ��ȡ���  
			while(!m_pRs->adoEOF)  
			{  
				// ��ȡ��¼��  
				vCarNum=m_pRs->GetCollect("car_ID");
				vCarPlate=m_pRs->GetCollect("car_plate");
				vCarLogo=m_pRs->GetCollect("car_logo");
				vCarSize=m_pRs->GetCollect("car_type");
				vCarColor=m_pRs->GetCollect("car_colour");
				vCarTime=m_pRs->GetCollect("car_time");
				vCarLocate=m_pRs->GetCollect("car_locate");

				nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
				my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				// �ƶ���¼��ָ�뵽��һ��  
				m_pRs->MoveNext();  
			}  

			MessageBox("�����Ƴ����������ѯ��¼!");
		} 
		else
		{
			MessageBox("�����Ƴ�����");
		}
		CString Drop;
		Drop.Format("DROP TABLE hr.TEMP");
		m_pConn->Execute((_bstr_t)Drop,&RecordsAffected,adCmdText);
	}

	
}


//��������׷��
void CTestCarDatabaseDlg::OnTrack()
{
	CTrack a;
	int i=a.DoModal();

	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;
		car_time=a.car_time;
		car_locate=a.car_locate;
		face_Img=a.face_Img;
		matchPar=a.matchPar;

		//����ʵ��
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

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

		//����ָ����
		CString biao;  
		biao.Format("select * from hr.carchrct where  car_plate like '%s%s%s' and car_logo like '%s%s%s' and car_type like '%s%s%s' and car_colour like '%s%s%s' and car_locate like '%s%s%s' and car_time like '%s%s%s' order by car_plate,car_time","%",car_plate,"%","%",car_logo,"%","%",car_type,"%","%",car_colour,"%","%",car_locate,"%","%",car_time,"%"); 
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 


		my_ctllist1.DeleteAllItems();  //  ����б��  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  


		if (face_Img.data)
		{
			cv::Mat_<double> train_Img,assist_Img,test_Img; //ѵ�������������Ͳ��Լ�����
			int face_ID;   //ѵ����ͼƬ���
			std::string face_Path;   //����ͼƬ��·��
			cv::Mat_<double> img,imgData;
			int nTotals=0;  //��¼����

			while(!m_pRs->adoEOF)  //ͳ��һ���ж��ټ�¼
			{  
				nTotals++;
				m_pRs->MoveNext(); 
			}

			if (nTotals)
			{
				int *recordNum=new int[nTotals];   //��¼�����
				//int *recordState=new int[nTotals]();  //��¼��״̬��1��ʾʶ��ɹ���0��ʾδʶ��
				int k=0;

				m_pRs->MoveFirst();  //ָ��ع�
				// �жϼ�¼��ָ���־���Ƿ��ȡ���  
				while(!m_pRs->adoEOF)  
				{  
					//��ȡ��¼��  
					face_ID=m_pRs->GetCollect("car_ID");
					recordNum[k]=face_ID;
					k++;

					std::ostringstream oss;
					oss<<face_ID;
					face_Path="FaceImage//"+oss.str()+".jpg";

					img=cv::imread(face_Path,0);
					if(img.data)
					{
						cv::resize(img,imgData,cv::Size(20,20));
						imgData=imgData.t();
						cv::Mat_<double> colImgs=imgData.reshape(imgData.rows*imgData.cols);
						train_Img.push_back(colImgs);
					}

					m_pRs->MoveNext();  
				}  

				train_Img=train_Img.t();  //ת�ú�õ����յ�ѵ������ͼƬ���д�ţ�

				cv::FileStorage oos(".\\Assist.xml", cv::FileStorage::READ);  
				oos["Assist"] >>assist_Img;     //�õ�����������
				oos.release();


				cv::Mat test=face_Img.clone();  //����һ������ͼ��
				cv::Mat test_resize;
				cv::resize(test, test_resize,cv::Size(20,20));
				cv::Mat gray;
				// �ҶȻ�
				if(test_resize.channels()==3)
					cv::cvtColor(test_resize,gray,CV_BGR2GRAY);
				else
					gray=test_resize;

				cv::Mat_<double> testData=gray;  //��ͼ������תΪdouble�ͣ����ں��������ʶ��
				testData=testData.t();
				test_Img=testData.reshape((testData.rows)*(testData.cols));   //�õ�����ͼƬ�ľ���
				test_Img = test_Img.t();

				int RecNum=(int)(nTotals*(1-(double)(matchPar)/100));
				if (RecNum==0)
				{
					RecNum++;
				}

				//��������ʶ����
				FaceRec a;
				int *result=a.recognition(train_Img,assist_Img,test_Img,RecNum,nTotals);

				my_ctllist1.DeleteAllItems();  //  ����б��  

				for(int i=0;i<RecNum;i++)
				{
					m_pRs->MoveFirst();  //ָ��ع�
					int j=result[i];
					while(j>0)//ѭ�����������ҵ�ƥ�䵽�ļ�¼
					{
						// �ƶ���¼��ָ�뵽��һ��  
						m_pRs->MoveNext();  
						j--;
					}

					// ��ȡ��¼��  
					vCarNum=m_pRs->GetCollect("car_ID");
					vCarPlate=m_pRs->GetCollect("car_plate");
					vCarLogo=m_pRs->GetCollect("car_logo");
					vCarSize=m_pRs->GetCollect("car_type");
					vCarColor=m_pRs->GetCollect("car_colour");
					vCarTime=m_pRs->GetCollect("car_time");
					vCarLocate=m_pRs->GetCollect("car_locate");

					nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
					my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
					my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
					my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
					my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
					my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
					my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				} 
			}
			else
			{
				my_ctllist1.DeleteAllItems();  //  ����б��
			}
		} 
		else
		{
			// �жϼ�¼��ָ���־���Ƿ��ȡ���  
			while(!m_pRs->adoEOF)  
			{  
				// ��ȡ��¼��  
				vCarNum=m_pRs->GetCollect("car_ID");
				vCarPlate=m_pRs->GetCollect("car_plate");
				vCarLogo=m_pRs->GetCollect("car_logo");
				vCarSize=m_pRs->GetCollect("car_type");
				vCarColor=m_pRs->GetCollect("car_colour");
				vCarTime=m_pRs->GetCollect("car_time");
				vCarLocate=m_pRs->GetCollect("car_locate");

				nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
				my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
				my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
				my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
				// �ƶ���¼��ָ�뵽��һ��  
				m_pRs->MoveNext();  
			}  
		}
	}
}



//����¼��
void CTestCarDatabaseDlg::OnInputOne()
{
	CInputOne a;

	a.DataSourse=DataSourse;
	a.UseName=UseName;
	a.PassWord=PassWord;

	a.DoModal();
	
}

//ѡ���б��е�ĳһ��
void CTestCarDatabaseDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(pNMItemActivate != NULL)
	{
		nItem = pNMItemActivate->iItem;
	}

	*pResult = 0;
}

//��ʾͼƬ
void CTestCarDatabaseDlg::OnShowImg()
{
	if(nItem<0){
		AfxMessageBox("����ѡ���б���е�һ�У�");
		return;
	}

	GetDlgItem(IDC_T_STATIC2)->ShowWindow(true);
	GetDlgItem(IDC_T_STATIC3)->ShowWindow(true);
	GetDlgItem(IDC_T_STATIC4)->ShowWindow(true);
	GetDlgItem(IDC_T_STATIC5)->ShowWindow(true);
	GetDlgItem(IDC_T_CAR_IMG)->ShowWindow(true);
	GetDlgItem(IDC_T_FACE_IMG)->ShowWindow(true);
	GetDlgItem(IDC_T_PLATE_IMG)->ShowWindow(true);
	GetDlgItem(IDC_T_LOGO_IMG)->ShowWindow(true);

	CString CarID=my_ctllist1.GetItemText(nItem,0);  //��ȡ�����
	CString car_Path="CarImage//"+CarID+".jpg";   //·��
	CString face_Path="FaceImage//"+CarID+".jpg";
	CString plate_Path="PlateImage//"+CarID+".jpg";
	CString logo_Path="LogoImage//"+CarID+".jpg";

	face_Img=cv::imread(face_Path.GetBuffer(0));
	car_Img=cv::imread(car_Path.GetBuffer(0));
	plate_Img=cv::imread(plate_Path.GetBuffer(0));
	logo_Img=cv::imread(logo_Path.GetBuffer(0));

	IplImage img1=car_Img;    //��mat��ʽתΪIplImage���Ա��ڿؼ�����ʾͼƬ
	IplImage img2=face_Img;
	IplImage img3=plate_Img;
	IplImage img4=logo_Img;
	DrawPicToHDC(img1, IDC_T_CAR_IMG); 
	DrawPicToHDC(img2, IDC_T_FACE_IMG); 
	DrawPicToHDC(img3, IDC_T_PLATE_IMG); 
	DrawPicToHDC(img4, IDC_T_LOGO_IMG); 

	nItem=-1;
}

//����ͼƬ
void CTestCarDatabaseDlg::OnHideImg()
{
	GetDlgItem(IDC_T_STATIC2)->ShowWindow(false);
	GetDlgItem(IDC_T_STATIC3)->ShowWindow(false);
	GetDlgItem(IDC_T_STATIC4)->ShowWindow(false);
	GetDlgItem(IDC_T_STATIC5)->ShowWindow(false);
	GetDlgItem(IDC_T_CAR_IMG)->ShowWindow(false);
	GetDlgItem(IDC_T_FACE_IMG)->ShowWindow(false);
	GetDlgItem(IDC_T_PLATE_IMG)->ShowWindow(false);
	GetDlgItem(IDC_T_LOGO_IMG)->ShowWindow(false);
}


//��ͼƬ��ʾ��picture�ؼ���
void CTestCarDatabaseDlg::DrawPicToHDC(IplImage img, UINT ID)
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


//ɾ��ָ����¼
void CTestCarDatabaseDlg::OnDeleteData()
{
	if(nItem<0){
		AfxMessageBox("����ѡ���б���е�һ�У�");
		return;
	}
	CString CarID=my_ctllist1.GetItemText(nItem,0);  //��ȡ�����

	CString car_Path="CarImage//"+CarID+".jpg";   //·��
	CString face_Path="FaceImage//"+CarID+".jpg";
	int car__id=atoi(CarID);       //��CStringת����Int����

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

	//����ָ����
	CString biao;  
	biao.Format("DELETE FROM HR.CARCHRCT WHERE CAR_ID ='%d'",car__id);  
	m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText);   //ɾ����¼

	//ɾ��ͼƬ
	DeleteFile(car_Path);
	DeleteFile(face_Path);

	CString biao2;  
	biao2.Format("select * from hr.carchrct order by car_ID");  
	m_pRs1->Open((_bstr_t)biao2,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
	my_ctllist1.DeleteAllItems();  //  ����б��  
	int nItem;  
	_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

	// �жϼ�¼��ָ���־���Ƿ��ȡ���  
	while(!m_pRs1->adoEOF)  
	{  
		// ��ȡ��¼��  
		vCarNum=m_pRs1->GetCollect("car_ID");
		vCarPlate=m_pRs1->GetCollect("car_plate");
		vCarLogo=m_pRs1->GetCollect("car_logo");
		vCarSize=m_pRs1->GetCollect("car_type");
		vCarColor=m_pRs1->GetCollect("car_colour");
		vCarTime=m_pRs1->GetCollect("car_time");
		vCarLocate=m_pRs1->GetCollect("car_locate");

		nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
		my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
		my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
		my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
		// �ƶ���¼��ָ�뵽��һ��  
		m_pRs1->MoveNext();  
	}  
	MessageBox("ɾ���ɹ���");
}


//��������
void CTestCarDatabaseDlg::OnUpdataData()
{
	if(nItem<0){
		AfxMessageBox("����ѡ���б���е�һ�У�");
		return;
	}
	CString CarID=my_ctllist1.GetItemText(nItem,0);  //��ȡ�����
	int car__id=atoi(CarID);       //��CStringת����Int����

	CUpdataData a;
	int i=a.DoModal();

	if (i==1)
	{
		car_ID=a.car_ID;
		car_plate=a.car_plate;
		car_logo=a.car_logo;
		car_type=a.car_type;
		car_colour=a.car_colour;
		car_time=a.car_time;
		car_locate=a.car_locate;

		//����ʵ��
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   

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

		_RecordsetPtr m_pRs1;//��¼��ָ��
		m_pRs1.CreateInstance(__uuidof(Recordset));

		if (car_plate!="")
		{
			//����ָ����
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_PLATE='%s' WHERE CAR_ID = '%d'",car_plate,car__id); 
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}
		
		if (car_logo!="")
		{
			//����ָ����
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_LOGO='%s' WHERE CAR_ID = '%d'",car_logo,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_type!="")
		{
			//����ָ����
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_TYPE='%s' WHERE CAR_ID = '%d'",car_type,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_colour!="")
		{
			//����ָ����
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_COLOUR='%s' WHERE CAR_ID = '%d'",car_colour,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_time!="")
		{
			//����ָ����
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_TIME='%s' WHERE CAR_ID = '%d'",car_time,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}

		if (car_locate!="")
		{
			//����ָ����
			CString sql1;  
			sql1.Format("UPDATE HR.CARCHRCT SET CAR_LOCATE='%s' WHERE CAR_ID = '%d'",car_locate,car__id);  
			m_pRs1->Open((_bstr_t)sql1,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		}


		CString biao;  
		biao.Format("select * from hr.carchrct order by car_ID");  
		m_pRs->Open((_bstr_t)biao,_variant_t((IDispatch  *)m_pConn,true),adOpenStatic,adLockOptimistic,adCmdText); 
		my_ctllist1.DeleteAllItems();  //  ����б��  
		int nItem;  
		_variant_t vCarNum,vCarPlate,vCarLogo,vCarSize,vCarColor,vCarTime,vCarLocate;  

		// �жϼ�¼��ָ���־���Ƿ��ȡ���  
		while(!m_pRs->adoEOF)  
		{  
			// ��ȡ��¼��  
			vCarNum=m_pRs->GetCollect("car_ID");
			vCarPlate=m_pRs->GetCollect("car_plate");
			vCarLogo=m_pRs->GetCollect("car_logo");
			vCarSize=m_pRs->GetCollect("car_type");
			vCarColor=m_pRs->GetCollect("car_colour");
			vCarTime=m_pRs->GetCollect("car_time");
			vCarLocate=m_pRs->GetCollect("car_locate");

			nItem = my_ctllist1.InsertItem(0xffff,(_bstr_t)vCarNum);  
			my_ctllist1.SetItem(nItem,1,1,(_bstr_t)vCarPlate,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,2,1,(_bstr_t)vCarLogo,NULL,0,0,0);  
			my_ctllist1.SetItem(nItem,3,1,(_bstr_t)vCarSize,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,4,1,(_bstr_t)vCarColor,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,5,1,(_bstr_t)vCarTime,NULL,0,0,0);
			my_ctllist1.SetItem(nItem,6,1,(_bstr_t)vCarLocate,NULL,0,0,0);
			// �ƶ���¼��ָ�뵽��һ��  
			m_pRs->MoveNext();  
		}  

		MessageBox("���³ɹ���");
	}
}


//����¼��
void CTestCarDatabaseDlg::OnInputMulti()
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


	//ѡ��ͼƬ�ļ���·��
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner =GetSafeHwnd();
	bInfo.lpszTitle ="��ѡ��ͼƬ�ļ���·��:";
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS; 

	LPITEMIDLIST lpDlist; //�������淵����Ϣ��IDList
	lpDlist = SHBrowseForFolder(&bInfo); //��ʾѡ��Ի���
	if(lpDlist != NULL) //�û�����ȷ����ť
	{
		TCHAR chPath[MAX_PATH]; //�����洢·�����ַ���
		SHGetPathFromIDList(lpDlist, chPath);//����Ŀ��ʶ�б�ת�����ַ���
		picturePath = chPath; //��TCHAR���͵��ַ���ת��ΪCString���͵��ַ���
	}


	// ѡ��txt�ļ����洢ͼƬ��Ϣ���ļ�����·��
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files(*.txt)||*.txt"), NULL);                           
	dlg.m_ofn.lpstrTitle = _T("ѡ��洢ͼƬ��Ϣ��txt�ļ�");    // ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )                    // �ж��Ƿ����ļ�
		return;

	txtPath= dlg.GetPathName();            // ��ȡ�ļ�·��


	//����ͼƬ�ļ��С���ȡtxt�ļ�
	if ((txtPath!="")&&(picturePath!=""))
	{
		//����ͼƬ�ļ����������
		CFileFind ff;
		CString strUDDir = picturePath;
		CString sk=strUDDir+"\\*.*";
		BOOL res = ff.FindFile(sk);
		CString trainImgPath;
		cv::Mat carImage;   //���Ŷ�ȡ����ͼƬ

		//��ȡtxt�ļ��������
		std::ifstream in(txtPath);  //��ָ����txt�ļ�
		std::string line,word;   //txt�е�ÿ�У�ÿ���е�ÿ������
		int nextID=0;

		//����ͼƬ�ļ���
		while(res)
		{
			res = ff.FindNextFile();
			//��������Ŀ¼
			if(!ff.IsDirectory() && !ff.IsDots())
			{
				//��txt�ļ������ж�ȡ���ݲ�����line�У�ֱ������ȫ����ȡ
				std::getline(in,line);
				std::istringstream ss(line);
				int i=1;
				while(ss>>word)
				{
					if(i==1)
						car_plate=word.c_str();
					if(i==2)
						car_logo=word.c_str();
					if(i==3)
						car_type=word.c_str();
					if(i==4)
						car_colour=word.c_str();
					if(i==5)
						car_time=word.c_str();
					if(i==6)
						car_locate=word.c_str();
					i++;    
				}

				if (car_plate!=""&&car_logo!=""&&car_type!=""&&car_colour!=""&&car_time!=""&&car_locate!="")
				{
					//��ȡͼƬ����
					trainImgPath=ff.GetFilePath();
					carImage=cv::imread(trainImgPath.GetBuffer(0),1);

					//��ȡ��һ��car_ID
					if (nextID==0)
					{
						CString sql_next="SELECT MAX(CAR_ID)  MAXID FROM hr.CARCHRCT";
						m_pRs1->Open((_bstr_t)sql_next,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);
						try
						{
							nextID = m_pRs1->GetCollect("MAXID");
							nextID++;
						}
						catch (_com_error e)
						{
							nextID = 1;
						}

					} 
					else
					{
						nextID++;
					}
					

					//����ͼƬ�����������ơ�����ȣ�
					cv::Mat ImageData; 

					//��ȡ����
					//��ȡ��������
					FaceDetect2 fd;
					if(carImage.data)
					{
						fd.draw(carImage);
						cv::Mat faceImage = fd.face_Image; //����FaceDetect���������⺯��

						if(faceImage.data)
							cv::resize(faceImage,ImageData,cv::Size(150,150));
						else
						{
							MessageBox("û����ȡ����������","������ʾ��");
							return;
						}
					}
					else
					{
						MessageBox("����ͼƬ����","������ʾ��");
						return;
					}  

					//���滹����ȡ���ơ����������



					// ���泵�������������ơ�����ͼƬ��ָ���ļ���
					std::string car_path,face_path;  //·��
					std::ostringstream oss;
					oss<<nextID;
					car_path="CarImage//"+oss.str()+".jpg";
					face_path="FaceImage//"+oss.str()+".jpg";
					cv::imwrite(face_path,ImageData);
					cv::imwrite(car_path,carImage);


					//����ָ����
					CString sql="SELECT * FROM hr.CARCHRCT";

					if (m_pRs->State)
					{
						;
					//	m_pRs->Close();
					//	m_pRs->Release();
					} 
					else
					{
						m_pRs->Open((_bstr_t)sql,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);
					}
					
					try  
					{   
						CString sql1;
						sql1.Format("insert into hr.CARCHRCT (car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate) values ('%d','%s','%s','%s','%s','%s','%s')", nextID,car_plate,car_logo,car_type,car_colour,car_time,car_locate);
						m_pConn->Execute((_bstr_t)sql1,NULL,adExecuteNoRecords);  
						MessageBox("��ӳɹ���","��ʾ");  
					}  
					catch(_com_error e)  
					{  
						MessageBox("���ʧ�ܣ�","��ʾ"); 
					}  
				}
				else
					MessageBox("TXT�ļ����������ݿⲻƥ�䣡");
						
			}
		}

		ff.Close(); // ��Ҫ���ǹر�	
		MessageBox("���������ɣ�");
	}
	else
	{
		MessageBox("��ѡ����ȷ·��","������ʾ��");
	}
}


//������ʾ����
void CTestCarDatabaseDlg::OnStep()
{
	CStep a;
	a.DoModal();
}

//������ʾ����
void CTestCarDatabaseDlg::OnConstant()
{
	CConstant a;
	a.DoModal();
}
