#pragma once


// CInFoDialog 对话框

class CInFoDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CInFoDialog)

public:
	CInFoDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInFoDialog();

// 对话框数据
	enum { IDD = IDD_DIALOG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
