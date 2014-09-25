// SDClosedlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SDClosedlg.h"
#include "afxdialogex.h"


// SDClosedlg �Ի���

IMPLEMENT_DYNAMIC(SDClosedlg, CDialogEx)

SDClosedlg::SDClosedlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SDClosedlg::IDD, pParent)
{

	Checked = 0;
}

SDClosedlg::~SDClosedlg()
{
}

void SDClosedlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SAVE, m_CheckSave);
}


BEGIN_MESSAGE_MAP(SDClosedlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_SAVE, &SDClosedlg::OnBnClickedCheckSave)
	ON_BN_CLICKED(IDC_MINI, &SDClosedlg::OnBnClickedMini)
END_MESSAGE_MAP()


// SDClosedlg ��Ϣ�������


BOOL SDClosedlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_CheckSave.SetCheck(BST_CHECKED);
	Checked = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void SDClosedlg::OnBnClickedCheckSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Checked = m_CheckSave.GetCheck();
}


void SDClosedlg::OnBnClickedMini()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(IDC_MINI);
}
