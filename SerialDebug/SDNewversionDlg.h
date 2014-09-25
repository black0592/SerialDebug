#pragma once


// SDNewversionDlg 对话框

class SDNewversionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SDNewversionDlg)

public:
	SDNewversionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SDNewversionDlg();

// 对话框数据
	enum { IDD = IDD_SDNEWVERSION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString IniPath;
	afx_msg void OnClickedCheckIfnew();
};
