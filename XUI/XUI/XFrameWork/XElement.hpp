#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XMsg.hpp"
#include "XProperty.hpp"
#include "../WTL/atlctrls.h"

//////////////////////////////////////////////////////////////////////////

// define property functions
#define XProperty_Begin
#define XProperty_Get(_name) \
	virtual XResult Get##_name (Property::_name##Type& value) \
	{ \
		value = Property::_name##DefaultValue; \
		return m_property.GetProperty(L#_name,value); \
	}
#define XProperty_Set(_name) \
public: \
	virtual XResult Set##_name (Property::_name##Type param) \
	{ \
		XResult result = m_property.SetProperty(L#_name,param); \
		CXMsg_PropertyChanged msg; \
		msg.name = L#_name; \
		ProcessXMessage(msg); \
		return  result;\
	}
#define XProperty(_name) \
	XProperty_Get(_name) \
	XProperty_Set(_name)

#define XFakeProperty_Get(_name) \
	virtual XResult Get##_name (Property::_name##Type& value);
#define XFakeProperty_Set(_name) \
	virtual XResult Set##_name (Property::_name##Type param);
#define XFakeProperty(_name) \
	XFakeProperty_Get(_name); \
	XFakeProperty_Set(_name);

#define XProperty_End

// define XML converters
#define XMLConvert_Begin(_name,_value) \
{ \
	CString& propName = _name; \
	CString& propVaule = _value;
#define XMLConvert(_name) \
	if (Property::_name == propName) \
	{ \
		m_property.SetProperty(propName,Property::_name##XMLConverter::ConvertToValue(propVaule)); \
		CXMsg_PropertyChanged msg; \
		msg.name = propName; \
		ProcessXMessage(msg); \
	}else
#define XMLFakeConvert(_name) \
	if (Property::_name == propName) \
	{ \
		return Set##_name(Property::_name##XMLConverter::ConvertToValue(propVaule)); \
	}else
#define XMLConvert_End	{} \
}

// define XMsg router
#define BEGIN_XMSG_MAP(_msg) \
{ \
	CXMsg_GetListenList* pListMsg = dynamic_cast<CXMsg_GetListenList*>(&_msg);
#define OnXMsg(_msg) \
	if (pListMsg) \
	{ \
		pListMsg->XMsgList.push_back(_msg::GetXMsgName()); \
	} \
	else if( msg.GetMyMsgName().CompareNoCase(_msg::GetXMsgName()) == 0) \
	{ \
		_msg* pDeriMsg = dynamic_cast<_msg*>(&msg); \
		ATLASSERT(pDeriMsg && "invalid XMessage response!!!!!!!"); \
		On_##_msg(*pDeriMsg); \
		if(msg.msgHandled) \
		return XResult_Handled; \
	}
#define END_XMSG_MAP \
}

//////////////////////////////////////////////////////////////////////////

class CXElement : public CXBase
	, public CXTreeNode
{
	XClass(CXBase);
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
	XProperty_End;

	// XMsg的接收入口函数
	virtual XResult ProcessXMessage(CXMsg& msg);

	// 对string类型的属性值的解析支持，用于XML的实例化，可通过该函数知道XML支持什么属性
	virtual XResult SetXMLProperty(CString name,CString value);

	Property::CXProperty& GetPrpertyRef()	{return m_property;};

protected:
	VOID _SendXMessageToChildren(CXMsg& pMsg);
	VOID _RaiseXMessageUp(CXMsg& msg);

	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
	VOID On_CXMsg_SizeChanged(CXMsg_SizeChanged& arg);
	VOID On_CXMsg_Layout(CXMsg_Layout& arg);
	VOID On_CXMsg_Paint(CXMsg_Paint& arg);
	VOID On_CXMsg_PaintElement(CXMsg_PaintElement& arg);
	VOID On_CXMsg_MouseEnter(CXMsg_MouseEnter& arg);
	VOID On_CXMsg_MouseLeave(CXMsg_MouseLeave& arg);
	VOID On_CXMsg_AttachDC(CXMsg_AttachDC& arg);
protected:
	Property::CXProperty	m_property;
	BOOL	m_isLayouting;
	CToolTipCtrl	m_toolTip;
	XPtr<CGDIMemDC>	m_memDC;
};

typedef XSmartPtr<CXElement> ElementRef;

#include "XElementUtil.hpp"

MyNameIs(CXElement)
	I_Provide("可绘制元素的基本属性及X消息路由,对xml创建ui元素做了属性上的支持")
End_Description;

//////////////////////////////////////////////////////////////////////////
#include "XLayouter/Layouter.hpp"

CXElement::CXElement()
	: m_isLayouting(FALSE)
	, m_memDC(nullptr)
{
}

CXElement::~CXElement()
{
	if (m_memDC)
	{
		m_memDC = nullptr;
	}
}

XResult CXElement::SetXMLProperty( CString name,CString value )
{
	XMLConvert_Begin(name,value)
		XMLConvert(Rect)
		XMLFakeConvert(Position)
		XMLFakeConvert(Size)
		XMLFakeConvert(ID)
		XMLConvert(Color)
		XMLConvert(AutoWidth)
		XMLConvert(AutoHeight)
		XMLConvert(ExpandWidth)
		XMLConvert(ExpandHeight)
		XMLConvert(LayoutType)
		XMLConvert(LayoutDirection)
		XMLConvert(Align)
		XMLConvert(BorderColor)
		XMLConvert(BorderWidth)
		XMLConvert(HitTest)
		XMLConvert(Padding)
		XMLConvert(ToolTip)
		XMLConvert(Ghost)
	XMLConvert_End

	return XResult_NotSupport;
}

XResult CXElement::ProcessXMessage( CXMsg& msg )
{
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged)
		OnXMsg(CXMsg_SizeChanged)
		OnXMsg(CXMsg_Layout)
		OnXMsg(CXMsg_Paint)
		OnXMsg(CXMsg_MouseEnter)
		OnXMsg(CXMsg_MouseLeave)
		OnXMsg(CXMsg_PaintElement)
		OnXMsg(CXMsg_AttachDC)
	END_XMSG_MAP;
	if (!msg.msgHandled)
	{
		_SendXMessageToChildren(msg);
	}
	return XResult_OK;
}

//////////////////////////////////////////////////////////////////////////

VOID CXElement::_SendXMessageToChildren( CXMsg& pMsg )
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
		XSmartPtr<CXElement> pElement = *i;
		pElement->ProcessXMessage( pMsg );
		if (pMsg.msgHandled)
		{
			return;
		}
	}
}

