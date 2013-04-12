#pragma once
#include "XBase.hpp"

class CXTreeNode : public IXNode
{
	XClass;
public:
	CXTreeNode();
	virtual ~CXTreeNode();
	XResult SetID(CString id) override;
	CString GetID() const override;
	XResult	AppendChild(XNodeRef pChild) override;
	XResult InsertAfter(XNodeRef pChild,XNodeRef pAfterMe) override;
	XResult InsertBefore(XNodeRef pChild,XNodeRef pAfterMe) override;
	XResult GetFirstChild(XNodeRef& pChild) override;
	XResult GetChild(CString id,XNodeRef& pChild) override;
	XResult SearchChild(CString id,XNodeRef& pChild) override;	// search tree
	XResult	SwapNode(XNodeRef pNode1,XNodeRef pNode2) override;
	XResult	GetSibling(XNodeRef& pBefore,XNodeRef& pAfter) override;
	XResult	GetLSibling(XNodeRef& pSibling) override;
	XResult	GetRSibling(XNodeRef& pSibling) override;
	XResult RemoveChild(XNodeRef pChild) override;
	XNodeRef GetFather() override;
	XResult RemoveFromTree() override; // pick it out from father node
	XResult RIPMySelf() override;

	XResult IncreaseZOrder() override;
	XResult DecreaseZOrder() override;
	XResult MoveZOrderBottom() override;

protected:
	typedef std::list<XNodeRef> XNodeList;
	XResult _SetZOrder(INT zOrder);
	XNodeList::iterator _GetNodeIter(XNodeRef pNode);
protected:
	CString m_ID;
	CXTreeNode* m_father;
	XNodeList m_children;
};

//////////////////////////////////////////////////////////////////////////

inline CXTreeNode::CXTreeNode()
: m_father(nullptr)
{}

inline CXTreeNode::~CXTreeNode()
{
	m_children.clear();
}

inline XResult CXTreeNode::SetID(CString id)
{
	m_ID = id;
	return XResult_OK;
}

inline CString CXTreeNode::GetID() const
{
	return m_ID;
}

inline XResult CXTreeNode::AppendChild( XNodeRef pChild )
{
	if (!pChild)
	{
		return XResult_InvalidArg;
	}
	m_children.push_back(pChild);
	XPtr<CXTreeNode>(pChild)->m_father = this;
	return XResult_OK;
}

inline XResult CXTreeNode::RemoveChild( XNodeRef pChild )
{
	XNodeList::iterator i = _GetNodeIter(pChild);
	if (i != m_children.end())
	{
		m_children.erase(i);
		XPtr<CXTreeNode>(pChild)->m_father = NULL;
		return XResult_OK;
	}
	return XResult_Fail;
}

inline XNodeRef CXTreeNode::GetFather()
{
	return m_father;
}

inline XResult CXTreeNode::RIPMySelf()
{
	if (m_father)
	{
		m_father->RemoveChild(this);
	}
	for (auto& i:m_children)
	{
		XPtr<CXTreeNode>(i)->m_father = nullptr;
	}
	return XResult_OK;
}

inline CXTreeNode::XNodeList::iterator CXTreeNode::_GetNodeIter( XNodeRef pNode )
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

inline XResult CXTreeNode::GetFirstChild( XNodeRef& pChild )
{
	if (m_children.empty())
	{
		return XResult_NotFound;
	}
	pChild = *m_children.begin();
	return XResult_OK;
}

inline XResult CXTreeNode::GetChild( CString id,XNodeRef& pChild )
{
	auto i = m_children.begin();
	while (i != m_children.end())
	{
		if ((*i)->GetID() == id)
		{
			pChild = *i;
			return XResult_OK;
		}
		++i;
	}
	return XResult_NotFound;
}

inline XResult CXTreeNode::GetSibling( XNodeRef& pBefore,XNodeRef& pAfter )
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

inline XResult CXTreeNode::GetLSibling(XNodeRef& pSibling)
{
	pSibling = nullptr;
	if (!m_father)
	{
		return XResult_Fail;
	}
	auto i = m_father->_GetNodeIter(this);
	if (i != m_father->m_children.end())
	{
		if (i != m_father->m_children.begin())
		{
			--i;
			pSibling = *i;
			return XResult_OK;
		}
	}
	return XResult_Fail;
}

inline XResult CXTreeNode::GetRSibling(XNodeRef& pSibling)
{
	pSibling = nullptr;
	if (!m_father)
	{
		return XResult_Fail;
	}
	auto i = m_father->_GetNodeIter(this);
	if (i != m_father->m_children.end())
	{
		++i;
		if (i != m_father->m_children.end())
		{
			pSibling = *i;
			return XResult_OK;
		}
	}
	return XResult_Fail;
}

inline XResult CXTreeNode::SearchChild( CString id,XNodeRef& pChild )
{
	pChild = nullptr;
	GetChild(id,pChild);
	if (!pChild)
	{
		for (auto& i:m_children)
		{
			XNodeRef childNode = i;
			childNode->SearchChild(id,pChild);
			if (pChild)
			{
				return XResult_OK;
			}
		}
	}
	return XResult_NotFound;
}

inline XResult CXTreeNode::RemoveFromTree()
{
	if (m_father)
	{
		m_father->RemoveChild(this);
	}
	return XResult_OK;
}

inline XResult CXTreeNode::InsertAfter( XNodeRef pChild,XNodeRef pAfterMe )
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
		if (*i == pAfterMe)
		{
			if (i == m_children.end())
			{
				m_children.push_back(pChild);
			}
			else
			{
				++i;
				m_children.insert(i,pChild);
			}
			return XResult_OK;
		}
	}
	return XResult_Fail;
}

inline XResult CXTreeNode::InsertBefore( XNodeRef pChild,XNodeRef pAfterMe )
{
	for (auto i=m_children.begin(); i!=m_children.end(); ++i)
	{
		if (*i == pAfterMe)
		{
			m_children.insert(i,pChild);
			return XResult_OK;
		}
	}
	return XResult_Fail;
}

inline XResult CXTreeNode::SwapNode( XNodeRef pNode1,XNodeRef pNode2 )
{
	auto i1 = std::find(m_children.begin(),m_children.end(),pNode1);
	auto i2 = std::find(m_children.begin(),m_children.end(),pNode2);
	if (i1!=m_children.end() && i2!=m_children.end())
	{
		auto swaper = *i1;
		*i1 = *i2;
		*i2 = swaper;
		return XResult_OK;
	}
	return XResult_Fail;
}

inline XResult CXTreeNode::IncreaseZOrder()
{
	return XResult_NotImpl;
}

inline XResult CXTreeNode::DecreaseZOrder()
{
	return XResult_NotImpl;
}

inline XResult CXTreeNode::MoveZOrderBottom()
{
	return XResult_NotImpl;
}
