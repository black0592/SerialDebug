// SerialDebugDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SerialDebugDlg.h"
#include "EnumSerial.h"
#include "crc.h"
#include <Atlbase.h>

#define IDM_SETTINGBOX                   0x0020
#define IDM_FORMERBOX                    0x0021
//#define IDM_NEWVERSION                   0x0022

#define IDM_WIKI		                 0x0023
#define IDM_MANAGER		                 0x0024
#define IDM_WINMANAGER	                 0x0025
//#define IDM_FORMERBOX                    0x0024
//#define IDM_NEWVERSION                   0x0025

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HOTKEY_SEND 0x1000
#define HOTKEY_PASTEDEC 0x1001
#define HOTKEY_PASTEHEX 0x1002
#define UM_TRAYNOTIFY WM_USER + 0x01	//�����Զ�����Ϣ��ID


int e = 1;
CInFoDialog InfoDlg;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	
// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink10(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK10, &CAboutDlg::OnNMClickSyslink10)
END_MESSAGE_MAP()

// CSerialDebugDlg �Ի���


CSerialDebugDlg::CSerialDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialDebugDlg::IDD, pParent)
	, saddr(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	isendcount = 0;
	iresvcount = 0;
	memset(&m_nid, 0, sizeof(m_nid));	//memset:���һ���ṹ�����͵ı���.Memset������һ���ڴ�ռ�ȫ������Ϊĳ���ַ���һ�����ڶԶ�����ַ������г�ʼ��Ϊ' '��'\0'��
	m_nid.cbSize = sizeof(m_nid);	//Ϊm_nid�ṹ�����ռ�
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;	//��־��������Щ��Ա����������Ч�ģ�NIF_ICON,NIF_MESSAGE,NIF_TIP���ֱ������������Ч�ĳ�Ա��hIcon,uCallbackMessage,szTip������ֵ�����á�|����ϵ��һ��
	wcscpy_s(m_nid.szTip, L"ѩ�������ڵ�������");//ͼ����ʾΪ"���Գ���"
	
}

void CSerialDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_SEND, SendContent);
	DDX_Control(pDX, IDC_EDIT_RESV, ResvContent);
	DDX_Control(pDX, IDC_BUTTON_SEND, SendButton);
	DDX_Control(pDX, IDC_EDIT_ADDDEC, MEditAddDec);
	DDX_Control(pDX, IDC_EDIT_ADDHEX, MEditAddHex);
	DDX_Control(pDX, IDC_EDIT_SENDCON, edSendCount);
	DDX_Control(pDX, IDC_EDIT3, edResvCount);
	DDX_Control(pDX, IDC_CHECK5, chkFilesSend);
	DDX_Control(pDX, IDC_CHECK6, ckbFileResv);
	DDX_Control(pDX, IDC_EDIT_LOOPTIME, cedtLoopTime);
	DDX_Control(pDX, IDC_CHECK1, cchkLoopEn);
	DDX_Control(pDX, IDC_RICHEDIT21, statusbar);
	DDX_Control(pDX, IDC_STATIC_ADD, statAdd);
	DDX_Control(pDX, IDC_EDIT2, modehex);
	//DDX_Control(pDX, IDC_STATIC_MODE, editmode);
	DDX_Control(pDX, IDC_COMBO_PORT, ComboPort);
	DDX_Control(pDX, IDC_CHECK_AUTOOPEN, chkAutoOpen);
	DDX_Control(pDX, IDC_BTN_APISEND, btnAPISend);
}

BEGIN_MESSAGE_MAP(CSerialDebugDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, &CSerialDebugDlg::OnCbnDropdownComPort)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSerialDebugDlg::OnBnClickedButtonOpen)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, &CSerialDebugDlg::OnCbnSelchangeComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CSerialDebugDlg::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_PARITY, &CSerialDebugDlg::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA, &CSerialDebugDlg::OnCbnSelchangeCombo)
	ON_CBN_SELCHANGE(IDC_COMBO_STOP, &CSerialDebugDlg::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSerialDebugDlg::OnBnClickedButtonSend)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_CLEAROUT, &CSerialDebugDlg::OnNMClickSyslinkClearout)
	ON_BN_CLICKED(IDC_CHECK_STOP, &CSerialDebugDlg::OnBnClickedCheckStop)
	ON_BN_CLICKED(IDC_CHECK_BORTRATE_MODE, &CSerialDebugDlg::OnBnClickedCheckBortrateMode)
	ON_EN_CHANGE(IDC_EDIT_BORTRATE, &CSerialDebugDlg::OnEnChangeEditBortrate)
	ON_BN_CLICKED(IDC_CHECK_HEXOUT, &CSerialDebugDlg::OnBnClickedCheckHexout)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CSerialDebugDlg::OnNMClickSyslink1)
	ON_BN_CLICKED(IDC_CHECK_SENDHEX, &CSerialDebugDlg::OnBnClickedCheckSendhex)
	ON_CBN_SELCHANGE(IDC_COMBO_MULTIHOST_MODE, &CSerialDebugDlg::OnCbnSelchangeComboMultihostMode)
	ON_EN_CHANGE(IDC_EDIT_ADDDEC, &CSerialDebugDlg::OnEnChangeEditAdddec)
	ON_EN_CHANGE(IDC_EDIT_ADDHEX, &CSerialDebugDlg::OnEnChangeEditAddhex)
	ON_BN_CLICKED(IDC_CHECK3, &CSerialDebugDlg::OnBnClickedCheck3)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &CSerialDebugDlg::OnNMClickSyslink2)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK4, &CSerialDebugDlg::OnNMClickSyslink4)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK5, &CSerialDebugDlg::OnNMClickSyslink5)
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_CHECK5, &CSerialDebugDlg::OnBnClickedCheck5)
	ON_EN_CHANGE(IDC_EDIT_SEND, &CSerialDebugDlg::OnEnChangeEditSend)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK3, &CSerialDebugDlg::OnNMClickSyslink3)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK6, &CSerialDebugDlg::OnNMClickSyslink6)
	ON_BN_CLICKED(IDC_CHECK6, &CSerialDebugDlg::OnBnClickedCheck6)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK7, &CSerialDebugDlg::OnNMClickSyslink7)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK8, &CSerialDebugDlg::OnNMClickSyslink8)
	//ON_EN_CHANGE(IDC_EDIT_LOOPTIME, &CSerialDebugDlg::OnEnChangeEditLooptime)
	ON_WM_TIMER()
	ON_CBN_SETFOCUS(IDC_COMBO_PORT, &CSerialDebugDlg::OnCbnSetfocusComboPort)
	ON_CBN_SELENDCANCEL(IDC_COMBO_PORT, &CSerialDebugDlg::OnCbnSelendcancelComboPort)
	ON_CBN_EDITCHANGE(IDC_COMBO_PORT, &CSerialDebugDlg::OnCbnEditchangeComboPort)
	ON_CBN_EDITUPDATE(IDC_COMBO_PORT, &CSerialDebugDlg::OnCbnEditupdateComboPort)
	ON_CBN_KILLFOCUS(IDC_COMBO_PORT, &CSerialDebugDlg::OnCbnKillfocusComboPort)
	ON_MESSAGE(UM_TRAYNOTIFY, &CSerialDebugDlg::OnUmTraynotify) //ע����Ϣ
	//ON_STN_CLICKED(IDC_STATIC_MODE, &CSerialDebugDlg::OnStnClickedStaticMode)
	//ON_STN_DBLCLK(IDC_STATIC_MODE, &CSerialDebugDlg::OnStnDblclickStaticMode)
	ON_BN_CLICKED(IDC_CHECK_DTR, &CSerialDebugDlg::OnBnClickedCheckDtr)
	ON_BN_CLICKED(IDC_CHECK_RTS, &CSerialDebugDlg::OnBnClickedCheckRts)
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON2, &CSerialDebugDlg::OnBnClickedButton2)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCHITTEST()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_APISEND, &CSerialDebugDlg::OnBnClickedBtnApisend)
END_MESSAGE_MAP()


// CSerialDebugDlg ��Ϣ�������

void ThreadRecv(CSerialDebugDlg * pClass);
void ThreadSend(CSerialDebugDlg * pClass);
void ThreadMoniter(CSerialDebugDlg * pClass);
void OpenMoniter(CSerialDebugDlg * pClass);		//���Դ򿪴����߳�

