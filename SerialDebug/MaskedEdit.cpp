// MaskedEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SerialDebug.h"
#include "MaskedEdit.h"


// CMaskedEdit

IMPLEMENT_DYNAMIC(CMaskedEdit, CEdit)

CMaskedEdit::CMaskedEdit()
{

	allowed = _T("");
}

CMaskedEdit::~CMaskedEdit()
{
}


BEGIN_MESSAGE_MAP(CMaskedEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CMaskedEdit ��Ϣ�������




void CMaskedEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if((allowed.Find(TCHAR(nChar)) == -1)&&(nChar!=0x08))
	{		
		return;
	}
	else CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void CMaskedEdit::SetAllowedString(CString alloweds)
{
	allowed = alloweds;
}
