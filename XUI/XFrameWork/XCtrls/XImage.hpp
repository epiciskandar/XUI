#pragma once
#include "../XElement.h"
#include "../XResPool.hpp"
#pragma warning(push)
#pragma warning(disable:4996)
#include "../../CXImage/CxImage/ximage.h"
#pragma warning(pop)

class CXImage : public CXElement , virtual public IXImage
{
	RefCountImplAt(CXElement);

	XFakeProperty_Set(File);
	XProperty_Get(File);
	XProperty(Offset);

	virtual XResult ProcessXMessage(IXMsg& msg) override;

protected:
	CxImage m_img;

public:
	VOID On_CXMsg_Paint(CXMsg_Paint& msg);
};

//////////////////////////////////////////////////////////////////////////

inline VOID CXImage::On_CXMsg_Paint(CXMsg_Paint& msg)
{
	__super::On_CXMsg_Paint(msg);
	CRect rect;
	GetRectInClientCoord(rect);
	CPoint imgOffset;
	GetOffset (imgOffset);

	if (msg.drawDevice.IsRectNeedRePaint(rect))
	{
		int imgWidth = m_img.GetWidth(), imgHeight = m_img.GetHeight();
		if (imgOffset.x <= imgWidth && imgOffset.x > 0 
			&& imgOffset.y <= imgHeight && imgOffset.y >0)
		{
			m_img.Draw(msg.drawDevice.dc,-imgOffset.x,-imgOffset.y,-1,-1,rect);
		}
		else
		{
			m_img.Draw(msg.drawDevice.dc,0,0,-1,-1,rect);
		}
	}
	msg.msgHandled = TRUE;
}

inline XResult CXImage::ProcessXMessage( IXMsg& msg )
{
	__super::ProcessXMessage(msg);
	
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_Paint);
	END_XMSG_MAP;

	return XResult_OK;
}

inline XResult CXImage::SetFile(Property::FileType param)
{
	m_property->SetProperty(Property::File, param);
	CString filePath(param);
	CXResPool::GetInstance().TranslateResPath(filePath);
	m_img.Load(filePath);

	CSize size;
	XResult result = GetSize(size);
	if (result == XResult_NotFound)
	{
		UINT width = m_img.GetWidth();
		UINT height = m_img.GetHeight();
		size.cx = width;
		size.cy = height;
		SetSize(size);
	}
	return XResult_OK;
}