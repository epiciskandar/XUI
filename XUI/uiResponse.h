#pragma once
#include "XUI/XUI.h"

class CXEar
{
public:
	CXEar() : m_earID(0){}
	virtual ~CXEar(){}

	virtual XResult Listen(NodeRef node)
	{
		ElementRef element(node);
		if (element)
		{
			m_uiElement = element;
			return element->Listen(std::bind(&CXEar::Hear,this,std::placeholders::_1),m_earID);
		}
		return XResult_InvalidArg;
	}
	virtual XResult GoDeaf()
	{
		if (m_uiElement && m_earID)
		{
			ElementRef notifier(m_uiElement);
			notifier->StopListen(m_earID);
		}
		return XResult_OK;
	}
	virtual XResult Hear(CXMsg& msg)
	{
		BEGIN_XMSG_MAP(msg)
			OnXMsgFunc(CXMsg_BeforeDeaf,OnBeforeDeaf);
		END_XMSG_MAP;
		return XResult_OK;
	}
	virtual XResult OnBeforeDeaf(CXMsg_BeforeDeaf& msg)
	{
		URP(msg);
		m_uiElement = nullptr;
		m_earID = 0;
		return XResult_OK;
	}
	DWORD m_earID;
	ElementRef m_uiElement;
};

class CCloseListener : public CXEar
{
public:
	virtual XResult Hear(CXMsg& msg) override
	{
		BEGIN_XMSG_MAP(msg)
			OnXMsgFunc(CXMsg_FrameClick,OnFrameClick);
			OnXMsgFunc(CXMsg_RealWndClosing,OnWndClose);
		END_XMSG_MAP;
		return __super::Hear(msg);
	}

	XResult OnFrameClick(CXMsg_FrameClick& msg)
	{
		URP(msg);
		CXMsg_GetHWnd msg_GetHWnd;
		m_uiElement->ProcessXMessage(msg_GetHWnd);
		PostMessage(msg_GetHWnd.hWnd,WM_CLOSE,0,0);
		return XResult_OK;
	}

	XResult OnWndClose(CXMsg_RealWndClosing& msg)
	{
		URP(msg);
		GoDeaf();
		delete this;
		return XResult_OK;
	}
};

class CXUIListenerRegister
{
public:
	XResult OnCreateElement(NodeRef node)
	{
		if (node->GetID() == _T("Close"))
		{
			CCloseListener* elementListener = new CCloseListener;
			elementListener->Listen(node);
		}
		return XResult_OK;
	}
};