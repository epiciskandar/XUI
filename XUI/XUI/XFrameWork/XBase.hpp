#pragma once
#include "XDefine.hpp"

class CXBase
{
public:
	virtual CString GetDescription() = 0;
	virtual CString GetMyName() = 0;
};

//////////////////////////////////////////////////////////////////////////

#define XClass \
virtual CString GetDescription(); \
virtual CString GetMyName();

#define Description_Begin(_className) \
CString _className :: GetMyName() \
{ \
	return CString(_CRT_WIDE(#_className)); \
} \
CString _className :: GetDescription() \
{ \
	CString description; \
	description.AppendFormat(_T("This is class %s.\n"),CString(_CRT_WIDE(#_className)));

#define Description_End \
	return description; \
}