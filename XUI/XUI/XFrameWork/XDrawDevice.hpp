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
