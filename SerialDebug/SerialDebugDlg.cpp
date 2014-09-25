// SerialDebugDlg.cpp : 实现文件
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
#define UM_TRAYNOTIFY WM_USER + 0x01	//定义自定义消息的ID


int e = 1;
CInFoDialog InfoDlg;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
// 实现
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

// CSerialDebugDlg 对话框


CSerialDebugDlg::CSerialDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialDebugDlg::IDD, pParent)
	, saddr(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	isendcount = 0;
	iresvcount = 0;
	memset(&m_nid, 0, sizeof(m_nid));	//memset:清空一个结构体类型的变量.Memset用来对一段内存空间全部设置为某个字符，一般用在对定义的字符串进行初始化为' '或'\0'；
	m_nid.cbSize = sizeof(m_nid);	//为m_nid结构体分配空间
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;	//标志着其他哪些成员的数据是有效的，NIF_ICON,NIF_MESSAGE,NIF_TIP，分别代表着数据有效的成员是hIcon,uCallbackMessage,szTip。三个值可以用“|”联系到一起
	wcscpy_s(m_nid.szTip, L"雪莉蓝串口调试助手");//图标提示为"测试程序"
	
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
	ON_MESSAGE(UM_TRAYNOTIFY, &CSerialDebugDlg::OnUmTraynotify) //注册信息
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


// CSerialDebugDlg 消息处理程序

void ThreadRecv(CSerialDebugDlg * pClass);
void ThreadSend(CSerialDebugDlg * pClass);
void ThreadMoniter(CSerialDebugDlg * pClass);
void OpenMoniter(CSerialDebugDlg * pClass);		//尝试打开串口线程

BOOL CSerialDebugDlg::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	m_strTitle = L"雪莉蓝串口调试助手 V1.3.0";
	CMenu menu;
	menu.CreateMenu();
	menu.AppendMenu(MF_STRING,IDM_WINMANAGER,L"Windows设备管理器");
	//menu.AppendMenu(MF_STRING,112,"Bye");


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;	 //这部分代码是获取ABOUTBOX控件的字符串ID，并把它赋值给字符串
		CString strSettingMenu;
		CString strFormerMenu;
		CString strNewversionMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);		//从 资源 里的 字符串资源 加载字符串到CString对象里
		strSettingMenu.LoadString(IDS_SETTINGBOX);
		strFormerMenu.LoadString(IDS_FORMERBOX);
		strNewversionMenu.LoadString(IDS_NEWVERSION);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_SETTINGBOX, strSettingMenu);			
			//pSysMenu->AppendMenu(MF_STRING, IDM_MANAGER, "串口管理");	
			pSysMenu->AppendMenu(MF_STRING, IDM_FORMERBOX, strFormerMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_WIKI, L"串口百科");
			pSysMenu->AppendMenu(MF_SEPARATOR);		 //绘制一条水平分隔线
			pSysMenu->AppendMenu(MF_STRING, IDM_NEWVERSION, strNewversionMenu);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);	//绘制“关于”菜单
			pSysMenu->InsertMenu(9,MF_POPUP|MF_BYPOSITION,(UINT)menu.m_hMenu,L"串口管理");
		}
	}

	menuSys.LoadMenu(IDR_MENU_SYS);	//装入菜单IDR_MENU_TRAY

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	TCHAR lpBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,lpBuffer);	//获得当前执行的exe的文件的绝对路径,获得的路径存到了 lpBuffer字符串变量里
	CString VendorListPath(lpBuffer);	//用lpBuffer 初始化了 VendorListPath‘
	McuDirectory = VendorListPath;

	GetClientRect(&m_rect);

	CWnd *pWnd;
    pWnd = GetDlgItem(IDC_EDIT_RESV);     //获取控件句柄
	pWnd->GetWindowRect(&rect1);
	ScreenToClient(&rect1);
	pWnd = GetDlgItem(IDC_EDIT_SEND);     //获取控件句柄
	pWnd->GetWindowRect(&rect2);
	ScreenToClient(&rect2);
	pWnd = GetDlgItem(IDC_BUTTON_SEND);     //获取控件句柄
	pWnd->GetWindowRect(&rect3);
	ScreenToClient(&rect3);
	pWnd = GetDlgItem(IDC_EDIT_SENDCON);     //获取控件句柄
	pWnd->GetWindowRect(&rect4);
	ScreenToClient(&rect4);
	pWnd = GetDlgItem(IDC_EDIT3);     //获取控件句柄
	pWnd->GetWindowRect(&rect5);
	ScreenToClient(&rect5);
	pWnd = GetDlgItem(IDC_SYSLINK5);     //获取控件句柄
	pWnd->GetWindowRect(&rect6);
	ScreenToClient(&rect6);
	pWnd = GetDlgItem(IDC_SYSLINK4);     //获取控件句柄
	pWnd->GetWindowRect(&rect7);
	ScreenToClient(&rect7);
	pWnd = GetDlgItem(IDC_SYSLINK2);     //获取控件句柄
	pWnd->GetWindowRect(&rect8);
	ScreenToClient(&rect8);
	pWnd = GetDlgItem(IDC_SPLITTER1);     //获取控件句柄
	pWnd->GetWindowRect(&rect9);
	ScreenToClient(&rect9);
	pWnd = GetDlgItem(IDC_COMBO_PORT);     //获取控件句柄
	pWnd->GetWindowRect(&rect10);
	ScreenToClient(&rect10);
	pWnd = GetDlgItem(IDC_STATIC_ADD);     //获取控件句柄
	pWnd->GetWindowRect(&rect11);
	ScreenToClient(&rect11);
	pWnd = GetDlgItem(IDC_EDIT2);     //获取控件句柄
	pWnd->GetWindowRect(&rect12);
	ScreenToClient(&rect12);
	pWnd = GetDlgItem(IDC_BTN_APISEND);     //获取控件句柄
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
	edSendCount.SetWindowText(L"发送：0");
	edResvCount.SetWindowText(L"接收：0");

	//::RegisterHotKey(m_hWnd,HOTKEY_SEND,MOD_CONTROL,VK_RETURN); 
	//::RegisterHotKey(SendContent.m_hWnd,HOTKEY_PASTEDEC,MOD_CONTROL,'D'); 
	//::RegisterHotKey(SendContent.m_hWnd,HOTKEY_PASTEHEX,MOD_CONTROL,'H'); 
	
	m_nid.hWnd = GetSafeHwnd();		//得到一个窗口对象（CWnd的派生对象）指针的句柄（HWND）
	m_nid.uCallbackMessage = UM_TRAYNOTIFY;		//消息标志，当用鼠标对托盘区相应图标进行操作的时候，就会传递消息给Hwnd所代表的窗口，一般为自定义的消息

	// Set tray icon and tooltip
	m_nid.hIcon = m_hIcon;	//加载托盘图标
	//((CButton*)GetDlgItem(IDC_CHECK_AUTOENTER))->SetCheck(BST_CHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_BORTRATE_MODE))->SetCheck(BST_CHECKED);
	OnBnClickedCheckBortrateMode();
	SendContent.insert = true;
	InfoDlg.Create(IDD_DIALOG_INFO,this);

	ComboPort.Init();

	CHARFORMAT2 cf1;
	CHARFORMAT cf;

	statusbar.SetWindowText(L"串口关闭状态");

		//声明为CHARFORMAT2结构，详细见MSDN
    ZeroMemory(&cf1, sizeof(CHARFORMAT2));
    cf1.cbSize = sizeof(CHARFORMAT2);
    cf1.dwMask = CFM_BACKCOLOR;

    cf1.crBackColor=RGB(255, 0, 0); //背景颜色为绿色
    statusbar.SetSel(0, 6); //设置处理区域
    statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

    ZeroMemory(&cf, sizeof(CHARFORMAT));
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
                        CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
    cf.dwEffects = 0;
    cf.yHeight = 20*20;//文字高度
    cf.crTextColor = RGB(255, 255, 255); //文字颜色
	wcscpy(cf.szFaceName, L"隶书");//设置字体
    statusbar.SetSel(0, 6); //设置处理区域
    statusbar.SetSelectionCharFormat(cf);
	statusbar.Invalidate();
	
	hasapidata = false;
	hDllInst = NULL;

	hThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadRecv,(void *)this,0,&ThreadID);
	hThreadSend = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadSend,(void *)this,0,&ThreadID);
	hThreadMoniter = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadMoniter,(void *)this,0,&ThreadID);
	hOpenMoniter = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)OpenMoniter,(void *)this,0,&ThreadID);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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
		int CloseAuto = GetPrivateProfileInt(L"SD",L"CLOSEAUTO",1,ConfigFile.GetBuffer());	//GetPrivateProfileInt():从私有初始化文件获取整型数值 
		if(CloseAuto == 0){
			writting = true;
			nRet = MyClosedlg.DoModal();	//调用该成员函数使用模态对话框并返回对话框结果。如果函数不能创建对话框，则返回-1；如果出现其它错误，则返回IDABORT。
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
					ShowWindow(SW_HIDE);	//隐藏主窗口
					Shell_NotifyIcon(NIM_ADD, &m_nid);	//在托盘区添加图标
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
				ShowWindow(SW_HIDE);	//隐藏主窗口
				Shell_NotifyIcon(NIM_ADD, &m_nid);	//在托盘区添加图标
			}
		}
        return;
    }else if(nID == IDM_FORMERBOX)
	{

	}	
	else	 //否则，这里也就是没选中“关于”或“设置”菜单
	{
		CDialog::OnSysCommand(nID, lParam);		//调用CDialog类中的OnSysCommand进行消息处理
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSerialDebugDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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


	SerialCount = 0;// 串口计数
	CString strSerialList[256];  // 临时定义 256 个字符串组
	CArray<SSerInfo,SSerInfo&> asi;
	SSerInfo mySer;
	EnumSerialPorts(asi,FALSE);// 参数为 TRUE 时枚举当前可以打开的串口， // 否则枚举所有串口

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
		MessageBox(L"您的电脑没有串口，可能需要插入USB转串口设备才能正常使用本程序",L"注意");
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

    pWnd = GetDlgItem(IDC_EDIT_RESV);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.right = rect1.left + rect1.Width() + cx - m_rect.Width();/////调整控件大小
		/*CString top;
		top.Format("%ld",rect.right);
		cedtMsg.SetWindowTextA(top);*/
		rect.bottom = rect1.top + rect1.Height() + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT_SEND);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.right = rect2.left + rect2.Width() + cx - m_rect.Width();/////调整控件大小
		rect.top = rect2.top + cy - m_rect.Height();
		rect.bottom = rect2.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_BUTTON_SEND);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.left = rect3.left + cx - m_rect.Width();/////调整控件大小
		rect.right = rect3.right + cx - m_rect.Width();/////调整控件大小
		rect.top = rect3.top + cy - m_rect.Height();
		rect.bottom = rect3.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT_SENDCON);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.top = rect4.top + cy - m_rect.Height();
		rect.bottom = rect4.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT3);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.top = rect5.top + cy - m_rect.Height();
		rect.bottom = rect5.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SYSLINK5);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.top = rect6.top + cy - m_rect.Height();
		rect.bottom = rect6.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SYSLINK4);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.top = rect7.top + cy - m_rect.Height();
		rect.bottom = rect7.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SYSLINK2);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.top = rect8.top + cy - m_rect.Height();
		rect.bottom = rect8.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_SPLITTER);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.right = rect9.left + rect9.Width() + cx - m_rect.Width();/////调整控件大小
		rect.top = rect9.top + cy - m_rect.Height();
		rect.bottom = rect9.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_COMBO_PORT);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.right = rect10.left + rect10.Width() + cx - m_rect.Width();/////调整控件大小
		//rect.top = rect10.top + cy - m_rect.Height();
		//rect.bottom = rect10.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_STATIC_ADD);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.right = rect11.left + rect11.Width() + cx - m_rect.Width();/////调整控件大小
		rect.top = rect11.top + cy - m_rect.Height();
		rect.bottom = rect11.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}

	pWnd = GetDlgItem(IDC_EDIT2);     //获取控件句柄
	if(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.right = rect12.left + rect12.Width() + cx - m_rect.Width();/////调整控件大小
		rect.top = rect12.top + cy - m_rect.Height();
		rect.bottom = rect12.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
		pWnd->Invalidate();
	}


	pWnd = GetDlgItem(IDC_BTN_APISEND);     //获取控件句柄
	if (pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		rect.left = rect13.left + cx - m_rect.Width();/////调整控件大小
		rect.right = rect13.right + cx - m_rect.Width();/////调整控件大小
		rect.top = rect13.top + cy - m_rect.Height();
		rect.bottom = rect13.bottom + cy - m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
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
					temp3.Format(L"接收：%d",pClass->iresvcount);
					pClass->edResvCount.SetWindowText((LPCTSTR)(temp3));
					RxBuff[nBytesRead+1] = 0x00;
					RxBuff[nBytesRead+2] = 0x00;
				
					//检测不能显示的ASCII码并用空格替换
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

						//将接受到的数据显示出来
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

				int SerialCount = 0;// 串口计数
				CString strSerialList[256];  // 临时定义 256 个字符串组
				CArray<SSerInfo,SSerInfo&> asi;
				SSerInfo mySer;
				EnumSerialPorts(asi,FALSE);// 参数为 TRUE 时枚举当前可以打开的串口， // 否则枚举所有串口

				SerialCount = asi.GetSize();

				for (int i=0; i<asi.GetSize(); i++)
				{
					mySer = asi[i];
					if((L"\\\\.\\"+ mySer.strPortName) == pClass->port)founded = true;
					//因虚拟串口的原因，算法有待改进
				}

				if(founded != true)
				{
					pClass->OnBnClickedButtonOpen();
					pClass->MessageBox(L"串口已拔出自动关闭，请重新插入USB串口设备，再次打开串口");
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
		if(pClass->chkAutoOpen.GetCheck() && portname == L"取消等待其它软件释放串口"){
			
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
					pClass->SetDlgItemText(IDC_BUTTON_OPEN,L"关闭串口"); 
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
					tips += L"已打开\n";
					tips1.Format(L"波特率：%dbps",pClass->dcb.BaudRate);
					tips += tips1;
					wcscpy_s(pClass->m_nid.szTip, tips);//图标提示为"测试程序"
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
		//判断串口状态
		CString portname;
		GetDlgItemText(IDC_BUTTON_OPEN,portname);
		if(portname == "打开串口")
		{
			
			CString temp;
			CString SeialPortDesc;
			CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
			pComboBox->GetWindowText(temp);
			CArray<SSerInfo,SSerInfo&> asi;
			EnumSerialPorts(asi,FALSE);// 参数为 TRUE 时枚举当前可以打开的串口， // 否则枚举所有串口
			
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
				SetDlgItemText(IDC_BUTTON_OPEN,L"关闭串口"); 
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
					szCaption = L"设置端口属性错误:" + szCaption;
					MessageBox(szCaption,L"错误",0);
					CloseHandle(hComm);
					hComm = INVALID_HANDLE_VALUE;
					portopen = false;
					SetDlgItemText(IDC_BUTTON_OPEN,L"打开串口");
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

					statusbar.SetWindowText(L"串口关闭状态");

						//声明为CHARFORMAT2结构，详细见MSDN
					ZeroMemory(&cf1, sizeof(CHARFORMAT2));
					cf1.cbSize = sizeof(CHARFORMAT2);
					cf1.dwMask = CFM_BACKCOLOR;

					cf1.crBackColor=RGB(255, 0, 0); //背景颜色为绿色
					statusbar.SetSel(0, 6); //设置处理区域
					statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

					ZeroMemory(&cf, sizeof(CHARFORMAT));
					cf.cbSize = sizeof(CHARFORMAT);
					cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
										CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
					cf.dwEffects = 0;
					cf.yHeight = 20*20;//文字高度
					cf.crTextColor = RGB(255, 255, 255); //文字颜色
					wcscpy(cf.szFaceName ,_T("隶书"));//设置字体
					statusbar.SetSel(0, 6); //设置处理区域
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
				tips += L"已打开\n";
				tips1.Format(L"波特率：%dbps",dcb.BaudRate);
				tips += tips1;
				wcscpy_s(m_nid.szTip, tips);//图标提示为"测试程序"
				//readnow = true;
				Invalidate();
			}
			else
			{
				dwError = GetLastError();
				CString   szCaption;   
				szCaption.Format(L"%d",dwError);
				if(dwError == 5 && chkAutoOpen.GetCheck()){
					SetDlgItemText(IDC_BUTTON_OPEN,L"取消等待其它软件释放串口");
					return;
				}
				switch(dwError)
				{
					case 2: 
						szCaption = L"对应所填写串口号的串口不存在";
						break;
					case 5:
						szCaption = L"串口被其它软件占用";
						
						break;						
					default:
						szCaption = L"串口打开错误:" + szCaption;
						break;
				}
				MessageBox(szCaption,L"错误",0);
				CHARFORMAT2 cf1;
				CHARFORMAT cf;
				statusbar.SetWindowText(L"串口关闭状态");

					//声明为CHARFORMAT2结构，详细见MSDN
				ZeroMemory(&cf1, sizeof(CHARFORMAT2));
				cf1.cbSize = sizeof(CHARFORMAT2);
				cf1.dwMask = CFM_BACKCOLOR;

				cf1.crBackColor=RGB(255, 0, 0); //背景颜色为绿色
				statusbar.SetSel(0, 6); //设置处理区域
				statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

				ZeroMemory(&cf, sizeof(CHARFORMAT));
				cf.cbSize = sizeof(CHARFORMAT);
				cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
									CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
				cf.dwEffects = 0;
				cf.yHeight = 20*20;//文字高度
				cf.crTextColor = RGB(255, 255, 255); //文字颜色
				wcscpy(cf.szFaceName ,_T("隶书"));//设置字体
				statusbar.SetSel(0, 6); //设置处理区域
				statusbar.SetSelectionCharFormat(cf);
				statusbar.Invalidate();
			}
		}else if(portname == "取消等待其它软件释放串口"){
			CString temp;
			CString SeialPortDesc;
			CArray<SSerInfo,SSerInfo&> asi;
			EnumSerialPorts(asi,FALSE);// 参数为 TRUE 时枚举当前可以打开的串口， // 否则枚举所有串口
			
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
			SetDlgItemText(IDC_BUTTON_OPEN,L"打开串口");
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
		SetDlgItemText(IDC_BUTTON_OPEN,L"打开串口");
		CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
		pComboBox->ModifyStyle(WS_DISABLED,0);  
		pComboBox->SetEditSel(-1,-1);
		pComboBox = (CComboBox*)GetDlgItem(IDC_BUTTON_SEND);
		pComboBox->ModifyStyle(0,WS_DISABLED);

		CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_DTR);
		pButton->ModifyStyle(0,WS_DISABLED);
		pButton = (CButton*)GetDlgItem(IDC_CHECK_RTS);
		pButton->ModifyStyle(0,WS_DISABLED);

		SendButton.SetWindowText(L"发送CTRL+ENTER");
		wcscpy_s(m_nid.szTip,L"BLUE串口调试助手");
		if(timerrun == true)KillTimer(1);
		Invalidate();

		CHARFORMAT2 cf1;
		CHARFORMAT cf;
		statusbar.SetWindowText(L"串口关闭状态");

			//声明为CHARFORMAT2结构，详细见MSDN
		ZeroMemory(&cf1, sizeof(CHARFORMAT2));
		cf1.cbSize = sizeof(CHARFORMAT2);
		cf1.dwMask = CFM_BACKCOLOR;

		cf1.crBackColor=RGB(255, 0, 0); //背景颜色为绿色
		statusbar.SetSel(0, 6); //设置处理区域
		statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

		ZeroMemory(&cf, sizeof(CHARFORMAT));
		cf.cbSize = sizeof(CHARFORMAT);
		cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
							CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
		cf.dwEffects = 0;
		cf.yHeight = 20*20;//文字高度
		cf.crTextColor = RGB(255, 255, 255); //文字颜色
		wcscpy(cf.szFaceName ,_T("隶书"));//设置字体
		statusbar.SetSel(0, 6); //设置处理区域
		statusbar.SetSelectionCharFormat(cf);
		statusbar.Invalidate();
	}

}

