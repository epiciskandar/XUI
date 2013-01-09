#pragma once
#include "XBase.hpp"

class CXDrawDevice
{
public:
	CRect invalidRect;
	CDCHandle dc;
	BOOL IsRectNeedRePaint(const CRect& elementRect)
	{
		CRect testRect;
		testRect.IntersectRect(invalidRect,elementRect);
		return !testRect.IsRectEmpty();
	}
	CXDrawDevice& operator=(const CXDrawDevice& rhs)
	{
		dc = rhs.dc;
		invalidRect = rhs.invalidRect;
		return *this;
	}
};

class CGDIHandleSwitcher
{
public:
	CGDIHandleSwitcher(HDC dc,HGDIOBJ GDIObj,BOOL autoDelete=TRUE)
		: m_dc(dc),m_oldGDIObj(0),m_autoDelete(autoDelete)
	{
		m_oldGDIObj = SelectObject(dc,GDIObj);
	}
	~CGDIHandleSwitcher()
	{
		HGDIOBJ hObj = SelectObject(m_dc,m_oldGDIObj);
		if (m_autoDelete)
		{
			DeleteObject(hObj);
		}
	}
protected:
	HDC		m_dc;
	HGDIOBJ	m_oldGDIObj;
	BOOL	m_autoDelete;
};