VOID CXElement::_RaiseXMessageUp(CXMsg& msg)
{
	ElementRef fatherElement;
	fatherElement = GetFather();
	while (fatherElement && !msg.msgHandled)
	{
		fatherElement->ProcessXMessage(msg);
	}
}

//////////////////////////////////////////////////////////////////////////

XResult CXElement::GetPosition(Property::PositionType& value)
{
	CRect rect;
	GetRect(rect);
	value = rect.TopLeft();
	return XResult_OK;
}

XResult CXElement::SetPosition(Property::PositionType param)
{
	CRect rect;
	XResult result = GetRect(rect);
	if (XFAILED(result))
	{
		return result;
	}
	CSize size = rect.Size();
	rect.TopLeft() = param;
	rect.right = rect.left + size.cx;
	rect.bottom = rect.top + size.cy;
	return SetRect(rect);
}

XResult CXElement::GetSize(Property::SizeType& value)
{
	SetDefPropertyValue(Size,value);

	CRect rect;
	XResult result = GetRect(rect);
	if (XFAILED(result))
	{
		return result;
	}
	value = rect.BottomRight() - rect.TopLeft();
	return XResult_OK;
}

XResult CXElement::SetSize(Property::SizeType param)
{
	CRect rect;
	GetRect(rect);
	rect.BottomRight() = rect.TopLeft() + param;
	return SetRect(rect);
}

XResult CXElement::GetLayoutRect(Property::LayoutRectType& value)
{
	GetRect(value);
	CRect padding;
	GetPadding(padding);
	value.left += padding.left;
	value.right -= padding.right;
	value.top += padding.top;
	value.bottom -= padding.bottom;
	return XResult_OK;
}

XResult CXElement::SetLayoutRect(Property::LayoutRectType param)
{
	CRect padding;
	GetPadding(padding);
	param.left -= padding.left;
	param.right += padding.right;
	param.top -= padding.top;
	param.bottom += padding.bottom;
	return SetSize(param.Size());
}

//////////////////////////////////////////////////////////////////////////

VOID CXElement::On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg)
{
	URP(arg);
	if (arg.name == Property::Size)
	{
		CXMsg_PaintElement msg;
		msg.paintChildren = FALSE;
		ProcessXMessage(msg);
	}
	return;
}

VOID CXElement::On_CXMsg_SizeChanged( CXMsg_SizeChanged& arg )
{
	URP(arg);
}

