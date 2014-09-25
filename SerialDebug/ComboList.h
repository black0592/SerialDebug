#pragma once


// CComboList

class CComboList : public CListBox
{
	DECLARE_DYNAMIC(CComboList)

public:
	CComboList();
	virtual ~CComboList();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


