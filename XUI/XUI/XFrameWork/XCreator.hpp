#pragma once
#include "XBase.hpp"
#include "XElement.hpp"
#include "../Loki/SmartPtr.h"

class CXTreeNode;

typedef Loki::SmartPtr<CXTreeNode> NodeRef;

class CXTreeNode
{
	XClass;
public:
	CXTreeNode();
	virtual ~CXTreeNode();
	XResult	AppendChild(NodeRef pChild);
	XResult InsertAfter(NodeRef pChild,NodeRef pAfterMe);
	XResult InsertBefore(NodeRef pChild,NodeRef pAfterMe);
	XResult	SwapNode(NodeRef pNode1,NodeRef pNode2);
	XResult	GetSibling(NodeRef pBefore,NodeRef pAfter);
	XResult RemoveChild(NodeRef pChild);
	NodeRef GetFather();
	XResult RIPMySelf();

	XResult IncreaseZOrder();
	XResult DecreaseZOrder();
	XResult MoveZOrderBottom();
protected:
	typedef std::list<NodeRef> XNodeList;
	XResult _SetZOrder(INT zOrder);
	XNodeList::iterator _GetNodeIter(NodeRef pNode);
protected:
	NodeRef m_father;
	XNodeList m_children;
};

MyNameIs(CXTreeNode)
	I_Can("提供控件树的维护功能")
	I_Provide("子控件Z序维护，值从0起始")
	And_You_Should_Notice("绘制顺序为队列从前向后，在后方的会覆盖前方的绘制。")
End_Description;

//////////////////////////////////////////////////////////////////////////

CXTreeNode::CXTreeNode() : m_father(NULL)
{}

CXTreeNode::~CXTreeNode()
{
	m_children.clear();
}

XResult CXTreeNode::AppendChild( NodeRef pChild )
{
	m_children.push_back(pChild);
	return XResult_OK;
}

XResult CXTreeNode::RemoveChild( NodeRef pChild )
{
	XNodeList::iterator i = _GetNodeIter(pChild);
	if (i != m_children.end())
	{
		m_children.erase(i);
		return XResult_OK;
	}
	return XResult_Fail;
}

NodeRef CXTreeNode::GetFather()
{
	return m_father;
}

XResult CXTreeNode::RIPMySelf()
{
	if (m_father)
	{
		m_father->RemoveChild(NodeRef(this));
		return XResult_OK;
	}
	return XResult_Fail;
}

CXTreeNode::XNodeList::iterator CXTreeNode::_GetNodeIter( NodeRef pNode )
{
	auto i=m_children.begin();
	for (; i!=m_children.end(); ++i)
	{
		if (*i == pNode)
		{
			break;
		}
	}
	return i;
}
