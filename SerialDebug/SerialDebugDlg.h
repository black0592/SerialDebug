// SerialDebugDlg.h : 头文件
//

#pragma once

#include "SplitterControl.h"
#include "CHexEdit.h"
#include "MaskedEdit.h"
#include "AddBytes.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "SDSettingDlg.h"
#include "SDClosedlg.h"
#include "SDNewversionDlg.h"
#include "BLComboBox.h"
#include "InFoDialog.h"
#include "../OpenDev/OpenDevDll.h"

// CSerialDebugDlg 对话框
class CSerialDebugDlg : public CDialog
{
// 构造
public:
	CSerialDebugDlg(CWnd* pParent = NULL);	// 标准构造函数
	
	bool m_bRun;
	bool sendRun;
	bool readnow;
	bool writenow;
	bool showhex;
	bool sendhex;
	bool timerrun;
	bool portopen;
	bool manual;
	//CButton CheckStop;
	HANDLE hComm;
	DWORD dwError;
	COMMTIMEOUTS timeouts;
	COMSTAT cs;
	CAddBytes CAddDiag;
	bool m_parity;
	unsigned char lastchar;
	unsigned char RxBuff[4096];
	int SerialCount;				//本机串口数
	unsigned char saddr;
	int isendcount;
	int iresvcount;
	int looptime;
	int timer;
	CString filesendname;
	CString fileresvname;
	CString port;
	CTime ModifyTime;
	CFile resvfile;

	COMMPROP commprop;
	DCB dcb;
	DWORD baudrate;
	BYTE bytesize;
	BYTE parity;
	BYTE stopbits;

	/*API发送用开始*/
	SERIALUSERDATA SeialUserData;
	SERIALCALLBACK SeialCallback;
	bool hasapidata;
	HINSTANCE hDllInst;
	CButton btnAPISend;
	/*API发送用结束*/


// 对话框数据
	enum { IDD = IDD_SERIALDEBUG_DIALOG };

	//关闭选择，托盘按钮，在线更新用全局变量
	CString McuDirectory;
	NOTIFYICONDATA m_nid;	//NOTIFYICONDATA结构

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
	CSplitterControl m_wndSplitter;
	//{{AFX_DATA(CSerialDebugDlg)

	HANDLE hThread;
	HANDLE hThreadSend;
	HANDLE hThreadMoniter;
	HANDLE hOpenMoniter;
	DWORD ThreadID;

	//int SerialCount;



	
	DWORD lrc;
	OVERLAPPED overlapped;
	CRect m_rect;
	DWORD eventmask;
	CRect rect1;
	CRect rect2;
	CRect rect3;
	CRect rect4;
	CRect rect5;
	CRect rect6;
	CRect rect7;
	CRect rect8;
	CRect rect9;
	CRect rect10;
	CRect rect11;
	CRect rect12;
	CRect rect13;

	CString m_strTitle;

	CRect m_rtIcon;
	CRect m_rtButtExit;
	CRect m_rtButtMax;
	CRect m_rtButtMin;
	CRect m_rtButtHelp;

	CMenu menuSys;		//定义CMenu类对象
	//}}AFX_DATA
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void DoResize(int delta);

// 实现
protected:
	HICON m_hIcon;
	SDClosedlg MyClosedlg;
	SDSettingDlg MySettingDlg;
	SDNewversionDlg MyNewversionDlg;
	bool writting;


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdownComPort();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnCbnSelchangeComboPort();
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnNMClickSyslinkClearout(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckStop();
	afx_msg void OnBnClickedCheckBortrateMode();
	afx_msg void OnEnChangeEditBortrate();
	afx_msg void OnBnClickedCheckHexout();
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckSendhex();
	afx_msg void OnCbnSelchangeComboMultihostMode();
	CHexEdit	ResvContent;
	CHexEdit	SendContent;
	CButton	SendButton;
	CMaskedEdit MEditAddDec;
	CMaskedEdit MEditAddHex;
	afx_msg void OnEnChangeEditAdddec();
	afx_msg void OnEnChangeEditAddhex();
private:

protected:
	afx_msg LRESULT OnUmTraynotify(WPARAM wParam, LPARAM lParam);	// 添加托盘消息响应

public:
	afx_msg void OnBnClickedCheck3();
	CEdit edSendCount;
	afx_msg void OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit edResvCount;
	afx_msg void OnNMClickSyslink4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheck5();
	CButton chkFilesSend;
	afx_msg void OnEnChangeEditSend();
	afx_msg void OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck6();
	CButton ckbFileResv;
	afx_msg void OnNMClickSyslink7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink8(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit cedtLoopTime;
	afx_msg void OnEnChangeEditLooptime();
	CButton cchkLoopEn;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSetfocusComboPort();
	afx_msg void OnCbnSelendcancelComboPort();
	afx_msg void OnCbnEditchangeComboPort();
	afx_msg void OnCbnEditupdateComboPort();
	afx_msg void OnCbnKillfocusComboPort();
	CRichEditCtrl statusbar;
	CStatic statAdd;
	//afx_msg void OnStnClickedStaticMode();
	CEdit modehex;
	//CStatic editmode;
	//afx_msg void OnStnDblclickStaticMode();
	void UpdateAddByre(void);
	CBLComboBox ComboPort;
	afx_msg void OnBnClickedCheckDtr();
	afx_msg void OnBnClickedCheckRts();
	void DrawTitleBar(CDC *pDC);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void MenuProcess(CMenu *pMenu,int MenuId);
	CButton chkAutoOpen;
	afx_msg void OnBnClickedBtnApisend();
	void APIDataload();
};
