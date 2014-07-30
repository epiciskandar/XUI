#pragma once
#include "XPropertyDefine.h"
#include "XElementDef.h"

class IXProperty
{
public:
#define SupportType(_type) \
	virtual XResult SetProperty(LPCTSTR key, const _type& value) = 0; \
	virtual XResult GetProperty(LPCTSTR key, _type& value) const = 0;

	SupportType(CString);
	SupportType(DWORD);
	SupportType(INT);
	SupportType(CRect);
	SupportType(CPoint);
	SupportType(CSize);
	SupportType(HWND);
	SupportType(XNodeRef);

#undef SupportType

	virtual bool IsChanged(LPCTSTR key) = 0;
	virtual void ChangeRestore(LPCTSTR key/*empty means all*/) = 0;
};

class IXMLPropertyParsable
{
public:
	virtual XResult SetXMLProperty(LPCTSTR name, LPCTSTR value) = 0;
};
