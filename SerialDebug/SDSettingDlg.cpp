// SDSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SDSettingDlg.h"
#include "afxdialogex.h"


// SDSettingDlg �Ի���

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


// SDSettingDlg ��Ϣ�������


BOOL SDSettingDlg::OnInitDialog()	//��ʼ���Ի���
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString ConfigFile = IniPath + L"\\.\\config.ini";
	//CString ConfigFile = "E:\\SerialDebug\\SerialDebug\\config.ini";
	int CloseType = GetPrivateProfileInt(L"SD",L"CLOSETYPE",1,ConfigFile.GetBuffer());
	int CloseAuto = GetPrivateProfileInt(L"SD",L"CLOSEAUTO",1,ConfigFile.GetBuffer());
	//MessageBox()
	if(CloseAuto == 1)CloseAuto = 0;
	else CloseAuto = 1;
	m_CheckTray.SetCheck(CloseType);	//SetCheck���û����õ�ѡ��ť�ĸ�ѡ״̬ ����������ť״̬��Ϊ��ѡ�У�δѡ�У���ȷ��״̬
	m_CheckAck.SetCheck(CloseAuto);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void SDSettingDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	WritePrivateProfileString(L"SD",L"CLOSETYPE",strCheck.GetBuffer(),ConfigFile.GetBuffer());	//lpAppName,ini�ļ��е�һ���ֶ���[����]�����кܶ������/lpKeyName,lpAppName�µ�һ������,Ҳ�����������ı�����/lpString,��ֵ,Ҳ��������/lpFileName,INI�ļ���·��
	WritePrivateProfileString(L"SD",L"CLOSEAUTO",strAckCheck.GetBuffer(),ConfigFile.GetBuffer());

	CDialogEx::OnOK();
}


void SDSettingDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
}
