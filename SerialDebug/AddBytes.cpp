// AddBytes.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "AddBytes.h"
#include "afxdialogex.h"


// CAddBytes �Ի���

IMPLEMENT_DYNAMIC(CAddBytes, CDialogEx)

CAddBytes::CAddBytes(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddBytes::IDD, pParent)
{
	userdefined = 0x01;
}

CAddBytes::~CAddBytes()
{
}

void CAddBytes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ADDTYPE, cmbAddType);
	DDX_Control(pDX, IDC_EDIT_DEFINEDEC, medDefDec);
	DDX_Control(pDX, IDC_EDIT_DEFINEHEX, medDefHex);
	DDX_Control(pDX, IDC_STATIC1, cs1);
	DDX_Control(pDX, IDC_STATIC2, cs2);
	DDX_Control(pDX, IDC_STATIC3, cs3);
}


BEGIN_MESSAGE_MAP(CAddBytes, CDialogEx)
	//ON_BN_CLICKED(IDC_RADIO1, &CAddBytes::OnBnClickedRadio1)
	//ON_BN_CLICKED(IDC_RADIO2, &CAddBytes::OnBnClickedRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO_ADDTYPE, &CAddBytes::OnCbnSelchangeComboAddtype)
	ON_EN_CHANGE(IDC_EDIT_DEFINEDEC, &CAddBytes::OnEnChangeEditDefinedec)
END_MESSAGE_MAP()


// CAddBytes ��Ϣ�������


BOOL CAddBytes::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	cmbAddType.SetCurSel(iAddType);
	
	medDefDec.SetAllowedString(L"0123456789");
	medDefDec.SetLimitText(3);
	medDefHex.SetAllowedString(L"0123456789abcdefABCDEF");
	medDefHex.SetLimitText(2);
	TCHAR cDecAddr[4];
	cDecAddr[0] = (userdefined/100)+0x30;
	cDecAddr[1] = ((userdefined%100)/10)+0x30;
	cDecAddr[2] = (userdefined%10)+0x30;
	cDecAddr[3] = 0;
	medDefDec.SetWindowText(cDecAddr);
	CStatic* pCS;
	if(iAddType == 3)
	{
		medDefDec.ShowWindow(SW_SHOW);
		medDefHex.ShowWindow(SW_SHOW);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC1);
		pCS->ShowWindow(SW_SHOW);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC2);
		pCS->ShowWindow(SW_SHOW);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC3);
		pCS->ShowWindow(SW_SHOW);
	}
	else
	{
		medDefDec.ShowWindow(SW_HIDE);
		medDefHex.ShowWindow(SW_HIDE);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC1);
		pCS->ShowWindow(SW_HIDE);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC2);
		pCS->ShowWindow(SW_HIDE);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC3);
		pCS->ShowWindow(SW_HIDE);
	}

	return TRUE;
}

void CAddBytes::OnCbnSelchangeComboAddtype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	iAddType = cmbAddType.GetCurSel();
	//if(iAddType >= 4)
	//MessageBox("�˹�����δʵ����ȴ���һ�汾","��ʾ");
	CStatic* pCS;
	if(iAddType == 3)
	{
		medDefDec.ShowWindow(SW_SHOW);
		medDefHex.ShowWindow(SW_SHOW);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC1);
		pCS->ShowWindow(SW_SHOW);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC2);
		pCS->ShowWindow(SW_SHOW);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC3);
		pCS->ShowWindow(SW_SHOW);
	}
	else
	{
		medDefDec.ShowWindow(SW_HIDE);
		medDefHex.ShowWindow(SW_HIDE);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC1);
		pCS->ShowWindow(SW_HIDE);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC2);
		pCS->ShowWindow(SW_HIDE);
		pCS = (CStatic*)GetDlgItem(IDC_STATIC3);
		pCS->ShowWindow(SW_HIDE);
	}
	//switch
}



void CAddBytes::OnEnChangeEditDefinedec()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int length = medDefDec.GetWindowTextLength();
	TCHAR cDecAddr[4];
	TCHAR cHexAddr[3];
	medDefDec.GetWindowText(LPTSTR(cDecAddr),length+1);
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
		medDefDec.SetWindowText(L"255");
		medDefHex.SetWindowText(L"FF");
	}else{
		cHexAddr[0] = temp/16;
		cHexAddr[1] = temp%16;
		cHexAddr[2] = 0;
		if((cHexAddr[0]>=0)&&(cHexAddr[0]<=9))cHexAddr[0]+=0x30;
		else cHexAddr[0]+=55;
		if((cHexAddr[1]>=0)&&(cHexAddr[1]<=9))cHexAddr[1]+=0x30;
		else cHexAddr[1]+=55;
		medDefHex.ShowWindow(SW_HIDE);
		medDefHex.SetWindowText(LPTSTR(cHexAddr));
		medDefHex.ShowWindow(SW_SHOW);
		
	}
	userdefined = temp;
}

