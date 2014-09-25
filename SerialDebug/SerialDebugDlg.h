// SerialDebugDlg.h : ͷ�ļ�
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

// CSerialDebugDlg �Ի���
class CSerialDebugDlg : public CDialog
{
// ����
public:
	CSerialDebugDlg(CWnd* pParent = NULL);	// ��׼���캯��
	
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
	int SerialCount;				//����������
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

	/*API�����ÿ�ʼ*/
	SERIALUSERDATA SeialUserData;
	SERIALCALLBACK SeialCallback;
	bool hasapidata;
	HINSTANCE hDllInst;
	CButton btnAPISend;
	/*API�����ý���*/


// �Ի�������
	enum { IDD = IDD_SERIALDEBUG_DIALOG };

	//�ر�ѡ�����̰�ť�����߸�����ȫ�ֱ���
	CString McuDirectory;
	NOTIFYICONDATA m_nid;	//NOTIFYICONDATA�ṹ

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	
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

	CMenu menuSys;		//����CMenu�����
	//}}AFX_DATA
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void DoResize(int delta);

// ʵ��
protected:
	HICON m_hIcon;
	SDClosedlg MyClosedlg;
	SDSettingDlg MySettingDlg;
	SDNewversionDlg MyNewversionDlg;
	bool writting;


	// ���ɵ���Ϣӳ�亯��
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
	afx_msg LRESULT OnUmTraynotify(WPARAM wParam, LPARAM lParam);	// ���������Ϣ��Ӧ

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
