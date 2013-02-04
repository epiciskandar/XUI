#pragma once
#include "XBase.hpp"
#include "XTree.hpp"
#include "XMsg.hpp"
#include "XProperty.hpp"

//////////////////////////////////////////////////////////////////////////

// define property functions
#define XProperty_Begin
#define XProperty_Get(_name) \
	virtual XResult Get##_name (Property::_name##Type& value) \
	{ \
		return m_property.GetProperty(L#_name,value); \
	}
#define XProperty_Set(_name) \
public: \
	virtual XResult Set##_name (Property::_name##Type param) \
	{ \
		XResult result = m_property.SetProperty(L#_name,param); \
		CXMsg_PropertyChanged msg; \
		msg.name = L#_name; \
		ProcessXMessage(msg); \
		return  result;\
	}
#define XProperty(_name) \
	XProperty_Get(_name) \
	XProperty_Set(_name)

#define XFakeProperty(_name) \
	virtual XResult Get##_name (Property::_name##Type& value); \
	virtual XResult Set##_name (Property::_name##Type param);

#define XProperty_End

// define XML converters
#define XMLConvert_Begin(_name,_value) \
{ \
	CString& propName = _name; \
	CString& propVaule = _value;
#define XMLConvert(_name) \
	if (Property::_name == propName) \
	{ \
		m_property.SetProperty(propName,Property::_name##XMLConverter::ConvertToValue(propVaule)); \
		CXMsg_PropertyChanged msg; \
		msg.name = propName; \
		ProcessXMessage(msg); \
	}else
#define XMLFakeConvert(_name) \
	if (Property::_name == propName) \
	{ \
		return Set##_name(Property::_name##XMLConverter::ConvertToValue(propVaule)); \
	}else
#define XMLConvert_End	{} \
}

// define XMsg router
#define BEGIN_XMSG_MAP(_msg) \
{ \
	CXMsg_GetListenList* pListMsg = dynamic_cast<CXMsg_GetListenList*>(&_msg);
#define OnXMsg(_msg) \
	if (pListMsg) \
	{ \
		pListMsg->XMsgList.push_back(_msg::GetXMsgName()); \
	} \
	else if( msg.GetMyMsgName().CompareNoCase(_msg::GetXMsgName()) == 0) \
	{ \
		_msg* pDeriMsg = dynamic_cast<_msg*>(&msg); \
		ATLASSERT(pDeriMsg && "invalid XMessage response!!!!!!!"); \
		On_##_msg(*pDeriMsg); \
		if(msg.msgHandled) \
		return XResult_Handled; \
	}
#define END_XMSG_MAP \
}

//////////////////////////////////////////////////////////////////////////

class CXElement : public CXBase
	, public CXTreeNode
{
	XClass(CXBase);
public:
	CXElement();
	~CXElement();

	XProperty_Begin
		XProperty(Rect)
		XFakeProperty(Position)
		XFakeProperty(Size)
	XProperty_End;

	// XMsg的接收入口函数
	virtual XResult ProcessXMessage(CXMsg& msg);

	// 对string类型的属性值的解析支持，用于XML的实例化，可通过该函数知道XML支持什么属性
	virtual XResult SetXMLProperty(CString name,CString value);

	CXProperty& GetPrpertyRef()	{return m_property;};

protected:
	VOID _SendXMessageToChildren(CXMsg& pMsg);

	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
protected:
	CXProperty m_property;
};

typedef XSmartPtr<CXElement> ElementRef;

MyNameIs(CXElement)
	I_Provide("可绘制元素的基本属性及X消息路由,对xml创建ui元素做了属性上的支持")
End_Description;

//////////////////////////////////////////////////////////////////////////

CXElement::CXElement()
{
}

CXElement::~CXElement()
{
}

XResult CXElement::SetXMLProperty( CString name,CString value )
{
	XMLConvert_Begin(name,value)
		XMLConvert(Rect)
		XMLFakeConvert(Position)
		XMLFakeConvert(Size)
	XMLConvert_End

	return XResult_NotSupport;
}

VOID CXElement::_SendXMessageToChildren( CXMsg& pMsg )
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
		XSmartPtr<CXElement> pElement = TransformNode<XSmartPtr<CXElement>>(*i);
		pElement->ProcessXMessage( pMsg );
	}
}

VOID CXElement::On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg)
{
	if (arg.name = Property::Size)
	{
		CXMsg_SizeChanged msg;
		msg.node = this;
		msg.sizeType = SizeType_Restored;

		ProcessXMessage(msg);

		msg.msgHandled = TRUE;
	}
	return;
}

XResult CXElement::ProcessXMessage( CXMsg& msg )
{
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged)
	END_XMSG_MAP;
	if (!msg.msgHandled)
	{
		_SendXMessageToChildren(msg);
	}
	return XResult_OK;
}

//////////////////////////////////////////////////////////////////////////

XResult CXElement::GetPosition(Property::PositionType& value)
{
	CRect rect;
	GetRect(rect);
	value = rect.TopLeft();
	return XResult_OK;
}

XResult CXElement::SetPosition(Property::PositionType param)
{
	CRect rect;
	GetRect(rect);
	rect.TopLeft() = param;
	return SetRect(rect);
}

XResult CXElement::GetSize(Property::SizeType& value)
{
	CRect rect;
	GetRect(rect);
	value = rect.BottomRight();
	return XResult_OK;
}

XResult CXElement::SetSize(Property::SizeType param)
{
	CRect rect;
	GetRect(rect);
	rect.BottomRight() = param;
	return SetRect(rect);
}