VOID CXElement::On_CXMsg_Layout( CXMsg_Layout& arg )
{
	URP(arg);

	if (m_isLayouting)
	{
		return;
	}

	BOOL layoutinvalid;
	GetLayoutInvalid(layoutinvalid);
	if (layoutinvalid)
	{
		std::list<ElementRef> needExpandElements;
		for (auto& i:m_children)
		{
			ElementRef childElement(i);
			BOOL expandWidth;
			childElement->GetExpandWidth(expandWidth);
			BOOL expandHeight;
			childElement->GetExpandHeight(expandHeight);
			if (expandWidth || expandHeight)
			{
				needExpandElements.push_back(childElement);
			}
			else
			{
				childElement->ProcessXMessage(arg);
			}
		}

		m_isLayouting = TRUE;
		Layouter::LayouterRef layouter;
		Property::ELayoutType type = Property::LayoutTypeDefaultValue;
		GetLayoutType(type);
		Layouter::GetLayouter(type,layouter);
		if (layouter)
		{
			layouter->Layout(this);
		}
		else
		{
			WTF;
		}
		m_isLayouting = FALSE;
		SetLayoutInvalid(FALSE);

		for (auto i:needExpandElements)
		{
			i->ProcessXMessage(arg);
		}
	}
}

VOID CXElement::On_CXMsg_Paint( CXMsg_Paint& arg )
{
	URP(arg);
	BOOL layoutInvalid = TRUE;
	GetLayoutInvalid(layoutInvalid);
	if (layoutInvalid)
	{
		CXMsg_Layout msg;
		ProcessXMessage(msg);
	}
	CRect rect;
	GetRect(rect);
	rect.OffsetRect(arg.offsetFix);

	CRect testRect;
	if (m_memDC && 
		testRect.IntersectRect(arg.drawDevice.invalidRect,rect))
	{
		HDC hDC = m_memDC->m_hDC;
		CPoint srcPoint = testRect.TopLeft();
		srcPoint -= rect.TopLeft();
		arg.drawDevice.dc.BitBlt(testRect.left,testRect.top,testRect.Width(),testRect.Height(),
			m_memDC->m_hDC,srcPoint.x,srcPoint.y,SRCCOPY);
	}

	CPoint point;
	GetPosition(point);
	CPoint oriOffset = arg.offsetFix;
	arg.offsetFix += point;

	_SendXMessageToChildren(arg);
	arg.offsetFix = oriOffset;

	arg.msgHandled = TRUE;
}

VOID CXElement::On_CXMsg_PaintElement( CXMsg_PaintElement& arg )
{
	arg.msgHandled = TRUE;
	if (!m_memDC)
	{
		return;
	}
	CRect rect;
	GetRect(rect);
	rect.OffsetRect(-rect.left,-rect.top);

	COLORREF color;
	if (XSUCCEEDED(GetColor(color)))
	{
		LOGBRUSH brushLog;
		brushLog.lbColor = color;
		brushLog.lbStyle = BS_SOLID;
		brushLog.lbHatch = 0;
		HBRUSH brush = CreateBrushIndirect(&brushLog);
		m_memDC->FillRect(rect,brush);
		DeleteObject(brush);
	}

	COLORREF borderColor;
	if(XSUCCEEDED(GetBorderColor(borderColor)))
	{
		DWORD borderWidth;
		GetBorderWidth(borderWidth);
		HPEN pen = CreatePen(PS_SOLID,borderWidth,borderColor);
		CGDIHandleSwitcher handleSwitcher(m_memDC->m_hDC,pen);
		m_memDC->Rectangle(rect);
	}

	if (arg.paintChildren)
	{
		_SendXMessageToChildren(arg);
	}
}

VOID CXElement::On_CXMsg_AttachDC( CXMsg_AttachDC& arg )
{
	if (arg.hostDC)
	{
		if (m_memDC)
		{
			m_memDC = nullptr;
		}
		CRect rect;
		GetRect(rect);
		rect.OffsetRect(-rect.left,-rect.top);
		m_memDC = new CGDIMemDC(arg.hostDC,rect);
		if (! m_memDC)
		{
			WTF;
		}
		m_memDC->ClearDrawDevice();
		
		CXMsg_PaintElement msg;
		msg.paintChildren = FALSE;
		ProcessXMessage(msg);
	}
	else
	{
		if (m_memDC)
		{
			m_memDC = nullptr;
		}
	}
	arg.msgHandled = FALSE;
}

VOID CXElement::On_CXMsg_MouseEnter( CXMsg_MouseEnter& arg )
{
	//SetBorderWidth(3);
}

VOID CXElement::On_CXMsg_MouseLeave( CXMsg_MouseLeave& arg )
{
	//SetBorderWidth(1);
}
