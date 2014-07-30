#pragma once
#include "XProperty.h"

namespace Property
{
	bool operator==(const RECT& lhs, const CRect& rhs)
	{
		if (lhs.left == rhs.left &&
			lhs.right == rhs.right &&
			lhs.top == rhs.top &&
			lhs.bottom == rhs.bottom)
		{
			return true;
		}
		return false;
	}
	bool operator !=(const RECT& lhs, const CRect& rhs){ return !operator==(lhs, rhs); };

	bool operator==(const POINT& lhs, const CPoint& rhs)
	{
		if (lhs.x == rhs.x &&
			lhs.y == rhs.y)
		{
			return true;
		}
		return false;
	}
	bool operator !=(const POINT& lhs, const CPoint& rhs){ return !operator==(lhs, rhs); };

	bool operator==(const SIZE& lhs, const CSize& rhs)
	{
		if (lhs.cx == rhs.cx &&
			lhs.cy == rhs.cy)
		{
			return true;
		}
		return false;
	}
	bool operator !=(const SIZE& lhs, const CSize& rhs){ return !operator==(lhs, rhs); };


	XResult CXProperty::SetProperty(LPCTSTR key, const XNodeRef& value)
	{
		auto& oldVer = m_propertyMap[key].value.node;
		if (oldVer != value)
		{
			m_propertyMap[key].changed = true;
		}
		oldVer = value;
		if (value)
		{
			value->AddRef();
		}
		return XResult_OK;
	}

	XResult CXProperty::SetProperty(LPCTSTR key, const CString& value)
	{
		auto& oldValue = m_propertyMap[key];
		if (!oldValue.value.strValue)
		{
			oldValue.value.strValue = new CString(value);
		}
		else
		{
			if (wcscmp(*oldValue.value.strValue, value) != 0)
			{
				oldValue.changed = true;
				*oldValue.value.strValue = value;
			}
		}
		return XResult_OK;
	}

	XResult CXProperty::SetProperty(LPCTSTR key, const DWORD& value)
	{
		auto& oldValue = m_propertyMap[key];
		if (oldValue.value.dwValue != value)
		{
			oldValue.changed = true;
			oldValue.value.dwValue = value;
		}
		return XResult_OK;
	}

	XResult CXProperty::SetProperty(LPCTSTR key, const INT& value)
	{
		auto& oldValue = m_propertyMap[key];
		if (oldValue.value.intValue != value)
		{
			oldValue.changed = true;
			oldValue.value.intValue = value;
		}
		return XResult_OK;
	}

	XResult CXProperty::SetProperty(LPCTSTR key, const CRect& value)
	{
		auto& oldValue = m_propertyMap[key];
		if (oldValue.value.rectValue != value)
		{
			oldValue.changed = true;
			oldValue.value.rectValue = value;
		}
		return XResult_OK;
	}

	XResult CXProperty::SetProperty(LPCTSTR key, const CPoint& value)
	{
		auto& oldValue = m_propertyMap[key];
		if (oldValue.value.ptValue != value)
		{
			oldValue.changed = true;
			oldValue.value.ptValue = value;
		}
		return XResult_OK;
	}

	XResult CXProperty::SetProperty(LPCTSTR key, const CSize& value)
	{
		auto& oldValue = m_propertyMap[key];
		if (oldValue.value.sizeValue != value)
		{
			oldValue.changed = true;
			oldValue.value.sizeValue = value;
		}
		return XResult_OK;
	}

	XResult CXProperty::SetProperty(LPCTSTR key, const HWND& value)
	{
		auto& oldValue = m_propertyMap[key];
		if (oldValue.value.hwndValue != value)
		{
			oldValue.changed = true;
			oldValue.value.hwndValue = value;
		}
		return XResult_OK;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, XNodeRef& value) const
	{
		auto ci = m_propertyMap.find(key);
		if (ci == m_propertyMap.end())
		{
			return XResult_NotFound;
		}
		value = ci->second.value.node;
		return XResult_OK;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, CString& value) const
	{
		auto iFind = m_propertyMap.find(key);
		if (iFind != m_propertyMap.end())
		{
			if (iFind->second.value.strValue)
			{
				value = *(iFind->second.value.strValue);
				return XResult_OK;
			}
			return XResult_Fail;
		}
		return XResult_NotFound;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, DWORD& value) const{
		auto iFind = m_propertyMap.find(key);
		if (iFind != m_propertyMap.end()){
			value = iFind->second.value.dwValue;
			return XResult_OK;
		}
		return XResult_NotFound;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, INT& value) const
	{
		auto iFind = m_propertyMap.find(key);
		if (iFind != m_propertyMap.end()){
			value = iFind->second.value.intValue;
			return XResult_OK;
		}
		return XResult_NotFound;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, CRect& value) const
	{
		auto iFind = m_propertyMap.find(key);
		if (iFind != m_propertyMap.end()){
			value = iFind->second.value.rectValue;
			return XResult_OK;
		}
		return XResult_NotFound;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, CPoint& value) const
	{
		auto iFind = m_propertyMap.find(key);
		if (iFind != m_propertyMap.end()){
			value = iFind->second.value.ptValue;
			return XResult_OK;
		}
		return XResult_NotFound;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, CSize& value) const
	{
		auto iFind = m_propertyMap.find(key);
		if (iFind != m_propertyMap.end()){
			value = iFind->second.value.sizeValue;
			return XResult_OK;
		}
		return XResult_NotFound;
	}

	XResult CXProperty::GetProperty(LPCTSTR key, HWND& value) const
	{
		auto iFind = m_propertyMap.find(key);
		if (iFind != m_propertyMap.end()){
			value = iFind->second.value.hwndValue;
			return XResult_OK;
		}
		return XResult_NotFound;
	}

	bool CXProperty::IsChanged(LPCTSTR key)
	{
		auto i = m_propertyMap.find(key);
		if (i != m_propertyMap.end())
		{
			return i->second.changed;
		}
		return FALSE;
	}

	void CXProperty::ChangeRestore(LPCTSTR key)
	{
		CString strKey(key);
		if (strKey.GetLength() != 0)
		{
			auto i = m_propertyMap.find(strKey);
			if (i != m_propertyMap.end())
			{
				i->second.changed = FALSE;
			}
		}
		else
		{
			for (auto i = m_propertyMap.begin(); i != m_propertyMap.end(); ++i)
			{
				i->second.changed = FALSE;
			}
		}
	}

}
