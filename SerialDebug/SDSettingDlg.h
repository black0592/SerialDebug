#pragma once


// SDSettingDlg �Ի���

class SDSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SDSettingDlg)

public:
	SDSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SDSettingDlg();
	CString IniPath;

// �Ի�������
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	CButton m_CheckTray;
	afx_msg void OnBnClickedCheckTray();
	CButton m_CheckAck;
};
