// SerialCombo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "SerialCombo.h"


// CSerialCombo

IMPLEMENT_DYNAMIC(CSerialCombo, CComboBoxEx)

CSerialCombo::CSerialCombo()
{

}

CSerialCombo::~CSerialCombo()
{
}


BEGIN_MESSAGE_MAP(CSerialCombo, CComboBoxEx)
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CSerialCombo ��Ϣ�������




void CSerialCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if((nChar >= '0' && nChar <= '9')||nChar == VK_BACK ||nChar == VK_DELETE) //msdn��virtual key
	{ 
		MessageBox("aa");
		CComboBoxEx::OnChar(nChar, nRepCnt, nFlags);
	} 	
}


void CSerialCombo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	MessageBox("bb");
	CComboBoxEx::OnLButtonDown(nFlags, point);
}
