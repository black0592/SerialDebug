#pragma once
#include "afxwin.h"
#include "MaskedEdit.h"


// CAddBytes 对话框

class CAddBytes : public CDialogEx
{
	DECLARE_DYNAMIC(CAddBytes)
	//{{AFX_DATA(CAddBytes)
public:
	CAddBytes(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddBytes();

// 对话框数据
	enum { IDD = IDD_ADDBYTES };
	//}}AFX_DATA

// 实现
protected:
	// 生成的消息映射函数
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CComboBox cmbAddType;
	int iAddType;
	unsigned char userdefined;
	afx_msg void OnCbnSelchangeComboAddtype();
	CMaskedEdit medDefDec;
	CMaskedEdit medDefHex;
	afx_msg void OnEnChangeEditDefinedec();
	CStatic cs1;
	CStatic cs2;
	CStatic cs3;
};
