#pragma once

#include "ComboList.h"

// CBLComboBox

class CBLComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CBLComboBox)

public:
	bool m_bAllowTrack;
	COMBOBOXINFO info;
	CBLComboBox();
	virtual ~CBLComboBox();

protected:
	CComboList m_ListBox;
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	void Init(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


