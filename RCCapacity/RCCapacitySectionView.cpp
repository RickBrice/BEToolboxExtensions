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

   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
   auto beam_shape = pDoc->GetBeamShape();

   CComPtr<iPointDisplayObject> dispObj;
   dispObj.CoCreateInstance(CLSID_PointDisplayObject);

   CComPtr<iCompoundDrawPointStrategy> compound_strategy;
   compound_strategy.CoCreateInstance(CLSID_CompoundDrawPointStrategy);

   // the main girder shape
   CComPtr<iShapeDrawStrategy2> girder_draw_strategy;
   girder_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy2);
   girder_draw_strategy->SetShape(std::move(beam_shape->CreateClone()));
   girder_draw_strategy->SetSolidLineColor(BLACK);
   girder_draw_strategy->HasBoundingShape(false);
   compound_strategy->AddStrategy(girder_draw_strategy);
   auto Hg = beam_shape->GetBoundingBox().Height();

   auto slab_shape = pDoc->GetSlabShape();
   if(slab_shape)
   {
      CComPtr<iShapeDrawStrategy2> slab_draw_strategy;
      slab_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy2);
      slab_draw_strategy->SetShape(slab_shape->CreateClone());
      slab_draw_strategy->SetSolidLineColor(BLACK);
      slab_draw_strategy->HasBoundingShape(false);
      compound_strategy->AddStrategy(slab_draw_strategy);
   }

   const auto& modelData = pDoc->GetModelData();
   for (const auto& rebar : modelData.Rebar)
   {
      Float64 y = pDoc->GetReinforcementLocation(rebar, Hg, modelData.DeckThickness);
      Float64 x = -1.0* (rebar.nBars - 1) * rebar.spacing / 2.0;
      for (int i = 0; i < rebar.nBars; i++)
      {
         WBFL::Geometry::Circle circle;

         const auto* pBar = WBFL::LRFD::RebarPool::GetInstance()->GetRebar(modelData.RebarType, modelData.RebarGrade, rebar.size);
         circle.SetRadius(pBar->GetNominalDimension() / 2);
         circle.Move(WBFL::Geometry::Shape::LocatorPoint::CenterCenter, WBFL::Geometry::Point2d(x, y));

         CComPtr<iShapeDrawStrategy2> rebar_draw_strategy;
         rebar_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy2);
         rebar_draw_strategy->SetShape(std::move(circle.CreateClone()));
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
         WBFL::Geometry::Circle circle;

         circle.SetRadius(pDoc->GetStrandDiameter(modelData.StrandSize) / 2.0);
         circle.Move(WBFL::Geometry::Shape::LocatorPoint::CenterCenter, WBFL::Geometry::Point2d(x, y));

         CComPtr<iShapeDrawStrategy2> strand_draw_strategy;
         strand_draw_strategy.CoCreateInstance(CLSID_ShapeDrawStrategy2);
         strand_draw_strategy->SetShape(std::move(circle.CreateClone()));
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
