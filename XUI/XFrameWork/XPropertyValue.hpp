#pragma once
#include "XBase.hpp"
#include "XTree.hpp"

#include <map>
#include "..\WTL\atlmisc.h"

class CChangable
{
public:
	BOOL valueChanged;

	CChangable():valueChanged(FALSE){}

	VOID Change()	{valueChanged = TRUE;}
	BOOL IsChanged(){return valueChanged;}
	VOID Restore()	{valueChanged = FALSE;}
};

// because of using placement new,and we won't call destructor,so destructor is useless.
template <class ValueTypeT>
class CPropertyValue : public CChangable
{
public:
	typedef ValueTypeT ValueType;

	ValueType	m_value;

	CPropertyValue():valueChanged(FALSE)
	{};
	CPropertyValue& operator=(const CPropertyValue& rhs)
	{
		m_value=rhs.m_value;
		Change();
		return *this;
	}
	CPropertyValue(const CPropertyValue& rhs)
	{
		Change();
		m_value=rhs.m_value;
	}
	CPropertyValue(const ValueType& rhs)
	{
		m_value=rhs;
		Change();
	};
	CPropertyValue& operator=(const ValueType& rhs)
	{
		m_value=rhs;
		Change();
		return *this;
	}
	operator const ValueType&() const
	{
		return m_value;
	}; 
};

class CXProperter : public CXTreeNode
{
public:
	virtual XResult SetXMLProperty(CString key,CString value) = 0;
};
