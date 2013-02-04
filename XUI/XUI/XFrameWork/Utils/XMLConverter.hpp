#pragma once

class CXProperty;

class CXMLConverterBase
{
public:
	CXProperty& m_prop;
public:
	CXMLConverterBase(CXProperty& prop) : m_prop(prop){}
	virtual XResult Convert(CString name,CString value)=0;
};

class CXMLConverter_CString : CXMLConverterBase
{
public:
	CXMLConverter_CString(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		return m_prop.SetProperty(name , value);
	}
};

class CXMLConverter_INT : CXMLConverterBase
{
public:
	CXMLConverter_INT(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		INT intValue;
		intValue = _ttoi(value);
		return m_prop.SetProperty(name , intValue);
	}
};

class CXMLConverter_DWORD : CXMLConverterBase
{
public:
	CXMLConverter_DWORD(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		DWORD dwValue;
		_stscanf_s(value,_T("%u"),&dwValue);
		return m_prop.SetProperty(name , dwValue);
	}
};

typedef CXMLConverter_DWORD CXMLConverter_HWND;

class CXMLConverter_COLORREF : CXMLConverterBase
{
public:
	CXMLConverter_COLORREF(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		return m_prop.SetProperty(name,value);
	}
};

class CXMLConverter_BOOL : CXMLConverterBase
{
public:
	CXMLConverter_BOOL(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		INT intValue;
		intValue = _ttoi(value);
		return m_prop.SetProperty(name , (intValue==0? FALSE: TRUE));
	}
};

class CXMLConverter_CRect : CXMLConverterBase
{
public:
	CXMLConverter_CRect(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		CRect rect;
		_stscanf_s(value,_T("%d,%d,%d,%d"),&rect.left,&rect.top,&rect.right,&rect.bottom);

		return m_prop.SetProperty(name ,rect);
	}
};

class CXMLConverter_CPoint : CXMLConverterBase
{
public:
	CXMLConverter_CPoint(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		CPoint point;
		_stscanf_s(value,_T("%d,%d"),&point.x,&point.y);
		return m_prop.SetProperty(name ,point);
	}
};

class CXMLConverter_CSize : CXMLConverterBase
{
public:
	CXMLConverter_CSize(CXProperty& prop) : CXMLConverterBase(prop){}
	XResult Convert(CString name,CString value)
	{
		CSize size;
		_stscanf_s(value,_T("%d,%d"),&size.cx,&size.cy);
		return m_prop.SetProperty(name ,size);
	}
};
