#pragma once
#include "../XUI.h"

#include <list>
#include <set>

#include "../Third/WTL/atlwinx.h"		// reflection
#include "../Third/WTL/atlframe.h"
#include "../Third/WTL/atlcrack.h"	// message handle
#include "../Third/WTL/atlctrls.h"
#include "../Third/CFile.hpp"

#include "../Third/BLog.hpp"

//////////////////////////////////////////////////////////////////////////

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)	\
private: \
	TypeName(const TypeName&); \
	void operator=(const TypeName&);
#endif

#define SupportCopyAndAssign(_type,_value) \
	_type& operator=(const _type& rhs){_value=rhs._value;return *this;} \
	_type(const _type& rhs){_value=rhs._value;}

#define XSingleton(ClassName) \
DISALLOW_COPY_AND_ASSIGN(ClassName) \
private: \
	ClassName(); \
public: \
	static ClassName& GetInstance(){static ClassName _instance;return _instance;};

#define XLOG(...)	blog::CBLog::GetInstance().Logf(DeviceMask_All,__VA_ARGS__)


//////////////////////////////////////////////////////////////////////////

// define property functions
#define XProperty_Get(_name) \
	virtual XResult Get##_name (Property::_name##Type& value){ \
		if (m_property){ \
			value = Property::_name##DefaultValue; \
			return m_property->GetProperty(L#_name,value); \
		} \
		else return XResult_Fail; \
	}

#define XProperty_Set(_name) \
public: \
	virtual XResult Set##_name (Property::_name##Type param){ \
		if (m_property){ \
			return m_property->SetProperty(L#_name,param); \
		}\
		else return XResult_Fail; \
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
