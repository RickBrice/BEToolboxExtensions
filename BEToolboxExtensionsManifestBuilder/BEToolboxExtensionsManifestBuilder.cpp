///////////////////////////////////////////////////////////////////////
// BEToolboxExtensionsManifestBuilder
// Copyright © 1999-2026  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source
// License along with this program; if not, write to the Washington
// State Department of Transportation, Bridge and Structures Office,
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

/// @brief This application creates the manifest file for the BEToolbox
/// Extensions tools (RCCapacity, SVT, M3C).
///
/// Technically, the manifest file could be written by hand, but it is much
/// easier to do it programmatically. Hand-coding requires dealing with GUIDs
/// and json file format.

#include <targetver.h>

#include "afx.h"

#include <iostream>
#include <tchar.h>

#include <WBFLTypes.h>
#include <EAF/ComponentManager.h>

#include <initguid.h>
#include "../../BEToolbox/BEToolboxCLSID.h"

#include "../CLSID.h"

void BEToolboxExtensionsTools()
{
   // Creates the manifest for BEToolbox Extensions tools
   auto& cm = WBFL::EAF::ComponentManager::GetInstance();
   cm.Reset();
   cm.RegisterComponent(_T("RCCapacity Tool"), CLSID_RCCapacityTool, _T("BEToolboxToolExtensions.dll"), CATID_BEToolboxTool);
   cm.RegisterComponent(_T("SVT Tool"), CLSID_SVTTool, _T("BEToolboxToolExtensions.dll"), CATID_BEToolboxTool);
   cm.RegisterComponent(_T("M3C Tool"), CLSID_M3CTool, _T("BEToolboxToolExtensions.dll"), CATID_BEToolboxTool);
   cm.Save(_T("DoNotDistribute_Experimental.Manifest.BEToolbox"));
}

int main()
{
   BEToolboxExtensionsTools();
}
