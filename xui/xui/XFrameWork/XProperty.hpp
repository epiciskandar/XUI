#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XPropertyValue.hpp"
#include "../Others/CBuffer.h"

#include <map>

#define SupportType(_prop,_type) \
	XResult SetProperty(CString key,const _type::ValueType& value) \
	{ \
		CBuffer &buffer = _prop[key]; \
		_type *propValue = (_type*)buffer.GetBuffer(); \
		if (propValue == nullptr) \
		{ \
			buffer.Alloc(sizeof(_type)); \
			propValue = new(buffer.GetBuffer()) _type(value); \
		} \
		*propValue = value; \
		return XResult_OK; \
	} \
	XResult GetProperty(CString key,_type::ValueType& value) const \
	{ \
		auto i = _prop.find(key); \
		if (i == _prop.end()) \
		{ \
			return XResult_NotFound; \
		} \
		const CBuffer &buffer = i->second; \
		const _type *propValue = nullptr; \
		propValue = (const _type*)buffer.GetBuffer(); \
		value = propValue->m_value; \
		return XResult_OK; \
	}

class CXProperty
{
	XClass(VOID);
public:
 	SupportType(m_propertyMap,CPropertyValue<CString>);
	SupportType(m_propertyMap,CPropertyValue<DWORD>);
	SupportType(m_propertyMap,CPropertyValue<INT>);
 	SupportType(m_propertyMap,CPropertyValue<NodeRef>);
 	SupportType(m_propertyMap,CPropertyValue<CRect>);
	SupportType(m_propertyMap,CPropertyValue<CPoint>);
	SupportType(m_propertyMap,CPropertyValue<CSize>);
	SupportType(m_propertyMap,CPropertyValue<HWND>);
	SupportType(m_propertyMap,CPropertyValue<LayoutType>);

	BOOL IsChanged(CString key);
	VOID ChangeRestore(CString key/*empty means all*/);

	CXProperty& operator=(const CXProperty& rhs);
	XResult	Switch(CXProperty& rhs);
protected:
	std::map<CString,CBuffer>	m_propertyMap;
};

MyNameIs(CXProperty)
	I_Provide("属性描述表及属性的保持工具")
End_Description;


//////////////////////////////////////////////////////////////////////////

BOOL CXProperty::IsChanged( CString key )
{
	auto i = m_propertyMap.find(key);
	if (i != m_propertyMap.end())
	{
		CBuffer& buffer = i->second;
		CChangable* changable = static_cast<CChangable*>((VOID*)buffer.GetBuffer());
		return changable->IsChanged();
	}
	return FALSE;
}

VOID CXProperty::ChangeRestore( CString key )
{
	if (key.GetLength() != 0)
	{
		auto i = m_propertyMap.find(key);
		if (i != m_propertyMap.end())
		{
			CBuffer& buffer = i->second;
			CChangable* changable = static_cast<CChangable*>((VOID*)buffer.GetBuffer());
			changable->Restore();
		}
	}
	else
	{
		for (auto i=m_propertyMap.begin(); i!=m_propertyMap.end(); ++i)
		{
			CBuffer& buffer = i->second;
			CChangable* changable = static_cast<CChangable*>((VOID*)buffer.GetBuffer());
			changable->Restore();
		}
	}
}

#include "Utils/XMLConverter.hpp"

// contains each properties definition
namespace Property
{
#define DefineProperty(_name,_type,_defaultValue)	\
	static LPCTSTR _name = _T(#_name); \
	static _type _name##DefaultValue = _defaultValue; \
	typedef _type _name##Type; \
	typedef CXMLConverter_##_type _name##XMLConverter; \

	DefineProperty(Rect		,CRect,		CRect());
	DefineProperty(Position	,CPoint,	CPoint());
	DefineProperty(Size		,CSize,		CSize());
	DefineProperty(Text		,CString,	_T(""));
	DefineProperty(Title	,CString,	_T(""));
	DefineProperty(Color	,COLORREF,	0);
	DefineProperty(TextColor,COLORREF,	0);
	DefineProperty(Style	,DWORD,		WS_OVERLAPPEDWINDOW|WS_VISIBLE);
	DefineProperty(ExStyle	,DWORD,		0);
	DefineProperty(HWnd		,HWND,		0);
	DefineProperty(ShowState,BOOL,		FALSE);
};