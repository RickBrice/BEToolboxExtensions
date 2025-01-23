///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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
   ON_WM_RBUTTONDOWN()
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

   SetMappingMode(WBFL::DManip::MapMode::Isotropic);

   m_pDispMgr->CreateDisplayList(DISPLAY_LIST_ID);

   CDisplayView::OnInitialUpdate();
}

#if defined USE_COM_GEOMETRY
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
      const auto& pNodeValues = pDoc->GetNodeOrdinates();
      const auto& pFDMesh = pDoc->GetFDMesh();
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
            if (pElement->Node[i] != INVALID_INDEX)
            {
               value += pNodeValues[pElement->Node[i]];
            }
         }
         value /= 4;

         auto color = GetColor(min_value,max_value,value);
         CComPtr<iShapeDrawStrategy> shape_draw_strategy;
         shape_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
         shape_draw_strategy->SetShape(clone);
         shape_draw_strategy->SetSolidLineStyle(lsNull);
         shape_draw_strategy->DoFill(true);
         shape_draw_strategy->SetSolidFillColor(RGB(255*std::get<0>(color), 255*std::get<1>(color), 255*std::get<2>(color)));
         compound_strategy->AddStrategy(shape_draw_strategy);
      } // next mesh element
   } // if results computed


   dispObj->SetDrawingStrategy(compound_strategy);

   display_list->AddDisplayObject(dispObj);

   ScaleToFit();
}

#else

void CSVTToolView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   CDisplayView::OnUpdate(pSender, lHint, pHint);
   CDManipClientDC dc(this);

   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();
   const auto& shape = pDoc->GetShape();

   m_pDispMgr->ClearDisplayObjects();

   auto display_list = m_pDispMgr->FindDisplayList(DISPLAY_LIST_ID);
   display_list->Clear();

   auto dispObj = WBFL::DManip::PointDisplayObject::Create();

   auto compound_strategy = WBFL::DManip::CompoundDrawPointStrategy::Create();


   // the main girder shape
   auto shape_draw_strategy = WBFL::DManip::ShapeDrawStrategy::Create();
   shape_draw_strategy->SetShape(shape->CreateClone());
   shape_draw_strategy->SetSolidLineColor(BLUE);
   compound_strategy->AddStrategy(shape_draw_strategy);

   WBFL::Geometry::Line2d y_axis(0.0, WBFL::Geometry::Vector2d(-1.0, 0.0));

   // the finite difference grid
   auto vMesh = pDoc->GetMesh();
   for (auto& mesh_element : vMesh)
   {
      auto shape_draw_strategy = WBFL::DManip::ShapeDrawStrategy::Create();
      auto element_shape = mesh_element.CreateClone();
      element_shape->Reflect(y_axis);
      shape_draw_strategy->SetShape(std::move(element_shape));
      compound_strategy->AddStrategy(shape_draw_strategy);
   }

   if (pDoc->IsComputed())
   {
      const auto& results = pDoc->GetTorsionalConstant();
      const auto& pNodeValues = results.solution.GetFiniteDifferenceSolution();
      const auto& pFDMesh = results.solution.GetFiniteDifferenceMesh();
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
         // Mirror shape about X = 0 (Y-axis)
         //vMesh[elementIdx].GetHookPoint()->X() = -vMesh[elementIdx].GetHookPoint()->X();

         const auto* pElement = pFDMesh->GetElement(elementIdx);
         Float64 value = 0;
         for (IndexType i = 0; i < 4; i++)
         {
            if (pElement->Node[i] != INVALID_INDEX)
            {
               value += pNodeValues[pElement->Node[i]];
            }
         }
         value /= 4;

         auto color = GetColor(min_value, max_value, value);
         auto shape_draw_strategy = WBFL::DManip::ShapeDrawStrategy::Create();
         shape_draw_strategy->SetShape(vMesh[elementIdx].CreateClone());
         shape_draw_strategy->SetSolidLineStyle(WBFL::DManip::LineStyleType::Null);
         shape_draw_strategy->Fill(true);
         shape_draw_strategy->SetSolidFillColor(RGB(255 * std::get<0>(color), 255 * std::get<1>(color), 255 * std::get<2>(color)));

         if (elementIdx == results.MaxSlopeElementIdx)
         {
            shape_draw_strategy->SetSolidFillColor(WHITE);
         }

         compound_strategy->AddStrategy(shape_draw_strategy);
      } // next mesh element
   } // if results computed


   dispObj->SetDrawingStrategy(compound_strategy);

   display_list->AddDisplayObject(dispObj);

   ScaleToFit();
}
#endif

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


void CSVTToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
   // TODO: Add your message handler code here and/or call default

   CDisplayView::OnRButtonDown(nFlags, point);

   ActivateZoomTask();
}
