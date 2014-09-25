#if !defined(AFX_CHEXEDIT_H__FB77A406_629C_452D_9CCC_57A5F3EB5E30__INCLUDED_)
#define AFX_CHEXEDIT_H__FB77A406_629C_452D_9CCC_57A5F3EB5E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CHexEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHexEdit window

class CHexEdit : public CEdit
{
// Construction
public:
	CHexEdit();

private:

	

// Attributes
public:
	void SetHexMode();
	void SetNormalMode();

// Operations
public:
	bool noinput;
	bool hexmode;
	bool insert;
	bool pressed;
	int StartChar;
 // Overrides
 // ClassWizard generated virtual function overrides
 //{{AFX_VIRTUAL(CHexEdit)
 //}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHexEdit();

 // Generated message map functions
protected:
 //{{AFX_MSG(CHexEdit)
 afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
 afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
 afx_msg LRESULT OnPaste(WPARAM,LPARAM);
 afx_msg LRESULT OnClear(WPARAM,LPARAM);
 afx_msg LRESULT OnCut(WPARAM,LPARAM);
 //}}AFX_MSG

 DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void PasteDec(WPARAM m,LPARAM l);
	void PasteHex(void);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHEXEDIT_H__FB77A406_629C_452D_9CCC_57A5F3EB5E30__INCLUDED_)

