// SDNewversionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SDNewversionDlg.h"
#include "afxdialogex.h"
#include <urlmon.h>
#pragma comment (lib,"Urlmon.lib") 


// SDNewversionDlg �Ի���

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


// SDNewversionDlg ��Ϣ�������


void SDNewversionDlg::OnClickedCheckIfnew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SetDlgItemText(IDC_EDIT,L"���ڼ�飬��ȴ�������������");
	CoInitialize(NULL);  //��ʼ���ӿ�
	int ret = URLDownloadToFile(NULL,L"http://www.sherryblue.org/soft/SHB_Version.ini",L"SHB_Version.ini",0,NULL);
	if(ret == S_OK) //������سɹ�
	{
		TCHAR path[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		*wcsrchr(path, '\\') = '\0';
		CString temp;
		temp.Format(L"%s",path);
		temp = temp + L"\\SHB_Version.ini";
		//MessageBox( temp );
		int newversion = GetPrivateProfileInt (L"Version",L"Version",110,temp);  // ��ȡVersion �ε����ݣ��õ��°汾��
		if (newversion > 120) //����ǰ�汾�Ƚϣ�10��ʾĿǰ�汾V1.0 �����ݾ���汾�Լ�����
		{
			SetDlgItemText(IDC_EDIT,L"���°��ṩ����,���ڿ�ʼ����.");
			TCHAR new_url[100];
			GetPrivateProfileString (L"URL",L"URL",0,new_url,100,temp);
			ShellExecute(this->m_hWnd,L"open",new_url,NULL,L"d:",SW_SHOW);
		}else{
			SetDlgItemText(IDC_EDIT,L"��ʹ�õ��Ѿ������°汾");
		}
	}
}
