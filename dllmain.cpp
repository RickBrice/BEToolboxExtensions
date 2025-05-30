// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "dllmain.h"

#include <Inventor\Win\SoWin.h>

#include <EAF\ComponentModule.h>
#include "RCCapacity/RCCapacityTool.h"
#include "M3C/M3CTool.h"
#include "SVT/SVTTool.h"
#include "CLSID.h"
WBFL::EAF::ComponentModule Module_;

EAF_BEGIN_OBJECT_MAP(ObjectMap)
   EAF_OBJECT_ENTRY(CLSID_RCCapacityTool, CRCCapacityTool)
   EAF_OBJECT_ENTRY(CLSID_SVTTool, CSVTTool)
   EAF_OBJECT_ENTRY(CLSID_M3CTool, CM3CTool)
EAF_END_OBJECT_MAP()

class CBEToolboxExtensionsApp : public CWinApp
{
public:

// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CBEToolboxExtensionsApp, CWinApp)
END_MESSAGE_MAP()

CBEToolboxExtensionsApp theApp;

BOOL CBEToolboxExtensionsApp::InitInstance()
{
   Module_.Init(ObjectMap);
	GXInit();

	// Initialize open inventor
	AfxEnableMemoryTracking(FALSE);
	if (SoWin::getTopLevelWidget() == nullptr)
	{
	   SoWin::init("WSDOT"); // this guy is a big memory leaker... disable memory tracking so we don't have to deal with leaks we can't fix
	}
	AfxEnableMemoryTracking(TRUE);

	return CWinApp::InitInstance();
}

int CBEToolboxExtensionsApp::ExitInstance()
{
	GXTerminate();
	Module_.Term();
	return CWinApp::ExitInstance();
}