BOOL CSerialDebugDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	m_strTitle = L"ѩ�������ڵ������� V1.3.0";
	CMenu menu;
	menu.CreateMenu();
	menu.AppendMenu(MF_STRING,IDM_WINMANAGER,L"Windows�豸������");
	//menu.AppendMenu(MF_STRING,112,"Bye");


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;	 //�ⲿ�ִ����ǻ�ȡABOUTBOX�ؼ����ַ���ID����������ֵ���ַ���
		CString strSettingMenu;
		CString strFormerMenu;
		CString strNewversionMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);		//�� ��Դ ��� �ַ�����Դ �����ַ�����CString������
		strSettingMenu.LoadString(IDS_SETTINGBOX);
		strFormerMenu.LoadString(IDS_FORMERBOX);
		strNewversionMenu.LoadString(IDS_NEWVERSION);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_SETTINGBOX, strSettingMenu);			
			//pSysMenu->AppendMenu(MF_STRING, IDM_MANAGER, "���ڹ���");	
			pSysMenu->AppendMenu(MF_STRING, IDM_FORMERBOX, strFormerMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_WIKI, L"���ڰٿ�");
			pSysMenu->AppendMenu(MF_SEPARATOR);		 //����һ��ˮƽ�ָ���
			pSysMenu->AppendMenu(MF_STRING, IDM_NEWVERSION, strNewversionMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);	//���ơ����ڡ��˵�
			pSysMenu->InsertMenu(9,MF_POPUP|MF_BYPOSITION,(UINT)menu.m_hMenu,L"���ڹ���");
		}
	}

	menuSys.LoadMenu(IDR_MENU_SYS);	//װ��˵�IDR_MENU_TRAY

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	TCHAR lpBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,lpBuffer);	//��õ�ǰִ�е�exe���ļ��ľ���·��,��õ�·���浽�� lpBuffer�ַ���������
	CString VendorListPath(lpBuffer);	//��lpBuffer ��ʼ���� VendorListPath��
	McuDirectory = VendorListPath;

	GetClientRect(&m_rect);

	CWnd *pWnd;
    pWnd = GetDlgItem(IDC_EDIT_RESV);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	pWnd = GetDlgItem(IDC_EDIT_SEND);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect2);
	ScreenToClient(&rect2);
	pWnd = GetDlgItem(IDC_BUTTON_SEND);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect3);
	ScreenToClient(&rect3);
	pWnd = GetDlgItem(IDC_EDIT_SENDCON);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect4);
	ScreenToClient(&rect4);
	pWnd = GetDlgItem(IDC_EDIT3);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect5);
	ScreenToClient(&rect5);
	pWnd = GetDlgItem(IDC_SYSLINK5);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect6);
	ScreenToClient(&rect6);
	pWnd = GetDlgItem(IDC_SYSLINK4);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect7);
	ScreenToClient(&rect7);
	pWnd = GetDlgItem(IDC_SYSLINK2);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect8);
	ScreenToClient(&rect8);
	pWnd = GetDlgItem(IDC_SPLITTER1);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect9);
	ScreenToClient(&rect9);
	pWnd = GetDlgItem(IDC_COMBO_PORT);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect10);
	ScreenToClient(&rect10);
	pWnd = GetDlgItem(IDC_STATIC_ADD);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect11);
	ScreenToClient(&rect11);
	pWnd = GetDlgItem(IDC_EDIT2);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect12);
	ScreenToClient(&rect12);
	pWnd = GetDlgItem(IDC_BTN_APISEND);     //��ȡ�ؼ����
	pWnd->GetWindowRect(&rect13);
	ScreenToClient(&rect13);
	//E_OUTOFMEMORY
	OnCbnDropdownComPort();
	//MessageBox(SerialCount);
	OnCbnSelchangeComboPort();
	OnCbnEditupdateComboPort();
	OnCbnSelendcancelComboPort();
	
	CComboBox *pComboBox;
	if(SerialCount!=0){
	pComboBox	= (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
		pComboBox->SetCurSel(0);  
	}
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MULTIHOST_MODE);
	pComboBox->SetCurSel(0);  
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	pComboBox->SetCurSel(12);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PARITY);
	pComboBox->SetCurSel(0);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DATA);
	pComboBox->SetCurSel(4);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STOP);
	pComboBox->SetCurSel(0);

	CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_DTR);
	pButton->ModifyStyle(0,WS_DISABLED);
	pButton = (CButton*)GetDlgItem(IDC_CHECK_RTS);
	pButton->ModifyStyle(0,WS_DISABLED);

	((CEdit *)GetDlgItem(IDC_EDIT_BORTRATE))->SetLimitText(6);
	((CEdit *)GetDlgItem(IDC_EDIT_BORTRATE))->SetWindowText(L"115200");
	((CEdit *)GetDlgItem(IDC_BUTTON_SEND))->SetLimitText(4096);

	CRect rc;

	pWnd = GetDlgItem(IDC_SPLITTER1);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSplitter.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER);
	m_wndSplitter.SetRange(400, 120, -1);

	DWORD style = WS_VISIBLE|WS_CHILD;

	CString port;
	portopen = false;
	if(SerialCount!=0)
	{
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);	
		pComboBox->GetLBText(pComboBox->GetCurSel(),port);
		port.Delete(0,3); 
		int pos = port.Find('-');
		port = port.Left(pos-1);
	}

	baudrate = CBR_115200;
	bytesize = 8;
	parity = NOPARITY;
	stopbits = ONESTOPBIT;
	m_parity = 0;
	readnow = false;
	writenow = false;
	showhex = false;
	sendhex = false;
	sendRun = false;
	timerrun = false;
	looptime = 1;
	manual = false;
	timer = 100;

	MEditAddDec.SetAllowedString(L"0123456789");
	MEditAddDec.SetLimitText(3);
	MEditAddDec.SetWindowText(L"001");
	MEditAddHex.SetAllowedString(L"0123456789abcdefABCDEF");
	MEditAddHex.SetLimitText(2);
	MEditAddHex.SetWindowText(L"01");
	cedtLoopTime.SetLimitText(4);
	cedtLoopTime.SetWindowText(L"1");
	saddr = 0x01;
	CAddDiag.iAddType = 0;
	edSendCount.SetWindowText(L"���ͣ�0");
	edResvCount.SetWindowText(L"���գ�0");

	//::RegisterHotKey(m_hWnd,HOTKEY_SEND,MOD_CONTROL,VK_RETURN); 
	//::RegisterHotKey(SendContent.m_hWnd,HOTKEY_PASTEDEC,MOD_CONTROL,'D'); 
	//::RegisterHotKey(SendContent.m_hWnd,HOTKEY_PASTEHEX,MOD_CONTROL,'H'); 
	
	m_nid.hWnd = GetSafeHwnd();		//�õ�һ�����ڶ���CWnd����������ָ��ľ����HWND��
	m_nid.uCallbackMessage = UM_TRAYNOTIFY;		//��Ϣ��־������������������Ӧͼ����в�����ʱ�򣬾ͻᴫ����Ϣ��Hwnd������Ĵ��ڣ�һ��Ϊ�Զ������Ϣ

	// Set tray icon and tooltip
	m_nid.hIcon = m_hIcon;	//��������ͼ��
	//((CButton*)GetDlgItem(IDC_CHECK_AUTOENTER))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_BORTRATE_MODE))->SetCheck(BST_CHECKED);
	OnBnClickedCheckBortrateMode();
	SendContent.insert = true;
	InfoDlg.Create(IDD_DIALOG_INFO,this);

	ComboPort.Init();

	CHARFORMAT2 cf1;
	CHARFORMAT cf;

	statusbar.SetWindowText(L"���ڹر�״̬");

		//����ΪCHARFORMAT2�ṹ����ϸ��MSDN
    ZeroMemory(&cf1, sizeof(CHARFORMAT2));
    cf1.cbSize = sizeof(CHARFORMAT2);
    cf1.dwMask = CFM_BACKCOLOR;

    cf1.crBackColor=RGB(255, 0, 0); //������ɫΪ��ɫ
    statusbar.SetSel(0, 6); //���ô�������
    statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

    ZeroMemory(&cf, sizeof(CHARFORMAT));
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
                        CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
    cf.dwEffects = 0;
    cf.yHeight = 20*20;//���ָ߶�
    cf.crTextColor = RGB(255, 255, 255); //������ɫ
	wcscpy(cf.szFaceName, L"����");//��������
    statusbar.SetSel(0, 6); //���ô�������
    statusbar.SetSelectionCharFormat(cf);
	statusbar.Invalidate();
	
	hasapidata = false;
	hDllInst = NULL;

	hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadRecv,(void *)this,0,&ThreadID);
	hThreadSend = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadSend,(void *)this,0,&ThreadID);
	hThreadMoniter = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadMoniter,(void *)this,0,&ThreadID);
	hOpenMoniter = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OpenMoniter,(void *)this,0,&ThreadID);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSerialDebugDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == IDM_SETTINGBOX)
	{
		MySettingDlg.IniPath = McuDirectory;
		MySettingDlg.DoModal();
	}
	else if(nID == SC_CLOSE)
	{
		//MessageBox("aaa");
		int nRet = -1;
		CString ConfigFile = McuDirectory + L"\\.\\config.ini";
		//MessageBox(ConfigFile);
		int CloseType; 
		int CloseAuto = GetPrivateProfileInt(L"SD",L"CLOSEAUTO",1,ConfigFile.GetBuffer());	//GetPrivateProfileInt():��˽�г�ʼ���ļ���ȡ������ֵ 
		if(CloseAuto == 0){
			writting = true;
			nRet = MyClosedlg.DoModal();	//���øó�Ա����ʹ��ģ̬�Ի��򲢷��ضԻ�����������������ܴ����Ի����򷵻�-1������������������򷵻�IDABORT��
			writting = false;
			int CheckState = MyClosedlg.Checked;
			if(CheckState == 1)
			{
				WritePrivateProfileString(L"SD",L"CLOSEAUTO",L"1",ConfigFile.GetBuffer());
			}
			//BST_UNCHECKED
			switch ( nRet )
			{
				case IDOK:
					if(CheckState == 1)
					{
						WritePrivateProfileString(L"SD",L"CLOSETYPE",L"0",ConfigFile.GetBuffer());
					}
					CDialog::OnSysCommand(nID, lParam);
				break;
				case IDC_MINI:
					if(CheckState == 1)
					{
						WritePrivateProfileString(L"SD",L"CLOSETYPE",L"1",ConfigFile.GetBuffer());
					}
					ShowWindow(SW_HIDE);	//����������
					Shell_NotifyIcon(NIM_ADD, &m_nid);	//�����������ͼ��
				break;
				case IDCANCEL:
				break;
				default:
					CDialog::OnSysCommand(nID, lParam);
				break;
			}
		}
		else {
			CloseType = GetPrivateProfileInt(L"SD",L"CLOSETYPE",1,ConfigFile.GetBuffer());
			if(CloseType == 0)
			{
				CDialog::OnSysCommand(nID, lParam);
			}
			else{
				ShowWindow(SW_HIDE);	//����������
				Shell_NotifyIcon(NIM_ADD, &m_nid);	//�����������ͼ��
			}
		}
        return;
    }else if(nID == IDM_FORMERBOX)
	{

	}	
	else	 //��������Ҳ����ûѡ�С����ڡ������á��˵�
	{
		CDialog::OnSysCommand(nID, lParam);		//����CDialog���е�OnSysCommand������Ϣ����
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSerialDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSerialDebugDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSerialDebugDlg::OnCbnDropdownComPort()
{
	
	CString SeialPortDesc;
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEND);
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	pComboBox->ResetContent();


	SerialCount = 0;// ���ڼ���
	CString strSerialList[256];  // ��ʱ���� 256 ���ַ�����
	CArray<SSerInfo,SSerInfo&> asi;
	SSerInfo mySer;
	EnumSerialPorts(asi,FALSE);// ����Ϊ TRUE ʱö�ٵ�ǰ���Դ򿪵Ĵ��ڣ� // ����ö�����д���

	SerialCount = asi.GetSize();

    for (int i=0; i<asi.GetSize(); i++)
    {
        mySer = asi[i];
		SeialPortDesc = L"";
		SeialPortDesc += mySer.strPortName; 
		SeialPortDesc += L" - " + mySer.strPortDesc; 
		pComboBox->AddString(SeialPortDesc);
		mySer.strFriendlyName;
    }

	if(SerialCount == 0)
	{
		pComboBox = (CComboBox*)GetDlgItem(IDC_BUTTON_OPEN);
		//pComboBox->ModifyStyle(0,WS_DISABLED);
		//pComboBox->Invalidate();
		MessageBox(L"���ĵ���û�д��ڣ�������Ҫ����USBת�����豸��������ʹ�ñ�����",L"ע��");
		pComboBox->AddString(L"COM1");
		//SerialCount++; 
	}
	else
	{
		pComboBox = (CComboBox*)GetDlgItem(IDC_BUTTON_OPEN);
		pComboBox->ModifyStyle(WS_DISABLED,0);
		pComboBox->Invalidate();
		CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
		pComboBox->SetCurSel(0);
	}

	/*CRegKey RegKey;   

    SerialCount = 0;   


    if(RegKey.Open(HKEY_LOCAL_MACHINE, "Hardware\\DeviceMap\\SerialComm") == ERROR_SUCCESS)   
    {   
        while(true)   
        {   
            char ValueName[_MAX_PATH];   
            unsigned char ValueData[_MAX_PATH];   
            DWORD nValueSize = _MAX_PATH;   
            DWORD nDataSize = _MAX_PATH;   
            DWORD nType;   
            
			if(::RegEnumValue(HKEY(RegKey), SerialCount, ValueName, &nValueSize, NULL, &nType, ValueData, &nDataSize) == ERROR_NO_MORE_ITEMS)   
            {   
                break;   
            }                 

			pComboBox->AddString(CString(ValueData));
			SerialCount++;   
        }   
		RegKey.Close();
    }  */

}
LRESULT CSerialDebugDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITTER)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoResize(pHdr->delta);
		}
	}	
	LRESULT lrst  = CDialog::DefWindowProc(message, wParam, lParam);
	
	if (!::IsWindow(m_hWnd))
		return lrst;
	
	if (message==WM_MOVE||message==WM_PAINT||message==WM_NCPAINT||message==WM_NCACTIVATE ||message == WM_NOTIFY)
	{
		CDC* pWinDC = GetWindowDC();
		if (pWinDC)
			DrawTitleBar(pWinDC);
		ReleaseDC(pWinDC);
	}
	return lrst;
}
void CSerialDebugDlg::DoResize(int delta)
{
	//CSplitterControl::ChangeWidth(&m_wndType, delta);
	CSplitterControl::ChangeHeight(&ResvContent,delta,CW_TOPALIGN);
	CSplitterControl::ChangeHeight(&SendContent, -delta,CW_BOTTOMALIGN);
	CSplitterControl::ChangeHeight(&SendButton, -delta,CW_BOTTOMALIGN);
	Invalidate();
	UpdateWindow();
}

HBRUSH CSerialDebugDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	CEdit * pEditReadOnly = (CEdit *)GetDlgItem(IDC_EDIT_RESV);

	HWND hWndReadOnly = pEditReadOnly->GetSafeHwnd();
	
	/*if(nCtlColor == CTLCOLOR_EDIT && hWndReadOnly == pWnd->GetSafeHwnd())
	{
		COLORREF  clr  =  RGB(255,255,255);
		hbr  =  ::CreateSolidBrush(clr);
	}*/

	/*if(nCtlColor == CTLCOLOR_STATIC && hWndReadOnly == pWnd->GetSafeHwnd())
	{
		COLORREF  clr  =  RGB(255,255,255); 
		hbr  =  ::CreateSolidBrush(clr);
		pDC->SetBkColor(clr);
		//pDC->SetTextColor(RGB(255,255,255));
	}*/
	return hbr;
}

void CSerialDebugDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd *pWnd;
	CWnd *pWnd1;
	Invalidate();

    pWnd = GetDlgItem(IDC_EDIT_RESV);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.right = rect1.left + rect1.Width() + cx - m_rect.Width();/////�����ؼ���С
		/*CString top;
		top.Format("%ld",rect.right);
		cedtMsg.SetWindowTextA(top);*/
		rect.bottom = rect1.top + rect1.Height() + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT_SEND);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.right = rect2.left + rect2.Width() + cx - m_rect.Width();/////�����ؼ���С
		rect.top = rect2.top + cy - m_rect.Height();
		rect.bottom = rect2.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_BUTTON_SEND);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.left = rect3.left + cx - m_rect.Width();/////�����ؼ���С
		rect.right = rect3.right + cx - m_rect.Width();/////�����ؼ���С
		rect.top = rect3.top + cy - m_rect.Height();
		rect.bottom = rect3.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT_SENDCON);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.top = rect4.top + cy - m_rect.Height();
		rect.bottom = rect4.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT3);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.top = rect5.top + cy - m_rect.Height();
		rect.bottom = rect5.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SYSLINK5);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.top = rect6.top + cy - m_rect.Height();
		rect.bottom = rect6.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SYSLINK4);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.top = rect7.top + cy - m_rect.Height();
		rect.bottom = rect7.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SYSLINK2);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.top = rect8.top + cy - m_rect.Height();
		rect.bottom = rect8.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SPLITTER);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.right = rect9.left + rect9.Width() + cx - m_rect.Width();/////�����ؼ���С
		rect.top = rect9.top + cy - m_rect.Height();
		rect.bottom = rect9.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_COMBO_PORT);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.right = rect10.left + rect10.Width() + cx - m_rect.Width();/////�����ؼ���С
		//rect.top = rect10.top + cy - m_rect.Height();
		//rect.bottom = rect10.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_STATIC_ADD);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.right = rect11.left + rect11.Width() + cx - m_rect.Width();/////�����ؼ���С
		rect.top = rect11.top + cy - m_rect.Height();
		rect.bottom = rect11.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT2);     //��ȡ�ؼ����
	if(pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.right = rect12.left + rect12.Width() + cx - m_rect.Width();/////�����ؼ���С
		rect.top = rect12.top + cy - m_rect.Height();
		rect.bottom = rect12.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}


	pWnd = GetDlgItem(IDC_BTN_APISEND);     //��ȡ�ؼ����
	if (pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ��С
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
		rect.left = rect13.left + cx - m_rect.Width();/////�����ؼ���С
		rect.right = rect13.right + cx - m_rect.Width();/////�����ؼ���С
		rect.top = rect13.top + cy - m_rect.Height();
		rect.bottom = rect13.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
		pWnd->Invalidate();
	}



	

}

void CSerialDebugDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    
	lpMMI->ptMinTrackSize.x = 685; 
    lpMMI->ptMinTrackSize.y = 730; 
 
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void ThreadRecv(CSerialDebugDlg * pClass)
{
	DWORD nBytesRead;
	CString temp,temp3;
	pClass->m_bRun = true;
	HANDLE hComm;
	DWORD dwError;
	COMSTAT cs;
	bool lastcharset;
	unsigned char RxBuff[4100];
	unsigned char DisplayBuff[4100*3];
	unsigned int i;
	DWORD sizerequired;

	while(pClass->m_bRun)
	{
		hComm = pClass->hComm;
		if((pClass->readnow == true)&&(hComm != INVALID_HANDLE_VALUE))
		{
			if(ClearCommError(hComm ,&dwError,&cs) != 0)
			{
				if(cs.cbInQue > sizeof(RxBuff))
				{
					PurgeComm(hComm,PURGE_RXCLEAR);
					Sleep(1);
					continue;
				}
		
				if(cs.cbInQue > 0)
				{
					ReadFile(hComm,(RxBuff+1),cs.cbInQue,&nBytesRead,NULL);
					pClass->iresvcount += nBytesRead;
					temp3.Format(L"���գ�%d",pClass->iresvcount);
					pClass->edResvCount.SetWindowText((LPCTSTR)(temp3));
					RxBuff[nBytesRead+1] = 0x00;
					RxBuff[nBytesRead+2] = 0x00;
				
					//��ⲻ����ʾ��ASCII�벢�ÿո��滻
					if(pClass->showhex == false)
					{
						for(i=1;i<=nBytesRead;i++)
						{
							if(pClass->m_parity == 0)
							{
								if((RxBuff[i]>=0)&&(RxBuff[i]<=31)&&(RxBuff[i]!=13)&&(RxBuff[i]!=0x0A))
								{
									//RxBuff[i] = 32;
									RxBuff[i] = 0x08;
									pClass->m_parity = 0;
								}
								else if(RxBuff[i] == 0x7f)
								{
									RxBuff[i] = 0x08;
									//RxBuff[i] = 32;
									pClass->m_parity = 0;
								}
								else if((RxBuff[i]>=0)&&(RxBuff[i]<=127)) pClass->m_parity = 0;
								else 
								{
									pClass->m_parity = 1;
									if(i == nBytesRead)
									{
										pClass->lastchar = RxBuff[nBytesRead];
										RxBuff[nBytesRead] = 0x00;
									}
								}
							}
							else 
							{
								pClass->m_parity = 0;
								if(i == 1)
								{
									RxBuff[0] = pClass->lastchar;
									lastcharset = true;
								}
								else lastcharset = false;
							}
						}

						//�����ܵ���������ʾ����
						if((nBytesRead==1)&&(RxBuff[nBytesRead]==0))
						{
							Sleep(1);
						}
						else
						{
							CString  str; 
							//CString ResvStr;
							TCHAR widestr[1000];
							if (lastcharset == true){
								sizerequired = MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(RxBuff), -1, widestr, 0);
								MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(RxBuff), -1, widestr, sizerequired);
							}
							else{
								sizerequired = MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(RxBuff + 1), -1, widestr, 0);
								MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(RxBuff + 1), -1, widestr, sizerequired);
							}
							str.Format(L"%s", widestr);
							//printf("%d", status);
							pClass->ResvContent.ModifyStyle(0,WS_DISABLED);
							//pClass->ResvContent.GetWindowTextA(ResvStr);
							if(pClass->ResvContent.GetWindowTextLength() > 20000)
							{
								pClass->ResvContent.SetSel(0,-1);
								pClass->ResvContent.SetWindowText(L"");
								//ResvStr = ResvStr.Right(pClass->ResvContent.GetWindowTextLengthA()-str.GetLength());
							}
							//ResvStr += str;
							//pClass->ResvContent.SetWindowTextA(ResvStr);
							//pClass->ResvContent.SetSel(pClass->ResvContent.GetWindowTextLengthA(),-1);
							pClass->ResvContent.noinput = true;
							Sleep(2);
							pClass->ResvContent.SetSel(pClass->ResvContent.GetWindowTextLength(),pClass->ResvContent.GetWindowTextLength()); 
							Sleep(2);
							pClass->ResvContent.SetSel(pClass->ResvContent.GetWindowTextLength(),pClass->ResvContent.GetWindowTextLength()); 
							pClass->ResvContent.ReplaceSel(str);
							pClass->ResvContent.noinput = false;
							if((pClass->ckbFileResv.GetCheck()) == BST_CHECKED)
							{
								pClass->resvfile.Write(str,str.GetLength());
							}
							pClass->ResvContent.ModifyStyle(WS_DISABLED,0);							
						}
					}
					else
					{
						//DisplayBuff
						for(i=1;i<=nBytesRead;i++)
						{
							if((((RxBuff[i]>>4)&0x0F) >= 0)&&(((RxBuff[i]>>4)&0x0F) <= 9))
								DisplayBuff[3*i] = ((RxBuff[i]>>4)&0x0F) + 48;
							if((((RxBuff[i]>>4)&0x0F) >= 10)&&(((RxBuff[i]>>4)&0x0F) <= 15))
								DisplayBuff[3*i] = ((RxBuff[i]>>4)&0x0F) + 55;
							if(((RxBuff[i]&0x0F) >= 0)&&((RxBuff[i]&0x0F) <= 9))
								DisplayBuff[3*i+1] = (RxBuff[i]&0x0F) + 48;
							if(((RxBuff[i]&0x0F) >= 10)&&((RxBuff[i]&0x0F) <= 15))
								DisplayBuff[3*i+1] = (RxBuff[i]&0x0F) + 55;
							DisplayBuff[3*i+2] = 32;
						}
						DisplayBuff[3*(nBytesRead+1)] = '\0';
						
						CString  str; 
						
						TCHAR widestr[1000];
						sizerequired = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(DisplayBuff + 3), -1, widestr, 0);
						MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(DisplayBuff + 3), -1, widestr, sizerequired);

						str.Format(L"%s", widestr);
						pClass->ResvContent.ModifyStyle(0,WS_DISABLED);
						pClass->ResvContent.noinput = true;
						Sleep(2);
						pClass->ResvContent.SetSel(pClass->ResvContent.GetWindowTextLength(),pClass->ResvContent.GetWindowTextLength()); 
						Sleep(2);
						pClass->ResvContent.SetSel(pClass->ResvContent.GetWindowTextLength(),pClass->ResvContent.GetWindowTextLength());
						pClass->ResvContent.ReplaceSel(str);
						pClass->ResvContent.noinput = false;
						if((pClass->ckbFileResv.GetCheck()) == BST_CHECKED)
							{
								pClass->resvfile.Write(str,str.GetLength());
							}
						pClass->ResvContent.ModifyStyle(WS_DISABLED,0);
						Sleep(1);
					}

				}
				else
				{
					Sleep(1);
					continue;
				}
			}
		}
		else
		{
			Sleep(1);
		}
	}
}

void ThreadMoniter(CSerialDebugDlg * pClass)
{
	CRegKey myKey;
	CString portname;
	DWORD mModemStat;
	DCB	mDCB;
	while(1)
	{
		if(pClass->portopen == true)
		{
			//DWORD dwStateTp = ((CButton*)pClass->GetDlgItem(IDC_CHECK_CD))->GetCheck(); 
			//((CButton*)pClass->GetDlgItem(IDC_CHECK_CD))->SetCheck(1);
			if(GetCommModemStatus(pClass->hComm,&mModemStat))
			{	
				if(mModemStat & MS_CTS_ON)((CButton*)pClass->GetDlgItem(IDC_CHECK_CTS))->SetCheck(1);
				else ((CButton*)pClass->GetDlgItem(IDC_CHECK_CTS))->SetCheck(0);

				if(mModemStat & MS_DSR_ON)((CButton*)pClass->GetDlgItem(IDC_CHECK_DSR))->SetCheck(1);
				else ((CButton*)pClass->GetDlgItem(IDC_CHECK_DSR))->SetCheck(0);

				if(mModemStat & MS_RING_ON)((CButton*)pClass->GetDlgItem(IDC_CHECK_RI))->SetCheck(1);
				else ((CButton*)pClass->GetDlgItem(IDC_CHECK_RI))->SetCheck(0);

				if(mModemStat & MS_RLSD_ON)((CButton*)pClass->GetDlgItem(IDC_CHECK_CD))->SetCheck(1);
				else ((CButton*)pClass->GetDlgItem(IDC_CHECK_CD))->SetCheck(0);

			}

			if(GetCommState(pClass->hComm,&mDCB))
			{
				if(mDCB.fDtrControl == DTR_CONTROL_ENABLE)((CButton*)pClass->GetDlgItem(IDC_CHECK_DTR))->SetCheck(1);
				else ((CButton*)pClass->GetDlgItem(IDC_CHECK_DTR))->SetCheck(0);
				if(mDCB.fRtsControl == RTS_CONTROL_ENABLE)((CButton*)pClass->GetDlgItem(IDC_CHECK_RTS))->SetCheck(1);
				else ((CButton*)pClass->GetDlgItem(IDC_CHECK_RTS))->SetCheck(0);
			}
			
			if(pClass->manual == false)
			{
				bool founded = false;

				int SerialCount = 0;// ���ڼ���
				CString strSerialList[256];  // ��ʱ���� 256 ���ַ�����
				CArray<SSerInfo,SSerInfo&> asi;
				SSerInfo mySer;
				EnumSerialPorts(asi,FALSE);// ����Ϊ TRUE ʱö�ٵ�ǰ���Դ򿪵Ĵ��ڣ� // ����ö�����д���

				SerialCount = asi.GetSize();

				for (int i=0; i<asi.GetSize(); i++)
				{
					mySer = asi[i];
					if((L"\\\\.\\"+ mySer.strPortName) == pClass->port)founded = true;
					//�����⴮�ڵ�ԭ���㷨�д��Ľ�
				}

				if(founded != true)
				{
					pClass->OnBnClickedButtonOpen();
					pClass->MessageBox(L"�����Ѱγ��Զ��رգ������²���USB�����豸���ٴδ򿪴���");
				}
			}
        }   
	Sleep(100);
    }  
}

