#pragma once
#include "XFrameWork/XBase.hpp"

class CXFont : public CFont
	, public CXTreeNode
	, public IXMLPropertyParsable
{
public:
    CString GetFontName() {return _fontName;}
    INT GetFontSize() {return _fontSize;}
    void SetFont(CString fontName, INT fontSize);
	XResult SetXMLProperty(LPCTSTR name, LPCTSTR value) override;
    void ChangeWork();
    CXFont& operator=(const CXFont& rhs)
    {
        _fontName = rhs._fontName;
        _fontSize = rhs._fontSize;
        return *this;
    }
private:
    CString _fontName;
    INT _fontSize;
    HFONT _hFont;
};


inline void CXFont::ChangeWork()
{
    if (!_hFont)
    {
        _hFont = CreatePointFont(_fontSize*10, _fontName);
    }    
}

inline XResult CXFont::SetXMLProperty(LPCTSTR name, LPCTSTR value)//Set Class Property
{
	CString strName(name);
	if (strName == _T("ID"))
	{
		SetID(value);
	}
	if (strName == _T("FontName"))
    {
        _fontName = value;
    }
	else if (strName == _T("FontSize"))
    {
        _fontSize = StrToInt(value);
    }
    return XResult_OK;
}
