#pragma once
#include "afxwin.h"
#include "MaskedEdit.h"


// CAddBytes �Ի���

class CAddBytes : public CDialogEx
{
	DECLARE_DYNAMIC(CAddBytes)
	//{{AFX_DATA(CAddBytes)
public:
	CAddBytes(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddBytes();

// �Ի�������
	enum { IDD = IDD_ADDBYTES };
	//}}AFX_DATA

// ʵ��
protected:
	// ���ɵ���Ϣӳ�亯��
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
