//////////////////////////////////////////////////////////////////////////
// ���������е����ԡ�������ö��ֵ��

#pragma once
#define _WTL_NO_CSTRING
#include "Third/WTL/atlapp.h"
#include "Third/WTL/atlmisc.h"

namespace Property
{

	enum class ELayoutType
	{
		Offset,			// �Ծ������궨λԪ��λ��
		Block			// ��Ԫ�ص��໥�ѻ�����Ű�
	};

	namespace LayoutTypeString
	{
		static LPCTSTR Layout_OffsetString = _T("offset");
		static LPCTSTR Layout_BlockString = _T("block");
	}

	// LayoutTypeΪBlockʱ����
	enum class ELayoutDirection
	{
		Horizon,		// ���ŷ�
		Vertical		// ���ŷ�
	};

	namespace LayoutDirectionString
	{
		static LPCTSTR LayoutDirecting_HorizonString = _T("horizon");
		static LPCTSTR LayoutDirection_VerticalString = _T("vertical");
	}

	// LayoutTypeΪBlockʱ���ã�Ԫ���������еĶ��뷽ʽ
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
	DefineProperty(ID, CString, _T(""));
	DefineProperty(Rect, CRect, CRect());
	DefineProperty(Position, CPoint, CPoint());
	DefineProperty(Size, CSize, CSize());
	DefineProperty(Text, CString, _T(""));
	DefineProperty(Title, CString, _T(""));
	DefineProperty(Color, COLORREF, 0);
	DefineProperty(TextColor, COLORREF, 0);
	DefineProperty(BorderColor, COLORREF, RGB(0, 0, 0));
	DefineProperty(BorderWidth, DWORD, 1);
	DefineProperty(WinStyle, DWORD, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	DefineProperty(WinExStyle, DWORD, 0);
	DefineProperty(HWnd, HWND, 0);
	DefineProperty(CenterWindow, BOOL, TRUE);
	DefineProperty(ShowState, BOOL, FALSE);
	DefineProperty(LayoutType, ELayoutType, ELayoutType::Block);
	DefineProperty(LayoutInvalid, BOOL, TRUE);
	DefineProperty(LayoutDirection, ELayoutDirection, ELayoutDirection::Horizon);
	DefineProperty(InnerLayoutRect, CRect, CRect());	// sizeȥ��padding����ڲ��������ڼ����Ű�
	DefineProperty(OuterLayoutRect, CRect, CRect());	// size����margin����������ڼ����Ű�
	DefineProperty(Align, EAlignType, EAlignType::Left);
	DefineProperty(AutoWidth, BOOL, FALSE);
	DefineProperty(AutoHeight, BOOL, FALSE);
	DefineProperty(ExpandWidth, BOOL, FALSE);
	DefineProperty(ExpandHeight, BOOL, FALSE);
	DefineProperty(SizeLimit, CRect, CRect(-1, -1, -1, -1));
	DefineProperty(BorderFix, BOOL, FALSE);
	DefineProperty(File, CString, _T(""));
	DefineProperty(ImageWidth, DWORD, 0);
	DefineProperty(HitTest, DWORD, HTCLIENT);
	DefineProperty(Margin, CRect, CRect());
	DefineProperty(Padding, CRect, CRect());
	DefineProperty(Ghost, BOOL, FALSE);
	DefineProperty(NeedRealPaint, BOOL, FALSE);
	DefineProperty(Dirty, BOOL, FALSE);
	DefineProperty(ToolTip, CString, _T(""));
	DefineProperty(FontName, CString, _T(""));
	DefineProperty(FontSize, INT, 11);
	DefineProperty(Offset, CPoint, CPoint(0, 0));
	DefineProperty(XFont, XNodeRef, nullptr);

}