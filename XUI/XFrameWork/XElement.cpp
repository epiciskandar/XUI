#pragma once
#include "XElement.h"
#include "XLayouter/Layouter.hpp"

CXElement::CXElement()
{
	m_property = new Property::CXProperty;
}

CXElement::~CXElement()
{
	if (m_property)
	{
		delete m_property;
	}
}

XResult CXElement::ProcessXMessage( IXMsg& msg )
{
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_SizeChanged)
		OnXMsg(CXMsg_Layout)
		OnXMsg(CXMsg_Paint)
		OnXMsg(CXMsg_MouseEnter)
		OnXMsg(CXMsg_MouseLeave)
		OnXMsg(CXMsg_AttachDC)
		OnXMsg(CXMsg_FrameClick)
		OnXMsg(CXMsg_RealWndClosing)
	END_XMSG_MAP;
	return XResult_OK;
}

VOID CXElement::MsgDown(IXMsg& msg)
{
	for (auto& i:m_children)
	{
		XPtr<IXElement> pChildElement(i);
		if (pChildElement)
		{
			pChildElement->ProcessXMessage(msg);
			pChildElement->MsgDown(msg);
		}
	}
}

VOID CXElement::MsgUp(IXMsg& msg)
{
	ProcessXMessage(msg);
	XPtr<IXNode> pFatherNode(m_father);
	XPtr<IXElement> pFather(pFatherNode);
	if (pFather)
	{
		pFather->ProcessXMessage(msg);
		pFather->MsgUp(msg);
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

XResult CXElement::GetInnerLayoutRect( Property::InnerLayoutRectType& value )
{
	GetRect(value);
	value.OffsetRect(-value.left, -value.top); // 不需要当前元素的位置信息
	CSize size = value.Size();
	CheckSizeLimit(size);
	value.Size() = size;
	CRect padding;
	GetPadding(padding);
	value.left += padding.left;
	value.right -= padding.right;
	if (value.right < value.left)
	{
		value.right = value.left;
	}
	value.top += padding.top;
	value.bottom -= padding.bottom;
	if (value.bottom < value.top)
	{
		value.bottom = value.top;
	}
	return XResult_OK;
}

XResult CXElement::SetInnerLayoutRect( Property::InnerLayoutRectType param )
{
	CRect padding;
	GetPadding(padding);
	param.left -= padding.left;
	param.right += padding.right;
	param.top -= padding.top;
	param.bottom += padding.bottom;
	return SetRect(param);
}

XResult CXElement::GetOuterLayoutRect( Property::OuterLayoutRectType& value )
{
	GetRect(value);
	value.OffsetRect(-value.left, -value.top); // 不需要当前元素的位置信息
	CSize size = value.Size();
	CheckSizeLimit(size);
	value.Size() = size;
	CRect margin;
	GetMargin(margin);
	value.left -= margin.left;
	value.right += margin.right;
	value.top -= margin.top;
	value.bottom += margin.bottom;
	if (value.left < 0)
	{
		value.OffsetRect(-value.left, 0);
	}
	if (value.top < 0)
	{
		value.OffsetRect(0, -value.top);
	}
	return XResult_OK;
}

XResult CXElement::SetOuterLayoutRect( Property::OuterLayoutRectType param )
{
	CRect margin;
	GetMargin(margin);
	param.left += margin.left;
	param.right -= margin.right;
	param.top += margin.top;
	param.bottom -= margin.bottom;
	XLOG(L"set element %s rect(%d,%d,%d,%d)\n", m_ID, param.left, param.right, param.top, param.bottom);
	return SetRect(param);
}

VOID CXElement::CheckSizeLimit(CSize& size)
{
	CRect sizeLimit;
	GetSizeLimit(sizeLimit);
	if (sizeLimit.left >= 0)	size.cx = MAX(size.cx, sizeLimit.left);
	if (sizeLimit.right >= 0)	size.cx = MIN(size.cx, sizeLimit.right);
	if (sizeLimit.top >= 0)	size.cy = MAX(size.cy, sizeLimit.top);
	if (sizeLimit.bottom >= 0)	size.cy = MIN(size.cy, sizeLimit.bottom);
}

VOID CXElement::CheckOuterSizeLimit(CSize& size)
{
	CRect sizeLimit;
	GetSizeLimit(sizeLimit);
	CRect margin;
	GetMargin(margin);
	CSize smallestSize(sizeLimit.left, sizeLimit.top);
	if (smallestSize.cx >= 0)
	{
		smallestSize.cx += margin.left + margin.right;
		size.cx = MAX(MAX(smallestSize.cx, 0), size.cx);
	}
	if (smallestSize.cy >= 0)
	{
		smallestSize.cy += margin.top + margin.bottom;
		size.cy = MAX(MAX(smallestSize.cy, 0), size.cy);
	}
	CSize biggestSize(sizeLimit.right, sizeLimit.bottom);
	if (biggestSize.cx >= 0)
	{
		biggestSize.cx += margin.left + margin.right;
		size.cx = MIN(biggestSize.cx, size.cx);
	}
	if (biggestSize.cy >= 0)
	{
		biggestSize.cy += margin.top + margin.bottom;
		size.cy = MIN(biggestSize.cy, size.cy);
	}
}

VOID CXElement::CheckInnerSizeLimit(CSize& size)
{
	CRect sizeLimit;
	GetSizeLimit(sizeLimit);
	CRect padding;
	GetPadding(padding);
	CSize smallestSize(sizeLimit.left, sizeLimit.top);
	if (smallestSize.cx >= 0)
	{
		smallestSize.cx += padding.left + padding.right;
		size.cx = MAX(MAX(smallestSize.cx, 0), size.cx);
	}
	if (smallestSize.cy >= 0)
	{
		smallestSize.cy += padding.top + padding.bottom;
		size.cy = MAX(MAX(smallestSize.cy, 0), size.cy);
	}
	CSize biggestSize(sizeLimit.right, sizeLimit.bottom);
	if (biggestSize.cx >= 0)
	{
		biggestSize.cx += padding.left + padding.right;
		size.cx = MIN(biggestSize.cx, size.cx);
	}
	if (biggestSize.cy >= 0)
	{
		biggestSize.cy += padding.top + padding.bottom;
		size.cy = MIN(biggestSize.cy, size.cy);
	}
}

VOID CXElement::On_CXMsg_SizeChanged( CXMsg_SizeChanged& arg )
{
	URP(arg);
}

VOID CXElement::On_CXMsg_Layout( CXMsg_Layout& arg )
{
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
	BOOL layoutInvalid = TRUE;
	GetLayoutInvalid(layoutInvalid);
	if (layoutInvalid)
	{
		CXMsg_Layout msg;
		ProcessXMessage(msg);
	}

	BOOL invalid = FALSE;
	GetInvalid(invalid);
	if (invalid)
	{
		Render();
		SetInvalid(FALSE);
	}
	Paint(arg);

	MsgDown(arg);

	arg.msgHandled = TRUE;
}

VOID CXElement::Paint( CXMsg_Paint& arg )
{
	BOOL bGhost = FALSE;
	GetGhost(bGhost);
	BOOL needRealPaint;
	GetNeedRealPaint(needRealPaint);
	if (bGhost || !needRealPaint) //Ghost 属性为真 跳过绘制
	{
		return;
	}
	CRect paintDstRect;
	CPoint srcOffset;

	if (PaintCheck(arg.drawDevice.invalidRect, paintDstRect, srcOffset))
	{
		arg.drawDevice.dc.BitBlt(paintDstRect.left, paintDstRect.top
			, paintDstRect.Width(), paintDstRect.Height(),
			m_memDC->m_hDC, srcOffset.x, srcOffset.y, SRCCOPY);
	}
}

BOOL CXElement::PaintCheck(CRect invalidRect, CRect& paintDstRect, CPoint& paintSrcPt)
{
	CRect rect;
	GetRectInClientCoord(rect);
	XPtr<CXElement> pFatherElement(GetFather());
	if (pFatherElement)
	{
		CRect fatherRect;
		pFatherElement->GetRectInClientCoord(fatherRect);
		rect.IntersectRect(rect, fatherRect);
	}

	if (m_memDC && 
		paintDstRect.IntersectRect(invalidRect, rect))
	{
		paintSrcPt = paintDstRect.TopLeft();
		paintSrcPt -= rect.TopLeft();
		return TRUE;
	}
	return FALSE;
}

VOID CXElement::Render()
{
	XLOG(_T("Render Element %s\n"), GetID());
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

	SetNeedRealPaint(updated);
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

		SetNeedRealPaint(TRUE);
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
		MsgUp(msg);
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
	CString propName = _name; \
	LPCTSTR propValue = _value; \

#define XMLAdvancedConvert(_name,_converter) \
	if (Property::_name == propName) \
	{ \
		Set##_name(_converter::ConvertToValue(propName, propValue, *m_property)); \
	}else

#define XMLFakeConverter(_name, _converter) \
	if (Property::_name == propName) \
	{ \
		Set##_name(_converter::ConvertToValue(propValue)); \
	}else

#define GetConverter(_name) Property::_name##XMLConverterType
#define XMLParse(_name) \
	if(Property::_name == propName) \
	{ \
		m_property->SetProperty(propName,GetConverter(_name)::ConvertToValue(propValue)); \
	}else

#define XMLParseToType(_name,_type) \
	if(Property::_name == propName){ \
		m_property->SetProperty(propName,(_type)GetConverter(_name)::ConvertToValue(propValue)); \
	}else

#define XMLConvert_End	{} \
}

XResult CXElement::SetXMLProperty( LPCTSTR name,LPCTSTR value )
{
	XMLConvert_Begin(name, value)
		XMLFakeConverter(Position, Property::CXMLConverter_CPoint)
		XMLFakeConverter(ID, Property::CXMLConverter_CString)

		if (!m_property)
		{
			return XResult_Fail;
		}
		// 下面的都需要property对象
		XMLAdvancedConvert(Size, Property::CXMLConverter_CSize)

		XMLParse(Rect)
		XMLParse(SizeLimit)
		XMLParse(Text)
		XMLParse(Title)
		XMLParse(Color)
		XMLParse(TextColor)
		XMLParse(BorderColor)
		XMLParse(BorderWidth)
		XMLParse(WinStyle)
		XMLParse(WinExStyle)
		XMLParse(CenterWindow)
		XMLParse(ShowState)
		XMLParseToType(LayoutType, INT)
		XMLParseToType(LayoutDirection, INT)
		XMLParseToType(Align, INT)
		XMLParse(AutoWidth)
		XMLParse(AutoHeight)
		XMLParse(ExpandWidth)
		XMLParse(ExpandHeight)
		XMLParse(SizeLimit)
		XMLParse(Sizable)
		XMLParse(File)
		XMLParse(ImageWidth)
		XMLParse(HitTest)
		XMLParse(Margin)
		XMLParse(Padding)
		XMLParse(Ghost)
		XMLParse(ToolTip)
		XMLParse(FontName)
		XMLParse(FontSize)
		XMLParse(Offset)
		XMLParse(IsLayerWin)
		XMLParse(HasWinBorder)
		XMLParse(HasSysBar)
		XMLParse(Maximizable)
		XMLParse(Minimizable)
		XMLParse(Disabled)
		XMLParse(IsToolWnd)
		XMLParse(IsPopupWnd)
	XMLConvert_End

	return XResult_OK;
}

VOID CXElement::GetRectInClientCoord(CRect& rect)
{
	GetRect(rect);
	XPtr<IXElement> ptr = GetFather();
	while (ptr)
	{
		CPoint pos;
		ptr->GetPosition(pos);
		rect.OffsetRect(pos);
		ptr = XPtr<IXNode>(ptr)->GetFather();
	}
}

XResult CXElement::GetLayoutType(Property::ELayoutType& type)
{
	type = Property::LayoutTypeDefaultValue;
	if (m_property)
	{
		return m_property->GetProperty(Property::LayoutType, (int&)type);
	}
	return XResult_Fail;
}

XResult CXElement::SetLayoutType(Property::ELayoutType type)
{
	if (m_property)
	{
		return m_property->SetProperty(Property::LayoutType, (int)type);
	}
	return XResult_Fail;
}

XResult CXElement::GetLayoutDirection(Property::ELayoutDirection& direction)
{
	direction = Property::LayoutDirectionDefaultValue;
	if (m_property)
	{
		return m_property->GetProperty(Property::LayoutDirection, (int&)direction);
	}
	return XResult_Fail;
}

XResult CXElement::SetLayoutDirection(Property::ELayoutDirection direction)
{
	if (m_property)
	{
		return m_property->GetProperty(Property::LayoutDirection, (int&)direction);
	}
	return XResult_Fail;
}

XResult CXElement::GetAlign(Property::EAlignType& type)
{
	type = Property::AlignDefaultValue;
	if (m_property)
	{
		return m_property->GetProperty(Property::Align, (int&)type);
	}
	return XResult_Fail;
}

XResult CXElement::SetAlign(Property::EAlignType type)
{
	if (m_property)
	{
		return m_property->GetProperty(Property::Align, (int&)type);
	}
	return XResult_Fail;
}