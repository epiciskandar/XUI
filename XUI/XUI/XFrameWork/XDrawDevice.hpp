#pragma once
#include "XBase.hpp"
#include "XUtil.hpp"

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
	CGDIHandleSwitcher(HDC dc,HGDIOBJ GDIObj,BOOL autoDelete=TRUE,BOOL select=TRUE)
		: m_dc(dc),m_oldGDIObj(0),m_autoDelete(autoDelete)
	{
		if (select)
		{
			m_oldGDIObj = SelectObject(dc,GDIObj);
		}
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

class CGDIMemDC : public CMemoryDC , public Util::Class::CRefCountImpl
{
public:
	CGDIMemDC(HDC hDC, RECT& rcPaint):CMemoryDC(hDC,rcPaint)
	{}

	VOID ClearDrawDevice()
	{
		HBRUSH brush = GetStockBrush(NULL_BRUSH);
		SelectBrush(brush);
		m_switcher.push_back(CGDIHandleSwitcher
			(m_hDC,brush,TRUE,FALSE));

		HPEN pen = GetStockPen(NULL_PEN);
		SelectPen(pen);
		m_switcher.push_back(CGDIHandleSwitcher
			(m_hDC,pen,TRUE,FALSE));
		SetBkMode(TRANSPARENT);
	}
protected:
	std::list<CGDIHandleSwitcher> m_switcher;
};
