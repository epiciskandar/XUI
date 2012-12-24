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
	static CString GetMyClassName();
};

//////////////////////////////////////////////////////////////////////////

#define XClass \
public: \
	virtual CString GetDescription(); \
	static CString GetMyClassName();

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

#define XProperty_Begin
#define XProperty_Interface(_name,_paramtype)\
public: \
	virtual XResult Set##_name (_paramtype param) ; \
	virtual _paramtype Get##_name (); \
protected:
#define XProperty(_name,_paramtype) \
	XProperty_Interface(_name,_paramtype) \
	_paramtype m_##_name;
#define XProperty_End

//////////////////////////////////////////////////////////////////////////

#define On_XMessage(_msg) \
if (uMsg == WM_USER_XUIMSG) \
{ \
	CXMsg* pMsg = (CXMsg*)(void*)wParam; \
	if(pMsg->GetMyMsgName().CompareNoCase(_msg::GetXMsgName()) == 0) \
	{ \
		_msg* pDeriMsg = dynamic_cast<_msg*>(pMsg); \
		ATLASSERT(pDeriMsg && "invalid XMessage response!!!!!!!"); \
		lResult = On_##_msg(pDeriMsg); \
		if(IsMsgHandled()) \
		return TRUE; \
	} \
}

//////////////////////////////////////////////////////////////////////////

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)	\
private: \
	TypeName(const TypeName&); \
	void operator=(const TypeName&);
#endif

#ifndef Singleton
#define Singleton(ClassName) \
DISALLOW_COPY_AND_ASSIGN(ClassName) \
private: \
	ClassName();
#endif

#ifndef RefCountImpl
#define RefCountImpl \
virtual ULONG STDMETHODCALLTYPE AddRef() \
{ \
	return InterlockedIncrement(&m_refCount); \
} \
virtual ULONG STDMETHODCALLTYPE Release() \
{ \
	unsigned long ul = 0; \
	if ((ul = InterlockedDecrement(&m_refCount)) == 0) \
	{ \
		delete this; \
	} \
	return ul; \
} \
unsigned long m_refCount;
#endif

//////////////////////////////////////////////////////////////////////////

template <class T>
class XSmartPtr
{
public:
	typedef T*			PointerType;
	typedef T* const	ConstPointerType;
	typedef const T&	ConstRefType;

	XSmartPtr() : m_ptr(NULL){};
	XSmartPtr(PointerType const rhs) : m_ptr(rhs){if(m_ptr){m_ptr->AddRef();}};
	XSmartPtr(const XSmartPtr& rhs) : m_ptr(NULL)
	{
		m_ptr = rhs._GetPointer();
		if (m_ptr)
		{
			m_ptr->AddRef();
		}
	};
	virtual ~XSmartPtr(){if(m_ptr){m_ptr->Release();}}

	operator bool() const
	{
		return (m_ptr==NULL)? false: true;
	}
	ConstPointerType operator ->() const
	{
		return m_ptr;
	}
	PointerType operator ->()
	{
		return m_ptr;
	}
protected:
	PointerType _GetPointer() const
	{
		return m_ptr;
	}

	template <class DestTypeRef,class SrcTypeRef>
	friend DestTypeRef TransformNode(SrcTypeRef& rhs);

protected:
	T* m_ptr;
};

template <class DestTypeRef,class SrcTypeRef>
DestTypeRef TransformNode(SrcTypeRef& rhs)
{
	SrcTypeRef::PointerType ptr = rhs._GetPointer();
	DestTypeRef newref(dynamic_cast<DestTypeRef::PointerType>(ptr));
	return newref;
}