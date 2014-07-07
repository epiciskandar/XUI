#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XPropertyValue.hpp"
#include "../Others/CBuffer.h"
#include "XCtrls\XFont.hpp"

#include <map>

namespace Property
{ \

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
	SupportType(m_propertyMap,CPropertyValue<CString>);
	SupportType(m_propertyMap,CPropertyValue<DWORD>);
	SupportType(m_propertyMap,CPropertyValue<INT>);
	SupportType(m_propertyMap,CPropertyValue<CRect>);
	SupportType(m_propertyMap,CPropertyValue<CPoint>);
	SupportType(m_propertyMap,CPropertyValue<CSize>);
	SupportType(m_propertyMap,CPropertyValue<HWND>);
	SupportType(m_propertyMap,CPropertyValue<ELayoutType>);
	SupportType(m_propertyMap,CPropertyValue<EAlignType>);
	SupportType(m_propertyMap,CPropertyValue<ELayoutDirection>);

	// 基于引用计数的对象不能简单地用内存buffer完成
	XResult SetProperty(CString key,const XNodeRef& value)
	{
		m_nodeMap[key] = value;
		return XResult_OK;
	}
	XResult GetProperty(CString key,XNodeRef& value) const
	{
		auto ci = m_nodeMap.find(key);
		if (ci == m_nodeMap.end())
		{
			return XResult_NotFound;
		}
		value = ci->second;
		return XResult_OK;
	}

	BOOL IsChanged(CString key);
	VOID ChangeRestore(CString key/*empty means all*/);

	CXProperty& operator=(const CXProperty& rhs);
	XResult	Switch(CXProperty& rhs);
protected:
	std::map<CString,CBuffer>	m_propertyMap;
	std::map<CString,XNodeRef>	m_nodeMap;
};


//////////////////////////////////////////////////////////////////////////

inline BOOL CXProperty::IsChanged( CString key )
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

inline VOID CXProperty::ChangeRestore( CString key )
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

//////////////////////////////////////////////////////////////////////////

// define property functions
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


}