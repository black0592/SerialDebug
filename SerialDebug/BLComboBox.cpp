// BLComboBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "BLComboBox.h"
#include "InfoDialog.h"

extern CInFoDialog InfoDlg;


// CBLComboBox

IMPLEMENT_DYNAMIC(CBLComboBox, CComboBox)

CBLComboBox::CBLComboBox()
{
	
	//GetComboBoxInfo(&info);
	//m_ListBox.SubclassWindow(info.hwndList);
	m_bAllowTrack = true;
	//int	id = GetWindowLong(info.hwndList,GWL_ID);

}

CBLComboBox::~CBLComboBox()
{
}


BEGIN_MESSAGE_MAP(CBLComboBox, CComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_CHAR()
END_MESSAGE_MAP()




void CBLComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_bAllowTrack)
	{
		TRACKMOUSEEVENT tme;
	
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 10;	//HOVER_DEFAULT;
		tme.hwndTrack = m_hWnd;

		_TrackMouseEvent(&tme);

		m_bAllowTrack = false;
	}

	CString temp;
	temp.Format(L"%d",point.x);
	//SetWindowTextA(temp);
	//KillTimer(2);
	//InfoDlg.ShowWindow(SW_HIDE);
	//SetTimer(this->GetParent(),2,500,NULL);
	CComboBox::OnMouseMove(nFlags, point);

}


void CBLComboBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CComboBox::OnTimer(nIDEvent);
}


void CBLComboBox::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CComboBox::OnMouseHover(nFlags, point);
}


void CBLComboBox::OnMouseLeave()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bAllowTrack = true;
	CComboBox::OnMouseLeave();
	//MessageBox("Leave");
}


void CBLComboBox::Init(void)
{
	//GetComboBoxInfo(&info);
	//m_ListBox.SubclassWindow(info.hwndList);
}


HBRUSH CBLComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_LISTBOX)
    {
         //ListBox control
         /*if (m_ListBox.GetSafeHwnd() == NULL)
            m_ListBox.SubclassWindow(pWnd->GetSafeHwnd());*/
    }

	
	
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CBLComboBox::OnDestroy()
{
	/*if (m_ListBox.GetSafeHwnd() != NULL)
         m_ListBox.UnsubclassWindow();*/

	CComboBox::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}


void CBLComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int nStartOff,nEndOff;
	//GetSel(nStartOff,nEndOff);
	/*if(nChar == 0x08)
	{	
		MessageBox("aa");
	}else{
		CComboBox::OnChar(nChar, nRepCnt, nFlags);
	}*/

	
}
