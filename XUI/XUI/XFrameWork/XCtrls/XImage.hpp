#pragma once
#include "../XElement.hpp"
#include "../XResPool.hpp"
#pragma warning(push)
#pragma warning(disable:4996)
#include "../../CXImage/CxImage/ximage.h"
#pragma warning(pop)

class CXImage : public CXElement
{
	XClass(CXElement);

	XProperty_Begin
		XProperty(File)
	XProperty_End;

	virtual XResult SetXMLProperty( CString name,CString value );

	virtual XResult ProcessXMessage(CXMsg& msg);

protected:
	CxImage m_img;

public:
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
};

MyNameIs(CXImage)
	I_Can("image")
End_Description;

//////////////////////////////////////////////////////////////////////////

inline VOID CXImage::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	CRect rect;
	GetRect(rect);
	if (msg.drawDevice.IsRectNeedRePaint(rect))
	{
		m_img.Draw(msg.drawDevice.dc,0,0,-1,-1,rect);

		_SendXMsg(msg);
	}
	msg.msgHandled = TRUE;
}

inline XResult CXImage::ProcessXMessage( CXMsg& msg )
{
	BaseClass::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_Paint);
		OnXMsg(CXMsg_PropertyChanged);
	END_XMSG_MAP;

	return XResult_OK;
}

inline XResult CXImage::SetXMLProperty( CString name,CString value )
{
	BaseClass::SetXMLProperty(name,value);

	XMLConvert_Begin(name,value)
		XMLConvert(File)
	XMLConvert_End

	return XResult_OK;
}

inline VOID CXImage::On_CXMsg_PropertyChanged( CXMsg_PropertyChanged& arg )
{
	if (arg.name == Property::File)
	{
		CString filePath;
		GetFile(filePath);
		CXResPool::GetInstance().TranslateResPath(filePath);
		m_img.Load(filePath);

		CSize size;
		XResult result = GetSize(size);
		if (result == XResult_NotFound)
		{
			UINT width = m_img.GetWidth();
			UINT height = m_img.GetHeight();
			size.cx = width;
			size.cy	= height;
			SetSize(size);
		}
	}
}