#pragma once
#include "XFrameWork/XBase.hpp"
#include "XFrameWork/XGaia.hpp"

class CXUI : public IXUI
{
	XSingleton(CXUI);
public:
	BOOL Initialize(HINSTANCE hInst) override;
	VOID Finalize() override;
	VOID Work() override;
	IXGaia& GetGaia() override;
private:
	CAppModule m_atlModule;
	HINSTANCE m_hInst;

	blog::CLogDeviceFile	m_fileLogger;
	blog::CLogDeviceConsole	m_consoleLogger;
};
