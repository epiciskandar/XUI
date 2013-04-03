#pragma once
#include "../XBase.hpp"
#include "../XPropertyValue.hpp"
class CXFont : public CFont
    ,public CXProperter
{
    XClass(CXFont);
public:
    CXFont()
    {
        return;
    }
    ~CXFont()
    {
        return;
    }
    inline CString GetFontName() {return _fontName;}
    inline INT GetFontSize() {return _fontSize;}
    inline void SetFont(CString fontName, INT fontSize);
    inline XResult SetXMLProperty( CString name,CString value );//Set Class Property
    inline void ChangeWork();
    inline CXFont& operator=(const CXFont& rhs)
    {
        _fontName = rhs._fontName;
        _fontSize = rhs._fontSize;
        return *this;
    }
private:
    CString _fontName;
    INT _fontSize;

    
};
MyNameIs(CXFont)
	I_Can("��������")
End_Description;


inline void CXFont::ChangeWork()
{
    CreatePointFont(_fontSize*10, _fontName);
}

inline XResult CXFont::SetXMLProperty( CString name,CString value )//Set Class Property
{
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