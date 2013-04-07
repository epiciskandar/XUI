#pragma once
#include "../XElement.h"
#include "../XResPool.hpp"
#pragma warning(push)
#pragma warning(disable:4996)
#include "../../CXImage/CxImage/ximage.h"
#pragma warning(pop)

class CXImage : public CXElement
{
	XClass;

	XProperty_Begin
		XProperty(File)
        XProperty(Offset)
	XProperty_End;

	virtual XResult SetXMLProperty( CString name,CString value );

	virtual XResult ProcessXMessage(CXMsg& msg);

protected:
	CxImage m_img;

public:
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
};

//////////////////////////////////////////////////////////////////////////

inline VOID CXImage::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	CRect rect;
	GetRect(rect);
    CPoint pt;
    GetOffset (pt);

	if (msg.drawDevice.IsRectNeedRePaint(rect))
	{
        int x = m_img.GetWidth(), y = m_img.GetHeight();
        if (pt.x <= x && pt.x > 0 && pt.y <= y && pt.y >0)
        {
            m_img.Draw(msg.drawDevice.dc,-pt.x,-pt.y,-1,-1,rect);
        }
        else
        {
            m_img.Draw(msg.drawDevice.dc,0,0,-1,-1,rect);
        }        

		_SendXMsg(msg);
	}
	msg.msgHandled = TRUE;
}

inline XResult CXImage::ProcessXMessage( CXMsg& msg )
{
	__super::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_Paint);
		OnXMsg(CXMsg_PropertyChanged);
	END_XMSG_MAP;

	return XResult_OK;
}

inline XResult CXImage::SetXMLProperty( CString name,CString value )
{
	__super::SetXMLProperty(name,value);

	XMLConvert_Begin(name,value)
		XMLConvert(File)
        XMLConvert(Offset)
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