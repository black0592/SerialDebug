// ComboList.cpp : ʵ���ļ�
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



// CComboList ��Ϣ�������




//HBRUSH CComboList::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
//{
//	// TODO:  �ڴ˸��� DC ���κ�����
//
//	// TODO:  �����Ӧ���ø����Ĵ�������򷵻ط� null ����
//	return NULL;
//}


void CComboList::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//MessageBox("List");
	CListBox::OnMouseMove(nFlags, point);
}
