// CHexEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "download.h"
#include "CHexEdit.h"

#include <afxole.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MES_UNDO        _T("取消(&U)")
#define MES_CUT         _T("剪切(&T)")
#define MES_COPY        _T("复制(&C)")
#define MES_PASTE       _T("粘贴(&P)")
#define MES_PASTEDEC    _T("粘贴DEC(CTRL+D)")
#define MES_PASTEHEX    _T("粘贴HEX(CTRL+H)")
#define MES_DELETE      _T("删除(&D)")
#define MES_SELECTALL   _T("全选(&A)")
#define ME_SELECTALL    WM_USER + 0x7000
#define ME_PASTEDEC    WM_USER + 0x7001
#define ME_PASTEHEX    WM_USER + 0x7002

#define HOTKEY_PASTEDEC 0x1001
#define HOTKEY_PASTEHEX 0x1002

/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit()
{
	hexmode = false;
	noinput = false;
	insert = false;
	pressed = false;
	//BOOL test = RegisterHotKey(m_hWnd,HOTKEY_PASTEDEC,MOD_CONTROL,'H'); 
}

CHexEdit::~CHexEdit()
{
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
 //{{AFX_MSG_MAP(CHexEdit)
 ON_WM_CHAR()
 ON_WM_KEYDOWN()
 ON_MESSAGE(WM_PASTE,OnPaste)
 ON_MESSAGE(WM_CLEAR,OnClear)
 ON_MESSAGE(WM_CUT,OnCut)
 ON_WM_HOTKEY() 
 //}}AFX_MSG_MAP
 ON_WM_CONTEXTMENU()
// ON_WM_CREATE()
 ON_WM_DESTROY()
 ON_WM_LBUTTONDBLCLK()
 ON_WM_LBUTTONDOWN()
 ON_WM_LBUTTONUP()
 ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers

void CHexEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default 
	int nStartOff,nEndOff;
	UINT nChar2;

	if(hexmode == true)
	{

		if(nChar == '\t' || nChar == ' ' || nChar == 0xd || nChar == 0xa)
		{
			//skip tab key and ' ' key.
			return;
		}

		GetSel(nStartOff,nEndOff);

		if(GetKeyState(VK_CONTROL) & 0x80000000)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}

		if(nChar == 0x08)//backspace.
		{
			if(nStartOff == nEndOff)
			{
				switch(nStartOff%3)
				{
				case 0:	 //第一个字母前
					CEdit::OnChar(nChar, nRepCnt, nFlags);
					CEdit::OnChar(nChar, nRepCnt, nFlags);
					CEdit::OnChar(nChar, nRepCnt, nFlags);
					break;
				case 1:  //第二个字母前
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					nChar = '0';
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					if(nStartOff == 1)SetSel(0,0);
					else SetSel(nStartOff-2,nStartOff-2);
					break;
				case 2:	 //第二个字母后
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					nChar = '0';
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					SetSel(nStartOff-1,nStartOff-1);
					break;
				}
				/*if(nStartOff%3 != 1)
				{
					if(nStartOff == 2)SetSel(3,3);
					CEdit::OnChar(nChar, nRepCnt, nFlags);
					CEdit::OnChar(nChar, nRepCnt, nFlags);
					CEdit::OnChar(nChar, nRepCnt, nFlags);
				}*/
			}
			return;
		}

		if((nChar >= '0' && nChar <= '9' ) || 
		   (nChar >= 'A' && nChar <= 'F' ) ||
		   (nChar >= 'a' && nChar <= 'f' ))
		{
			if(nChar >= 'a' && nChar <= 'f' )nChar -= 32;

			if(nStartOff != nEndOff){

				CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
				nChar = '0';
				CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
				nChar = ' ';
				CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
				return;
			}
			/*if(((nStartOff%3==0)&&(GetWindowTextLength()>nStartOff)) || ((nStartOff%3==1)&&(GetWindowTextLength()-2>nStartOff)))
			{
				if(nStartOff%3 == 2)
				{
					SetSel(nStartOff+1,nEndOff+2);
				}
				else
					SetSel(nStartOff,nEndOff+1);

				//CEdit::OnChar (nChar, nRepCnt , nFlags );
				CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
				nChar = '0';
				CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
				nChar = ' ';
				CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
				return;
			}*/

			switch(nStartOff%3)
			{
				case 0:  //第一个字母前
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					nChar = '0';
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					nChar = ' ';
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					SetSel(nStartOff+1,nEndOff+1);
					break;
				case 1:  //第二个字母前
					SetSel(nStartOff+1,nEndOff+1);
					CWnd::DefWindowProc (WM_CHAR, 0x08 , MAKELPARAM (nRepCnt, nFlags ));
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					SetSel(nStartOff+2,nEndOff+2);
					break;
				case 2:  //第二个字母后
					SetSel(nStartOff+1,nEndOff+1);
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					nChar2 = '0';
					CWnd::DefWindowProc (WM_CHAR, nChar2 , MAKELPARAM (nRepCnt, nFlags ));
					nChar2 = ' ';
					CWnd::DefWindowProc (WM_CHAR, nChar2 , MAKELPARAM (nRepCnt, nFlags ));
					SetSel(nStartOff+2,nEndOff+2);
					break;

			}	
		}

	}
	else 
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		//CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
	}
	return;
}

void CHexEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(hexmode == true)
	{
	
		if (nChar == VK_DELETE)
		{
			int nStartOff,nEndOff;
	  
			GetSel(nStartOff,nEndOff);

			if(nStartOff == nEndOff)
			{
			if(nStartOff%3 == 0)
				{
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
					nChar = '0';
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					SetSel(nStartOff,nEndOff);
					
				}else if(nStartOff%3 == 1)
				{
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
					nChar = '0';
					CWnd::DefWindowProc (WM_CHAR, nChar , MAKELPARAM (nRepCnt, nFlags ));
					SetSel(nStartOff,nEndOff);
				}
				else
				{
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
					CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				}
			}
			else{
				CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
				
				/*if(nStartOff%3 == 1)nStartOff--;
				else if(nStartOff%3 == 2)nStartOff++;

				if(nEndOff%3 == 1)nEndOff+=2;
				else if(nEndOff%3 == 2)nEndOff++;

				CString temp,templ,tempr;
				GetWindowTextA(temp);
				templ = temp.Left(nStartOff);
				tempr = temp.Right(GetWindowTextLengthA()-nEndOff);
				temp = templ +  tempr;
				SetWindowTextA(temp);*/
			}
			return;
		}
		
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	}
	else
	{
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	}

}
void CHexEdit::SetHexMode()
{
	hexmode = true;
	int size = this->GetWindowTextLength();
	int i;
	TCHAR TxBuffWide[4100];
	TCHAR TxBuffWide2[4100*3];
	DWORD sizerequired;
	if(size>4096/3)size = 4096/3;
	unsigned char * buffer = NULL,* ibuffer = NULL;
	ibuffer = new unsigned char[size+1];
	buffer = new unsigned char[(size+1)*3+1];
	this->GetWindowText(TxBuffWide, size + 1);

	sizerequired = WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)ibuffer, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)ibuffer, sizerequired, NULL, NULL);

	for (i = 0; i<sizerequired-1; i++)
	{
		if((((ibuffer[i]>>4)&0x0F) >= 0)&&(((ibuffer[i]>>4)&0x0F) <= 9))
			buffer[3*i] = ((ibuffer[i]>>4)&0x0F) + 48;
		if((((ibuffer[i]>>4)&0x0F) >= 10)&&(((ibuffer[i]>>4)&0x0F) <= 15))
			buffer[3*i] = ((ibuffer[i]>>4)&0x0F) + 55;
		if(((ibuffer[i]&0x0F) >= 0)&&((ibuffer[i]&0x0F) <= 9))
			buffer[3*i+1] = (ibuffer[i]&0x0F) + 48;
		if(((ibuffer[i]&0x0F) >= 10)&&((ibuffer[i]&0x0F) <= 15))
			buffer[3*i+1] = (ibuffer[i]&0x0F) + 55;
		buffer[3*i+2] = 32;
	}
	buffer[3 * (sizerequired-1)] = '\0';

	CString  str; 

	sizerequired = MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(buffer), -1, TxBuffWide2, 0);
	MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(buffer), -1, TxBuffWide2, sizerequired);
	str.Format(L"%s", TxBuffWide2);
	SetWindowText(str);

	delete[] ibuffer;
	delete[] buffer;
}
void CHexEdit::SetNormalMode()
{
	hexmode = false;

	int size = this->GetWindowTextLength();
	int i;
	unsigned char temp1,temp2;
	unsigned char * buffer = NULL,* ibuffer = NULL;
	TCHAR TxBuffWide[4100*3];
	TCHAR TxBuffWide2[4100];
	DWORD sizerequired;
	ibuffer = new unsigned char[size+2];
	buffer = new unsigned char[(size+1)/3+1];
	this->GetWindowText((LPTSTR)TxBuffWide, size + 2);

	sizerequired = WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)ibuffer, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, TxBuffWide, -1, (LPSTR)ibuffer, sizerequired, NULL, NULL);

	for (i = 0; i<(sizerequired + 1) / 3; i++)
	{
		if((ibuffer[i*3] >= '0')&&(ibuffer[i*3] <= '9'))
			temp1 = ibuffer[i*3] - 48;
		if((ibuffer[i*3] >= 'A')&&(ibuffer[i*3] <= 'F'))
			temp1 = ibuffer[i*3] - 55;
		if((ibuffer[i*3+1] >= '0')&&(ibuffer[i*3+1] <= '9'))
			temp2 = ibuffer[i*3+1] - 48;
		if((ibuffer[i*3+1] >= 'A')&&(ibuffer[i*3+1] <= 'F'))
			temp2 = ibuffer[i*3+1] - 55;
		buffer[i] = temp1*16 + temp2;
	}
	buffer[i] = '\0';

	CString  str; 

	sizerequired = MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(buffer), -1, TxBuffWide2, 0);
	MultiByteToWideChar(CP_ACP, MB_USEGLYPHCHARS, (LPCSTR)(buffer), -1, TxBuffWide2, sizerequired);

	str.Format(L"%s", TxBuffWide2);
	SetWindowText(str);

	delete[] ibuffer;
	delete[] buffer;
}

