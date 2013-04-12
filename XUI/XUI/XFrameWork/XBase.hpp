#pragma once
#include "../XUI_include.h"

#include <list>
#include <set>

#define _WTL_NO_CSTRING
#include "../WTL/atlapp.h"
#include "../WTL/atlwinx.h"		// reflection
#include "../WTL/atlframe.h"
#include "../WTL/atlcrack.h"	// message handle
#include "../WTL/atlmisc.h"
#include "../Others/CFile.hpp"

#include "../Others/BLog.hpp"

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

#define URP(...)	(__VA_ARGS__);
#define IF_RETURN(_exp,_ret)	{if(_exp){return _ret;}}
#define Return_OnXError(_exp)	{XResult result = _exp;if(XFAILED(result)){return result;}}

//////////////////////////////////////////////////////////////////////////

#include "XUtil.hpp"
