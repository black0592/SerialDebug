#pragma once


// SDNewversionDlg �Ի���

class SDNewversionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SDNewversionDlg)

public:
	SDNewversionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SDNewversionDlg();

// �Ի�������
	enum { IDD = IDD_SDNEWVERSION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString IniPath;
	afx_msg void OnClickedCheckIfnew();
};
