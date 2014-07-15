XUI
===
建设中。

库的使用
---
库形式为 **单dll**，仅有一个导出函数，通过包含`xui_include.h`可以获取所有相关声明 :

>\#pragma once

> class IXUI

> {

> public:
>> virtual BOOL **Initialize**(HINSTANCE hInst) = 0; 

>> virtual VOID **Finalize**() = 0; 

>> virtual VOID **Work**() = 0; 

>> virtual IXGaia& **GetGaia**() = 0; 

>> virtual IXResPool& **GetResPool**() = 0; 

> };

> __declspec(dllexport) BOOL **GetIXUI**(IXUI** pXUI);

通过GetIXUI可以获取到IXUI对象，该对象为全局单实例。接口调用步骤大致为：

- 调用Initialize初始化XUI
- 可选，调用GetResPool，设置ResPool，登记你的ui资源位置
- 调用GetGaia， **通过Gaia创建出需要的UI对象**
- 调用Work，此时XUI将会占用当前线程作为UI线程使用，当所有窗口关闭后才会继续执行下去
- 调用Finalize，通知XUI退出

可参考根目录下的 **uier工程**，它提供了一个非常完整的使用示例

Gaia
---
所有的UI都是由 *[Gaia] []* 创建生成的。

> class IXGaia
> {
> public:
>> virtual XNodeRef **Create**(CString className) = 0;

>> virtual XNodeRef **CreateFromXML**(CString xmlFile) = 0;

>> virtual XResult **SetListenerRegister**(ListenerRegister reger) = 0;

> };

### Gaia::Create ###
通过类名创建出UI对象。作为运行期手动创建ui元素的辅助手段

### Gaia:CreateFromXML ###
通过XML文件创建出UI元素，是 **最方便也是最常规的创建UI的方法** 。

XML的示例请参考Resource目录下的XML文件。

所有可用的控件名称及运行时接口皆定义在`XElementDef.h`文件中。

### Gaia::SetListenerRegister ###
`ListenerRegister`的定义如下:

> typedef std::function< **XResult(XNodeRef node)**>   ListenerRegister;

这是一个回调，每当Gaia创建一个元素时，会被调起，传入的node即为当前被创建的对象。你可以通过该node的ID来判断这是哪一个UI元素，如果是你感兴趣的ui元素，可以在这个时候挂载这个ui对象的监听。

<br />
XNode
---
**所有可操作的UI对象，都继承自XNode**

XNode提供了对象树的维护，每一个XNode都可以与其它XNode组成父子型的树形结构。一个经典的窗口，其根即为一个Win32真窗口型的XNode元素。

> class IXNode

> {

> public:
>> virtual XResult **SetID**(CString id) = 0;

>> virtual XResult **AppendChild**(XNodeRef pChild) = 0;

>}

对象引用计数及类型转换
---
每一个UI对象都是基于引用计数维护其生命周期的，XUI提供了自己的智能指针，保证了引用计数的正确性和易用性，也可随时转换对象类型，其定义在`XSmartPtr.h`中:
> template < class T>

> class XSmartPtr

> {

> };

> \#define **XPtr** XSmartPtr

使用XPtr较为方便。每一个UI对象将有一和其对应的智能指针类型定义，如XNode，有对应的XNodeRef定义:

>typedef XPtr< *IXNode*> **XNodeRef**;

当然， **在编写时极力推荐直接使用XPtr< IXNode>这样的模式** 。使用起来类似COM技术中常用的CComPtr<>

为了使接口看起来简洁，XUI导出类函数都使用的是typedef之后的类型，X????Ref即为X????型的UI对象的智能指针，看到这种定义知道其真正类型即可。

### 类型转换 ###

通过XPtr，可以转换对象类型：
```
XPtr<IXNode> pWndNode;
pWndNode = Gaia.CreateFromXML(_T("test.xml"));
XPtr<IXRealWnd> pRealWnd(pWndNode);
pRealWnd->Create();   // 创建窗口
XPtr<IXNode> pTextNode;
pTextNode = pWndNode->GetChild(_T("Title"));  // 获取子元素
XPtr<IXText> pText(pTextNode);
pTextNode->SetText(_T("Hello World"));
```

UI元素
---
**参见[Element.md] []**

[VNOC]: https://github.com/VNOC/vnoc
[Gaia]: http://en.wikipedia.org/wiki/Gaia_%28mythology%29
[Element.md]: https://github.com/epiciskandar/XUI/blob/master/Element.md
