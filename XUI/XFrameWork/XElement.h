#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XNotifier.hpp"
#include "XMsg.hpp"
#include "XProperty.hpp"

class CXElement 
	: public virtual IXElement
	, public CXTreeNode
	, public CXNotifier
{
	RefCountImplAt(CXTreeNode);
public:
	CXElement();
	~CXElement();

	XFakeProperty(Position);
	XFakeProperty(Size);
	XProperty(Rect);
	XProperty(LayoutType);
	XProperty(LayoutInvalid);
	XProperty(LayoutDirection);
	XProperty(Padding);
	XProperty(Margin);
	XProperty(Align);
	XProperty(AutoWidth);
	XProperty(AutoHeight);
	XProperty(ExpandWidth);
	XProperty(ExpandHeight);
	XProperty(Color);
	XProperty(BorderColor);
	XProperty(BorderWidth);
	XProperty(HitTest);
	XProperty(ToolTip);
	XProperty(Ghost);
	XProperty(XFont);
	XProperty(NeedRealPaint);
	//////////////////////////////////////////////////////////////////////////
	// 排版专用属性，不暴露给外边.
	XFakeProperty(InnerLayoutRect);
	XFakeProperty(OuterLayoutRect);
	//////////////////////////////////////////////////////////////////////////


	virtual XResult Listen(XEar ear,DWORD& earID){return CXNotifier::Listen(ear,earID);};
	virtual XResult StopListen(DWORD earID){return CXNotifier::StopListen(earID);};
	virtual XResult Whisper(IXMsg& msg){return CXNotifier::Whisper(msg);};

	// XMsg的接收入口函数
	virtual XResult ProcessXMessage(IXMsg& msg);

	XResult SetXMLProperty( CString name,CString value );

	Property::CXProperty& GetPrpertyRef()	{return m_property;};

protected:
	VOID _SendXMsg(IXMsg& pMsg);

	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
	VOID On_CXMsg_SizeChanged(CXMsg_SizeChanged& arg);
	VOID On_CXMsg_Layout(CXMsg_Layout& arg);
	VOID On_CXMsg_Paint(CXMsg_Paint& arg);
	VOID On_CXMsg_RenderElement(CXMsg_RenderElement& arg);
	VOID On_CXMsg_MouseEnter(CXMsg_MouseEnter& arg);
	VOID On_CXMsg_MouseLeave(CXMsg_MouseLeave& arg);
	VOID On_CXMsg_AttachDC(CXMsg_AttachDC& arg);
	VOID On_CXMsg_FrameClick(CXMsg_FrameClick& arg);
	VOID On_CXMsg_RealWndClosing(CXMsg_RealWndClosing& arg);
protected:
	BOOL _NeedPaint(const CXMsg_Paint& arg,CRect& paintingRect,CPoint& srcPt);
protected:
	Property::CXProperty	m_property;
	BOOL	m_isLayouting;
	CToolTipCtrl	m_toolTip;
	XPtr<CGDIMemDC>	m_memDC;
};

typedef XSmartPtr<CXElement> ElementRef;

#include "XElementUtil.hpp"