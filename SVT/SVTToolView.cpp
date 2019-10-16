///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2019  Washington State Department of Transportation
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

// SVTToolView.cpp : implementation file
//

#include "stdafx.h"
#include "SVTToolView.h"
#include "SVTToolDoc.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSVTToolView

IMPLEMENT_DYNCREATE(CSVTToolView, CDisplayView)

CSVTToolView::CSVTToolView()
{

}

CSVTToolView::~CSVTToolView()
{
}

BEGIN_MESSAGE_MAP(CSVTToolView, CDisplayView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CSVTToolView::OnFilePrint)
   ON_WM_SIZE()
END_MESSAGE_MAP()

// CSVTToolView diagnostics

#ifdef _DEBUG
void CSVTToolView::AssertValid() const
{
//   CDisplayView::AssertValid();
}

#ifndef _WIN32_WCE
void CSVTToolView::Dump(CDumpContext& dc) const
{
   CDisplayView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSVTToolView message handlers
#define DISPLAY_LIST_ID 100
void CSVTToolView::OnInitialUpdate()
{
   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();

   m_pFrame = (CSVTToolChildFrame*)GetParentFrame();

   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   SetMappingMode(DManip::Isotropic);

   CComPtr<iDisplayList> dl;
   ::CoCreateInstance(CLSID_DisplayList, nullptr, CLSCTX_ALL, IID_iDisplayList, (void**)&dl);
   dl->SetID(DISPLAY_LIST_ID);
   dispMgr->AddDisplayList(dl);

   CDisplayView::OnInitialUpdate();
}

void CSVTToolView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   CDisplayView::OnUpdate(pSender, lHint, pHint);

   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   CDManipClientDC dc(this);

   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();
   CComPtr<IShape> pShape;
   pDoc->GetShape(&pShape);

   dispMgr->ClearDisplayObjects();

   CComPtr<iDisplayList> display_list;
   dispMgr->FindDisplayList(DISPLAY_LIST_ID, &display_list);
   display_list->Clear();

   CComPtr<iPointDisplayObject> dispObj;
   dispObj.CoCreateInstance(CLSID_PointDisplayObject);

   CComPtr<iCompoundDrawPointStrategy> compound_strategy;
   compound_strategy.CoCreateInstance(CLSID_CompoundDrawPointStrategy);


   // the main girder shape
   CComPtr<iShapeDrawStrategy> shape_draw_strategy;
   shape_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
   shape_draw_strategy->SetShape(pShape);
   shape_draw_strategy->SetSolidLineColor(BLUE);
   compound_strategy->AddStrategy(shape_draw_strategy);

   // the finite difference grid
   std::vector<CComPtr<IRectangle>> vMesh = pDoc->GetMesh();
   for (auto mesh_element : vMesh)
   {
      CComQIPtr<IShape> shape(mesh_element);
      CComPtr<iShapeDrawStrategy> shape_draw_strategy;
      shape_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
      shape_draw_strategy->SetShape(shape);
      compound_strategy->AddStrategy(shape_draw_strategy);
   }

   if (pDoc->IsComputed())
   {
      Float64* pNodeValues = pDoc->GetNodeOrdinates();
      const auto* pFDMesh = pDoc->GetFDMesh();
      IndexType nElements = pFDMesh->GetElementCount();
      ATLASSERT(vMesh.size() == nElements);
      Float64 min_value = 0;
      Float64 max_value = 0;
      IndexType nNodes = pFDMesh->GetInteriorNodeCount();
      for (IndexType nodeIdx = 0; nodeIdx < nNodes; nodeIdx++)
      {
         max_value = Max(max_value, pNodeValues[nodeIdx]);
      }

      for (IndexType elementIdx = 0; elementIdx < nElements; elementIdx++)
      {
         CComQIPtr<IShape> shape(vMesh[elementIdx]);
         CComPtr<IShape> clone;
         shape->Clone(&clone);
         CComQIPtr<IRectangle> rect(clone);
         CComPtr<IPoint2d> pnt;
         rect->get_HookPoint(&pnt);
         Float64 X;
         pnt->get_X(&X);
         pnt->put_X(-X);


         const auto* pElement = pFDMesh->GetElement(elementIdx);
         Float64 value = 0;
         for (IndexType i = 0; i < 4; i++)
         {
            if (pElement->Node[i] == INVALID_INDEX)
            {
               // boundary element
               value += 0;
            }
            else
            {
               value += pNodeValues[pElement->Node[i]];
            }
         }
         value /= 4;

         COLORREF color = GetColor(min_value,max_value,value);
         CComPtr<iShapeDrawStrategy> shape_draw_strategy;
         shape_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
         shape_draw_strategy->SetShape(clone);
         shape_draw_strategy->SetSolidLineStyle(lsNull);
         shape_draw_strategy->DoFill(true);
         shape_draw_strategy->SetSolidFillColor(color);
         compound_strategy->AddStrategy(shape_draw_strategy);
      }
   }


   dispObj->SetDrawingStrategy(compound_strategy);

   display_list->AddDisplayObject(dispObj);

   ScaleToFit();
}

// https://stackoverflow.com/questions/22607043/color-gradient-algorithm
//COLORREF InverseSRGBCompanding(COLORREF c)
//{
//   float r = GetRValue(c) / 255.;
//   float g = GetGValue(c) / 255.;
//   float b = GetBValue(c) / 255.;
//
//   r = (0.04045 < r ? pow((r + 0.055) / 1.055, 2.4) : r / 12.92);
//   g = (0.04045 < g ? pow((g + 0.055) / 1.055, 2.4) : g / 12.92);
//   b = (0.04045 < b ? pow((b + 0.055) / 1.055, 2.4) : b / 12.92);
//
//   return RGB(255 * r, 255 * g, 255 * b);
//}
//
//COLORREF SRGBCompanding(COLORREF c)
//{
//   float r = GetRValue(c) / 255.;
//   float g = GetGValue(c) / 255.;
//   float b = GetBValue(c) / 255.;
//
//   r = (0.0031308 < r ? 1.055*pow(r, 1 / 2.4) - 0.055 : r*12.92);
//   g = (0.0031308 < g ? 1.055*pow(g, 1 / 2.4) - 0.055 : g*12.92);
//   b = (0.0031308 < b ? 1.055*pow(b, 1 / 2.4) - 0.055 : b*12.92);
//
//   return RGB(255 * r, 255 * g, 255 * b);
//}

COLORREF CSVTToolView::GetColor(Float64 min, Float64 max, Float64 value)
{
   Float64 ratio = 2 * (value - min) / (max - min);
   int b = (int)Max(0., 255 * (1 - ratio));
   int r = (int)Max(0., 255 * (ratio - 1));
   int g = 255 - b - r;
   return RGB(r, g, b);

   // Color gradient between two colors
   //COLORREF C1 = BLUE;
   //COLORREF C2 = RED;
   //float mix = (value - min) / (max - min);

   //COLORREF c1 = InverseSRGBCompanding(C1);
   //COLORREF c2 = InverseSRGBCompanding(C2);

   //COLORREF c = RGB(GetRValue(c1)*(1 - mix) + GetRValue(c2)*mix,
   //                 GetGValue(c1)*(1 - mix) + GetGValue(c2)*mix,
   //                 GetBValue(c1)*(1 - mix) + GetBValue(c2)*mix);

   //return SRGBCompanding(c);
}

void CSVTToolView::OnSize(UINT nType, int cx, int cy)
{
   CDisplayView::OnSize(nType, cx, cy);

   CRect rect;
   GetClientRect(&rect);
   rect.DeflateRect(5, 5, 5, 5);

   CSize size = rect.Size();
   size.cx = Max(0L, size.cx);
   size.cy = Max(0L, size.cy);

   SetLogicalViewRect(MM_TEXT, rect);

   SetScrollSizes(MM_TEXT, size, CScrollView::sizeDefault, CScrollView::sizeDefault);

   CDManipClientDC dc(this);
   ScaleToFit();
}

void CSVTToolView::OnDraw(CDC* pDC)
{
   __super::OnDraw(pDC);
}
