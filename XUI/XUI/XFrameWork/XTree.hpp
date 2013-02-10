#pragma once
#include "XBase.hpp"

class CXTreeNode;
typedef XSmartPtr<CXTreeNode> NodeRef;

class CXTreeNode : public Util::Class::CRefCountImpl
{
	XClass(VOID);
public:
	CXTreeNode();
	virtual ~CXTreeNode();
	XResult SetID(CString id);
	CString GetID() const;
	XResult	AppendChild(NodeRef pChild);
	XResult InsertAfter(NodeRef pChild,NodeRef pAfterMe);
	XResult InsertBefore(NodeRef pChild,NodeRef pAfterMe);
	XResult GetFirstChild(NodeRef& pChild);
	XResult GetChild(CString id,NodeRef& pChild);
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
	CString m_ID;
	NodeRef m_father;
	XNodeList m_children;
};

MyNameIs(CXTreeNode)
	I_Can("提供控件树的维护功能")
	I_Provide("子控件Z序维护，值从0起始")
	And_You_Should_Notice("绘制顺序为队列从前向后，在后方的会覆盖前方的绘制。 \
						  节点的生命周期通过引用计数维护，不要使用类裸指针操作，而是使用 \
						  每个类提供的Ref类。")
End_Description;

//////////////////////////////////////////////////////////////////////////

CXTreeNode::CXTreeNode()
: m_father(NodeRef())
{}

CXTreeNode::~CXTreeNode()
{
	m_children.clear();
}

XResult CXTreeNode::SetID(CString id)
{
	m_ID = id;
	return XResult_OK;
}

CString CXTreeNode::GetID() const
{
	return m_ID;
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

XResult CXTreeNode::GetFirstChild( NodeRef& pChild )
{
	if (m_children.empty())
	{
		return XResult_NotFound;
	}
	pChild = *m_children.begin();
	return XResult_OK;
}

XResult CXTreeNode::GetChild( CString id,NodeRef& pChild )
{
	auto i = m_children.begin();
	while (i != m_children.end())
	{
		if ((*i)->GetID() == id)
		{
			pChild = *i;
			return XResult_OK;
		}
	}
	return XResult_NotFound;
}

XResult CXTreeNode::GetSibling( NodeRef pBefore,NodeRef pAfter )
{
	pBefore = nullptr;
	pAfter = nullptr;
	if (m_father)
	{
		auto i = m_father->_GetNodeIter(this);
		if ( i != m_father->m_children.end())
		{
			if (i != m_father->m_children.begin())
			{
				auto iprev = i;
				--iprev;
				pBefore = *iprev;
			}
			{
				auto inext = i;
				++inext;
				if (inext != m_father->m_children.end())
				{
					pAfter = *inext;
				}
			}
		}
	}
	return XResult_OK;
}
