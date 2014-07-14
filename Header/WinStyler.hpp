#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

class CWinStyler
{
public:
	DWORD GetWinStyle(){ return m_style; };
	DWORD GetWinExStyle(){ return m_exStyle; };

	VOID SetStyle(
		BOOL isLayeredWnd = FALSE,
		BOOL hasBorder = TRUE,
		BOOL hasSysBar = TRUE,
		BOOL maximizable = TRUE,
		BOOL minimizable = TRUE,
		BOOL visible = TRUE,
		BOOL activable = TRUE,
		BOOL sizable = TRUE,
		BOOL isToolWnd = FALSE,
		BOOL isPopup = FALSE)
	{
		m_style = 0;
		m_exStyle = 0;
		if (isLayeredWnd)
		{
			m_exStyle = WS_EX_LAYERED;
			return;
		}
		if (isToolWnd)
		{
			m_exStyle |= WS_EX_TOOLWINDOW;
		}
		if (isPopup)
		{
			m_style |= WS_POPUP;
		}

		if (hasBorder)
		{
			m_style |= WS_BORDER;
		}
		else
		{
			sizable = FALSE;
		}
		if (hasSysBar)
		{
			m_style |= WS_CAPTION;
		}
		else
		{
			maximizable = FALSE;
			maximizable = FALSE;
		}
		if (maximizable || minimizable)
		{
			m_style |= WS_SYSMENU;
		}
		if (maximizable)
		{
			m_style |= WS_MAXIMIZEBOX;
		}
		if (minimizable)
		{
			m_style |= WS_MINIMIZEBOX;
		}
		if (visible)
		{
			m_style |= WS_VISIBLE;
		}
		if (!activable)
		{
			m_style |= WS_DISABLED;
		}
		if (sizable)
		{
			m_style |= WS_THICKFRAME;
		}
	}
private:
	DWORD m_style = 0;
	DWORD m_exStyle = 0;
};