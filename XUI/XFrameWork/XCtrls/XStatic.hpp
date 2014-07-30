#pragma once
#include "../XElement.h"
#include "XFont.hpp"

class CXText : public CXElement , virtual public IXText
{
	RefCountImplAt(CXElement);
public:
	XProperty(Text);
	XProperty(TextColor);
	XProperty(XFont);

	virtual XResult ProcessXMessage(IXMsg& msg) override;

public:
	virtual VOID Render();
};

//////////////////////////////////////////////////////////////////////////

inline VOID CXText::Render()
{
	CXElement::Render();

	CRect rect;
	GetRect(rect);
	CString text;
	GetText(text);

	COLORREF color;
	GetColor(color);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, color);

	XNodeRef fontNode;
	GetXFont(fontNode);
	XPtr<CXFont> font(fontNode);
	if (font && font->GetFontName() != _T(""))
	{
		/* ÐÞ¸Ä×ÖÌå */
		font->ChangeWork();
		m_memDC->SelectFont(font->m_hFont);
	}


	CGDIHandleSwitcher switcher(*m_memDC, pen, FALSE);
	DRAWTEXTPARAMS params;
	ZeroMemory(&params, sizeof(params));
	params.cbSize = sizeof(params);
	m_memDC->DrawTextEx(
		text.GetBuffer(text.GetLength()),
		text.GetLength(),
		rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER,
		&params);
	text.ReleaseBuffer();
}

inline XResult CXText::ProcessXMessage( IXMsg& msg )
{
	__super::ProcessXMessage(msg);
	return XResult_OK;
}
