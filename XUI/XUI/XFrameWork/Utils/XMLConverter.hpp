#pragma once

class XMLConverter
{
public:
	CXProperty& m_prop;
public:
	XMLConverter(CXProperty& prop) : m_prop(prop){}
	virtual XResult Convert(CString name,CString value)=0;
};

class XMLConverter_CString : XMLConverter
{
public:
	XMLConverter_CString(CXProperty& prop) : XMLConverter(prop){}
	XResult Convert(CString name,CString value)
	{
		return m_prop.SetProperty(name , value);
	}
};

class XMLConverter_INT : XMLConverter
{
public:
	XMLConverter_INT(CXProperty& prop) : XMLConverter(prop){}
	XResult Convert(CString name,CString value)
	{
		INT intValue;
		intValue = _ttoi(value);
		return m_prop.SetProperty(name , intValue);
	}
};

class XMLConverter_DWORD : XMLConverter
{
public:
	XMLConverter_DWORD(CXProperty& prop) : XMLConverter(prop){}
	XResult Convert(CString name,CString value)
	{
		DWORD dwValue;
		_stscanf_s(value,_T("%u"),&dwValue);
		return m_prop.SetProperty(name , dwValue);
	}
};

class XMLConverter_BOOL : XMLConverter
{
public:
	XMLConverter_BOOL(CXProperty& prop) : XMLConverter(prop){}
	XResult Convert(CString name,CString value)
	{
		INT intValue;
		intValue = _ttoi(value);
		return m_prop.SetProperty(name , (intValue==0? FALSE: TRUE));
	}
};

class XMLConverter_CRect : XMLConverter
{
public:
	XMLConverter_CRect(CXProperty& prop) : XMLConverter(prop){}
	XResult Convert(CString name,CString value)
	{
		CRect rect;
		_stscanf_s(value,_T("%d,%d,%d,%d"),&rect.left,&rect.top,&rect.right,&rect.bottom);

		return m_prop.SetProperty(name ,rect);
	}
};

class XMLConverter_CPoint : XMLConverter
{
public:
	XMLConverter_CPoint(CXProperty& prop) : XMLConverter(prop){}
	XResult Convert(CString name,CString value)
	{
		CPoint point;
		_stscanf_s(value,_T("%d,%d"),&point.x,&point.y);
		return m_prop.SetProperty(name ,point);
	}
};