LRESULT CHexEdit::OnPaste(WPARAM m,LPARAM l)
{
	COleDataObject obj;
	//int status = MessageBox("您复制的是16进制数据吗？\n\n\t粘贴16进制快捷键CTRL+H\n\t粘贴10进制快捷键CTRL+D","请选择",MB_YESNO);
	HGLOBAL hmem = NULL;	
	if(hexmode == true)
	{
		PasteHex();
	}else{
		PasteDec(m,l);
	}
	return 0;
}


LRESULT CHexEdit::OnClear(WPARAM,LPARAM)
{

 /*int nStartOff,nEndOff;

 GetSel(nStartOff,nEndOff);

 if(nStartOff%3 == 1 || nEndOff%3 == 1)
  return;

 nStartOff = (nStartOff+1)/3*3;
 nEndOff = nEndOff/3*3 + 2;
 SetSel(nStartOff,nEndOff);

 Clear();
*/
	return 0;
}

LRESULT CHexEdit::OnCut(WPARAM w,LPARAM l)
{

	if(hexmode == true)
	{
	
		int nStartOff,nEndOff;
	  
		GetSel(nStartOff,nEndOff);

		if(nStartOff%3 == 1)nStartOff--;
		else if(nStartOff%3 == 2)nStartOff++;

		if(nEndOff%3 == 1)nEndOff+=2;
		else if(nEndOff%3 == 2)nEndOff+=1;

		SetSel(nStartOff,nEndOff);
		
		CWnd::DefWindowProc (WM_CUT,w,l);//CEdit::OnCut(w,l);

	}
	else
	{
		CWnd::DefWindowProc (WM_CUT,w,l);//CEdit::OnCut(w,l);
	}
	return 0;
}


void CHexEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	SetFocus();
    CMenu menu;
    menu.CreatePopupMenu();
    BOOL bReadOnly = GetStyle() & ES_READONLY;
    DWORD flags; //
	//= CanUndo() && !bReadOnly ? 0 : MF_GRAYED;
    //menu.InsertMenu(0, MF_BYPOSITION | flags, EM_UNDO,MES_UNDO);

    //menu.InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR);

    DWORD sel = GetSel();
    flags = LOWORD(sel) == HIWORD(sel) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_COPY,MES_COPY);

    flags = (flags == MF_GRAYED || bReadOnly) ? MF_GRAYED : 0;
    menu.InsertMenu(2, MF_BYPOSITION | flags, WM_CUT, MES_CUT);
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_CLEAR,MES_DELETE);

    flags = IsClipboardFormatAvailable(CF_TEXT) &&!bReadOnly ? 0 : MF_GRAYED;
    menu.InsertMenu(4, MF_BYPOSITION | flags, WM_PASTE,MES_PASTE);
	/*menu.InsertMenu(4, MF_BYPOSITION | flags, ME_PASTEDEC,MES_PASTEDEC);
	menu.InsertMenu(4, MF_BYPOSITION | flags, ME_PASTEHEX,MES_PASTEHEX);*/

    menu.InsertMenu(6, MF_BYPOSITION | MF_SEPARATOR);

    int len = GetWindowTextLength();
    flags = (!len || (LOWORD(sel) == 0 && HIWORD(sel) == len)) ? MF_GRAYED : 0;
    menu.InsertMenu(7, MF_BYPOSITION | flags, ME_SELECTALL,MES_SELECTALL);

    menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON |TPM_RIGHTBUTTON, point.x, point.y, this);
}


