///////////////////////////////////////////////////////////////////////
// BEToolbox
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

#include "StdAfx.h"
#include "SVTToolViewer3D.h"

#include <Inventor/nodes/SoDirectionalLight.h>


CSVTToolViewer3D::CSVTToolViewer3D(HWND hwnd) :
   SoWinExaminerViewer(hwnd,"SVT Viewer 3D",
      TRUE, // embed in parent window
      BUILD_DECORATION,
      SoWinViewer::Type::BROWSER,
      TRUE)
{
   setFeedbackVisibility(true); // draw the axes

   // put a headlight on the camera
   setHeadlight(TRUE);
   SoDirectionalLight* light = this->getHeadlight();
   light->color.setValue(0.7f, 0.7f, 0.7f);

   //// background color of the scene (gl widget)
   //setBackgroundColor(SbColor(0.1,0.1,0.5));

   setClearBeforeRender(true);
}
