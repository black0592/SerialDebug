// InFoDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "InFoDialog.h"
#include "afxdialogex.h"


// CInFoDialog 对话框

IMPLEMENT_DYNAMIC(CInFoDialog, CDialogEx)

CInFoDialog::CInFoDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInFoDialog::IDD, pParent)
{

}

CInFoDialog::~CInFoDialog()
{
}

void CInFoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInFoDialog, CDialogEx)
END_MESSAGE_MAP()


// CInFoDialog 消息处理程序
