// SDNewversionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SDNewversionDlg.h"
#include "afxdialogex.h"
#include <urlmon.h>
#pragma comment (lib,"Urlmon.lib") 


// SDNewversionDlg 对话框

IMPLEMENT_DYNAMIC(SDNewversionDlg, CDialogEx)

SDNewversionDlg::SDNewversionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SDNewversionDlg::IDD, pParent)
{

	IniPath = _T("");
}

SDNewversionDlg::~SDNewversionDlg()
{
}

void SDNewversionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SDNewversionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_IFNEW, &SDNewversionDlg::OnClickedCheckIfnew)
END_MESSAGE_MAP()


// SDNewversionDlg 消息处理程序


void SDNewversionDlg::OnClickedCheckIfnew()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemText(IDC_EDIT,L"正在检查，请等待。。。。。。");
	CoInitialize(NULL);  //初始化接口
	int ret = URLDownloadToFile(NULL,L"http://www.sherryblue.org/soft/SHB_Version.ini",L"SHB_Version.ini",0,NULL);
	if(ret == S_OK) //如果下载成功
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		*wcsrchr(path, '\\') = '\0';
		CString temp;
		temp.Format(L"%s",path);
		temp = temp + L"\\SHB_Version.ini";
		//MessageBox( temp );
		int newversion = GetPrivateProfileInt (L"Version",L"Version",110,temp);  // 读取Version 段的数据，得到新版本号
		if (newversion > 120) //跟当前版本比较，10表示目前版本V1.0 ，根据具体版本自己设置
		{
			SetDlgItemText(IDC_EDIT,L"有新版提供下载,现在开始更新.");
			TCHAR new_url[100];
			GetPrivateProfileString (L"URL",L"URL",0,new_url,100,temp);
			ShellExecute(this->m_hWnd,L"open",new_url,NULL,L"d:",SW_SHOW);
		}else{
			SetDlgItemText(IDC_EDIT,L"您使用的已经是最新版本");
		}
	}
}
