#pragma once
#include "XElement.hpp"

class CXRealWnd :
	public CXElement,
	public CWindowImpl<CXRealWnd>
{
	XClass;
public:
	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
		TranslateToXMessage( MessageTranslate 
		,WM_PAINT )
	END_MSG_MAP();

	BEGIN_XMESSAGE_MAP
	END_XMESSAGE_MAP;
protected:
	LRESULT MessageTranslate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID OnDestroy();

};

typedef XSmartPtr<CXRealWnd> CXRealWndRef;

MyNameIs(CXRealWnd)
End_Description;

//////////////////////////////////////////////////////////////////////////

VOID CXRealWnd::OnDestroy()
{
	PostMessage(WM_QUIT,0,0);
}

LRESULT CXRealWnd::MessageTranslate( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult=0;
	switch(uMsg)
	{
	case WM_PAINT:
		{
			CXMsg_Paint msg;
			_SendXMessageToChildren(msg);
			lResult = msg.msgRet;
			SetMsgHandled(msg.msgHandled);
		}
		break;
	}

	return lResult;
}