BOOL CHexEdit::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (LOWORD(wParam))
    {
		case EM_UNDO:
		case WM_CUT:
		case WM_COPY:
		case WM_CLEAR:
		case WM_PASTE:
			return SendMessage(LOWORD(wParam));			
		case ME_SELECTALL:
	        return SendMessage (EM_SETSEL, 0, -1);
    default:
		    return CEdit::OnCommand(wParam, lParam);
    }
	return CEdit::OnCommand(wParam, lParam);
}


void CHexEdit::PasteDec(WPARAM m,LPARAM l)
{
	COleDataObject obj;
	HGLOBAL hmem = NULL;
	if(hexmode == true)
	{
		if (obj.AttachClipboard()) 
			{
				if (obj.IsDataAvailable(CF_TEXT)) 
				{	
					hmem = obj.GetGlobalData(CF_TEXT);
					LPBYTE p = (BYTE*)::GlobalLock(hmem);
					LPBYTE p1=p;
					int dwLen = ::GlobalSize(hmem);
					int length = GetWindowTextLength();
					unsigned char temph,templ;
					p1 = p;
					if((dwLen+length)>4096)MessageBox(L"你要粘贴的数据量较大,发送区最多发送4K字节,超过部分将不会发送",L"警告");
					for(int i = 0; i < dwLen-1; i++)
					{
						temph = (*p1)/16;
						templ = (*p1)%16;
						if((temph>=0)&&(temph<=9))SendMessage(WM_CHAR,(temph+0x30),0);
						else SendMessage(WM_CHAR,(temph+'A'-10),0);
						if((templ>=0)&&(templ<=9))SendMessage(WM_CHAR,(templ+0x30),0);
						else SendMessage(WM_CHAR,(templ+'A'-10),0);
						p1++;		
					}
				}
				::GlobalUnlock(hmem);
			}
		
	}else{
		CWnd::DefWindowProc (WM_PASTE,m,l);
	}
}


void CHexEdit::PasteHex(void)
{
	COleDataObject obj;
	HGLOBAL hmem = NULL;	
	if(hexmode == true)
	{
			if (obj.AttachClipboard()) 
			{
				if (obj.IsDataAvailable(CF_TEXT)) 
				{	
					hmem = obj.GetGlobalData(CF_TEXT);
					LPBYTE p = (BYTE*)::GlobalLock(hmem);
					LPBYTE p1=p;
					int dwLen = ::GlobalSize(hmem);
					int length = GetWindowTextLength();
					int otherbyte = 0;
					int cancel = 0;
					for(int i = 0; i < dwLen-1; i++)
					{
						if(((*p1)>='0'&&(*p1)<='9')||((*p1)>='a'&&(*p1)<='f')||((*p1)>='A'&&(*p1)<='F')||((*p1)==0x20)||((*p1)==0x0D)||((*p1)==0x0A))p1++;
						else{
							otherbyte = 1;
							p1++;
						}
					}
					cancel = MessageBox(L"确认粘贴？",L"警告",MB_OKCANCEL);
					if((cancel == IDOK)||(cancel == 0))
					{
						p1 = p;
						if((dwLen/2+length)>4096)MessageBox(L"你要粘贴的数据量较大,发送区最多发送4K字节,超过部分将不会发送",L"警告");
						for(int i = 0; i < dwLen-1; i++)
						{
							if(((*p1)>='0'&&(*p1)<='9')||((*p1)>='a'&&(*p1)<='f')||((*p1)>='A'&&(*p1)<='F')||((*p1)==0x0D)||((*p1)==0x0A))
							{
								SendMessage(WM_CHAR,*p1,0);
								p1++;								
							}
							else{
								p1++;
							}							
						}
					}
					::GlobalUnlock(hmem);
				}
			}
			
	}else{

		/*	if (obj.AttachClipboard()) 
			{
				HGLOBAL hmem = NULL;
				if (obj.IsDataAvailable(CF_TEXT)) 
				{	
					hmem = obj.GetGlobalData(CF_TEXT);
					LPBYTE p = (BYTE*)::GlobalLock(hmem);
					LPBYTE p1=p;
					int dwLen = ::GlobalSize(hmem);
					int length = GetWindowTextLengthA();
					int lastbyte = 0;
					int otherbyte = 0;
					int cancel = 0;
					for(int i = 0; i < dwLen-1; i++)
					{
						if(((*p1)>='0'&&(*p1)<='9')||((*p1)>='a'&&(*p1)<='f')||((*p1)>='A'&&(*p1)<='F')||((*p1)==32)||((*p1)==0x0D)||((*p1)==0x0A))p1++;
						else{
							otherbyte = 1;
							p1++;
						}
					}
					if(otherbyte == 1)cancel = MessageBox("你要粘贴16进制数据中包含非16进制字符，这些字符在粘贴过程中将被忽略，真的要粘贴吗？","警告",MB_OKCANCEL);
					if((cancel == IDOK)||(cancel == 0))
					{
						p1 = p;
						unsigned char bytehigh;
						unsigned char bytelow;
						if((dwLen/2+length)>4096)MessageBox("你要粘贴的数据量较大,发送区最多发送4K字节,超过部分将不会发送","警告");
						for(int i = 0; i < dwLen-1; i++)
						{
							if(((*p1)>='0'&&(*p1)<='9')){
								if(lastbyte == 0){
									bytehigh = (*p1)-'0';
									lastbyte = 1;
									p1++;
								}else
								{
									bytelow = (*p1)-'0';
									lastbyte = 0;
									SendMessage(WM_CHAR,(bytehigh*16+bytelow),0);
									p1++;
								}
							}
							else if(((*p1)>='a'&&(*p1)<='f')){
								if(lastbyte == 0){
									bytehigh = (*p1)-'a' + 10;
									lastbyte = 1;
									p1++;
								}else
								{
									bytelow = (*p1)-'a' + 10;
									lastbyte = 0;
									SendMessage(WM_CHAR,(bytehigh*16+bytelow),0);
									p1++;
								}
							}
							else if(((*p1)>='A'&&(*p1)<='F')){
								if(lastbyte == 0){
									bytehigh = (*p1)-'A' + 10;
									lastbyte = 1;
									p1++;
								}else
								{
									bytelow = (*p1)-'A' + 10;
									lastbyte = 0;
									SendMessage(WM_CHAR,(bytehigh*16+bytelow),0);
									p1++;
								}
							}
							else p1++;
						}
					}
					::GlobalUnlock(hmem);
				}
			}*/
	}
}


