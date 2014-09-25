// ComboList.cpp : 实现文件
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "ComboList.h"


// CComboList

IMPLEMENT_DYNAMIC(CComboList, CListBox)

CComboList::CComboList()
{

}

CComboList::~CComboList()
{
}


BEGIN_MESSAGE_MAP(CComboList, CListBox)
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CComboList 消息处理程序




//HBRUSH CComboList::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
//{
//	// TODO:  在此更改 DC 的任何特性
//
//	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔
//	return NULL;
//}


void CComboList::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//MessageBox("List");
	CListBox::OnMouseMove(nFlags, point);
}