//-------------------------------------------------------------//
//----------------------串口号选择框事件处理-------------------//
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CString baud;
	int iparity,idata,istop;
	CComboBox * pComboBox;
	CString tips;
	CString tips1;

	DWORD dwState = ((CButton*)GetDlgItem(IDC_CHECK_BORTRATE_MODE))->GetCheck();
	switch(dwState)
	{
	case BST_UNCHECKED:	//自定义波特率
		((CEdit *)GetDlgItem(IDC_EDIT_BORTRATE))->GetWindowText(baud);
		break;
	case BST_CHECKED:	//标准波特率
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

	if(portname == L"关闭串口")
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
			szCaption = L"设置端口属性错误:" + szCaption;
			MessageBox(szCaption,L"错误",0);
		}

				CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
				pComboBox->GetWindowText(tips);
				tips += L"已打开\n";
				tips1.Format(L"波特率：%dbps",dcb.BaudRate);
				tips += tips1;
				wcscpy_s(m_nid.szTip,tips);//图标提示为"测试程序"
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
	if(btnname == L"停止发送")
	{
		writenow = false;
		SendButton.SetWindowText(L"发送CTRL+ENTER");
		KillTimer(1);
		timerrun = false;
	}else{
	
	int length = SendContent.GetWindowTextLength();
	unsigned char SendBuff[4100];
	if(length ==0)
	{
		MessageBox(L"请输入数据再发送",L"错误");
		SendContent.SetFocus();
	}
	else{
		if(chkFilesSend.GetCheck() == BST_CHECKED)
		{
			CFile txtfile;
			if(!txtfile.Open(filesendname,CFile::modeRead))
			{
				MessageBox(L"你所选择的文件无法打开",L"错误");
				chkFilesSend.SetCheck(BST_UNCHECKED);
				return;
			}
			CFileStatus rStatus;
			txtfile.GetStatus(rStatus);
			CTime temp = rStatus.m_mtime;
			if(temp > ModifyTime)
			{
				int cancel = MessageBox(L"您打开的文件可能已经被其它软件修改，重新载入文件按确定，发送修改内容按取消，",L"提示",MB_OKCANCEL);
				if(cancel == IDOK){
			
					length = (int)txtfile.GetLength();
					if(length > 4096)
					{
						MessageBox(L"你要打开的文件的数据量较大,发送区最多发送4K字节,超过部分将不会发送",L"警告");
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
			SendButton.SetWindowText(L"停止发送");
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
		statusbar.SetWindowText(L"正常显示状态");

		 //声明为CHARFORMAT2结构，详细见MSDN
        ZeroMemory(&cf1, sizeof(CHARFORMAT2));
        cf1.cbSize = sizeof(CHARFORMAT2);
        cf1.dwMask = CFM_BACKCOLOR;

        cf1.crBackColor=RGB(0, 0, 255); //背景颜色为绿色
        statusbar.SetSel(0, 6); //设置处理区域
        statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

        ZeroMemory(&cf, sizeof(CHARFORMAT));
        cf.cbSize = sizeof(CHARFORMAT);
        cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
                            CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
        cf.dwEffects = 0;
        cf.yHeight = 20*20;//文字高度
        cf.crTextColor = RGB(255, 255, 255); //文字颜色
        wcscpy(cf.szFaceName ,_T("隶书"));//设置字体
        statusbar.SetSel(0, 6); //设置处理区域
        statusbar.SetSelectionCharFormat(cf);
		statusbar.Invalidate();

		break;
	case BST_CHECKED:
		readnow = false;
		
		statusbar.SetWindowText(L"停止接收显示");
		
        ZeroMemory(&cf1, sizeof(CHARFORMAT2));
        cf1.cbSize = sizeof(CHARFORMAT2);
        cf1.dwMask = CFM_BACKCOLOR;

        cf1.crBackColor=RGB(255, 0, 0); //背景颜色为绿色
        statusbar.SetSel(0, 6); //设置处理区域

        statusbar.SendMessage(EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf1);

        ZeroMemory(&cf, sizeof(CHARFORMAT));
        cf.cbSize = sizeof(CHARFORMAT);
        cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
                            CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
        cf.dwEffects = 0;
        cf.yHeight = 20*20;//文字高度
        cf.crTextColor = RGB(255, 255, 255); //文字颜色
        wcscpy(cf.szFaceName ,_T("隶书"));//设置字体
        statusbar.SetSel(0, 6); //设置处理区域
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
	case BST_UNCHECKED:	//自定义波特率
		((CEdit *)GetDlgItem(IDC_EDIT_BORTRATE))->GetWindowText(baud);
		break;
	case BST_CHECKED:	//标准波特率
		break;
	default:
		break; 
	}
	TCHAR * buffer = baud.GetBuffer();
	baudrate = _wtoi(buffer);

	CString portname;
	GetDlgItemText(IDC_BUTTON_OPEN,portname);

	if(portname == "关闭串口")
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
			szCaption = L"设置端口属性错误:" + szCaption;
			MessageBox(szCaption,L"错误",0);
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
				temp3.Format(L"发送：%d",pClass->isendcount);
				pClass->edSendCount.SetWindowText((LPCTSTR)(temp3));
				dcb.Parity = SPACEPARITY;
				SetCommState(hComm,&dcb);
			}

			if (pClass->hasapidata == true){
				WriteFile(hComm, pClass->SeialCallback.lpBuffer, pClass->SeialCallback.dwLength, &nBytesWritten, NULL);
				pClass->isendcount += nBytesWritten;
				temp3.Format(L"发送：%d", pClass->isendcount);
				pClass->edSendCount.SetWindowText((LPCTSTR)(temp3));
				pClass->writenow = false;
				pClass->hasapidata = false;
				delete pClass->SeialCallback.lpBuffer;
				if (pClass->SeialCallback.bRepeat){
					pClass->APIDataload();
				}
				else{
					pClass->btnAPISend.SetWindowText(L"API发送");
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
				temp3.Format(L"发送：%d",pClass->isendcount);
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
				temp3.Format(L"发送：%d",pClass->isendcount);
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
				temp3.Format(L"发送：%d",pClass->isendcount);
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
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
	// TODO:  在此添加控件通知处理程序代码
}

void CSerialDebugDlg::OnEnChangeEditAddhex()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
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
	// TODO:  在此添加控件通知处理程序代码
}

void CSerialDebugDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	isendcount = 0;
	edSendCount.SetWindowText(L"发送：0");
	*pResult = 0;
}


void CSerialDebugDlg::OnNMClickSyslink4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	iresvcount = 0;
	edResvCount.SetWindowText(L"接收：0");
	*pResult = 0;
}


void CSerialDebugDlg::OnNMClickSyslink5(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	isendcount = 0;
	edSendCount.SetWindowText(L"发送：0");
	iresvcount = 0;
	edResvCount.SetWindowText(L"接收：0");
	*pResult = 0;
}


void CSerialDebugDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加专用代码和/或调用基类
	::UnregisterHotKey(m_hWnd,HOTKEY_SEND);
	
	return CDialog::DestroyWindow();
}


//BOOL CSerialDebugDlg::OnCommand(WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 在此添加专用代码和/或调用基类
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
//	// TODO: 在此添加专用代码和/或调用基类
//
//	CDialog::OnCancel();
//}


BOOL CSerialDebugDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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
		CFileDialog openfile(true,NULL,NULL,OFN_HIDEREADONLY|OFN_PATHMUSTEXIST|OFN_READONLY|OFN_FILEMUSTEXIST,L"Text Files(*.txt)|*.txt|所有文件(*.*)|*.*|",AfxGetMainWnd()); 
		openfile.m_ofn.lpstrFile=lpFile; 
		openfile.m_ofn.nMaxFile=1000; 
		openfile.m_ofn.lStructSize=sizeof(openfile.m_ofn); 
		openfile.m_ofn.lpstrInitialDir = NULL;
		openfile.m_ofn.lpstrTitle =L"选择要发送的文件";
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
				MessageBox(L"你所选择的文件无法打开",L"错误");
				chkFilesSend.SetCheck(BST_UNCHECKED);
				return;
			}
			
			length = txtfile.GetLength();
			if(length > 4096)
			{
				MessageBox(L"你要打开的文件的数据量较大,发送区最多发送4K字节,超过部分将不会发送",L"警告");
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
			//this->SetWindowTextA("标题栏修改测试");
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
		savefile.m_ofn.lpstrTitle =L"选择要保存接收的文件";
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
				MessageBox(L"你所选择的文件无法打开",L"错误");
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:	//循环发送定时器
		writenow = true;
		CDialog::OnTimer(nIDEvent);
		break;
	case 2: //提示窗口显示定时器
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
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(this->m_hWnd,L"open",L"http://tech.sherryblue.org/index?action=update",L"",L"",SW_SHOW );
	*pResult = 0;
}

