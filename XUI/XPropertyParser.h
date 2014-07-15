#pragma once
#include "Header/XPropertyDefine.h"
#include "XFrameWork/XProperty.hpp"
#include <stdlib.h>

namespace Property
{

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
			_stscanf_s(value, _T("%u"), &dwValue);
			return dwValue;
		}
	};

	class CXMLConverter_HWND : public CXMLConverter_DWORD
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
			_stscanf_s(value, _T("%u,%u,%u,%u"), &r, &g, &b, &a);
			dwValue = RGBA(r, g, b, a);
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
				boolValue = _ttoi(value) == 0 ? FALSE : TRUE;
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
			_stscanf_s(value, _T("%d,%d,%d,%d"), &rect.left, &rect.top, &rect.right, &rect.bottom);
			return rect;
		}
	};

	class CXMLConverter_CPoint
	{
	public:
		static CPoint ConvertToValue(CString value)
		{
			CPoint point;
			_stscanf_s(value, _T("%d,%d"), &point.x, &point.y);
			return point;
		}
	};

	class CXMLConverter_CSize
	{
	public:
		static CSize ConvertToValue(CString value)
		{
			CSize size;
			_stscanf_s(value, _T("%d,%d"), &size.cx, &size.cy);
			return size;
		}
		static CSize ConvertToValue(LPCTSTR name, CString value, Property::CXProperty& prop)
		{
			URP(name);
			CSize size(0,0);
			CString subValue;
			int pos = 0;
			subValue = value.Tokenize(L",", pos);
			subValue = subValue.MakeLower();
			if (pos != -1)
			{
				if (subValue == L"auto")
				{
					prop.SetProperty(Property::AutoWidth, TRUE);
				}
				else if (subValue == L"expand")
				{
					prop.SetProperty(Property::ExpandWidth, TRUE);
				}
				else
				{
					size.cx = std::wcstol(subValue,nullptr,10);
				}
				subValue = value.Tokenize(L",", pos);
				subValue = subValue.MakeLower();
				if (pos != -1)
				{
					if (subValue == L"auto")
					{
						prop.SetProperty(Property::AutoHeight, TRUE);
					}
					else if (subValue == L"expand")
					{
						prop.SetProperty(Property::ExpandHeight, TRUE);
					}
					else
					{
						size.cy = std::wcstol(subValue, nullptr, 10);
					}
				}
			}
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

}