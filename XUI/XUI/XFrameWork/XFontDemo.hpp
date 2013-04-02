#pragma once
#include "XBase.hpp"
#include "XPropertyValue.hpp"

// 属性类应当有SetXMLProperty接口，以实现xml的读取功能
class CXFontDemo : public CFont
	, public CXProperter
{
	XClass(CFont);
public:
	XResult SetXMLProperty(CString key,CString value) override
	{
		if (key == _T("FontName"))
		{
			// setfont
			value = value;
		}
		return XResult_OK;
	}
};

class FontDemoXMLConverter{};

MyNameIs(CXFontDemo)
	I_Can("demo")
End_Description;