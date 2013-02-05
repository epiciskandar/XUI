#pragma once
#include "XElement.hpp"

//////////////////////////////////////////////////////////////////////////

// for process WM_XX faster,we need a map
#define TranslateToXMessage(_transfunc, ...) \
	{ \
	UINT msgList[] = { __VA_ARGS__ }; \
	static std::set<UINT> handleMsgSet; \
	if (handleMsgSet.empty()) \
	for(UINT n=0; n<_countof(msgList); ++n){handleMsgSet.insert(msgList[n]);} \
	if(handleMsgSet.find(uMsg) != handleMsgSet.end()) \
		{ \
		SetMsgHandled(TRUE); \
		lResult = _transfunc(uMsg, wParam, lParam); \
		if(IsMsgHandled()) \
		return TRUE; \
		} \
	}

#define XMsgTranslater(_msg,_func) \
	case _msg: \
	{ \
		return _func(wParam,lParam); \
	} \
	break;

//////////////////////////////////////////////////////////////////////////

class CXRealWnd :
	public CXElement,
	public CWindowImpl<CXRealWnd>
{
	XClass(CXElement);
public:
	CXRealWnd();

	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
		TranslateToXMessage( MessageTranslateFunc 
		, WM_PAINT
		, WM_SIZE
		)
	END_MSG_MAP();

	virtual XResult ProcessXMessage(CXMsg& msg);

	XProperty_Begin
		XProperty(Title)
		XProperty(Style)
		XProperty(ExStyle)
		XProperty(HWnd)
	XProperty_End;

	virtual XResult SetXMLProperty( CString name,CString value );

	XResult Create(HWND hwndParent=0);

protected:
	LRESULT MessageTranslateFunc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID	OnDestroy();

	LRESULT _Translate_WM_PAINT(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_Size(WPARAM wParam,LPARAM lParam);

	//--------------------------------//
};

typedef XSmartPtr<CXRealWnd> CXRealWndRef;

MyNameIs(CXRealWnd)
	I_Provide("将windows消息转换为内部消息，并提供真窗口的一些基本维护，暴漏WTL::CWindow对外")
End_Description;

//////////////////////////////////////////////////////////////////////////

CXRealWnd::CXRealWnd() : CWindowImpl()
{

}

VOID CXRealWnd::OnDestroy()
{
	PostMessage(WM_QUIT,0,0);
}

LRESULT CXRealWnd::MessageTranslateFunc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
		XMsgTranslater(WM_PAINT,_Translate_WM_PAINT);
		XMsgTranslater(WM_SIZE,_Translate_WM_Size);
	}

	return 0;
}

XResult CXRealWnd::Create( HWND hwndParent/*=0*/ )
{
	CString title;
	GetTitle(title);
	DWORD style=0;
	GetStyle(style);
	DWORD ExStyle=0;
	GetExStyle(ExStyle);
	HWND hWnd = CWindowImpl::Create(hwndParent,NULL,title,style,ExStyle);
	SetHWnd(hWnd);
	if (hWnd)
	{
		return XResult_OK;
	}
	return XResult_Fail;
}

XResult CXRealWnd::ProcessXMessage( CXMsg& msg )
{
	return BaseClass::ProcessXMessage(msg);
}

//////////////////////////////////////////////////////////////////////////
// message translate

LRESULT CXRealWnd::_Translate_WM_PAINT( WPARAM wParam,LPARAM lParam )
{
	URP(wParam,lParam);
	CXMsg_Paint msg;
	msg.msgRet = 0;
	PAINTSTRUCT ps;
	BeginPaint(&ps);
	msg.drawDevice.invalidRect = CRect(ps.rcPaint);
	msg.drawDevice.dc = ps.hdc;

	ProcessXMessage(msg);

	BitBlt(msg.drawDevice.dc,
		msg.drawDevice.invalidRect.left,
		msg.drawDevice.invalidRect.top,
		msg.drawDevice.invalidRect.Width(),
		msg.drawDevice.invalidRect.Height(),
		msg.drawDevice.dc,
		0, 0,
		SRCCOPY
		);

	ZeroMemory(&ps,sizeof(ps));
	ps.hdc = msg.drawDevice.dc;
	ps.rcPaint = msg.drawDevice.invalidRect;
	EndPaint(&ps);
	SetMsgHandled(msg.msgHandled);
	return msg.msgRet;
}

LRESULT CXRealWnd::_Translate_WM_Size( WPARAM wParam,LPARAM lParam )
{
	if (wParam != SIZE_RESTORED)
	{
		CXMsg_SizeChanged msg;
		msg.node = this;
		msg.sizeType = (SizeType)wParam;

		ProcessXMessage(msg);

		return msg.msgRet;
	}

	CSize size;
	size.cx = LOWORD(lParam);
	size.cy = HIWORD(lParam);
	SetSize(size);

	return 0;
}

XResult CXRealWnd::SetXMLProperty( CString name,CString value )
{
	XMLConvert_Begin(name,value)
		XMLConvert(Title)
		XMLConvert(ShowState)
		XMLConvert(Style)
		XMLConvert(ExStyle)
	XMLConvert_End

	return BaseClass::SetXMLProperty(name,value);
}
