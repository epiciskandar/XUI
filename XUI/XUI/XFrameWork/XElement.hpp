#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XNotifier.hpp"
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
	VOID _SendXMsg(CXMsg& pMsg);

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

inline CXElement::CXElement()
	: m_isLayouting(FALSE)
	, m_memDC(nullptr)
{
}

inline CXElement::~CXElement()
{
	if (m_memDC)
	{
		m_memDC = nullptr;
	}
}

inline XResult CXElement::SetXMLProperty( CString name,CString value )
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

inline XResult CXElement::ProcessXMessage( CXMsg& msg )
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
	_SendXMsg(msg);
	return XResult_OK;
}

//////////////////////////////////////////////////////////////////////////

inline VOID CXElement::_SendXMsg( CXMsg& pMsg )
{
	if (pMsg.msgPolicy==MsgDispatchPolicy::Processor && pMsg.msgHandled)
	{
		return;
	}

	switch (pMsg.msgDirection)
	{
	case MsgDirection::UpToRoot:
	case MsgDirection::UpToRootThenDown:
		if (m_father)
		{
			ElementRef(NodeRef(m_father))->ProcessXMessage(pMsg);
		}
		else
		{
			if (pMsg.msgDirection == MsgDirection::UpToRootThenDown)
			{
				pMsg.msgDirection = MsgDirection::Down;
				_SendXMsg(pMsg);
			}
		}
		break;
		break;
	case MsgDirection::Down:
		for (auto i=m_children.begin(); i!=m_children.end(); ++i)
		{
			if (pMsg.msgPolicy==MsgDispatchPolicy::Processor && pMsg.msgHandled)
			{
				return;
			}
			XSmartPtr<CXElement> pElement = *i;
			pElement->ProcessXMessage( pMsg );
		}
		break;
	default:
		WTF;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////

inline XResult CXElement::GetPosition(Property::PositionType& value)
{
	CRect rect;
	GetRect(rect);
	value = rect.TopLeft();
	return XResult_OK;
}

inline XResult CXElement::SetPosition(Property::PositionType param)
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

inline XResult CXElement::GetSize(Property::SizeType& value)
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

inline XResult CXElement::SetSize(Property::SizeType param)
{
	CRect rect;
	GetRect(rect);
	rect.BottomRight() = rect.TopLeft() + param;
	return SetRect(rect);
}

inline XResult CXElement::GetLayoutRect(Property::LayoutRectType& value)
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

inline XResult CXElement::SetLayoutRect(Property::LayoutRectType param)
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

inline VOID CXElement::On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg)
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

inline VOID CXElement::On_CXMsg_SizeChanged( CXMsg_SizeChanged& arg )
{
	URP(arg);
}

inline VOID CXElement::On_CXMsg_Layout( CXMsg_Layout& arg )
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

inline VOID CXElement::On_CXMsg_Paint( CXMsg_Paint& arg )
{
	URP(arg);
	BOOL bGhost = FALSE;
	GetGhost (bGhost);
	if (bGhost) //Ghost 属性为真 跳过绘制
	{
		return;
	}
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
		CPoint srcPoint = testRect.TopLeft();
		srcPoint -= rect.TopLeft();
		arg.drawDevice.dc.BitBlt(testRect.left,testRect.top,testRect.Width(),testRect.Height(),
			m_memDC->m_hDC,srcPoint.x,srcPoint.y,SRCCOPY);
	}

	CPoint point;
	GetPosition(point);
	CPoint oriOffset = arg.offsetFix;
	arg.offsetFix += point;

	_SendXMsg(arg);
	arg.offsetFix = oriOffset;

	arg.msgHandled = TRUE;
}

inline VOID CXElement::On_CXMsg_PaintElement( CXMsg_PaintElement& arg )
{

	arg.msgHandled = TRUE;
	if (!m_memDC)
	{
		return;
	}

	BOOL updated = FALSE;

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
		updated = TRUE;
	}

	COLORREF borderColor;
	if(XSUCCEEDED(GetBorderColor(borderColor)))
	{
		DWORD borderWidth;
		GetBorderWidth(borderWidth);
		HPEN pen = CreatePen(PS_SOLID,borderWidth,borderColor);
		CGDIHandleSwitcher handleSwitcher(m_memDC->m_hDC,pen);
		m_memDC->Rectangle(rect);
		updated = TRUE;
	}

	if (arg.paintChildren)
	{
		_SendXMsg(arg);
	}

	if (updated)
	{
		CXMsg_Invalidate msg;
		msg.invalidRect = ElementUtil::GetElementRectInClientCoord(this);
		_SendXMsg(msg);
	}
}

inline VOID CXElement::On_CXMsg_AttachDC( CXMsg_AttachDC& arg )
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

inline VOID CXElement::On_CXMsg_MouseEnter( CXMsg_MouseEnter& arg )
{
	CString toolTip;
	if (XSUCCEEDED(GetToolTip(toolTip)))
	{
		CXMsg_GetHWnd msg;
		_SendXMsg(msg);
		if (!m_toolTip.IsWindow())
		{
			m_toolTip.Create(msg.hWnd, NULL, NULL, TTS_ALWAYSTIP | TTS_NOPREFIX,WS_EX_TOPMOST);
			if (!m_toolTip.IsWindow())
			{
				WTF;
			}
			m_toolTip.SetWindowLongPtr(GWLP_USERDATA,(LONG_PTR)msg.hWnd);

			CSize size;
			GetSize(size);
			m_toolTip.SetMaxTipWidth(260);
			m_toolTip.AddTool(msg.hWnd,_T(""));
			m_toolTip.SetDelayTime( TTDT_AUTOPOP, static_cast<int>(GetDoubleClickTime() * 10) ) ;
			m_toolTip.SetDelayTime( TTDT_RESHOW, static_cast<int>(GetDoubleClickTime() * 10) ) ;
			m_toolTip.Activate(TRUE);
		}
		MSG mouseMsg = { msg.hWnd, WM_MOUSEMOVE, 0, MAKELONG (arg.pt.x,arg.pt.y)};
		m_toolTip.RelayEvent(&mouseMsg);

		m_toolTip.UpdateTipText((LPCTSTR)toolTip,msg.hWnd);
	}
	arg.msgHandled = TRUE;
}

inline VOID CXElement::On_CXMsg_MouseLeave( CXMsg_MouseLeave& arg )
{
	if (m_toolTip.IsWindow())
	{
		HWND hParentWnd = (HWND)m_toolTip.GetWindowLongPtr(GWLP_USERDATA);
		m_toolTip.UpdateTipText(_T(""),hParentWnd);
		m_toolTip.DestroyWindow();
	}
	arg.msgHandled = TRUE;
}
