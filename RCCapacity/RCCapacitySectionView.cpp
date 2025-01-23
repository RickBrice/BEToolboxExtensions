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

// RCCapacitySectionView.cpp : implementation file
//

#include "stdafx.h"
#include "RCCapacitySectionView.h"
#include "RCCapacityMainView.h"
#include "RCCapacityInputView.h"
#include "RCCapacityDoc.h"

#include <EAF\EAFHints.h>

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRCCapacitySectionView

IMPLEMENT_DYNCREATE(CRCCapacitySectionView, CDisplayView)

CRCCapacitySectionView::CRCCapacitySectionView()
{
   m_pInputView = nullptr;
}

CRCCapacitySectionView::~CRCCapacitySectionView()
{
}

BEGIN_MESSAGE_MAP(CRCCapacitySectionView, CDisplayView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CRCCapacityResultsView::OnFilePrint)
   ON_WM_SIZE()
END_MESSAGE_MAP()

// CRCCapacitySectionView diagnostics

#ifdef _DEBUG
void CRCCapacitySectionView::AssertValid() const
{
   //__super::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacitySectionView::Dump(CDumpContext& dc) const
{
   __super::Dump(dc);
}
#endif
#endif //_DEBUG


// CRCCapacitySectionView message handlers
#define DISPLAY_LIST_ID 100
void CRCCapacitySectionView::OnInitialUpdate()
{
   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   SetMappingMode(DManip::Isotropic);

   CComPtr<iDisplayList> dl;
   ::CoCreateInstance(CLSID_DisplayList, nullptr, CLSCTX_ALL, IID_iDisplayList, (void**)&dl);
   dl->SetID(DISPLAY_LIST_ID);
   dispMgr->AddDisplayList(dl);

   __super::OnInitialUpdate();

   Update();
}

void CRCCapacitySectionView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   __super::OnUpdate(pSender, lHint, pHint);
}

void CRCCapacitySectionView::Update()
{
   CComPtr<iDisplayMgr> dispMgr;
   GetDisplayMgr(&dispMgr);

   CComPtr<iDisplayList> display_list;
   dispMgr->FindDisplayList(DISPLAY_LIST_ID, &display_list);
   if (display_list == nullptr)
      return; // the view isn't initialized yet

   display_list->Clear();

   dispMgr->ClearDisplayObjects();

   CDManipClientDC dc(this);

   const ModelData& modelData = m_pInputView->GetModelData();

   CComPtr<IShape> pShape;
   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
   pDoc->GetBeamShape(modelData, &pShape);

   CComPtr<IPoint2d> origin;
   origin.CoCreateInstance(CLSID_Point2d);
   CComQIPtr<IXYPosition> position(pShape);
   position->put_LocatorPoint(lpTopCenter, origin);

   CComPtr<iPointDisplayObject> dispObj;
   dispObj.CoCreateInstance(CLSID_PointDisplayObject);

   CComPtr<iCompoundDrawPointStrategy> compound_strategy;
   compound_strategy.CoCreateInstance(CLSID_CompoundDrawPointStrategy);


   // the main girder shape
   CComPtr<iShapeDrawStrategy> girder_draw_strategy;
   girder_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
   girder_draw_strategy->SetShape(pShape);
   girder_draw_strategy->SetSolidLineColor(BLACK);
   compound_strategy->AddStrategy(girder_draw_strategy);

   if (modelData.bHasDeck)
   {
      CComPtr<IRectangle> rect;
      rect.CoCreateInstance(CLSID_Rect);
      rect->put_Width(modelData.DeckWidth);
      rect->put_Height(modelData.DeckThickness);

      CComQIPtr<IXYPosition> position(rect);
      position->put_LocatorPoint(lpBottomCenter, origin);

      CComPtr<iShapeDrawStrategy> deck_draw_strategy;
      deck_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
      CComQIPtr<IShape> shape(rect);
      deck_draw_strategy->SetShape(shape);
      deck_draw_strategy->SetSolidLineColor(BLACK);
      compound_strategy->AddStrategy(deck_draw_strategy);
   }

   CComPtr<IRect2d> bbox;
   pShape->get_BoundingBox(&bbox);
   Float64 Hg;
   bbox->get_Height(&Hg);

   for (const auto& rebar : modelData.Rebar)
   {
      Float64 y = pDoc->GetReinforcementLocation(rebar, Hg, modelData.DeckThickness);
      Float64 x = -1.0* (rebar.nBars - 1) * rebar.spacing / 2.0;
      for (int i = 0; i < rebar.nBars; i++)
      {
         CComPtr<ICircle> circle;
         circle.CoCreateInstance(CLSID_Circle);

         const auto* pBar = WBFL::LRFD::RebarPool::GetInstance()->GetRebar(modelData.RebarType, modelData.RebarGrade, rebar.size);
         circle->put_Radius(pBar->GetNominalDimension() / 2);
         CComPtr<IPoint2d> center;
         circle->get_Center(&center);
         center->Move(x, y);

         CComPtr<iShapeDrawStrategy> rebar_draw_strategy;
         rebar_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
         CComQIPtr<IShape> shape(circle);
         rebar_draw_strategy->SetShape(shape);
         rebar_draw_strategy->SetSolidLineColor(GREEN);
         rebar_draw_strategy->SetSolidFillColor(GREEN);
         rebar_draw_strategy->DoFill(true);
         compound_strategy->AddStrategy(rebar_draw_strategy);

         x += rebar.spacing;
      }
   }

   for (const auto& strand : modelData.Strands)
   {
      Float64 y = pDoc->GetReinforcementLocation(strand, Hg, modelData.DeckThickness);
      Float64 x = -1.0* (strand.nStrands - 1) * strand.spacing / 2.0;
      for (int i = 0; i < strand.nStrands; i++)
      {
         CComPtr<ICircle> circle;
         circle.CoCreateInstance(CLSID_Circle);
         circle->put_Radius(pDoc->GetStrandDiameter(modelData.StrandSize) / 2.0);
         CComPtr<IPoint2d> center;
         circle->get_Center(&center);
         center->Move(x, y);

         CComPtr<iShapeDrawStrategy> strand_draw_strategy;
         strand_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy);
         CComQIPtr<IShape> shape(circle);
         strand_draw_strategy->SetShape(shape);
         strand_draw_strategy->SetSolidLineColor(BLUE);
         strand_draw_strategy->SetSolidFillColor(BLUE);
         strand_draw_strategy->DoFill(true);
         compound_strategy->AddStrategy(strand_draw_strategy);

         x += strand.spacing;
      }
   }

   dispObj->SetDrawingStrategy(compound_strategy);

   display_list->AddDisplayObject(dispObj);

   ScaleToFit();
}

void CRCCapacitySectionView::OnSize(UINT nType, int cx, int cy)
{
   __super::OnSize(nType, cx, cy);

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
