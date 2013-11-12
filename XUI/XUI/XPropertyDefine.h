#pragma once

namespace Property
{ \

enum class ELayoutType
{
	Offset,			// 以绝对坐标定位元素位置
	Block			// 以元素的相互堆积完成排版
};

namespace LayoutTypeString
{
	static LPCTSTR Layout_OffsetString = _T("offset");
	static LPCTSTR Layout_BlockString = _T("block");
}

// LayoutType为Block时可用
enum class ELayoutDirection
{
	Horizon,		// 横着放
	Vertical		// 竖着放
};

namespace LayoutDirectionString
{
	static LPCTSTR LayoutDirecting_HorizonString = _T("horizon");
	static LPCTSTR LayoutDirection_VerticalString = _T("vertical");
}

// LayoutType为Block时可用，元素在容器中的对齐方式
enum class EAlignType
{
	Left,
	Top,
	Right,
	Bottom,
};

namespace AlignTypeString
{
	static LPCTSTR Align_LeftString = _T("left");
	static LPCTSTR Align_TopString = _T("top");
	static LPCTSTR Align_RightString = _T("right");
	static LPCTSTR Align_BottomString = _T("bottom");
}

//////////////////////////////////////////////////////////////////////////

class CXMLConverter_CString
{
public:
	static CString ConvertToValue(CString value)
	{
		return value;
	}
};

class CXMLConverter_INT
{
public:
	static INT ConvertToValue(CString value)
	{
		INT intValue;
		intValue = _ttoi(value);
		return intValue;
	}
};

class CXMLConverter_DWORD
{
public:
	static DWORD ConvertToValue(CString value)
	{
		DWORD dwValue;
		_stscanf_s(value,_T("%u"),&dwValue);
		return dwValue;
	}
};

class CXMLConverter_HWND: public CXMLConverter_DWORD
{};

class CXMLConverter_COLORREF
{
public:
	static COLORREF ConvertToValue(CString value)
	{
		COLORREF dwValue;
		DWORD r = 0;
		DWORD g = 0;
		DWORD b = 0;
		DWORD a = 0;
		_stscanf_s(value,_T("%u,%u,%u,%u"),&r,&g,&b,&a);
		dwValue = RGBA(r,g,b,a);
		return dwValue;
	}
};

class CXMLConverter_BOOL
{
public:
	static COLORREF ConvertToValue(CString value)
	{
		BOOL boolValue = FALSE;
		if (value == _T("true"))
		{
			boolValue = TRUE;
		}
		else
		{
			boolValue = _ttoi(value)==0? FALSE:TRUE;
		}
		return boolValue;
	}
};

class CXMLConverter_CRect
{
public:
	static CRect ConvertToValue(CString value)
	{
		CRect rect;
		_stscanf_s(value,_T("%d,%d,%d,%d"),&rect.left,&rect.top,&rect.right,&rect.bottom);
		return rect;
	}
};

class CXMLConverter_CPoint
{
public:
	static CPoint ConvertToValue(CString value)
	{
		CPoint point;
		_stscanf_s(value,_T("%d,%d"),&point.x,&point.y);
		return point;
	}
};

class CXMLConverter_CSize
{
public:
	static CSize ConvertToValue(CString value)
	{
		CSize size;
		_stscanf_s(value,_T("%d,%d"),&size.cx,&size.cy);
		return size;
	}
};

class CXMLConverter_ELayoutType
{
public:
	static ELayoutType ConvertToValue(CString value)
	{
		ELayoutType type = ELayoutType::Block;
		if (value == LayoutTypeString::Layout_OffsetString)
		{
			type = ELayoutType::Offset;
		}
		else if (value == LayoutTypeString::Layout_BlockString)
		{
			type = ELayoutType::Block;
		}
		else
		{
			ATLASSERT(FALSE && "invalid layout type");
		}
		return type;
	}
};

class CXMLConverter_ELayoutDirection
{
public:
	static ELayoutDirection ConvertToValue(CString value)
	{
		ELayoutDirection directionValue = ELayoutDirection::Horizon;
		if (value.CompareNoCase(LayoutDirectionString::LayoutDirecting_HorizonString) == 0)
		{
			directionValue = ELayoutDirection::Horizon;
		}
		else if (value.CompareNoCase(LayoutDirectionString::LayoutDirection_VerticalString) == 0)
		{
			directionValue = ELayoutDirection::Vertical;
		}
		return directionValue;
	}
};

class CXMLConverter_EAlignType
{
public:
	static EAlignType ConvertToValue(CString value)
	{
		EAlignType type = EAlignType::Left;
		if (value == AlignTypeString::Align_LeftString)
		{
			type = EAlignType::Left;
		}
		else if (value == AlignTypeString::Align_TopString)
		{
			type = EAlignType::Top;
		}
		else if (value == AlignTypeString::Align_RightString)
		{
			type = EAlignType::Right;
		}
		else if (value == AlignTypeString::Align_BottomString)
		{
			type = EAlignType::Bottom;
		}
		else
		{
			ATLASSERT(FALSE && "invalid align type");
		}
		return type;
	}
};
//////////////////////////////////////////////////////////////////////////

// define property supports
#define XProperty_SupportGet(_name) \
	virtual XResult Get##_name (Property::_name##Type& value) = 0;
#define XProperty_SupportSet(_name) \
	virtual XResult Set##_name (Property::_name##Type value) = 0;
#define XProperty_Support(_name) \
	XProperty_SupportGet(_name) \
	XProperty_SupportSet(_name)

//////////////////////////////////////////////////////////////////////////

#define DefineProperty(_name,_type,_defaultValue)	\
	static LPCTSTR _name = _T(#_name); \
	static const _type _name##DefaultValue = _defaultValue; \
	typedef _type _name##Type; \
	class CXMLConverter_##_type; \
	typedef CXMLConverter_##_type _name##XMLConverterType;

#define SetDefPropertyValue(_name,_var) _var = Property::_name##DefaultValue;

//				Name			Type				DefaultValue
DefineProperty(ID,				CString,			_T(""));
DefineProperty(Rect,			CRect,				CRect());
DefineProperty(Position,		CPoint,				CPoint());
DefineProperty(Size,			CSize,				CSize());
DefineProperty(Text,			CString,			_T(""));
DefineProperty(Title,			CString,			_T(""));
DefineProperty(Color,			COLORREF,			0);
DefineProperty(TextColor,		COLORREF,			0);
DefineProperty(BorderColor,		COLORREF,			RGB(0,0,0));
DefineProperty(BorderWidth,		DWORD,				1);
DefineProperty(WinStyle,		DWORD,				WS_OVERLAPPEDWINDOW|WS_VISIBLE);
DefineProperty(WinExStyle,		DWORD,				0);
DefineProperty(HWnd,			HWND,				0);
DefineProperty(CenterWindow,	BOOL,				TRUE);
DefineProperty(ShowState,		BOOL,				FALSE);
DefineProperty(LayoutType,		ELayoutType,		ELayoutType::Block);
DefineProperty(LayoutInvalid,	BOOL,				TRUE);
DefineProperty(LayoutDirection,	ELayoutDirection,	ELayoutDirection::Horizon);
DefineProperty(InnerLayoutRect,	CRect,				CRect());	// size去除padding后的内部区域，用于计算排版
DefineProperty(OuterLayoutRect,	CRect,				CRect());	// size加上margin后的区域，用于计算排版
DefineProperty(Align,			EAlignType,			EAlignType::Left);
DefineProperty(AutoWidth,		BOOL,				FALSE);
DefineProperty(AutoHeight,		BOOL,				FALSE);
DefineProperty(ExpandWidth,		BOOL,				FALSE);
DefineProperty(ExpandHeight,	BOOL,				FALSE);
DefineProperty(SizeLimit,		CRect,				CRect(-1,-1,-1,-1));
DefineProperty(BorderFix,		BOOL,				FALSE);
DefineProperty(File,			CString,			_T(""));
DefineProperty(ImageWidth,		DWORD,				0);
DefineProperty(HitTest,			DWORD,				HTCLIENT);
DefineProperty(Margin,			CRect,				CRect());
DefineProperty(Padding,			CRect,				CRect());
DefineProperty(Ghost,			BOOL,				FALSE);
DefineProperty(NeedRealPaint,	BOOL,				FALSE);
DefineProperty(Dirty,			BOOL,				FALSE);
DefineProperty(ToolTip,			CString,			_T(""));
DefineProperty(FontName,		CString,			_T(""));
DefineProperty(FontSize,		INT,				11);
DefineProperty(Offset,		    CPoint,				CPoint(0,0));
DefineProperty(XFont,		    XNodeRef,			nullptr);

}