void OpenMoniter(CSerialDebugDlg * pClass){
	while(1)
	{
		CString portname;
		pClass->GetDlgItemText(IDC_BUTTON_OPEN,portname);
		if(pClass->chkAutoOpen.GetCheck() && portname == L"ȡ���ȴ���������ͷŴ���"){
			
			pClass->hComm = CreateFile(pClass->port,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if(pClass->hComm != INVALID_HANDLE_VALUE){
				
				SetupComm(pClass->hComm,4096,4096);
				GetCommProperties(pClass->hComm,&pClass->commprop);
				GetCommState(pClass->hComm,&pClass->dcb);

				pClass->dcb.BaudRate = pClass->baudrate;
				pClass->dcb.ByteSize = pClass->bytesize;
				pClass->dcb.Parity = pClass->parity;
				pClass->dcb.StopBits = pClass->stopbits;
				if(!SetCommState(pClass->hComm,&pClass->dcb)){
					CloseHandle(pClass->hComm);
					pClass->hComm = INVALID_HANDLE_VALUE;
					pClass->portopen = false;
				}else{
					//pClass->MessageBox("aa");
					pClass->SetDlgItemText(IDC_BUTTON_OPEN,L"�رմ���"); 
					CString tips;
					CString tips1;
					CComboBox *pComboBox = (CComboBox*)pClass->GetDlgItem(IDC_COMBO_PORT);
					pComboBox->ModifyStyle(0,WS_DISABLED);  				
					pComboBox->SetEditSel(-1,-1);
					pComboBox->GetWindowText(tips);

					pComboBox = (CComboBox*)pClass->GetDlgItem(IDC_BUTTON_SEND);
					pComboBox->ModifyStyle(WS_DISABLED,0);

					CButton *pButton = (CButton*)pClass->GetDlgItem(IDC_CHECK_DTR);
					pButton->ModifyStyle(WS_DISABLED,0);
					pButton = (CButton*)pClass->GetDlgItem(IDC_CHECK_RTS);
					pButton->ModifyStyle(WS_DISABLED,0);

					pClass->portopen = true;
					pClass->OnBnClickedCheckStop();
					tips += L"�Ѵ�\n";
					tips1.Format(L"�����ʣ�%dbps",pClass->dcb.BaudRate);
					tips += tips1;
					wcscpy_s(pClass->m_nid.szTip, tips);//ͼ����ʾΪ"���Գ���"
					//readnow = true;
					pClass->Invalidate();
				}
			}
		}		
		Sleep(100);
	}
}

void CSerialDebugDlg::OnBnClickedButtonOpen()
{
	if(portopen == false)
	{
		//�жϴ���״̬
		CString portname;
		GetDlgItemText(IDC_BUTTON_OPEN,portname);
		if(portname == "�򿪴���")
		{
			
			CString temp;
			CString SeialPortDesc;
			CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
			pComboBox->GetWindowText(temp);
			CArray<SSerInfo,SSerInfo&> asi;
			EnumSerialPorts(asi,FALSE);// ����Ϊ TRUE ʱö�ٵ�ǰ���Դ򿪵Ĵ��ڣ� // ����ö�����д���
			
			SSerInfo mySer;
			SerialCount = asi.GetSize();
			int i;

			for (i=0; i<asi.GetSize(); i++)
			{
				mySer = asi[i];
				SeialPortDesc = L"";
				SeialPortDesc += mySer.strPortName; 
				SeialPortDesc += L" - " + mySer.strPortDesc; 

				if(temp == SeialPortDesc)break;
			}

			if(i == asi.GetSize())manual = true;
			else manual = false;

			hComm = CreateFile(port,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			int Error = GetLastError();
			if(hComm != INVALID_HANDLE_VALUE)
			{
				SetupComm(hComm,4096,4096);
				GetCommProperties(hComm,&commprop);
				SetDlgItemText(IDC_BUTTON_OPEN,L"�رմ���"); 
				GetCommState(hComm,&dcb);

				dcb.BaudRate = baudrate;
				dcb.ByteSize = bytesize;
				dcb.Parity = parity;
				dcb.StopBits = stopbits;
				m_parity = 0;

				if(!SetCommState(hComm,&dcb))
				{
					dwError = GetLastError();
					CString   szCaption;   
					szCaption.Format(L"%d",dwError);
					szCaption = L"���ö˿����Դ���:" + szCaption;
					MessageBox(szCaption,L"����",0);
					CloseHandle(hComm);
					hComm = INVALID_HANDLE_VALUE;
					portopen = false;
					SetDlgItemText(IDC_BUTTON_OPEN,L"�򿪴���");
					CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
					pComboBox->ModifyStyle(WS_DISABLED,0);  
					pComboBox->SetEditSel(-1,-1);
					//pComboBox->ModifyStyle(WS_DISABLED,0);
					pComboBox = (CComboBox*)GetDlgItem(IDC_BUTTON_SEND);

					CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_DTR);
					pButton->ModifyStyle(0,WS_DISABLED);
					pButton = (CButton*)GetDlgItem(IDC_CHECK_RTS);
					pButton->ModifyStyle(0,WS_DISABLED);

					Invalidate();

					CHARFORMAT2 cf1;
					CHARFORMAT cf;

					statusbar.SetWindowText(L"���ڹر�״̬");

						//����ΪCHARFORMAT2�ṹ����ϸ��MSDN
					ZeroMemory(&cf1, sizeof(CHARFORMAT2));
					cf1.cbSize = sizeof(CHARFORMAT2);
					cf1.dwMask = CFM_BACKCOLOR;

					cf1.crBackColor=RGB(255, 0, 0); //������ɫΪ��ɫ
					statusbar.SetSel(0, 6); //���ô�������
					statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

					ZeroMemory(&cf, sizeof(CHARFORMAT));
					cf.cbSize = sizeof(CHARFORMAT);
					cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
										CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
					cf.dwEffects = 0;
					cf.yHeight = 20*20;//���ָ߶�
					cf.crTextColor = RGB(255, 255, 255); //������ɫ
					wcscpy(cf.szFaceName ,_T("����"));//��������
					statusbar.SetSel(0, 6); //���ô�������
					statusbar.SetSelectionCharFormat(cf);
					statusbar.Invalidate();
					return;
				}
				CString tips;
				CString tips1;
				CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
				pComboBox->ModifyStyle(0,WS_DISABLED);  				
				pComboBox->SetEditSel(-1,-1);
				pComboBox->GetWindowText(tips);

				pComboBox = (CComboBox*)GetDlgItem(IDC_BUTTON_SEND);
				pComboBox->ModifyStyle(WS_DISABLED,0);

				CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_DTR);
				pButton->ModifyStyle(WS_DISABLED,0);
				pButton = (CButton*)GetDlgItem(IDC_CHECK_RTS);
				pButton->ModifyStyle(WS_DISABLED,0);

				portopen = true;
				OnBnClickedCheckStop();
				tips += L"�Ѵ�\n";
				tips1.Format(L"�����ʣ�%dbps",dcb.BaudRate);
				tips += tips1;
				wcscpy_s(m_nid.szTip, tips);//ͼ����ʾΪ"���Գ���"
				//readnow = true;
				Invalidate();
			}
			else
			{
				dwError = GetLastError();
				CString   szCaption;   
				szCaption.Format(L"%d",dwError);
				if(dwError == 5 && chkAutoOpen.GetCheck()){
					SetDlgItemText(IDC_BUTTON_OPEN,L"ȡ���ȴ���������ͷŴ���");
					return;
				}
				switch(dwError)
				{
					case 2: 
						szCaption = L"��Ӧ����д���ںŵĴ��ڲ�����";
						break;
					case 5:
						szCaption = L"���ڱ��������ռ��";
						
						break;						
					default:
						szCaption = L"���ڴ򿪴���:" + szCaption;
						break;
				}
				MessageBox(szCaption,L"����",0);
				CHARFORMAT2 cf1;
				CHARFORMAT cf;
				statusbar.SetWindowText(L"���ڹر�״̬");

					//����ΪCHARFORMAT2�ṹ����ϸ��MSDN
				ZeroMemory(&cf1, sizeof(CHARFORMAT2));
				cf1.cbSize = sizeof(CHARFORMAT2);
				cf1.dwMask = CFM_BACKCOLOR;

				cf1.crBackColor=RGB(255, 0, 0); //������ɫΪ��ɫ
				statusbar.SetSel(0, 6); //���ô�������
				statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

				ZeroMemory(&cf, sizeof(CHARFORMAT));
				cf.cbSize = sizeof(CHARFORMAT);
				cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
									CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
				cf.dwEffects = 0;
				cf.yHeight = 20*20;//���ָ߶�
				cf.crTextColor = RGB(255, 255, 255); //������ɫ
				wcscpy(cf.szFaceName ,_T("����"));//��������
				statusbar.SetSel(0, 6); //���ô�������
				statusbar.SetSelectionCharFormat(cf);
				statusbar.Invalidate();
			}
		}else if(portname == "ȡ���ȴ���������ͷŴ���"){
			CString temp;
			CString SeialPortDesc;
			CArray<SSerInfo,SSerInfo&> asi;
			EnumSerialPorts(asi,FALSE);// ����Ϊ TRUE ʱö�ٵ�ǰ���Դ򿪵Ĵ��ڣ� // ����ö�����д���
			
			SSerInfo mySer;
			SerialCount = asi.GetSize();
			int i;

			for (i=0; i<asi.GetSize(); i++)
			{
				mySer = asi[i];
				SeialPortDesc = L"";
				SeialPortDesc += mySer.strPortName; 
				SeialPortDesc += L" - " + mySer.strPortDesc; 

				if(temp == SeialPortDesc)break;
			}

			if(i == asi.GetSize())manual = true;
			else manual = false;
			Sleep(2);
			CloseHandle(hComm);
			hComm = INVALID_HANDLE_VALUE;
			portopen = false;
			readnow = false;
			SetDlgItemText(IDC_BUTTON_OPEN,L"�򿪴���");
		}
	}
	else
	{
		//m_bRun = false;	
		Sleep(2);
		CloseHandle(hComm);
		hComm = INVALID_HANDLE_VALUE;
		portopen = false;
		readnow = false;
		SetDlgItemText(IDC_BUTTON_OPEN,L"�򿪴���");
		CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
		pComboBox->ModifyStyle(WS_DISABLED,0);  
		pComboBox->SetEditSel(-1,-1);
		pComboBox = (CComboBox*)GetDlgItem(IDC_BUTTON_SEND);
		pComboBox->ModifyStyle(0,WS_DISABLED);

		CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_DTR);
		pButton->ModifyStyle(0,WS_DISABLED);
		pButton = (CButton*)GetDlgItem(IDC_CHECK_RTS);
		pButton->ModifyStyle(0,WS_DISABLED);

		SendButton.SetWindowText(L"����CTRL+ENTER");
		wcscpy_s(m_nid.szTip,L"BLUE���ڵ�������");
		if(timerrun == true)KillTimer(1);
		Invalidate();

		CHARFORMAT2 cf1;
		CHARFORMAT cf;
		statusbar.SetWindowText(L"���ڹر�״̬");

			//����ΪCHARFORMAT2�ṹ����ϸ��MSDN
		ZeroMemory(&cf1, sizeof(CHARFORMAT2));
		cf1.cbSize = sizeof(CHARFORMAT2);
		cf1.dwMask = CFM_BACKCOLOR;

		cf1.crBackColor=RGB(255, 0, 0); //������ɫΪ��ɫ
		statusbar.SetSel(0, 6); //���ô�������
		statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
							CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf.dwEffects = 0;
		cf.yHeight = 20*20;//���ָ߶�
		cf.crTextColor = RGB(255, 255, 255); //������ɫ
		wcscpy(cf.szFaceName ,_T("����"));//��������
		statusbar.SetSel(0, 6); //���ô�������
		statusbar.SetSelectionCharFormat(cf);
		statusbar.Invalidate();
	}

}

//-------------------------------------------------------------//
//----------------------���ں�ѡ����¼�����-------------------//
//-------------------------------------------------------------//
void CSerialDebugDlg::OnCbnSelchangeComboPort()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);	  
	
	if(SerialCount!=0)
	{	
		CString portnum;
		pComboBox->GetLBText(pComboBox->GetCurSel(),portnum);

		int index = portnum.Find(L" "); 
		if (index != -1)
		{
			portnum = portnum.Left(index); 
		}
		index = portnum.Find(L"-"); 
		if (index != -1)
		{
			portnum = portnum.Left(index); 
		}

		CString head = L"\\\\.\\";
		port = head + portnum;
		pComboBox->SetEditSel(-1,-1);
	}

}

void CSerialDebugDlg::OnCbnSetfocusComboPort()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	pComboBox->SetEditSel(-1,-1);
	//MessageBox("aa");
}

void CSerialDebugDlg::OnCbnEditupdateComboPort()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	DWORD dwSel = pComboBox->GetEditSel();
	WORD wSel = LOWORD( dwSel );
	if( wSel == 0 ) return;
	wSel--;
	TCHAR strEdit[100];
	memset( strEdit, 0, 100 );
	pComboBox->GetWindowText( strEdit, 100 );
	if(wSel>=3 && (strEdit[wSel] >= '0' && strEdit[wSel] <= '9')) return;
	strEdit[wSel] = 0;
	wcscpy(strEdit+wSel, strEdit+wSel+1);
	pComboBox->SetWindowText( strEdit );
	if(wSel<3)pComboBox->SetEditSel( LOWORD(dwSel)-1, HIWORD(dwSel)-1);
	else pComboBox->SetEditSel( LOWORD(dwSel), HIWORD(dwSel));
}

void CSerialDebugDlg::OnCbnSelendcancelComboPort()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);	
	{
		CString portnum;
		pComboBox->GetWindowText(portnum);

		int index = portnum.Find(L" "); 
		if (index != -1)
		{
			portnum = portnum.Left(index); 
		}
		index = portnum.Find(L"-"); 
		if (index != -1)
		{
			portnum = portnum.Left(index); 
		}

		CString head = L"\\\\.\\";
		port = head + portnum;
		//MessageBox(port);
		pComboBox->SetEditSel(-1,-1);
	}	
}


void CSerialDebugDlg::OnCbnEditchangeComboPort()
{
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);	
	{
		CString portnum;
		DWORD dwSel = pComboBox->GetEditSel();
		pComboBox->GetWindowText(portnum);

		int index = portnum.Find(L" "); 
		if (index != -1)
		{
			portnum = portnum.Left(index); 
		}
		index = portnum.Find(L"-"); 
		if (index != -1)
		{
			portnum = portnum.Left(index); 
		}

		int i = pComboBox->GetWindowTextLength();
		if(i<3)pComboBox->SetWindowText(L"COM");

		CString head = L"\\\\.\\";
		port = head + portnum;
		pComboBox->SetEditSel( LOWORD(dwSel)+1, HIWORD(dwSel)+1);
		//pComboBox->SetEditSel(-1,-1);
	}
}

void CSerialDebugDlg::OnCbnKillfocusComboPort()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);	
	CString portnum;
	pComboBox->GetWindowText(portnum);

	int index = portnum.Find(L" "); 
	if (index != -1)
	{
		portnum = portnum.Left(index); 
	}
	index = portnum.Find(L"-"); 
	if (index != -1)
	{
		portnum = portnum.Left(index); 
	}

	if(portnum == L"COM")pComboBox->SetWindowText(L"COM1");
}


//-------------------------------------------------------------//




