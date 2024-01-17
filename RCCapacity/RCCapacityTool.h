///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2024  Washington State Department of Transportation
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

#pragma once

#include "resource.h"
#include <ToolImpl.h>

#include "RCCapacityDocTemplate.h"
#include "RCCapacityDoc.h"
#include "RCCapacityTabView.h"
#include "RCCapacityChildFrame.h"

// {DB437766-BF3D-4E62-8FBD-5D9D4F2FF875}
DEFINE_GUID(CLSID_RCCapacityTool,
   0xdb437766, 0xbf3d, 0x4e62, 0x8f, 0xbd, 0x5d, 0x9d, 0x4f, 0x2f, 0xf8, 0x75);

class CRCCapacityTool : public CToolImpl<CRCCapacityTool, &CLSID_RCCapacityTool, IDR_RCCAPACITYTOOL, CRCCapacityDoc, CRCCapacityChildFrame, CRCCapacityTabView, CRCCapacityDocTemplate, IDR_RCCAPACITY>
{
public:
   virtual CString GetName() const override { return _T("RCCapacityTool"); }
};
OBJECT_ENTRY_AUTO(CLSID_RCCapacityTool, CRCCapacityTool)
