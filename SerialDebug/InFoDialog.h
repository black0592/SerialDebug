#pragma once


// CInFoDialog �Ի���

class CInFoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInFoDialog)

public:
	CInFoDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInFoDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
