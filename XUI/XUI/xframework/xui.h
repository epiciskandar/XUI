#pragma once
#include "XBase.hpp"
#include "XGaia.hpp"

class CXUI : public IXUI
{
	XSingleton(CXUI);
public:
	BOOL Initialize(HINSTANCE hInst) override;
	VOID Finalize() override;
	VOID Work() override;
	IXGaia& GetGaia() override;
	IXResPool& GetResPool() override;
private:
	CAppModule m_atlModule;
	HINSTANCE m_hInst;

	blog::CLogDeviceFile	m_fileLogger;
	blog::CLogDeviceConsole	m_consoleLogger;
};
