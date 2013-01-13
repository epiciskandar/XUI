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
#define XProperty(_m_prop,_paramtype,_name) \
public: \
	virtual XResult Set##_name (_paramtype param) \
	{ \
		return _m_prop.SetProperty(L#_name,param); \
	} \
	virtual XResult Get##_name (_paramtype& value) \
	{ \
		return _m_prop.GetProperty(L#_name,value); \
	}
#define XProperty_End

//////////////////////////////////////////////////////////////////////////

#define SupportXMessage		virtual BOOL ProcessXMessage(CXMsg& msg) = 0;

#define BEGIN_XMESSAGE_MAP	virtual BOOL ProcessXMessage(CXMsg& msg) \
{ \
	CXMsg_GetListenList* pListMsg = dynamic_cast<CXMsg_GetListenList*>(&msg);

#define On_XMessage(_msg) \
	if (pListMsg) \
	{ \
		pListMsg->XMsgList.push_back(_msg::GetXMsgName()); \
	} \
	else if( msg.GetMyMsgName().CompareNoCase(_msg::GetXMsgName()) == 0) \
	{ \
		_msg* pDeriMsg = dynamic_cast<_msg*>(&msg); \
		ATLASSERT(pDeriMsg && "invalid XMessage response!!!!!!!"); \
		On_##_msg(*pDeriMsg); \
		if(msg.msgHandled) \
		return TRUE; \
	}

#define END_XMESSAGE_MAP	return pListMsg? TRUE: FALSE; }

#define TranslateToXMessage(_transfunc, ...) \
	{ \
		UINT msgList[] = { __VA_ARGS__ }; \
		static std::set<UINT> handleMsgSet; \
		if (handleMsgSet.empty()) \
			for(UINT n=0; n<_countof(msgList); ++n){handleMsgSet.insert(msgList[n]);} \
		if(handleMsgSet.find(uMsg) != handleMsgSet.end()) \
		{ \
			SetMsgHandled(TRUE); \
			lResult = _transfunc(uMsg, wParam, lParam); \
			if(IsMsgHandled()) \
			return TRUE; \
		} \
	}

#define MessageTranslater(_msg) \
	VOID	Before##_msg(_msg& msg, WPARAM wParam, LPARAM lParam); \
	VOID	End##_msg(_msg& msg);

#define XMsgTranslater(_msg,_xmsg) \
	case _msg: \
	{ \
		_xmsg msg; \
		Before##_xmsg(msg,wParam,lParam); \
		_SendXMessageToChildren(msg); \
		End##_xmsg(msg); \
		lResult = msg.msgRet; \
		SetMsgHandled(msg.msgHandled); \
	} \
	break;
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

#define URP(...)	(__VA_ARGS__);
#define IF_RETURN(_exp,_ret)	{if(_exp){return _ret;}}
#define Return_OnXError(_exp)	{XResult result = _exp;if(XFAILED(result)){return result;}}

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

#define DefineRef(_Class) typedef XSmartPtr<_Class> _Class##Ref;

template <class DestTypeRef,class SrcTypeRef>
DestTypeRef TransformNode(SrcTypeRef& rhs)
{
	SrcTypeRef::PointerType ptr = rhs._GetPointer();
	DestTypeRef newref(dynamic_cast<DestTypeRef::PointerType>(ptr));
	return newref;
}

//////////////////////////////////////////////////////////////////////////

#include "XUtil.hpp"
