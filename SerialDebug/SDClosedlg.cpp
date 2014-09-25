// SDClosedlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SDClosedlg.h"
#include "afxdialogex.h"


// SDClosedlg 对话框

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


// SDClosedlg 消息处理程序


BOOL SDClosedlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_CheckSave.SetCheck(BST_CHECKED);
	Checked = 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void SDClosedlg::OnBnClickedCheckSave()
{
	// TODO: 在此添加控件通知处理程序代码
	Checked = m_CheckSave.GetCheck();
}


void SDClosedlg::OnBnClickedMini()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDC_MINI);
}
