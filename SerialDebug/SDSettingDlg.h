#pragma once


// SDSettingDlg 对话框

class SDSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SDSettingDlg)

public:
	SDSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SDSettingDlg();
	CString IniPath;

// 对话框数据
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	CButton m_CheckTray;
	afx_msg void OnBnClickedCheckTray();
	CButton m_CheckAck;
};
