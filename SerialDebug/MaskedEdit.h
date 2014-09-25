#pragma once


// CMaskedEdit

class CMaskedEdit : public CEdit
{
	DECLARE_DYNAMIC(CMaskedEdit)

public:
	CMaskedEdit();
	void SetAllowedString(CString alloweds);
	virtual ~CMaskedEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
private:
	CString allowed;
};


