#pragma once


// SDClosedlg �Ի���

class SDClosedlg : public CDialogEx
{
	DECLARE_DYNAMIC(SDClosedlg)

public:
	SDClosedlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SDClosedlg();

// �Ի�������
	enum { IDD = IDD_CLOSE_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckSave();
	virtual BOOL OnInitDialog();
	CButton m_CheckSave;
	int Checked;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedYes();
	//afx_msg void OnBnClickedButton1();
	//afx_msg void OnBnClickedButtonCancal();
	afx_msg void OnBnClickedMini();
};
