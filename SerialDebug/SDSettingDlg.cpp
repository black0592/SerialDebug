// SDSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SDSettingDlg.h"
#include "afxdialogex.h"


// SDSettingDlg 对话框

IMPLEMENT_DYNAMIC(SDSettingDlg, CDialogEx)

SDSettingDlg::SDSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SDSettingDlg::IDD, pParent)
{

}

SDSettingDlg::~SDSettingDlg()
{
}

void SDSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_TRAY, m_CheckTray);
	DDX_Control(pDX, IDC_CHECK_ACK, m_CheckAck);
}


BEGIN_MESSAGE_MAP(SDSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// SDSettingDlg 消息处理程序


BOOL SDSettingDlg::OnInitDialog()	//初始化对话框
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString ConfigFile = IniPath + L"\\.\\config.ini";
	//CString ConfigFile = "E:\\SerialDebug\\SerialDebug\\config.ini";
	int CloseType = GetPrivateProfileInt(L"SD",L"CLOSETYPE",1,ConfigFile.GetBuffer());
	int CloseAuto = GetPrivateProfileInt(L"SD",L"CLOSEAUTO",1,ConfigFile.GetBuffer());
	//MessageBox()
	if(CloseAuto == 1)CloseAuto = 0;
	else CloseAuto = 1;
	m_CheckTray.SetCheck(CloseType);	//SetCheck设置或重置单选按钮的复选状态 参数：讲按钮状态设为已选中，未选中，不确定状态
	m_CheckAck.SetCheck(CloseAuto);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void SDSettingDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString ConfigFile = IniPath + L"\\.\\config.ini";
	int checked = m_CheckTray.GetCheck();
	CString strCheck;
	//strCheck = "1";
	strCheck.Format(L"%d",checked);
	int ackchecked = m_CheckAck.GetCheck();
	if(ackchecked == 1)ackchecked = 0;
	else ackchecked = 1;
	CString strAckCheck;
	strAckCheck.Format(L"%d",ackchecked);
	WritePrivateProfileString(L"SD",L"CLOSETYPE",strCheck.GetBuffer(),ConfigFile.GetBuffer());	//lpAppName,ini文件中的一个字段名[节名]可以有很多个节名/lpKeyName,lpAppName下的一个键名,也就是里面具体的变量名/lpString,键值,也就是数据/lpFileName,INI文件的路径
	WritePrivateProfileString(L"SD",L"CLOSEAUTO",strAckCheck.GetBuffer(),ConfigFile.GetBuffer());

	CDialogEx::OnOK();
}


void SDSettingDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
}