//int CHexEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	//BOOL status = RegisterHotKey(m_hWnd,HOTKEY_PASTEDEC,MOD_CONTROL,'H'); 
//	if (CEdit::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	return 0;
//}


void CHexEdit::OnDestroy()
{
	CEdit::OnDestroy();	
}


void CHexEdit::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

    //判断响应了什么热键   
	switch(nHotKeyId)
	{
		case HOTKEY_PASTEDEC:
			MessageBox(L"你按下了组合键 ctrl + d",L"提示"); 
			PasteDec(0,0);
			break;
		case HOTKEY_PASTEHEX:
			MessageBox(L"你按下了组合键 ctrl + d",L"提示"); 
			PasteHex();
			break;
		break;
			default:
		break;
	} 

	CEdit::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CHexEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(noinput == false)
	CEdit::OnLButtonDblClk(nFlags, point);
}


void CHexEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int EndChar;
	if(noinput == false)
	{
		if(insert == true)
		{
			pressed = true;
			//GetSel(StartChar,EndChar);
			CEdit::OnLButtonDown(nFlags, point);
			StartChar = (CharFromPos(point))&0x0000FFFF;
			SetSel(StartChar,StartChar);
			StartChar = StartChar - StartChar%3;
			//
		}
		else
		{
			CEdit::OnLButtonDown(nFlags, point);
		}
	}
}


void CHexEdit::OnLButtonUp(UINT nFlags, CPoint point)
{

	if(noinput == false){
		
		if(insert == true)
		{
			pressed = false;
			CEdit::OnLButtonUp(nFlags, point);
			//int i = CharFromPos(point);
			//CString temp;
			//temp.Format("%d || %d",i/256,i%256);
			//MessageBox(temp);
		}
		else
		{
			CEdit::OnLButtonUp(nFlags, point);
		}		
	}
}


void CHexEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int EndChar,EndTemp;
	RECT rect;
	if(noinput == false){

		if(insert == true)
		{
			if(pressed == true)
			{
				GetClientRect(&rect);
				if(point.x < rect.left)point.x = rect.left;
				if(point.x > rect.right)point.x = rect.right;
				if(point.y > rect.bottom)point.y = rect.bottom;
				if(point.y < rect.top)point.y = rect.top;

				EndTemp = (CharFromPos(point))&0x0000FFFF;
				EndChar = EndTemp + 3 - EndTemp%3;
				if(EndChar>StartChar)EndChar-=3;
				if(point.x == rect.left)EndChar-=3;
				
				SetSel(StartChar,EndChar);
			}
		}
		else
		{
			CEdit::OnMouseMove(nFlags, point);
		}	
	}

}
