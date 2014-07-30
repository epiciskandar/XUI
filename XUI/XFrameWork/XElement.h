#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XNotifier.hpp"
#include "XMsg.hpp"
#include "Header/IXProperty.h"

class CXElement 
	: public IXElement
	, public CXTreeNode
	, public CXNotifier
	, public IXMLPropertyParsable
{
	RefCountImplAt(CXTreeNode);
public:
	CXElement();
	~CXElement();

	XFakeProperty(Position);
	XFakeProperty(Size);
	XProperty(Rect);
	XProperty(Invalid);
	XProperty(SizeLimit);
	XProperty(LayoutInvalid);
	XProperty(Padding);
	XProperty(Margin);
	XProperty(AutoWidth);
	XProperty(AutoHeight);
	XProperty(ExpandWidth);
	XProperty(ExpandHeight);
	XProperty(Color);
	XProperty(BorderColor);
	XProperty(BorderWidth);
	XProperty(HitTest);
	XProperty(ToolTip);
	XProperty(Ghost);
	XProperty(XFont);
	XProperty(NeedRealPaint);
	virtual XResult GetLayoutType(Property::ELayoutType& type);
	virtual XResult SetLayoutType(Property::ELayoutType type);
	virtual XResult GetLayoutDirection(Property::ELayoutDirection& direction);
	virtual XResult SetLayoutDirection(Property::ELayoutDirection direction);
	virtual XResult GetAlign(Property::EAlignType& type);
	virtual XResult SetAlign(Property::EAlignType type);
	//////////////////////////////////////////////////////////////////////////
	// �Ű�ר�����ԣ�����¶�����.
	XFakeProperty(InnerLayoutRect);
	XFakeProperty(OuterLayoutRect);
	VOID CheckSizeLimit(CSize& size);
	VOID CheckOuterSizeLimit(CSize& size);
	VOID CheckInnerSizeLimit(CSize& size);
	//////////////////////////////////////////////////////////////////////////


	virtual XResult Listen(XEar ear,DWORD& earID){return CXNotifier::Listen(ear,earID);};
	virtual XResult StopListen(DWORD earID){return CXNotifier::StopListen(earID);};
	virtual XResult Whisper(IXMsg& msg){return CXNotifier::Whisper(msg);};

	// XMsg�Ľ�����ں���
	virtual XResult ProcessXMessage(IXMsg& msg);
	virtual VOID MsgDown(IXMsg& msg);
	virtual VOID MsgUp(IXMsg& msg);

	XResult SetXMLProperty( LPCTSTR name,LPCTSTR value ) override;

	void GetProperty(IXProperty*& prop) { prop = m_property; }
	void SetProperty(IXProperty* prop) { m_property = prop; }

	// �������
	virtual VOID GetRectInClientCoord(CRect& rect);
	virtual VOID Render();
	virtual VOID Paint(CXMsg_Paint& arg);
	virtual BOOL PaintCheck(CRect invalidRect, CRect& paintDstRect, CPoint& paintSrcPt);

protected:
	VOID On_CXMsg_SizeChanged(CXMsg_SizeChanged& arg);
	VOID On_CXMsg_Layout(CXMsg_Layout& arg);
	VOID On_CXMsg_Paint(CXMsg_Paint& arg);
	VOID On_CXMsg_MouseEnter(CXMsg_MouseEnter& arg);
	VOID On_CXMsg_MouseLeave(CXMsg_MouseLeave& arg);
	VOID On_CXMsg_AttachDC(CXMsg_AttachDC& arg);
	VOID On_CXMsg_FrameClick(CXMsg_FrameClick& arg);
	VOID On_CXMsg_RealWndClosing(CXMsg_RealWndClosing& arg);
protected:
	IXProperty*	m_property=nullptr;		// ���Լ��ϣ��������Զ���������
	BOOL	m_isLayouting = FALSE;			// ���Ű�����в�������С�ı���¼�
	CToolTipCtrl	m_toolTip;
	XPtr<CGDIMemDC>	m_memDC;
};

typedef XSmartPtr<CXElement> ElementRef;

#include "XElementUtil.hpp"