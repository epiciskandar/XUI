#pragma once
#include "XTree.hpp"
#include "XDrawDevice.hpp"
#include "Header/IXMsg.h"

class CXMsg : public IXMsg
{
	XMessage(CXMsg);
#ifdef XUI_TRACEMSG
	~CXMsg()
	{
		if (!processStep.empty())
		{
			blog::CBLog& logger = blog::CBLog::GetInstance();
			logger.Logf(DeviceMask_All,_T("-------Msg : %s begin\n"),msgName);
			logger.IncreaseIndent();
			for (auto& i:processStep)
			{
				logger.Log(DeviceMask_All,(LPCTSTR)i);
			}
			logger.DecreaseIndent();
			logger.Logf(DeviceMask_All,_T("========Msg : %s end\n"),msgName);
		}
	}
#endif
};

//////////////////////////////////////////////////////////////////////////

class CXMsg_PropertyChanged : public CXMsg
{
	XMessage(CXMsg_PropertyChanged);

	CString name;
};

//////////////////////////////////////////////////////////////////////////

class CXMsg_Paint : public CXMsg
{
	XMessage(CXMsg_Paint);

	CXDrawDevice	drawDevice;
	CPoint			offsetFix;
};

class CXMsg_Invalidate : public CXMsg
{
	XMessage(CXMsg_Invalidate);

	CRect	invalidRect;
};

class CXMsg_RenderElement : public CXMsg
{
	XMessage(CXMsg_RenderElement);

	BOOL	paintChildren;

	CXMsg_RenderElement():paintChildren(FALSE){}
};

class CXMsg_AttachDC : public CXMsg
{
	XMessage(CXMsg_AttachDC);

	HWND hostWnd;
	HDC  hostDC;

	CXMsg_AttachDC():hostWnd(0),hostDC(0){}
};

class CXMsg_AppendElement : public CXMsg
{
	XMessage(CXMsg_AppendElement);

	XNodeRef element;
};

class CXMsg_SizeChanged : public CXMsg
{
	XMessage(CXMsg_SizeChanged);

	XNodeRef node;
	ESizeType sizeType;

	CXMsg_SizeChanged():sizeType(SizeType_Restored){}
};

class CXMsg_Layout : public CXMsg
{
	XMessage(CXMsg_Layout);
};

class CXMsg_MouseMove : public CXMsg
{
	XMessage(CXMsg_MouseMove);

	CPoint pt;
};

class CXMsg_MouseEnter : public CXMsg_MouseMove
{
	XMessage(CXMsg_MouseEnter);

	XNodeRef prevFocusNode;
};

class CXMsg_MouseLeave : public CXMsg_MouseMove
{
	XMessage(CXMsg_MouseLeave);
	XNodeRef newFocusNode;
};

class CXMsg_OnCtlColor : public CXMsg
{
	XMessage(CXMsg_OnCtlColor);
	HDC hDC;
	HWND hWnd;

	HGDIOBJ hBrush;
};

//////////////////////////////////////////////////////////////////////////

#undef XMessage
