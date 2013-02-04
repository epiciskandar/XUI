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

#define XProperty_End

// define XML converters
#define XMLConvert_Begin
#define XMLConvert(_name) \
	m_XMLConverterMap[L# _name] = [this](CString value)->XResult \
	{ \
		Property::_name##XMLConverter converter(m_property); \
		return converter.Convert(L# _name,value); \
	};
#define XMLConvert_End

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
		XProperty(Position)
		XProperty(Size)
	XProperty_End;

	// XMsg的接收入口函数
	virtual XResult ProcessXMessage(CXMsg& msg);

	// 对string类型的属性值的解析支持，用于XML的实例化，可通过该函数注册的类型知道XML支持什么属性
	virtual XResult RegisterXMLSupportProperty();
	virtual XResult SetXMLProperty(CString name,CString value);

	CXProperty& GetPrpertyRef()	{return m_property;};

protected:
	VOID _SendXMessageToChildren(CXMsg& pMsg);

	VOID On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg);
protected:
	CXProperty m_property;
	typedef std::map<CString,std::function<XResult(CString)>> XMLConverterMap;
	XMLConverterMap m_XMLConverterMap;
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

XResult CXElement::RegisterXMLSupportProperty()
{
	XMLConvert(Rect);
	XMLConvert(Position);
	XMLConvert(Size);
	return XResult_OK;
}

VOID CXElement::_SendXMessageToChildren( CXMsg& pMsg )
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
		XSmartPtr<CXElement> pElement = TransformNode<XSmartPtr<CXElement>>(*i);
		pElement->ProcessXMessage( pMsg );
	}
}

XResult CXElement::SetXMLProperty( CString name,CString value )
{
	auto i = m_XMLConverterMap.find(name);
	if (i != m_XMLConverterMap.end())
	{
		XResult result = i->second(value);
		if (XSUCCEEDED(result))
		{
			CXMsg_PropertyChanged msg;
			msg.name = name;
			ProcessXMessage(msg);
		}
		return result;
	}
	return XResult_NotSupport;
}

VOID CXElement::On_CXMsg_PropertyChanged(CXMsg_PropertyChanged& arg)
{
	if (arg.name == Property::Position)
	{
		CPoint pos;
		GetPosition(pos);

		CRect rect;
		GetRect(rect);
		rect.OffsetRect(pos);
		SetRect(rect);
	}
	else if (arg.name == Property::Size)
	{
		CSize size;
		GetSize(size);

		CRect rect;
		GetRect(rect);
		rect.BottomRight().Offset(size);
		SetRect(rect);
	}
	return;
}

XResult CXElement::ProcessXMessage( CXMsg& msg )
{
	BEGIN_XMSG_MAP(msg)
		OnXMsg(CXMsg_PropertyChanged)
	END_XMSG_MAP;
	return XResult_NotHandled;
}
