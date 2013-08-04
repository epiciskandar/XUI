#pragma once
#include "XElement.h"
#include "XLayouter/Layouter.hpp"

CXElement::CXElement() : m_isLayouting(FALSE)
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

XResult CXElement::ProcessXMessage( IXMsg& msg )
{
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged)
		OnXMsg(CXMsg_SizeChanged)
		OnXMsg(CXMsg_Layout)
		OnXMsg(CXMsg_Paint)
		OnXMsg(CXMsg_MouseEnter)
		OnXMsg(CXMsg_MouseLeave)
		OnXMsg(CXMsg_RenderElement)
		OnXMsg(CXMsg_AttachDC)
		OnXMsg(CXMsg_FrameClick)
		OnXMsg(CXMsg_RealWndClosing)
	END_XMSG_MAP;
	_SendXMsg(msg);
	return XResult_OK;
}

VOID CXElement::_SendXMsg( IXMsg& pMsg )
{
	if (pMsg.msgPolicy==MsgDispatchPolicy::Processor && pMsg.msgHandled)
	{
		return;
	}

	switch (pMsg.msgDirection)
	{
	case MsgDirection::Up:
		if (m_father)
		{
			ElementRef(XNodeRef(m_father))->ProcessXMessage(pMsg);
		}
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

XResult CXElement::GetPosition( Property::PositionType& value )
{
	CRect rect;
	GetRect(rect);
	value = rect.TopLeft();
	return XResult_OK;
}

XResult CXElement::SetPosition( Property::PositionType param )
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

XResult CXElement::GetSize( Property::SizeType& value )
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

XResult CXElement::SetSize( Property::SizeType param )
{
	CRect rect;
	GetRect(rect);
	rect.BottomRight() = rect.TopLeft() + param;
	return SetRect(rect);
}

XResult CXElement::GetLayoutRect( Property::LayoutRectType& value )
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

XResult CXElement::SetLayoutRect( Property::LayoutRectType param )
{
	CRect padding;
	GetPadding(padding);
	param.left -= padding.left;
	param.right += padding.right;
	param.top -= padding.top;
	param.bottom += padding.bottom;
	return SetSize(param.Size());
}

VOID CXElement::On_CXMsg_PropertyChanged( CXMsg_PropertyChanged& arg )
{
	URP(arg);
	if (arg.name == Property::Size)
	{
		CXMsg_RenderElement msg;
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
	BOOL bGhost = FALSE;
	GetGhost (bGhost);
	BOOL needRealPaint;
	GetNeedRealPaint(needRealPaint);
	if (bGhost || !needRealPaint) //Ghost 属性为真 跳过绘制
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
	CRect paintingRect;
	CPoint srcPt;

	if (_NeedPaint(arg,paintingRect,srcPt))
	{
		arg.drawDevice.dc.BitBlt(paintingRect.left,paintingRect.top,paintingRect.Width(),paintingRect.Height(),
			m_memDC->m_hDC,srcPt.x,srcPt.y,SRCCOPY);
	}

	CPoint point;
	GetPosition(point);
	CPoint oriOffset = arg.offsetFix;
	arg.offsetFix += point;

	_SendXMsg(arg);
	arg.offsetFix = oriOffset;

	arg.msgHandled = TRUE;
}

BOOL CXElement::_NeedPaint( const CXMsg_Paint& arg,CRect& paintingRect,CPoint& srcPt )
{
	CRect rect;
	GetRect(rect);
	rect.OffsetRect(arg.offsetFix);

	if (m_memDC && 
		paintingRect.IntersectRect(arg.drawDevice.invalidRect,rect))
	{
		srcPt = paintingRect.TopLeft();
		srcPt -= rect.TopLeft();
		return TRUE;
	}
	return FALSE;
}

VOID CXElement::On_CXMsg_RenderElement( CXMsg_RenderElement& arg )
{
	XLOG(_T("Render Element %s\n"), GetID());
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

	SetNeedRealPaint(updated);
	if (updated)
	{
		CXMsg_Invalidate msg;
		msg.invalidRect = ElementUtil::GetElementRectInClientCoord(this);
		_SendXMsg(msg);
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

		CXMsg_RenderElement msg;
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
	CString toolTip;
	if (XSUCCEEDED(GetToolTip(toolTip)))
	{
		CXMsg_GetRealWnd msg;
		_SendXMsg(msg);
		if (!msg.wnd)
		{
			return;
		}
		HWND hWnd = 0;
		msg.wnd->GetHWnd(hWnd);
		if (!m_toolTip.IsWindow())
		{
			m_toolTip.Create(hWnd, NULL, NULL, TTS_ALWAYSTIP | TTS_NOPREFIX,WS_EX_TOPMOST);
			if (!m_toolTip.IsWindow())
			{
				WTF;
			}
			m_toolTip.SetWindowLongPtr(GWLP_USERDATA,(LONG_PTR)hWnd);

			CSize size;
			GetSize(size);
			m_toolTip.SetMaxTipWidth(260);
			m_toolTip.AddTool(hWnd,_T(""));
			m_toolTip.SetDelayTime( TTDT_AUTOPOP, static_cast<int>(GetDoubleClickTime() * 10) ) ;
			m_toolTip.SetDelayTime( TTDT_RESHOW, static_cast<int>(GetDoubleClickTime() * 10) ) ;
			m_toolTip.Activate(TRUE);
		}
		MSG mouseMsg = { hWnd, WM_MOUSEMOVE, 0, MAKELONG (arg.pt.x,arg.pt.y)};
		m_toolTip.RelayEvent(&mouseMsg);

		m_toolTip.UpdateTipText((LPCTSTR)toolTip,hWnd);
	}
	arg.msgHandled = TRUE;
}

VOID CXElement::On_CXMsg_MouseLeave( CXMsg_MouseLeave& arg )
{
	if (m_toolTip.IsWindow())
	{
		HWND hParentWnd = (HWND)m_toolTip.GetWindowLongPtr(GWLP_USERDATA);
		m_toolTip.UpdateTipText(_T(""),hParentWnd);
		m_toolTip.DestroyWindow();
	}
	arg.msgHandled = TRUE;
}

VOID CXElement::On_CXMsg_FrameClick( CXMsg_FrameClick& arg )
{
	Whisper(arg);
	arg.msgHandled = TRUE;
}

VOID CXElement::On_CXMsg_RealWndClosing( CXMsg_RealWndClosing& arg )
{
	Whisper(arg);
}

// define XML converters
#define XMLConvert_Begin(_name,_value) \
{ \
	CString& propName = _name; \
	CString& propValue = _value; \

#define XMLFakeConvert(_name,_converter) \
	if (Property::_name == propName) \
	{ \
		return Set##_name(_converter::ConvertToValue(propValue)); \
	}else

#define GetConverter(_name) Property::_name##XMLConverterType
#define XMLConverter(_name) \
	if(Property::_name == propName) \
	{ \
		m_property.SetProperty(propName,GetConverter(_name)::ConvertToValue(propValue)); \
		CXMsg_PropertyChanged msg; \
		msg.name = propName; \
		ProcessXMessage(msg); \
	}else

#define XMLConvert_End	{} \
}

XResult CXElement::SetXMLProperty( CString name,CString value )
{
	XMLConvert_Begin(name,value)
		XMLFakeConvert(Position,Property::CXMLConverter_CPoint)
		XMLFakeConvert(Size,	Property::CXMLConverter_CSize)
		XMLFakeConvert(ID,		Property::CXMLConverter_CString)

		XMLConverter(Rect)
		XMLConverter(Text)
		XMLConverter(Title)
		XMLConverter(Color)
		XMLConverter(TextColor)
		XMLConverter(BorderColor)
		XMLConverter(BorderWidth)
		XMLConverter(WinStyle)
		XMLConverter(WinExStyle)
		XMLConverter(CenterWindow)
		XMLConverter(ShowState)
		XMLConverter(LayoutType)
		XMLConverter(LayoutDirection)
		XMLConverter(Align)
		XMLConverter(AutoWidth)
		XMLConverter(AutoHeight)
		XMLConverter(ExpandWidth)
		XMLConverter(ExpandHeight)
		XMLConverter(SizeLimit)
		XMLConverter(BorderFix)
		XMLConverter(File)
		XMLConverter(ImageWidth)
		XMLConverter(HitTest)
		XMLConverter(Margin)
		XMLConverter(Padding)
		XMLConverter(Ghost)
		XMLConverter(ToolTip)
		XMLConverter(FontName)
		XMLConverter(FontSize)
		XMLConverter(Offset)
	XMLConvert_End

	return XResult_OK;
}