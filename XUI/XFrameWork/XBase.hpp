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

#define XClass ;

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