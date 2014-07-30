#pragma once
#include "XElement.h"

class CXRealWnd :
	public CXElement,
	public CWindowImpl<CXRealWnd>,
	virtual public IXRealWnd
{
	RefCountImplAt(CXElement);
public:
	CXRealWnd();
	~CXRealWnd(){};

	BEGIN_MSG_MAP_EX(CXRealWnd)
		MSG_WM_DESTROY(OnDestroy)
	{
		UINT msgList[] = {
			WM_PAINT
			, WM_SIZE
			, WM_NCHITTEST
			, WM_MOUSEMOVE
			, WM_NCMOUSEMOVE
			, WM_LBUTTONDOWN
			, WM_LBUTTONUP
			, WM_LBUTTONDBLCLK
			, WM_CLOSE
			//, WM_CREATE
			, 0x00AE
			, 0x00AF
			, WM_CTLCOLOREDIT
		};
		static std::set<UINT> handleMsgSet;
		if (handleMsgSet.empty())
			for (UINT n = 0; n < _countof(msgList); ++n){ handleMsgSet.insert(msgList[n]); }
		if (handleMsgSet.find(uMsg) != handleMsgSet.end())
		{
			SetMsgHandled(TRUE);
			lResult = MessageTranslateFunc(uMsg, wParam, lParam);
			if (IsMsgHandled())
				return TRUE;
		}
	}
	END_MSG_MAP();

	virtual XResult ProcessXMessage(IXMsg& msg) override;

	XProperty(Title);
	XFakeProperty_Get(HWnd);
	XFakeProperty(Size);
	XFakeProperty(Rect);
	XFakeProperty(WindowPos);
	XProperty(CenterWindow);
	XProperty(Visible);
	XProperty(BorderArea);
	XProperty(IsLayerWin);
	XProperty(HasWinBorder);
	XProperty(HasSysBar);
	XProperty(Maximizable);
	XProperty(Minimizable);
	XProperty(Disabled);
	XProperty(IsToolWnd);
	XProperty(IsPopupWnd);
	XProperty(Sizable);

	XResult Create(HWND hwndParent = 0) override;

protected:
	LRESULT MessageTranslateFunc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID	OnDestroy();

	LRESULT _Translate_WM_PAINT(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_Size(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_NCHITTEST(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_MOUSEMOVE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_NCMOUSEMOVE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_MOUSEMOVE_MSG(WPARAM wParam,CPoint pointInClient);
	LRESULT _Translate_WM_CREATE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_LBUTTONDOWN(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_LBUTTONUP(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_LBUTTONDBLCLK(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_CLOSE(WPARAM wParam,LPARAM lParam);
	LRESULT _Translate_WM_CTLCOLOREDIT(WPARAM wParam,LPARAM lParam);

	//--------------------------------//

	VOID On_CXMsg_AppendElement(CXMsg_AppendElement& arg);
	VOID On_CXMsg_Invalidate(CXMsg_Invalidate& arg);
	VOID On_CXMsg_GetRealWnd(CXMsg_GetRealWnd& arg);

	VOID GetWinStyle(DWORD& style, DWORD& exStyle);

protected:
	BOOL m_ignorePropertyChange;
	BOOL m_firstPaint;
	ElementRef	m_currFocusElement;
};

typedef XSmartPtr<CXRealWnd> CXRealWndRef;