void CSerialDebugDlg::OnCbnSelchangeCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString baud;
	int iparity,idata,istop;
	CComboBox * pComboBox;
	CString tips;
	CString tips1;

	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK_BORTRATE_MODE))->GetCheck();
	switch(dwState)
	{
	case BST_UNCHECKED:	//�Զ��岨����
		((CEdit *)GetDlgItem(IDC_EDIT_BORTRATE))->GetWindowText(baud);
		break;
	case BST_CHECKED:	//��׼������
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
		pComboBox->GetLBText(pComboBox->GetCurSel(),baud);	
		break;
	default:
		break; 
	}
	TCHAR * buffer = baud.GetBuffer();
	baudrate = _wtoi(buffer);

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PARITY);
	iparity = pComboBox->GetCurSel();
	switch(iparity)
	{
	case 0:
		parity = NOPARITY;
		break;
	case 1:
		parity = ODDPARITY;
		break;
	case 2:
		parity = EVENPARITY;
		break;
	case 3:
		parity = MARKPARITY;
		break;
	case 4:
		parity = SPACEPARITY;
		break;
	}

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DATA);
	idata = pComboBox->GetCurSel();
	bytesize = idata + 4;

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STOP);
	istop = pComboBox->GetCurSel();
	switch(istop)
	{
	case 0:
		stopbits = ONESTOPBIT;
		break;
	case 1:
		stopbits = ONE5STOPBITS;
		break;
	case 2:
		stopbits = TWOSTOPBITS;
		break;
	}
	
	CString portname;
	GetDlgItemText(IDC_BUTTON_OPEN,portname);

	if(portname == L"�رմ���")
	{
		GetCommState(hComm,&dcb);

		dcb.BaudRate = baudrate;
		dcb.ByteSize = bytesize;
		dcb.Parity = parity;
		dcb.StopBits = stopbits;

		if(!SetCommState(hComm,&dcb))
		{
			dwError = GetLastError();
			CString   szCaption;   
			szCaption.Format(L"%d",dwError);
			szCaption = L"���ö˿����Դ���:" + szCaption;
			MessageBox(szCaption,L"����",0);
		}

				CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
				pComboBox->GetWindowText(tips);
				tips += L"�Ѵ�\n";
				tips1.Format(L"�����ʣ�%dbps",dcb.BaudRate);
				tips += tips1;
				wcscpy_s(m_nid.szTip,tips);//ͼ����ʾΪ"���Գ���"
	}
}

VOID WINAPI sendcomplete(	DWORD dwErrorCode,
							DWORD dwNumberOfBytesTransfered,
							LPOVERLAPPED lpOverlapped)
{

}

void CSerialDebugDlg::OnBnClickedButtonSend()
{
	
	//InfoDlg.ShowWindow(SW_SHOW);
	COMBOBOXINFO info = { sizeof(COMBOBOXINFO) };
	ComboPort.GetComboBoxInfo(&info);


	if(SendButton.GetStyle() & WS_DISABLED )return;
	CString btnname;
	GetDlgItemText(IDC_BUTTON_SEND,btnname);
	if(btnname == L"ֹͣ����")
	{
		writenow = false;
		SendButton.SetWindowText(L"����CTRL+ENTER");
		KillTimer(1);
		timerrun = false;
	}else{
	
	int length = SendContent.GetWindowTextLength();
	unsigned char SendBuff[4100];
	if(length ==0)
	{
		MessageBox(L"�����������ٷ���",L"����");
		SendContent.SetFocus();
	}
	else{
		if(chkFilesSend.GetCheck() == BST_CHECKED)
		{
			CFile txtfile;
			if(!txtfile.Open(filesendname,CFile::modeRead))
			{
				MessageBox(L"����ѡ����ļ��޷���",L"����");
				chkFilesSend.SetCheck(BST_UNCHECKED);
				return;
			}
			CFileStatus rStatus;
			txtfile.GetStatus(rStatus);
			CTime temp = rStatus.m_mtime;
			if(temp > ModifyTime)
			{
				int cancel = MessageBox(L"���򿪵��ļ������Ѿ�����������޸ģ����������ļ���ȷ���������޸����ݰ�ȡ����",L"��ʾ",MB_OKCANCEL);
				if(cancel == IDOK){
			
					length = (int)txtfile.GetLength();
					if(length > 4096)
					{
						MessageBox(L"��Ҫ�򿪵��ļ����������ϴ�,��������෢��4K�ֽ�,�������ֽ����ᷢ��",L"����");
						length = 4096;
					}

					// Read file header
					length = txtfile.Read(SendBuff,length);
					SendBuff[length] = 0;
					SendBuff[length+1] = 0;
					CFileStatus rStatus;
					txtfile.GetStatus(rStatus);
					ModifyTime = rStatus.m_mtime;
					SendContent.SetWindowText((LPCTSTR)SendBuff);
					if(cchkLoopEn.GetCheck() != BST_CHECKED)SendContent.SetModify(false);
								
				}
			}
			txtfile.Close();
			
		}
		if(cchkLoopEn.GetCheck() == BST_CHECKED){
			SendButton.SetWindowText(L"ֹͣ����");
			timerrun = true;
			SetTimer(1,timer,NULL);
		}
		else{
			writenow = true;
		}
	}
	
	}
}


void CSerialDebugDlg::OnNMClickSyslinkClearout(NMHDR *pNMHDR, LRESULT *pResult)
{
	ResvContent.SetSel(0,-1);
	ResvContent.ReplaceSel(L"");   
}

void CSerialDebugDlg::OnBnClickedCheckStop()
{
	CHARFORMAT2 cf1;
	CHARFORMAT cf;

	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK_STOP))->GetCheck();  

	switch(dwState)
	{
	case BST_UNCHECKED:
		if(hComm != NULL)PurgeComm(hComm,PURGE_RXCLEAR);
		readnow = true;
		statusbar.SetWindowText(L"������ʾ״̬");

		 //����ΪCHARFORMAT2�ṹ����ϸ��MSDN
        ZeroMemory(&cf1, sizeof(CHARFORMAT2));
        cf1.cbSize = sizeof(CHARFORMAT2);
        cf1.dwMask = CFM_BACKCOLOR;

        cf1.crBackColor=RGB(0, 0, 255); //������ɫΪ��ɫ
        statusbar.SetSel(0, 6); //���ô�������
        statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

        ZeroMemory(&cf, sizeof(CHARFORMAT));
        cf.cbSize = sizeof(CHARFORMAT);
        cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
                            CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
        cf.dwEffects = 0;
        cf.yHeight = 20*20;//���ָ߶�
        cf.crTextColor = RGB(255, 255, 255); //������ɫ
        wcscpy(cf.szFaceName ,_T("����"));//��������
        statusbar.SetSel(0, 6); //���ô�������
        statusbar.SetSelectionCharFormat(cf);
		statusbar.Invalidate();

		break;
	case BST_CHECKED:
		readnow = false;
		
		statusbar.SetWindowText(L"ֹͣ������ʾ");
		
        ZeroMemory(&cf1, sizeof(CHARFORMAT2));
        cf1.cbSize = sizeof(CHARFORMAT2);
        cf1.dwMask = CFM_BACKCOLOR;

        cf1.crBackColor=RGB(255, 0, 0); //������ɫΪ��ɫ
        statusbar.SetSel(0, 6); //���ô�������

        statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

        ZeroMemory(&cf, sizeof(CHARFORMAT));
        cf.cbSize = sizeof(CHARFORMAT);
        cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
                            CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
        cf.dwEffects = 0;
        cf.yHeight = 20*20;//���ָ߶�
        cf.crTextColor = RGB(255, 255, 255); //������ɫ
        wcscpy(cf.szFaceName ,_T("����"));//��������
        statusbar.SetSel(0, 6); //���ô�������
        statusbar.SetSelectionCharFormat(cf);
		statusbar.Invalidate();
		
		break;
	default:
		break; 
	}
}

void CSerialDebugDlg::OnBnClickedCheckBortrateMode()
{
	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK_BORTRATE_MODE))->GetCheck();
	switch(dwState)
	{
	case BST_UNCHECKED:
		((CButton*)GetDlgItem(IDC_COMBO_BAUDRATE))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_EDIT_BORTRATE))->ShowWindow(SW_SHOW);
		break;
	case BST_CHECKED:
		((CButton*)GetDlgItem(IDC_COMBO_BAUDRATE))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_EDIT_BORTRATE))->ShowWindow(SW_HIDE);
		break;
	default:
		break; 
	}
}

void CSerialDebugDlg::OnEnChangeEditBortrate()
{
	CString baud;
	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK_BORTRATE_MODE))->GetCheck();
	switch(dwState)
	{
	case BST_UNCHECKED:	//�Զ��岨����
		((CEdit *)GetDlgItem(IDC_EDIT_BORTRATE))->GetWindowText(baud);
		break;
	case BST_CHECKED:	//��׼������
		break;
	default:
		break; 
	}
	TCHAR * buffer = baud.GetBuffer();
	baudrate = _wtoi(buffer);

	CString portname;
	GetDlgItemText(IDC_BUTTON_OPEN,portname);

	if(portname == "�رմ���")
	{
		GetCommState(hComm,&dcb);

		dcb.BaudRate = baudrate;
		dcb.ByteSize = bytesize;
		dcb.Parity = parity;
		dcb.StopBits = stopbits;

		if(!SetCommState(hComm,&dcb))
		{
			dwError = GetLastError();
			CString   szCaption;   
			szCaption.Format(L"%d",dwError);
			szCaption = L"���ö˿����Դ���:" + szCaption;
			MessageBox(szCaption,L"����",0);
		}
	}
}

void CSerialDebugDlg::OnBnClickedCheckHexout()
{
	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK_HEXOUT))->GetCheck();
	switch(dwState)
	{
	case BST_UNCHECKED:
		showhex = false;
		break;
	case BST_CHECKED:
		showhex = true;
		break;
	default:
		break; 
	}
}

void CSerialDebugDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	SendContent.SetSel(0,-1);
	SendContent.ReplaceSel(L"");  
}

void CSerialDebugDlg::OnBnClickedCheckSendhex()
{
	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK_SENDHEX))->GetCheck();
	switch(dwState)
	{
	case BST_UNCHECKED:
		SendContent.SetNormalMode();
		sendhex = false;
		break;
	case BST_CHECKED:
		SendContent.SetHexMode();
		sendhex = true;
		break;
	default:
		break; 
	}
}

