// SerialDebug.cpp : ����Ӧ�ó��������Ϊ��
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


// CSerialDebugApp ����

CSerialDebugApp::CSerialDebugApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSerialDebugApp ����

CSerialDebugApp theApp;


// CSerialDebugApp ��ʼ��

BOOL CSerialDebugApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	//RegisterHotKey(NULL,HOTKEY_SEND,MOD_CONTROL,VK_RETURN); 

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
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
