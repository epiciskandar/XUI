#pragma once
#include "../XBase.hpp"
#include "../XPropertyValue.hpp"
class CXFont : public CFont
    ,public CXProperter
{
    XClass;
public:
    CString GetFontName() {return _fontName;}
    INT GetFontSize() {return _fontSize;}
    void SetFont(CString fontName, INT fontSize);
    XResult SetXMLProperty( CString name,CString value ) override;
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
};


inline void CXFont::ChangeWork()
{
    CreatePointFont(_fontSize*10, _fontName);
}

inline XResult CXFont::SetXMLProperty( CString name,CString value )//Set Class Property
{
	if (name == _T("ID"))
	{
		SetID(value);
	}
    if (name == _T("FontName"))
    {
        _fontName = value;
    }
    else if (name == _T("FontSize"))
    {
        _fontSize = StrToInt(value);
    }
    return XResult_OK;
}
