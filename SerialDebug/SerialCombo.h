#pragma once


// CSerialCombo

class CSerialCombo : public CComboBoxEx
{
	DECLARE_DYNAMIC(CSerialCombo)

public:
	CSerialCombo();
	virtual ~CSerialCombo();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