afx_msg LRESULT CSerialDebugDlg::OnUmTraynotify(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg = (UINT)lParam;
	CMenu menuTray;		//定义CMenu类对象
	CPoint point;
	int id;
	switch(uMsg)
	{
		case WM_RBUTTONUP:
			//MessageBox("右键事件");
			menuTray.LoadMenu(IDR_MENU_TRAY);	//装入菜单IDR_MENU_TRAY
			GetCursorPos(&point);	//检取光标的位置,以屏幕坐标表示
			SetForegroundWindow();	// 解决菜单失去焦点不消失的BUG
			id = menuTray.GetSubMenu(0)->TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y, this);
			/* GetSubMenu(0)来得到第一层子菜单的类指针   TrackPopuppMenu：在指定位置显示菜单并发送所选菜单项的ID
			——TPM_RETURNCMD  使函数将本该发送出去的菜单消息作为返回值返回。这时函数的返回值是UINT类型的，就是选中的菜单ID
				TPM_LEFTALIGN  菜单以（x,y）为左上角
				TPM_RIGHTBUTTON  连续按鼠标右键会连续弹出菜单，鼠标右键可用于选定菜单项*/
			MenuProcess(&menuTray,id);
		break;
		case WM_LBUTTONDBLCLK:
			//MessageBox("双击左键事件");
			Shell_NotifyIcon(NIM_DELETE,&m_nid);	// 清除托盘图标
			ShowWindow(SW_SHOWNORMAL);	// 激活并显示一个窗口
		break;
		default:
		break;
	}
	return 0;
}