void ThreadSend(CSerialDebugDlg * pClass)
{
	DWORD nBytesWritten;
	CString temp,temp3;
	HANDLE hComm;
	pClass->sendRun = true;
	DWORD length;
	unsigned char TxBuff[4100];
	TCHAR TxBuffWide[4100];
	unsigned char TxAdd[10];
	unsigned char SendBuff[4100/3];
	unsigned char temp1,temp2;
	unsigned int i;
	DWORD sizerequired;

	while(pClass->sendRun)
	{
		hComm = pClass->hComm;
		if((pClass->writenow == true)&&(hComm != INVALID_HANDLE_VALUE))
		{
			DWORD dwState = ((CButton*)pClass->GetDlgItem(IDC_CHECK_AUTOENTER))->GetCheck();
			DWORD dwState1 = ((CButton*)pClass->GetDlgItem(IDC_CHECK_STOP))->GetCheck();
			if((dwState == BST_CHECKED) && (dwState1 == BST_UNCHECKED))
			{
				if(((CButton*)pClass->GetDlgItem(IDC_CHECK_HEXOUT))->GetCheck() != BST_CHECKED)	
				if((pClass->ResvContent.GetWindowTextLength())!=0)
				{
					pClass->ResvContent.ModifyStyle(0,WS_DISABLED);
					pClass->ResvContent.SetSel(pClass->ResvContent.GetWindowTextLength(),-1);
					pClass->ResvContent.ReplaceSel(L"\r\n");
					if((pClass->ckbFileResv.GetCheck()) == BST_CHECKED)
					{
								pClass->resvfile.Write(L"\r\n",2);
					}
					
					pClass->ResvContent.ModifyStyle(WS_DISABLED,0);
				}
				//pClass->SendContent.SetSel(0,-1);
				//pClass->SendContent.ReplaceSel("");
			}

			CComboBox *pComboBox = (CComboBox*)pClass->GetDlgItem(IDC_COMBO_MULTIHOST_MODE);	  
			int iMode = pComboBox->GetCurSel();
			if(iMode == 1)
			{
				unsigned char addr = pClass->saddr;
				DCB dcb;
				GetCommState(hComm,&dcb);
				dcb.Parity = MARKPARITY;
				SetCommState(hComm,&dcb);
				WriteFile(hComm,&addr,1,&nBytesWritten,NULL);
				pClass->isendcount += nBytesWritten;
				temp3.Format(L"���ͣ�%d",pClass->isendcount);
				pClass->edSendCount.SetWindowText((LPCTSTR)(temp3));
				dcb.Parity = SPACEPARITY;
				SetCommState(hComm,&dcb);
			}

			if (pClass->hasapidata == true){
				WriteFile(hComm, pClass->SeialCallback.lpBuffer, pClass->SeialCallback.dwLength, &nBytesWritten, NULL);
				pClass->isendcount += nBytesWritten;
				temp3.Format(L"���ͣ�%d", pClass->isendcount);
				pClass->edSendCount.SetWindowText((LPCTSTR)(temp3));
				pClass->writenow = false;
				pClass->hasapidata = false;
				delete pClass->SeialCallback.lpBuffer;
				if (pClass->SeialCallback.bRepeat){
					pClass->APIDataload();
				}
				else{
					pClass->btnAPISend.SetWindowText(L"API����");
					if (pClass->hDllInst) { 						
						FreeLibrary(pClass->hDllInst); 
						pClass->hDllInst = NULL;
					}
				}
				continue;
			}

			if(pClass->sendhex == false)
			{
				if(pClass->cchkLoopEn.GetCheck() != BST_CHECKED)
					pClass->SendButton.ModifyStyle(0,WS_DISABLED);
				length = pClass->SendContent.GetWindowTextLength();
				pClass->SendContent.GetWindowText(TxBuffWide, length + 1);
				
				//WideCharToMultiByte()
				sizerequired = WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)TxBuff, 0, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)TxBuff, sizerequired, NULL, NULL);

				WriteFile(hComm, TxBuff, sizerequired-1, &nBytesWritten, NULL);
				pClass->isendcount += nBytesWritten;
				temp3.Format(L"���ͣ�%d",pClass->isendcount);
				pClass->edSendCount.SetWindowText((LPCTSTR)(temp3));
				pClass->writenow = false;
				if(pClass->cchkLoopEn.GetCheck() != BST_CHECKED)
					pClass->SendButton.ModifyStyle(WS_DISABLED,0);
			}
			else
			{
				if(pClass->cchkLoopEn.GetCheck() != BST_CHECKED)
					pClass->SendButton.ModifyStyle(0,WS_DISABLED);
				length = pClass->SendContent.GetWindowTextLength();
				pClass->SendContent.GetWindowText(TxBuffWide, length + 1);

				sizerequired = WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)TxBuff, 0, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)TxBuff, sizerequired, NULL, NULL);

				for (i = 0; i<(sizerequired) / 3; i++)
				{
					if((TxBuff[i*3] >= '0')&&(TxBuff[i*3] <= '9'))
						temp1 = TxBuff[i*3] - 48;
					if((TxBuff[i*3] >= 'A')&&(TxBuff[i*3] <= 'F'))
						temp1 = TxBuff[i*3] - 55;
					if((TxBuff[i*3+1] >= '0')&&(TxBuff[i*3+1] <= '9'))
						temp2 = TxBuff[i*3+1] - 48;
					if((TxBuff[i*3+1] >= 'A')&&(TxBuff[i*3+1] <= 'F'))
						temp2 = TxBuff[i*3+1] - 55;
					SendBuff[i] = temp1*16 + temp2;
				}
				WriteFile(hComm,SendBuff,i,&nBytesWritten,NULL);
				pClass->isendcount += nBytesWritten;
				temp3.Format(L"���ͣ�%d",pClass->isendcount);
				pClass->edSendCount.SetWindowText((LPCTSTR)(temp3));
				pClass->writenow = false;
				if(pClass->cchkLoopEn.GetCheck() != BST_CHECKED)
					pClass->SendButton.ModifyStyle(WS_DISABLED,0);
			}

			dwState = ((CButton*)pClass->GetDlgItem(IDC_CHECK3))->GetCheck();
			if(dwState == BST_CHECKED)
			{
				int addtype = pClass->CAddDiag.iAddType;
				unsigned int size = pClass->modehex.GetWindowTextLength();
				unsigned char temp1,temp2;
				unsigned char * ibuffer = NULL;
				int addlen = 0;
				ibuffer = new unsigned char[size+2];
				pClass->modehex.GetWindowText((LPTSTR)ibuffer,size+2);
				for(i=0;i<(size+1)/5;i++)
				{
					if((ibuffer[i*5+2] >= '0')&&(ibuffer[i*5+2] <= '9'))
						temp1 = ibuffer[i*5+2] - 48;
					if((ibuffer[i*5+2] >= 'A')&&(ibuffer[i*5+2] <= 'F'))
						temp1 = ibuffer[i*5+2] - 55;
					if((ibuffer[i*5+3] >= '0')&&(ibuffer[i*5+3] <= '9'))
						temp2 = ibuffer[i*5+3] - 48;
					if((ibuffer[i*5+3] >= 'A')&&(ibuffer[i*5+3] <= 'F'))
						temp2 = ibuffer[i*5+3] - 55;
					TxAdd[i] = temp1*16 + temp2;
					addlen++;
				}
				TxAdd[i] = '\0';
				delete[] ibuffer;

				WriteFile(hComm,&TxAdd,addlen,&nBytesWritten,NULL);
				pClass->isendcount += nBytesWritten;
				temp3.Format(L"���ͣ�%d",pClass->isendcount);
				pClass->edSendCount.SetWindowText((LPCTSTR)(temp3));
	
			}
			dwState = ((CButton*)pClass->GetDlgItem(IDC_CHECK_CLEAR))->GetCheck();
			if(dwState == BST_CHECKED)
			{
				pClass->SendContent.SetSel(0,-1);
				pClass->SendContent.ReplaceSel(L"");
			}
		}
		else
		{
			Sleep(1);
		}
	}

}

void CSerialDebugDlg::OnCbnSelchangeComboMultihostMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_MULTIHOST_MODE);	  
	int iMode = pComboBox->GetCurSel();
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PARITY);
	if(iMode == 0){
		pComboBox->ModifyStyle(WS_DISABLED,0);
	}else{
		pComboBox->ModifyStyle(0,WS_DISABLED);
	}
}


void CSerialDebugDlg::OnEnChangeEditAdddec()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	int length = MEditAddDec.GetWindowTextLength();
	TCHAR cDecAddr[4];
	TCHAR cHexAddr[3];
	MEditAddDec.GetWindowText(LPTSTR(cDecAddr),length+1);
	int temp=0,i,temp1;
	for(i=length-1;i>=0;i--)
	{
		temp1 = cDecAddr[i]-0x30;
		if(i==(length-1))temp1*=1;
		else if(i==(length-2))temp1*=10;
		else if(i==(length-3))temp1*=100;
		temp+=temp1;
	}
	if(temp>255){
		temp=255;
		MEditAddDec.SetWindowText(L"255");
		MEditAddHex.SetWindowText(L"FF");
	}else{
		cHexAddr[0] = temp/16;
		cHexAddr[1] = temp%16;
		cHexAddr[2] = 0;
		if((cHexAddr[0]>=0)&&(cHexAddr[0]<=9))cHexAddr[0]+=0x30;
		else cHexAddr[0]+=55;
		if((cHexAddr[1]>=0)&&(cHexAddr[1]<=9))cHexAddr[1]+=0x30;
		else cHexAddr[1]+=55;
		MEditAddHex.ShowWindow(SW_HIDE);
		MEditAddHex.SetWindowText(LPTSTR(cHexAddr));
		MEditAddHex.ShowWindow(SW_SHOW);
		
	}
	saddr = temp;
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CSerialDebugDlg::OnEnChangeEditAddhex()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	/*int length = MEditAddHex.GetWindowTextLengthA();
	TCHAR cDecAddr[4];
	TCHAR cHexAddr[3];
	MEditAddHex.GetWindowTextA(LPTSTR(cHexAddr),length+1);
	int temp=0,i,temp1;
	for(i=length-1;i>=0;i--)
	{
		if((cHexAddr[i]>='0')&&(cHexAddr[i]<='9'))cHexAddr[i]-=0x30;
		else cHexAddr[i]-=55;
		temp1 = cHexAddr[i];
		if(i==(length-1))temp1*=1;
		else if(i==(length-2))temp1*=16;
		temp+=temp1;
	}

	cDecAddr[0] = temp/100;
	cDecAddr[1] = (temp%100)/10;
	cDecAddr[2] = temp%10;
	cDecAddr[3] = 0;
	for(i=0;i<3;i++)cDecAddr[i]+=0x30;
	MEditAddDec.ShowWindow(SW_HIDE);
	MEditAddDec.SetWindowTextA(LPTSTR(cDecAddr));
	MEditAddDec.ShowWindow(SW_SHOW);	*/
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CSerialDebugDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	isendcount = 0;
	edSendCount.SetWindowText(L"���ͣ�0");
	*pResult = 0;
}


void CSerialDebugDlg::OnNMClickSyslink4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	iresvcount = 0;
	edResvCount.SetWindowText(L"���գ�0");
	*pResult = 0;
}


void CSerialDebugDlg::OnNMClickSyslink5(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	isendcount = 0;
	edSendCount.SetWindowText(L"���ͣ�0");
	iresvcount = 0;
	edResvCount.SetWindowText(L"���գ�0");
	*pResult = 0;
}


void CSerialDebugDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nHotKeyId)
	{
	case HOTKEY_SEND:
		OnBnClickedButtonSend();
		break;
	default:
		break;
	}

	CDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}


BOOL CSerialDebugDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	::UnregisterHotKey(m_hWnd,HOTKEY_SEND);
	
	return CDialog::DestroyWindow();
}


//BOOL CSerialDebugDlg::OnCommand(WPARAM wParam, LPARAM lParam)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//    switch (LOWORD(wParam))
//    {
//		case EM_UNDO:
//		case WM_CUT:
//		case WM_COPY:
//		case WM_CLEAR:
//		case WM_PASTE:
//	        return SendMessage(LOWORD(wParam));
//	    case ME_SELECTALL:
//			return SendMessage (EM_SETSEL, 0, -1);
//		default:
//			return CEdit::OnCommand(wParam, lParam);
//    }
//	return CDialog::OnCommand(wParam, lParam);
//}


//void CSerialDebugDlg::OnCancel()
//{
//	// TODO: �ڴ����ר�ô����/����û���
//
//	CDialog::OnCancel();
//}


BOOL CSerialDebugDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN &&( ((GetAsyncKeyState( VK_LCONTROL ) & 0x8000)) ||((GetAsyncKeyState( VK_RCONTROL ) & 0x8000))))
		{
			OnBnClickedButtonSend();
			return true;
		}
		else if(pMsg->wParam == VK_ESCAPE /*|| pMsg->wParam == VK_RETURN*/) 
		{ 
			return true;
		}		
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CSerialDebugDlg::OnBnClickedCheck5()
{
	//CFileDialog openfile(true);
	ULONGLONG length;
	unsigned char SendBuff[4100];
	if(chkFilesSend.GetCheck() == BST_CHECKED)
	{
		LPTSTR lpFile = filesendname.GetBuffer(1000);
		CFileDialog openfile(true,NULL,NULL,OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_READONLY|OFN_FILEMUSTEXIST,L"Text Files(*.txt)|*.txt|�����ļ�(*.*)|*.*|",AfxGetMainWnd()); 
		openfile.m_ofn.lpstrFile=lpFile; 
		openfile.m_ofn.nMaxFile=1000; 
		openfile.m_ofn.lStructSize=sizeof(openfile.m_ofn); 
		openfile.m_ofn.lpstrInitialDir = NULL;
		openfile.m_ofn.lpstrTitle =L"ѡ��Ҫ���͵��ļ�";
		int state = openfile.DoModal(); 
		if(state == IDCANCEL)
		{
			chkFilesSend.SetCheck(BST_UNCHECKED);
			return;
		}
		else
		{
			chkFilesSend.SetCheck(BST_CHECKED);
			CFile txtfile;
			if(!txtfile.Open(filesendname,CFile::modeRead))
			{
				MessageBox(L"����ѡ����ļ��޷���",L"����");
				chkFilesSend.SetCheck(BST_UNCHECKED);
				return;
			}
			
			length = txtfile.GetLength();
			if(length > 4096)
			{
				MessageBox(L"��Ҫ�򿪵��ļ����������ϴ�,��������෢��4K�ֽ�,�������ֽ����ᷢ��",L"����");
				length = 4096;
			}

			// Read file header
			length = (UINT)txtfile.Read(SendBuff,length);
			SendBuff[length] = 0;
			SendBuff[length+1] = 0;
			CFileStatus rStatus;
			txtfile.GetStatus(rStatus);
			ModifyTime = rStatus.m_mtime;
			txtfile.Close();
			SendContent.SetWindowText((LPCTSTR)SendBuff);
			SendContent.SetModify(false);
			((CButton*)GetDlgItem(IDC_CHECK_SENDHEX))->SetCheck(BST_UNCHECKED);
			//this->SetWindowTextA("�������޸Ĳ���");
		}
	}else{
		filesendname = L"";
	}
	
}


void CSerialDebugDlg::OnEnChangeEditSend()
{
	if(chkFilesSend.GetCheck() == BST_CHECKED)
	{
		SendContent.SetModify();
	}

	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	switch(dwState)
	{
	case BST_UNCHECKED:
		break;
	case BST_CHECKED:
		UpdateAddByre();
		break;
	}
		
}

void CSerialDebugDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(chkFilesSend.GetCheck() == BST_CHECKED)
	{
		ShellExecute(this->m_hWnd,L"open",filesendname,L"",L"",SW_SHOW );
		
		/*CString strFile;
		strFile.Format(" /e,/select,%s",filesendname);
		ShellExecute(NULL,"open","explorer.exe",(LPCTSTR)strFile,NULL,true);*/
	}
	*pResult = 0;
}


void CSerialDebugDlg::OnNMClickSyslink6(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(chkFilesSend.GetCheck() == BST_CHECKED)
	{
		//ShellExecute(this->m_hWnd,"open",filesendname,"","",SW_SHOW );
		
		CString strFile;
		strFile.Format(L" /e,/select,%s",filesendname);
		ShellExecute(NULL,L"open",L"explorer.exe",(LPCTSTR)strFile,NULL,true);
	}
	*pResult = 0;
}


void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(this->m_hWnd,L"open",L"http://tech.sherryblue.org/",L"",L"",SW_SHOW );
	*pResult = 0;
}


