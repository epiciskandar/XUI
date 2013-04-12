#pragma once
#include "XSmartPtr.h"
#include <functional>

class IXNode;
typedef XPtr<IXNode> XNodeRef;

class IXNode : public Util::Class::CRefCountImpl
{
public:
	virtual XResult SetID(CString id) = 0;
	virtual CString GetID() const = 0;
	virtual XResult	AppendChild(XNodeRef pChild) = 0;
	virtual XResult InsertAfter(XNodeRef pChild,XNodeRef pAfterMe) = 0;
	virtual XResult InsertBefore(XNodeRef pChild,XNodeRef pAfterMe) = 0;
	virtual XResult GetFirstChild(XNodeRef& pChild) = 0;
	virtual XResult GetChild(CString id,XNodeRef& pChild) = 0;
	virtual XResult SearchChild(CString id,XNodeRef& pChild) = 0;	// search tree
	virtual XResult	SwapNode(XNodeRef pNode1,XNodeRef pNode2) = 0;
	virtual XResult	GetSibling(XNodeRef& pBefore,XNodeRef& pAfter) = 0;
	virtual XResult	GetLSibling(XNodeRef& pSibling) = 0;
	virtual XResult	GetRSibling(XNodeRef& pSibling) = 0;
	virtual XResult RemoveChild(XNodeRef pChild) = 0;
	virtual XNodeRef GetFather() = 0;
	virtual XResult RemoveFromTree() = 0; // pick it out from father node
	virtual XResult RIPMySelf() = 0;

	virtual XResult IncreaseZOrder() = 0;
	virtual XResult DecreaseZOrder() = 0;
	virtual XResult MoveZOrderBottom() = 0;
};

class IXElement : public IXNode
{
public:
};
typedef XPtr<IXElement> XElementRef;

//////////////////////////////////////////////////////////////////////////

typedef std::function<XResult(XNodeRef node)>	ListenerRegister;

class IXGaia
{
public:
	virtual XNodeRef Create(CString className) = 0;
	virtual XNodeRef CreateFromXML(CString xmlFile) = 0;
	virtual XResult SetListenerRegister(ListenerRegister reger) = 0;
};

//////////////////////////////////////////////////////////////////////////