//---------------------自动附加位相关事件开始---------------------------//
void CSerialDebugDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
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
//---------------------自动附加位相关事件结束---------------------------//

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
		//取得标题栏的位置
		rtTitle.left = GetSystemMetrics(SM_CXFRAME);
		rtTitle.top = GetSystemMetrics(SM_CYFRAME);
		rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
		rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

		CPoint point;
		//填充顶部框架
		point.x = rtWnd.Width();
		point.y = GetSystemMetrics(SM_CYFRAME)+20;
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充左侧框架
		point.x = GetSystemMetrics(SM_CXFRAME) + 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充底部框架
		point.x = rtWnd.Width(); 
		point.y = GetSystemMetrics(SM_CYFRAME) + 1;
		pDC->PatBlt(0, rtWnd.Height()-point.y, point.x, point.y, PATCOPY);
		//填充右侧框架
		point.x = GetSystemMetrics(SM_CXFRAME) + 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(rtWnd.Width()-point.x, 0, point.x, point.y, PATCOPY);
		
		pDC->SelectObject(pOldBrush);

		//重画图标
		m_rtIcon.left = GetSystemMetrics(SM_CXFRAME);
		m_rtIcon.top = GetSystemMetrics(SM_CYFRAME);
		m_rtIcon.right = m_rtIcon.left + 32;
		m_rtIcon.bottom = m_rtIcon.top + 32;
		::DrawIconEx(pDC->m_hDC, m_rtIcon.left, m_rtIcon.top, m_hIcon, 
			16, 16, 0, NULL, DI_NORMAL);
		m_rtIcon.OffsetRect(rtWnd.TopLeft()); 
		
		//准备画XP风格按钮
		CBitmap* pBitmap = new CBitmap;
		CBitmap* pOldBitmap;
		CDC* pDisplayMemDC=new CDC;
		pDisplayMemDC->CreateCompatibleDC(pDC);

		//贴图
		/*point.x = rtWnd.Width();
		point.y = GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYFRAME)+20;
		pBitmap->LoadBitmap(IDB_BITMAP1);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(0, 0, point.x, point.y, pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();*/

		//重画关闭button
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

		//重画最大化/恢复button
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

		//重画最小化button
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
	
		//重画帮助button
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


		//拉伸图片
		//pDC->StretchBlt(x,y, nW, nH ,&memDc,0,0,
		//bm.bmWidth,bm.bmHeight,SRCCOPY);
		
		//重写标题文字
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

		//关闭button
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

		//最大化/恢复button
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

		//最小化button
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

		//帮助button
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
	
	//检测各按钮是否按到
	if (m_rtButtHelp.PtInRect(point)){		
		//SendMessage(WM_HELP);

		CPoint point;
		int id;
		
		GetCursorPos(&point);	//检取光标的位置,以屏幕坐标表示
		SetForegroundWindow();	// 解决菜单失去焦点不消失的BUG
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
	lpncsp->rgrc[0].top += 0;//尺度自己把握	
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
		MessageBox(L"DLL文件无效");
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		//处理拖动改变窗口大小
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//return 1;
	return CDialog::OnEraseBkgnd(pDC);
}


