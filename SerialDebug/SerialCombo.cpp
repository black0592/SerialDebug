// SerialCombo.cpp : 实现文件
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



// CSerialCombo 消息处理程序




void CSerialCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if((nChar >= '0' && nChar <= '9')||nChar == VK_BACK ||nChar == VK_DELETE) //msdn的virtual key
	{ 
		MessageBox("aa");
		CComboBoxEx::OnChar(nChar, nRepCnt, nFlags);
	} 	
}


void CSerialCombo::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	MessageBox("bb");
	CComboBoxEx::OnLButtonDown(nFlags, point);
}
