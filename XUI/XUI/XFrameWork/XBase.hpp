#pragma once
#include "XDefine.hpp"

#include <Windows.h>
#include <WindowsX.h>
#include <atlstr.h>
#include <atldef.h>
#include <list>
#include <set>

#define _WTL_NO_CSTRING
#include "../WTL/atlapp.h"
#include "../WTL/atlwinx.h"		// reflection
#include "../WTL/atlframe.h"
#include "../WTL/atlcrack.h"	// message handle
#include "../WTL/atlmisc.h"
#include "Others/CFile.hpp"

class CXBase
{
public:
	virtual CString GetDescription() = 0;
	static CString GetMyClassName();
};

//////////////////////////////////////////////////////////////////////////

#define XClass(_baseclass) \
public: \
	virtual CString GetDescription(); \
	static CString GetMyClassName(); \
	typedef _baseclass BaseClass;

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

#define I_Can(_use) description.AppendFormat(_T("I can %s.\n"),_CRT_WIDE(#_use));
#define I_Provide(_function) description.AppendFormat(_T("I provide functions like %s\n"),_CRT_WIDE(#_function));
#define And_You_Should_Notice(_instruction) description.AppendFormat(_T("%s\n"),_CRT_WIDE(#_instruction));
	
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

template <class T>
class XSmartPtr
{
public:
	typedef T*			PointerType;
	typedef T&			RefType;

	XSmartPtr() : m_ptr(NULL){};
	XSmartPtr(const PointerType rhs) : m_ptr(rhs){if(m_ptr){m_ptr->AddRef();}};

	XSmartPtr(const XSmartPtr& rhs)
	{
		m_ptr = dynamic_cast<const PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
	}

	template <typename T2>
	XSmartPtr(const XSmartPtr<T2>& rhs)
	{
		m_ptr = dynamic_cast<const PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
	}
	virtual ~XSmartPtr(){if(m_ptr){m_ptr->Release();}}

	XSmartPtr operator=(const XSmartPtr& rhs)
	{
		if (m_ptr)
		{
			m_ptr->Release();
		}
		m_ptr = dynamic_cast<PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
		return *this;
	}

	template <typename T2>
	XSmartPtr operator=(const XSmartPtr<T2>& rhs)
	{
		if (m_ptr)
		{
			m_ptr->Release();
		}
		m_ptr = dynamic_cast<PointerType>(rhs.GetPointer());
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
		return *this;
	}

	operator bool() const
	{
		return (m_ptr==NULL)? false: true;
	}
	bool operator==(const XSmartPtr& rhs)
	{
		return GetPointer() == rhs.GetPointer();
	}
	const PointerType operator ->() const
	{
		return m_ptr;
	}
	PointerType operator ->()
	{
		return m_ptr;
	}

	PointerType GetPointer() const
	{
		return m_ptr;
	}

protected:
	T* m_ptr;
};

#define XPtr XSmartPtr
typedef XSmartPtr<CXBase> BaseRef;

//////////////////////////////////////////////////////////////////////////

#include "XUtil.hpp"
