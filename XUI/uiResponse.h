#pragma once
#include "XUI/XUI.h"

class CXUIListenerRegister
{
public:
	XResult OnCreateElement(NodeRef node)
	{
		if (node->GetID() == _T("Close"))
		{
			DWORD id;
			ElementRef(node)->Listen(XEar(
				std::bind(&CXUIListenerRegister::Whisper,this,std::placeholders::_1)),id);
		}
		return XResult_OK;
	}

	XResult Whisper(CXMsg& msg)
	{URP(msg);
		return XResult_OK;
	}
};