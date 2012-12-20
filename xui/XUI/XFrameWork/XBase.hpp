#pragma once
#include "XDefine.hpp"

#include <Windows.h>
#include <WindowsX.h>
#include <atlstr.h>
#include <atldef.h>
#include <list>

#define _WTL_NO_CSTRING
#include "../WTL/atlapp.h"
#include "../WTL/atlwinx.h"		// reflection
#include "../WTL/atlframe.h"
#include "../WTL/atlcrack.h"	// message handle
#include "../WTL/atlmisc.h"

class CXBase
{
public:
	virtual CString GetDescription() = 0;
	virtual CString GetMyClassName() = 0;
};

class CXTreeNode
{
public:
	XResult	AddChild(CXTreeNode* pChild);
	XResult RemoveChild(CXTreeNode* pChild);
	CXTreeNode* GetFather();
	XResult RIPMySelf();
protected:
	CXTreeNode* m_father;
	std::list<CXTreeNode*> m_children;
};

template<class T>
T* GetXChildren(CXBase& ancestor,BOOL recursion = FALSE);

template<class T>
T* GetXClass(CXBase* base){return dynamic_cast<T>(base);};
//////////////////////////////////////////////////////////////////////////

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)	\
	TypeName(const TypeName&);				\
	void operator=(const TypeName&)
#endif

#define XClass \
public: \
	virtual CString GetDescription(); \
	virtual CString GetMyClassName();

#define MyNameIs(_className) \
CString _className :: GetMyClassName() \
{ \
	return CString(_CRT_WIDE(#_className)); \
} \
CString _className :: GetDescription() \
{ \
	CString description; \
	description.AppendFormat(_T("This is class %s.\n"),CString(_CRT_WIDE(#_className)));

#define End_Description \
	return description; \
}

#define I_Can(_use) description.AppendFormat(_T("It can %s.\n"),_CRT_WIDE(#_use));

//////////////////////////////////////////////////////////////////////////

#define XProperty_Begin
#define XProperty(_name,_paramtype) \
public: \
	virtual XResult Set##_name (_paramtype param) ; \
	virtual _paramtype Get##_name (); \
protected: \
	_paramtype m_##_name;
#define XProperty_End