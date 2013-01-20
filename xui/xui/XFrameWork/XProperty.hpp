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
	XClass;
public:
 	SupportType(m_propertyMap,PropertyValue<CString>);
 	SupportType(m_propertyMap,PropertyValue<DWORD>);
 	SupportType(m_propertyMap,PropertyValue<NodeRef>);
 	SupportType(m_propertyMap,PropertyValue<CRect>);
 	SupportType(m_propertyMap,PropertyValue<CPoint>);

	CXProperty& operator=(const CXProperty& rhs);
	XResult	Switch(CXProperty& rhs);
	XResult MarkSupportedProperty(CString key);
	XResult IsSupported(CString key);
protected:
	std::map<CString,CBuffer>	m_propertyMap;
	std::set<CString>			m_supportedProps;
};

MyNameIs(CXProperty)
	I_Provide("属性描述表及属性的保持工具")
End_Description;


//////////////////////////////////////////////////////////////////////////


XResult CXProperty::MarkSupportedProperty( CString key )
{
	m_supportedProps.insert(key);
	return XResult_OK;
}

XResult CXProperty::IsSupported( CString key )
{
	bool bFound = m_supportedProps.find(key) != m_supportedProps.end();
	return bFound? XResult_OK: XResult_NotSupport;
}
