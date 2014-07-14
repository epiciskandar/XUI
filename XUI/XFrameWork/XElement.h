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
	virtual VOID MsgDown(IXMsg& msg);
	virtual VOID MsgUp(IXMsg& msg);

	XResult SetXMLProperty( CString name,CString value );

	Property::CXProperty& GetPrpertyRef()	{return m_property;};
	VOID GetRectInClientCoord(CRect& rect);
	BOOL PaintCheck(CRect invalidRect, CRect& paintSrcRect, CPoint& paintDstOffset);

protected:
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
	Property::CXProperty	m_property;		// 属性集合，所有属性都存在这里
	BOOL	m_isLayouting = FALSE;			// 在排版过程中不触发大小改变等事件
	CToolTipCtrl	m_toolTip;
	XPtr<CGDIMemDC>	m_memDC;
};

typedef XSmartPtr<CXElement> ElementRef;

#include "XElementUtil.hpp"