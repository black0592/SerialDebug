// InFoDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "InFoDialog.h"
#include "afxdialogex.h"


// CInFoDialog �Ի���

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


// CInFoDialog ��Ϣ�������
