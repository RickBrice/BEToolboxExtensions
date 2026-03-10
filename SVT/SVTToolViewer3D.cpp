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
