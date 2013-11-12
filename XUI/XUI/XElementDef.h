#pragma once
#include "XSmartPtr.h"
#include <functional>

class IXNode;
typedef XPtr<IXNode> XNodeRef;

// 所有可操作元素基类，用于对象树的维护
class IXNode : virtual public Util::Class::CRefCountImpl
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

//////////////////////////////////////////////////////////////////////////

typedef std::function<XResult(XNodeRef node)>	ListenerRegister;

// 可通过Gaia创建UI出来
class IXGaia
{
public:
	virtual XNodeRef Create(CString className) = 0;
	virtual XNodeRef CreateFromXML(CString xmlFile) = 0;
	virtual XResult SetListenerRegister(ListenerRegister reger) = 0;
};

//////////////////////////////////////////////////////////////////////////

#include "XPropertyDefine.h"

typedef std::function<XResult(IXMsg& msg)> XEar;

// 所有可绘制元素基类
class IXElement : virtual public Util::Class::CRefCountImpl
{
public:
	XProperty_Support(Position);
	XProperty_Support(Size);
	XProperty_Support(Rect);
	XProperty_Support(LayoutType);
	XProperty_Support(LayoutInvalid);
	XProperty_Support(LayoutDirection);
	XProperty_Support(Padding);
	XProperty_Support(Align);
	XProperty_Support(AutoWidth);
	XProperty_Support(AutoHeight);
	XProperty_Support(ExpandWidth);
	XProperty_Support(ExpandHeight);
	XProperty_Support(Color);
	XProperty_Support(BorderColor);
	XProperty_Support(BorderWidth);
	XProperty_Support(HitTest);
	XProperty_Support(ToolTip);
	XProperty_Support(Ghost);
	XProperty_Support(XFont);

	virtual XResult ProcessXMessage(IXMsg& msg) = 0;

	virtual XResult Listen(XEar ear,DWORD& earID) = 0;
	virtual XResult StopListen(DWORD earID) = 0;
	virtual XResult Whisper(IXMsg& msg) = 0;
};
typedef XPtr<IXElement> XElementRef;

// 顶层窗口
class IXRealWnd : virtual public  Util::Class::CRefCountImpl
{
public:
	XProperty_Support(Title);
	XProperty_Support(WinStyle);
	XProperty_Support(WinExStyle);
	XProperty_SupportGet(HWnd);
	XProperty_Support(CenterWindow);

	virtual XResult Create(HWND hwndParent=0) = 0;
};
typedef XPtr<IXRealWnd> XRealWndRef;

// 图片
class IXImage : virtual public  Util::Class::CRefCountImpl
{
public:
	XProperty_Support(File);
	XProperty_Support(Offset);
};
typedef XPtr<IXImage> XImageRef;

// 文本
class IXText : virtual public  Util::Class::CRefCountImpl
{
public:
	XProperty_Support(Text);
	XProperty_Support(TextColor);
	XProperty_Support(XFont);
};
typedef XPtr<IXText> IXTextRef;

// 编辑框
class IXEdit : virtual public Util::Class::CRefCountImpl
{
public:
	XProperty_Support(Text);
	XProperty_Support(TextColor);
	XProperty_Support(XFont);
};
typedef XPtr<IXEdit> XEditRef;