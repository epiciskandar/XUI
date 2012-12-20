#pragma once
#include "XBase.hpp"

class CXElement : public CXBase
{
	XClass;

	XProperty_Begin
		XProperty(ID,CString)
	XProperty_End;

public:
	CXElement();
	~CXElement();
public:
	virtual XResult OnPaint();
};

MyNameIs(CXElement)
End_Description;

//////////////////////////////////////////////////////////////////////////