void CSerialDebugDlg::OnBnClickedCheck6()
{
//	ULONGLONG length;
	if(ckbFileResv.GetCheck() == BST_CHECKED)
	{
		LPTSTR lpFile = fileresvname.GetBuffer(1000);
		CFileDialog savefile(false,L"txt",L"Serialdata",OFN_OVERWRITEPROMPT,L"Text Files(*.txt)|*.txt|",AfxGetMainWnd()); 
		savefile.m_ofn.lpstrFile=lpFile; 
		savefile.m_ofn.nMaxFile=1000; 
		savefile.m_ofn.lStructSize=sizeof(savefile.m_ofn); 
		savefile.m_ofn.lpstrInitialDir = NULL;
		savefile.m_ofn.lpstrTitle =L"ѡ��Ҫ������յ��ļ�";
		int state = savefile.DoModal(); 
		if(state == IDCANCEL)
		{
			ckbFileResv.SetCheck(BST_UNCHECKED);
			return;
		}
		else
		{
			ckbFileResv.SetCheck(BST_CHECKED);
			if(!resvfile.Open(fileresvname,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
			{
				MessageBox(L"����ѡ����ļ��޷���",L"����");
				ckbFileResv.SetCheck(BST_UNCHECKED);
				return;
			}
			
		}
	}else{
		filesendname = "";
		resvfile.Close();
	}
}


void CSerialDebugDlg::OnNMClickSyslink7(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(ckbFileResv.GetCheck() == BST_CHECKED)
	{
		ShellExecute(this->m_hWnd,L"open",fileresvname,L"",L"",SW_SHOW );
		
		/*CString strFile;
		strFile.Format(" /e,/select,%s",filesendname);
		ShellExecute(NULL,"open","explorer.exe",(LPCTSTR)strFile,NULL,true);*/
	}
	*pResult = 0;
}


void CSerialDebugDlg::OnNMClickSyslink8(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(ckbFileResv.GetCheck() == BST_CHECKED)
	{
		//ShellExecute(this->m_hWnd,"open",fileresvname,"","",SW_SHOW );
		
		CString strFile;
		strFile.Format(L" /e,/select,%s",fileresvname);
		ShellExecute(NULL,L"open",L"explorer.exe",(LPCTSTR)strFile,NULL,true);
	}
	*pResult = 0;
	*pResult = 0;
}


void CSerialDebugDlg::OnEnChangeEditLooptime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int length = cedtLoopTime.GetWindowTextLength();
	TCHAR cLoopTime[5];
	cedtLoopTime.GetWindowText(LPTSTR(cLoopTime),length+1);
	int temp=0,i,temp1;
	for(i=length-1;i>=0;i--)
	{
		temp1 = cLoopTime[i]-0x30;
		if(i==(length-1))temp1*=1;
		else if(i==(length-2))temp1*=10;
		else if(i==(length-3))temp1*=100;
		else if(i==(length-4))temp1*=1000;
		temp+=temp1;
	}
	if(temp==0){
		temp=1;
		timer = temp*100;
		if(timerrun == true)SetTimer(1,temp*100,NULL);
		cedtLoopTime.SetWindowText(L"1");
	}
	else if(temp>1000){
		temp=255;
		timer = temp*100;
		if(timerrun == true)SetTimer(1,temp*100,NULL);
		cedtLoopTime.SetWindowText(L"1000");
	}else{
		CString cstrloogtime;
		cstrloogtime.Format(L"%d",temp);
		cedtLoopTime.SetWindowText(cstrloogtime);
		cedtLoopTime.SetSel(-1);
		timer = temp*100;
		if(timerrun == true)SetTimer(1,temp*100,NULL);
	}
}


void CSerialDebugDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case 1:	//ѭ�����Ͷ�ʱ��
		writenow = true;
		CDialog::OnTimer(nIDEvent);
		break;
	case 2: //��ʾ������ʾ��ʱ��
		InfoDlg.ShowWindow(SW_SHOW);
		CDialog::OnTimer(nIDEvent);
		break;
	default:
		CDialog::OnTimer(nIDEvent);
		break;
	}	
	
}


void CAboutDlg::OnNMClickSyslink10(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(this->m_hWnd,L"open",L"http://tech.sherryblue.org/index?action=update",L"",L"",SW_SHOW );
	*pResult = 0;
}

afx_msg LRESULT CSerialDebugDlg::OnUmTraynotify(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = (UINT)lParam;
	CMenu menuTray;		//����CMenu�����
	CPoint point;
	int id;
	switch(uMsg)
	{
		case WM_RBUTTONUP:
			//MessageBox("�Ҽ��¼�");
			menuTray.LoadMenu(IDR_MENU_TRAY);	//װ��˵�IDR_MENU_TRAY
			GetCursorPos(&point);	//��ȡ����λ��,����Ļ�����ʾ
			SetForegroundWindow();	// ����˵�ʧȥ���㲻��ʧ��BUG
			id = menuTray.GetSubMenu(0)->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
			/* GetSubMenu(0)���õ���һ���Ӳ˵�����ָ��   TrackPopuppMenu����ָ��λ����ʾ�˵���������ѡ�˵����ID
			����TPM_RETURNCMD  ʹ���������÷��ͳ�ȥ�Ĳ˵���Ϣ��Ϊ����ֵ���ء���ʱ�����ķ���ֵ��UINT���͵ģ�����ѡ�еĲ˵�ID
				TPM_LEFTALIGN  �˵��ԣ�x,y��Ϊ���Ͻ�
				TPM_RIGHTBUTTON  ����������Ҽ������������˵�������Ҽ�������ѡ���˵���*/
			MenuProcess(&menuTray,id);
		break;
		case WM_LBUTTONDBLCLK:
			//MessageBox("˫������¼�");
			Shell_NotifyIcon(NIM_DELETE,&m_nid);	// �������ͼ��
			ShowWindow(SW_SHOWNORMAL);	// �����ʾһ������
		break;
		default:
		break;
	}
	return 0;
}

//---------------------�Զ�����λ����¼���ʼ---------------------------//
void CSerialDebugDlg::OnBnClickedCheck3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	int addtype;
	CString stradd;
	//MessageBox("aa");
	unsigned char addbyte;
	switch(dwState)
	{
	case BST_UNCHECKED:
		statAdd.SetWindowText(L"NONE");
		modehex.SetWindowText(L"");
		break;
	case BST_CHECKED:
		CAddDiag.DoModal();
		addtype = CAddDiag.iAddType;
		switch(addtype)
		{
		case 0:
			statAdd.SetWindowText(L"CR");
			modehex.SetWindowText(L"0x0D");
			break;
		case 1:
			statAdd.SetWindowText(L"LF");
			modehex.SetWindowText(L"0x0A");
			break;
		case 2:
			statAdd.SetWindowText(L"CRLF");
			modehex.SetWindowText(L"0x0D 0x0A");
			break;
		case 3:
			statAdd.SetWindowText(L"BYTE");
			addbyte = CAddDiag.userdefined;
			stradd.Format(L"0x%02X",addbyte);
			modehex.SetWindowText(stradd);
			break;
		case 4:
			statAdd.SetWindowText(L"SUM8");
			UpdateAddByre();
			break;
		case 5:
			statAdd.SetWindowText(L"SUM16");
			UpdateAddByre();
			break;
		case 6:
			statAdd.SetWindowText(L"CRC16(8005H)");
			UpdateAddByre();
			break;
		case 7:
			statAdd.SetWindowText(L"CRC16(1021H)");
			UpdateAddByre();
			break;
		case 8:
			statAdd.SetWindowText(L"CRC32(04C11DB7H)");
			UpdateAddByre();
			break;
		}
		break;
	default:
		break; 
	}
}

void CSerialDebugDlg::UpdateAddByre(void)
{

	int length = SendContent.GetWindowTextLength();
	//MessageBox("bb");
	CString temp;
	SendContent.GetWindowText(temp);
	TCHAR buffer[1800];
	LPTSTR pbuff;
	pbuff = temp.GetBuffer();
	int crc,i,buflen = 0;
	if(SendContent.hexmode)
	{
		int size = SendContent.GetWindowTextLength();
		unsigned char temp1,temp2;
		unsigned char * ibuffer = NULL;
		ibuffer = new unsigned char[size+2];
		length = 0;
		SendContent.GetWindowText((LPTSTR)ibuffer,size+2);
		for(i=0;i<(size+1)/3;i++)
		{
			if((ibuffer[i*3] >= '0')&&(ibuffer[i*3] <= '9'))
				temp1 = ibuffer[i*3] - 48;
			if((ibuffer[i*3] >= 'A')&&(ibuffer[i*3] <= 'F'))
				temp1 = ibuffer[i*3] - 55;
			if((ibuffer[i*3+1] >= '0')&&(ibuffer[i*3+1] <= '9'))
				temp2 = ibuffer[i*3+1] - 48;
			if((ibuffer[i*3+1] >= 'A')&&(ibuffer[i*3+1] <= 'F'))
				temp2 = ibuffer[i*3+1] - 55;
			buffer[i] = temp1*16 + temp2;
			length++ ;
		}
		pbuff = (LPTSTR)buffer;
		delete[] ibuffer;
	}		
	int addtype = CAddDiag.iAddType;
	switch(addtype)
	{
	case 4:
		crc = 0;
		for(i=0;i<length;i++)crc+=pbuff[i];
		crc = crc & 0x00FF;
		temp.Format(L"0x%02X",(crc)&0x00FF);
		modehex.SetWindowText(temp);
		break;
	case 5:
		crc = 0;
		for(i=0;i<length;i++)crc+=pbuff[i];
		crc = crc & 0x0000FFFF;
		temp.Format(L"0x%02X 0x%02X",(crc)&0x00FF,(crc>>8)&0x00FF);
		modehex.SetWindowText(temp);
		break;
	case 6:	//CRC16(8005h)
		//crc = CRC16( pbuff,length);
		temp.Format(L"0x%02X 0x%02X",(crc)&0x00FF,(crc>>8)&0x00FF);
		modehex.SetWindowText(temp);
		break;
	case 7:	//CRC16(1021h)
		//crc = CRC16(pbuff,length);
		temp.Format(L"0x%02X 0x%02X",(crc)&0x00FF,(crc>>8)&0x00FF);
		modehex.SetWindowText(temp);
		break;
	case 8:
		//crc = crc32buf(pbuff,length);
		temp.Format(L"0x%02X 0x%02X 0x%02X 0x%02X",crc&0x000000FF,(crc>>8)&0x000000FF,(crc>>16)&0x000000FF,(crc>>24)&0x000000FF);
		modehex.SetWindowText(temp);
		break;
	}
	
}
//---------------------�Զ�����λ����¼�����---------------------------//

void CSerialDebugDlg::OnBnClickedCheckDtr()
{
	DCB mDCB;
	int state = ((CButton*)GetDlgItem(IDC_CHECK_DTR))->GetCheck();
	if(portopen == true)
	{
		if(GetCommState(hComm,&mDCB))
		{
			mDCB.fDtrControl = state;
			SetCommState(hComm,&mDCB);
		}
    }  
}


void CSerialDebugDlg::OnBnClickedCheckRts()
{
	DCB mDCB;
	int state = ((CButton*)GetDlgItem(IDC_CHECK_RTS))->GetCheck();
	if(portopen == true)
	{
		if(GetCommState(hComm,&mDCB))
		{
			mDCB.fRtsControl = state;
			SetCommState(hComm,&mDCB);
		}
    } 
}