void CSerialDebugDlg::MenuProcess(CMenu *pMenu, int Menuid)
{
	CAboutDlg dlgAbout;
	switch(Menuid)
	{
		case IDR_TRAY_RESTORE:
			Shell_NotifyIcon(NIM_DELETE,&m_nid);	// 清除托盘图标 
			ShowWindow(SW_SHOWNORMAL);	// 激活并显示一个窗口
		break;
		case IDR_TRAY_EXIT:
			OnOK();
			Shell_NotifyIcon(NIM_DELETE,&m_nid);	// 清除托盘图标
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
				SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);  //固顶
				pMenu->CheckMenuItem(IDR_TOPMOST,MF_BYCOMMAND | MF_UNCHECKED );  //添加对号
				
				e=1;
			}
			else
			{
				SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);  //取消固顶
				pMenu->CheckMenuItem(IDR_TOPMOST,MF_BYCOMMAND | MF_CHECKED );  //取消对号
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
	if (title == "API发送"){
		
		if (hDllInst) FreeLibrary(hDllInst);
		if (!hDllInst) hDllInst = LoadLibrary(L"OpenDev.dll");
		DllTestProc dllTest = (DllTestProc)GetProcAddress(hDllInst, "DllTest");
		bool flag = (*dllTest)();
		if (!flag){
			MessageBox(L"DLL文件无效");
		}

		SerialCallbackProc cb = (SerialCallbackProc)GetProcAddress(hDllInst, "SerialCallback");

		memset(&SeialUserData, 0, sizeof(SeialUserData));
		cb(&SeialCallback, &SeialUserData);

		if (SeialCallback.dwLength){
			btnAPISend.SetWindowText(L"强行停止发送");
			hasapidata = true;
			writenow = true;
		}
	}
	else{
		hasapidata = false;
		writenow = false;
		btnAPISend.SetWindowText(L"API发送");
	}

}


void CSerialDebugDlg::APIDataload()
{

	if (!hDllInst) hDllInst = LoadLibrary(L"OpenDev.dll");
	DllTestProc dllTest = (DllTestProc)GetProcAddress(hDllInst, "DllTest");
	bool flag = (*dllTest)();
	if (!flag){
		MessageBox(L"DLL文件无效");
	}

	SerialCallbackProc cb = (SerialCallbackProc)GetProcAddress(hDllInst, "SerialCallback");
	
	cb(&SeialCallback, &SeialUserData);

	if (SeialCallback.dwLength){
		hasapidata = true;
		writenow = true;
	}

}
