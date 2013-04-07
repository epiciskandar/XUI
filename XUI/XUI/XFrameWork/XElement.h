#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XNotifier.hpp"
#include "XMsg.hpp"
#include "XProperty.hpp"
#include "../WTL/atlctrls.h"

class CXElement 
	: public CXTreeNode
	, public CXNotifier
{
	XClass;
public:
	CXElement();
	~CXElement();

	XProperty_Begin
		XFakeProperty(Position)
		XFakeProperty(Size)
		XFakeProperty(LayoutRect)
		XProperty(Rect)
		XProperty(LayoutType)
		XProperty(LayoutInvalid)
		XProperty(LayoutDirection)
		XProperty(Padding)
		XProperty(Align)
		XProperty(AutoWidth)
		XProperty(AutoHeight)
		XProperty(ExpandWidth)
		XProperty(ExpandHeight)
		XProperty(Color)
		XProperty(BorderColor)
		XProperty(BorderWidth)
		XProperty(HitTest)
		XProperty(ToolTip)
		XProperty(Ghost)
		XProperty(XFont)
	XProperty_End;

	// XMsg的接收入口函数
	virtual XResult ProcessXMessage(CXMsg& msg);

	// 对string类型的属性值的解析支持，用于XML的实例化，可通过该函数知道XML支持什么属性
	virtual XResult SetXMLProperty(CString name,CString value);

	Property::CXProperty& GetPrpertyRef()	{return m_property;};

protected:
	VOID _SendXMsg(CXMsg& pMsg);

	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
	VOID On_CXMsg_SizeChanged(CXMsg_SizeChanged& arg);
	VOID On_CXMsg_Layout(CXMsg_Layout& arg);
	VOID On_CXMsg_Paint(CXMsg_Paint& arg);
	VOID On_CXMsg_PaintElement(CXMsg_PaintElement& arg);
	VOID On_CXMsg_MouseEnter(CXMsg_MouseEnter& arg);
	VOID On_CXMsg_MouseLeave(CXMsg_MouseLeave& arg);
	VOID On_CXMsg_AttachDC(CXMsg_AttachDC& arg);
	VOID On_CXMsg_FrameClick(CXMsg_FrameClick& arg);
	VOID On_CXMsg_RealWndClosing(CXMsg_RealWndClosing& arg);
protected:
	Property::CXProperty	m_property;
	BOOL	m_isLayouting;
	CToolTipCtrl	m_toolTip;
	XPtr<CGDIMemDC>	m_memDC;
};

typedef XSmartPtr<CXElement> ElementRef;

#include "XElementUtil.hpp"