void CSerialDebugDlg::DrawTitleBar(CDC *pDC)
{
	if (m_hWnd)
	{
		CBrush Brush(RGB(0,100,255));
		CBrush* pOldBrush = pDC->SelectObject(&Brush);

		CRect rtWnd, rtTitle, rtButtons;
		GetWindowRect(&rtWnd); 
		//ȡ�ñ�������λ��
		rtTitle.left = GetSystemMetrics(SM_CXFRAME);
		rtTitle.top = GetSystemMetrics(SM_CYFRAME);
		rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
		rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

		CPoint point;
		//��䶥�����
		point.x = rtWnd.Width();
		point.y = GetSystemMetrics(SM_CYFRAME)+20;
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//��������
		point.x = GetSystemMetrics(SM_CXFRAME) + 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//���ײ����
		point.x = rtWnd.Width(); 
		point.y = GetSystemMetrics(SM_CYFRAME) + 1;
		pDC->PatBlt(0, rtWnd.Height()-point.y, point.x, point.y, PATCOPY);
		//����Ҳ���
		point.x = GetSystemMetrics(SM_CXFRAME) + 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(rtWnd.Width()-point.x, 0, point.x, point.y, PATCOPY);
		
		pDC->SelectObject(pOldBrush);

		//�ػ�ͼ��
		m_rtIcon.left = GetSystemMetrics(SM_CXFRAME);
		m_rtIcon.top = GetSystemMetrics(SM_CYFRAME);
		m_rtIcon.right = m_rtIcon.left + 32;
		m_rtIcon.bottom = m_rtIcon.top + 32;
		::DrawIconEx(pDC->m_hDC, m_rtIcon.left, m_rtIcon.top, m_hIcon, 
			16, 16, 0, NULL, DI_NORMAL);
		m_rtIcon.OffsetRect(rtWnd.TopLeft()); 
		
		//׼����XP���ť
		CBitmap* pBitmap = new CBitmap;
		CBitmap* pOldBitmap;
		CDC* pDisplayMemDC=new CDC;
		pDisplayMemDC->CreateCompatibleDC(pDC);

		//��ͼ
		/*point.x = rtWnd.Width();
		point.y = GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYFRAME)+20;
		pBitmap->LoadBitmap(IDB_BITMAP1);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(0, 0, point.x, point.y, pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();*/

		//�ػ��ر�button
		//ClientToScreen(&rtButtons);
		rtButtons.left = rtTitle.right - 19;
		rtButtons.top = rtTitle.top;
		rtButtons.right = rtButtons.left + 19;
		rtButtons.bottom = rtButtons.top + 19;
		pBitmap->LoadBitmap(IDB_EXIT_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtExit = rtButtons;
		m_rtButtExit.OffsetRect(rtWnd.TopLeft()); 
		ScreenToClient(&m_rtButtExit);
		pBitmap->DeleteObject();

		//�ػ����/�ָ�button
		//ClientToScreen(&rtButtons);
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 20;
		if (IsZoomed())
			pBitmap->LoadBitmap(IDB_RESTORE_NORMAL);
		else
			pBitmap->LoadBitmap(IDB_MAX_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtMax = rtButtons;
		m_rtButtMax.OffsetRect(rtWnd.TopLeft());
		ScreenToClient(&m_rtButtMax);
		pBitmap->DeleteObject();

		//�ػ���С��button
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 20;
		pBitmap->LoadBitmap(IDB_MIN_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtMin = rtButtons;
		m_rtButtMin.OffsetRect(rtWnd.TopLeft());
		ScreenToClient(&m_rtButtMin);
		pBitmap->DeleteObject();
	
		//�ػ�����button
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 19;
		pBitmap->LoadBitmap(IDB_HELP_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtHelp = rtButtons;		
		m_rtButtHelp.OffsetRect(rtWnd.TopLeft());
		ScreenToClient(&m_rtButtHelp);
		pBitmap->DeleteObject();


		//����ͼƬ
		//pDC->StretchBlt(x,y, nW, nH ,&memDc,0,0,
		//bm.bmWidth,bm.bmHeight,SRCCOPY);
		
		//��д��������
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF clOldText=pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SelectStockObject(SYSTEM_FIXED_FONT);
		rtTitle.left += 22;
		//rtTitle.top += GetSystemMetrics(SM_CYSIZE);
		rtTitle.bottom = rtTitle.top + 30;
		//GetWindowText(m_strTitle);
		//MessageBox(m_strTitle);
		pDC->DrawText(m_strTitle, &rtTitle, DT_LEFT);
		pDC->SetBkMode(nOldMode);
		pDC->SetTextColor(clOldText);

		ReleaseDC(pDisplayMemDC);
		delete pDisplayMemDC;
		delete pBitmap;
	}
}


void CSerialDebugDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	CDC* pDC = GetWindowDC();
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(pDC);
	CBitmap* pBitmap = new CBitmap;
	CBitmap* pOldBitmap;
	CRect rtWnd, rtButton;

	//MessageBox("aa");

	if (pDC)
	{
		GetWindowRect(&rtWnd);

		//�ر�button
		//if (m_rtButtExit.PtInRect(point))
		//	pBitmap->LoadBitmap(IDB_EXIT_FOCUS);
		//else
		pBitmap->LoadBitmap(IDB_EXIT_NORMAL);
		rtButton = m_rtButtExit;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//���/�ָ�button
		if (m_rtButtMax.PtInRect(point))
		{
			//if (IsZoomed())
			//	pBitmap->LoadBitmap(IDB_RESTORE_FOCUS);
			//else
			//	pBitmap->LoadBitmap(IDB_MAX_FOCUS);
		}
		else
		{
			if (IsZoomed())
				pBitmap->LoadBitmap(IDB_RESTORE_NORMAL);
			else
				pBitmap->LoadBitmap(IDB_MAX_NORMAL);
		}
		rtButton = m_rtButtMax;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//��С��button
		//if (m_rtButtMin.PtInRect(point))
		//	pBitmap->LoadBitmap(IDB_MIN_FOCUS);
		//else
		pBitmap->LoadBitmap(IDB_MIN_NORMAL);
		rtButton = m_rtButtMin;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//����button
		//if (m_rtButtHelp.PtInRect(point))
		//	pBitmap->LoadBitmap(IDB_HELP_FOCUS);
		//else
		pBitmap->LoadBitmap(IDB_HELP_NORMAL);
		rtButton = m_rtButtHelp;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();
	}
	
	ReleaseDC(pDisplayMemDC);
	ReleaseDC(pDC);
	delete pDisplayMemDC;
	delete pBitmap;

	CDialog::OnNcMouseMove(nHitTest, point);

	//CDialog::OnNcMouseMove(nHitTest, point);
}


void CSerialDebugDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	WPARAM wParam = SC_SIZE;
	switch (nHitTest)
    {
		case HTTOP:
			wParam |= WMSZ_TOP;
			//Invalidate();
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));			
			break;
		case HTBOTTOM:
			wParam |= WMSZ_BOTTOM;
			//Invalidate();
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));			
			break;
		case HTLEFT:
			wParam |= WMSZ_LEFT;
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));
			break;
		case HTRIGHT:
			wParam |= WMSZ_RIGHT;
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));
			break;
		case HTTOPLEFT:
			wParam |= WMSZ_TOPLEFT;
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));
			break;
		case HTTOPRIGHT:
			wParam |= WMSZ_TOPRIGHT;
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));
			break;
		case HTBOTTOMRIGHT:
			wParam |= WMSZ_BOTTOMRIGHT;
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));
			break;
		case HTBOTTOMLEFT:
			wParam |= WMSZ_BOTTOMLEFT;
			SendMessage(WM_SYSCOMMAND, wParam, MAKELPARAM(point.x, point.y));
			break;
    }
	
	//������ť�Ƿ񰴵�
	if (m_rtButtHelp.PtInRect(point)){		
		//SendMessage(WM_HELP);

		CPoint point;
		int id;
		
		GetCursorPos(&point);	//��ȡ����λ��,����Ļ�����ʾ
		SetForegroundWindow();	// ����˵�ʧȥ���㲻��ʧ��BUG
		CMenu* pmenu = menuSys.GetSubMenu(0);
		id = pmenu->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
		pmenu->CheckMenuItem(IDR_TOPMOST,MF_BYCOMMAND | MF_CHECKED );
		MenuProcess(menuSys.GetSubMenu(0),id);
	}
	else if (m_rtButtExit.PtInRect(point))
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, MAKELPARAM(point.x, point.y));
	else if (m_rtButtMin.PtInRect(point))
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(point.x, point.y));
	else if (m_rtButtMax.PtInRect(point))
	{
		if (IsZoomed())
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
		else
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));
			Invalidate();
		}
	}
	else if (!IsZoomed())
		Default();

	CDialog::OnNcLButtonDown(nHitTest, point);
}


void CSerialDebugDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.y < 26){
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	CDialog::OnLButtonDown(nFlags, point);
}
void CSerialDebugDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	lpncsp->rgrc[0].top += 0;//�߶��Լ�����	
	CDialog::OnNcCalcSize(bCalcValidRects, lpncsp);
}


void CSerialDebugDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (point.y < 26){
		PostMessage(WM_NCMOUSEMOVE,HTCAPTION,MAKELPARAM(point.x,point.y));		
	}
	CDialog::OnMouseMove(nFlags, point);
}


void CSerialDebugDlg::OnBnClickedButton2()
{
	CString temp;
	
	HINSTANCE hInst;
	hInst = LoadLibrary(L"OpenDev.dll");					
	DllTestProc dllTest = (DllTestProc)GetProcAddress(hInst, "DllTest");
	bool flag = (*dllTest)();
	if (!flag){
		MessageBox(L"DLL�ļ���Ч");
	}

	SerialCallbackProc cb = (SerialCallbackProc)GetProcAddress(hInst, "SerialCallback");
	
	SERIALCALLBACK SeialCallback;	
	cb(&SeialCallback, &SeialUserData);

	delete SeialCallback.lpBuffer;

	FreeLibrary(hInst);
}


void CSerialDebugDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (point.y < 26){
		PostMessage(WM_NCLBUTTONDBLCLK,HTCAPTION,MAKELPARAM(point.x,point.y));		
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CSerialDebugDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	WINDOWPLACEMENT pwndpl;
	GetWindowPlacement(&pwndpl);
	if(pwndpl.showCmd == SW_SHOWMAXIMIZED){
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
	}else{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));
		Invalidate();
	}
	//CDialog::OnNcLButtonDblClk(nHitTest, point);
}


LRESULT CSerialDebugDlg::OnNcHitTest(CPoint point)
{
		//�����϶��ı䴰�ڴ�С
       int nFrameW = ::GetSystemMetrics(SM_CXFRAME);
       CRect winRc;
       GetWindowRect(winRc);
       CRect rcleft(winRc);
       CRect rcright(winRc);
       CRect rctop(winRc);
       CRect rcbottom(winRc);
 
 
       rcleft.right=rcleft.left+nFrameW;
       rcright.left=rcright.right-nFrameW;
       rctop.bottom=rctop.top+nFrameW;
       rcbottom.top=rcbottom.bottom-nFrameW;
 
	   if(rcleft.PtInRect(point) && rctop.PtInRect(point))
       {
              return HTTOPLEFT;
       }
	   if(rctop.PtInRect(point) && rcright.PtInRect(point))
       {
              return HTTOPRIGHT;
       }
	   if(rcbottom.PtInRect(point) && rcleft.PtInRect(point))
       {
              return HTBOTTOMLEFT;
       }
	   if(rcbottom.PtInRect(point) && rcright.PtInRect(point))
       {
              return HTBOTTOMRIGHT;
       }
	   if(rcleft.PtInRect(point))
       {
              return HTLEFT;
       }
 
       if(rcright.PtInRect(point))
       {
              return HTRIGHT;
       }
 
       if(rctop.PtInRect(point))
       {
              return HTTOP;
       }
 
       if(rcbottom.PtInRect(point))
       {
              return HTBOTTOM;
       }
	   return CDialog::OnNcHitTest(point);
}


BOOL CSerialDebugDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//return 1;
	return CDialog::OnEraseBkgnd(pDC);
}


void CSerialDebugDlg::MenuProcess(CMenu *pMenu, int Menuid)
{
	CAboutDlg dlgAbout;
	switch(Menuid)
	{
		case IDR_TRAY_RESTORE:
			Shell_NotifyIcon(NIM_DELETE,&m_nid);	// �������ͼ�� 
			ShowWindow(SW_SHOWNORMAL);	// �����ʾһ������
		break;
		case IDR_TRAY_EXIT:
			OnOK();
			Shell_NotifyIcon(NIM_DELETE,&m_nid);	// �������ͼ��
		break;
		case IDR_WINMANAGER:
			ShellExecute(this->m_hWnd,L"open",L"devmgmt.msc",L"",L"%SystemRoot%\\system32\\",SW_SHOW );
			break;
		case IDR_TRAY_SETTING:
			MySettingDlg.IniPath = McuDirectory;
			MySettingDlg.DoModal();
			break;
		case IDR_ABOUT:			
			dlgAbout.DoModal();
			break;
		case IDR_NEWVERSION:
			MyNewversionDlg.IniPath = McuDirectory;
			MyNewversionDlg.DoModal();
			break;
		case IDR_WINMANAGER1:
			ShellExecute(this->m_hWnd,L"open",L"devmgmt.msc",L"",L"%SystemRoot%\\system32\\",SW_SHOW );
			break;
		case IDR_TOPMOST:
			//CMenu* pSysMenu = GetSystemMenu(FALSE);	
			//MessageBox("aaa");
			if ( e==0 )
			{
				SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);  //�̶�
				pMenu->CheckMenuItem(IDR_TOPMOST,MF_BYCOMMAND | MF_UNCHECKED );  //��ӶԺ�
				
				e=1;
			}
			else
			{
				SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);  //ȡ���̶�
				pMenu->CheckMenuItem(IDR_TOPMOST,MF_BYCOMMAND | MF_CHECKED );  //ȡ���Ժ�
				e=0;
			}
			break;
		default:
		break;
	}
}


void CSerialDebugDlg::OnBnClickedBtnApisend()
{
	CString temp;
	CString title;
	btnAPISend.GetWindowText(title);
	if (title == "API����"){
		
		if (hDllInst) FreeLibrary(hDllInst);
		if (!hDllInst) hDllInst = LoadLibrary(L"OpenDev.dll");
		DllTestProc dllTest = (DllTestProc)GetProcAddress(hDllInst, "DllTest");
		bool flag = (*dllTest)();
		if (!flag){
			MessageBox(L"DLL�ļ���Ч");
		}

		SerialCallbackProc cb = (SerialCallbackProc)GetProcAddress(hDllInst, "SerialCallback");

		memset(&SeialUserData, 0, sizeof(SeialUserData));
		cb(&SeialCallback, &SeialUserData);

		if (SeialCallback.dwLength){
			btnAPISend.SetWindowText(L"ǿ��ֹͣ����");
			hasapidata = true;
			writenow = true;
		}
	}
	else{
		hasapidata = false;
		writenow = false;
		btnAPISend.SetWindowText(L"API����");
	}

}


void CSerialDebugDlg::APIDataload()
{

	if (!hDllInst) hDllInst = LoadLibrary(L"OpenDev.dll");
	DllTestProc dllTest = (DllTestProc)GetProcAddress(hDllInst, "DllTest");
	bool flag = (*dllTest)();
	if (!flag){
		MessageBox(L"DLL�ļ���Ч");
	}

	SerialCallbackProc cb = (SerialCallbackProc)GetProcAddress(hDllInst, "SerialCallback");
	
	cb(&SeialCallback, &SeialUserData);

	if (SeialCallback.dwLength){
		hasapidata = true;
		writenow = true;
	}

}
