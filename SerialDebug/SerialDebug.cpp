// SerialDebug.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SerialDebugDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HOTKEY_SEND 0x1000


// CSerialDebugApp

BEGIN_MESSAGE_MAP(CSerialDebugApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSerialDebugApp 构造

CSerialDebugApp::CSerialDebugApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSerialDebugApp 对象

CSerialDebugApp theApp;


// CSerialDebugApp 初始化

BOOL CSerialDebugApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	//RegisterHotKey(NULL,HOTKEY_SEND,MOD_CONTROL,VK_RETURN); 

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("BLUE_Serial_Debug"));
	//HMODULE hMod;
	//hMod = LoadLibrary(_T("riched20.dll"));
	AfxInitRichEdit2();

	CSerialDebugDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	//FreeLibrary(hMod);
	return FALSE;
}
