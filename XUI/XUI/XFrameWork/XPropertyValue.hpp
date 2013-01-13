#pragma once
#include "XBase.hpp"
#include "XTree.hpp"

#include <map>
#include "..\WTL\atlmisc.h"

// because of using placement new,and we won't call destructor,so destructor is useless.
template <class ValueTypeT>
class PropertyValue
{
public:
	typedef ValueTypeT ValueType;

	BOOL		valueChanged;
	ValueType	m_value;

	PropertyValue():valueChanged(FALSE)
	{};
	PropertyValue& operator=(const PropertyValue& rhs)
	{
		m_value=rhs.m_value;
		valueChanged = TRUE;
		return *this;
	}
	PropertyValue(const PropertyValue& rhs)
	{
		valueChanged = TRUE;
		m_value=rhs.m_value;
	}
	PropertyValue(const ValueType& rhs)
	{
		m_value=rhs;
		valueChanged = TRUE;
	};
	PropertyValue& operator=(const ValueType& rhs)
	{
		m_value=rhs;
		valueChanged = TRUE;
		return *this;
	}
	operator const ValueType&() const
	{
		return m_value;
	}; 
};
