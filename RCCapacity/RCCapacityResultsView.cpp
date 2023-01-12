///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

// RCCapacityView.cpp : implementation file
//

#include "stdafx.h"
#include "RCCapacityResultsView.h"
#include "RCCapacityDoc.h"
#include <GraphicsLib\GraphicsLib.h>
#include <MFCTools\Format.h>

#include <EAF\EAFHints.h>

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const COLORREF VOID_COLOR = WHITE;
static const COLORREF COMPRESSION_COLOR = RED;
static const COLORREF TENSION_COLOR = BLUE;
static const COLORREF SEGMENT_FILL_COLOR = GREY70;

// CRCCapacityResultsView

IMPLEMENT_DYNCREATE(CRCCapacityResultsView, CEAFView)

CRCCapacityResultsView::CRCCapacityResultsView()
{

}

CRCCapacityResultsView::~CRCCapacityResultsView()
{
}

BEGIN_MESSAGE_MAP(CRCCapacityResultsView, CEAFView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CRCCapacityResultsView::OnFilePrint)
   ON_WM_SIZE()
END_MESSAGE_MAP()

// CRCCapacityResultsView diagnostics

#ifdef _DEBUG
void CRCCapacityResultsView::AssertValid() const
{
   __super::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacityResultsView::Dump(CDumpContext& dc) const
{
   __super::Dump(dc);
}
#endif
#endif //_DEBUG


// CRCCapacityView message handlers
void CRCCapacityResultsView::OnInitialUpdate()
{
   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)GetDocument();

   m_pFrame = (CRCCapacityChildFrame*)GetParentFrame();

   __super::OnInitialUpdate();
}

void CRCCapacityResultsView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   __super::OnUpdate(pSender, lHint, pHint);
}

void CRCCapacityResultsView::OnSize(UINT nType, int cx, int cy)
{
   __super::OnSize(nType, cx, cy);
}


void CRCCapacityResultsView::OnDraw(CDC* pDC)
{
   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)GetDocument();
   CComPtr<IMomentCapacitySolution> solution;
   pDoc->GetCapacity(&solution);

   if (solution == nullptr)
      return;

   CComPtr<IGeneralSectionSolution> general_solution;
   solution->get_GeneralSectionSolution(&general_solution);

   if (general_solution == nullptr)
      return;


   CFont font;
   font.Attach(GetStockObject(DEFAULT_GUI_FONT));
   CFont* pOldFont = pDC->SelectObject(&font);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CollectionIndexType nSlices;
   general_solution->get_SliceCount(&nSlices);

   // determine the bounding box
   CComPtr<IRect2d> bbox;
   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      if (sliceIdx == 0)
      {
         shape->get_BoundingBox(&bbox);
      }
      else
      {
         CComPtr<IRect2d> box;
         shape->get_BoundingBox(&box);
         bbox->Union(box);
      }
   }

   Float64 wx, wy;
   bbox->get_Width(&wx);
   bbox->get_Height(&wy);

   // set up coordinate mapping to draw cross section
   grlibPointMapper mapper;
   mapper.SetMappingMode(grlibPointMapper::Isotropic);
   mapper.SetWorldExt(wx, wy);

   Float64 orgY;
   bbox->get_Top(&orgY);
   mapper.SetWorldOrg(0, orgY);

   CRect clientRect;
   GetClientRect(&clientRect);

   CRect sectionRect;
   sectionRect.top = clientRect.top + 10;
   sectionRect.bottom = clientRect.bottom;
   sectionRect.left = clientRect.left;
   sectionRect.right = clientRect.left + clientRect.Width() / 3;

   CRect strainRect;
   strainRect.top = sectionRect.top;
   strainRect.bottom = sectionRect.bottom;
   strainRect.left = sectionRect.right;
   strainRect.right = strainRect.left + clientRect.Width() / 3;

   CRect stressRect;
   stressRect.top = sectionRect.top;
   stressRect.bottom = sectionRect.bottom;
   stressRect.left = strainRect.right;
   stressRect.right = stressRect.left + clientRect.Width() / 3;

   sectionRect.DeflateRect(CSize(5, 5));
   strainRect.DeflateRect(CSize(5, 5));
   stressRect.DeflateRect(CSize(5, 5));

   pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
   pDC->TextOut(sectionRect.CenterPoint().x, sectionRect.top, _T("Discretized Section: Red = Compression, Blue = Tension"));
   pDC->TextOut(strainRect.CenterPoint().x, strainRect.top, _T("Strains: Orange = initial strain, Gray = change in strain, Blue = total strain"));
   pDC->TextOut(stressRect.CenterPoint().x, stressRect.top, _T("Stress profile and resultant forces"));

   mapper.SetDeviceExt(sectionRect.Width(), sectionRect.Height());
   mapper.SetDeviceOrg(sectionRect.CenterPoint().x,sectionRect.top);

   // draw each slice
   CPen columnPen(PS_SOLID, 1, WHITESMOKE);
   CBrush columnBrush(SEGMENT_FILL_COLOR);
   CBrush voidBrush(VOID_COLOR);
   CBrush tensionBrush(TENSION_COLOR);
   CBrush compressionBrush(COMPRESSION_COLOR);

   CPen* pOldPen = pDC->SelectObject(&columnPen);
   CBrush* pOldBrush = pDC->SelectObject(&columnBrush);

   //pDC->Rectangle(&sectionRect);
   //pDC->Rectangle(&strainRect);
   //pDC->Rectangle(&stressRect);

   // we don't want the larger pieces to cover over the smaller pieces
   // (e.g... we don't want girder slices to cover over the bars and strands)
   // Fill up a container with references to the pieces and their type (tension, compression, etc)
   // then sort by area so the bigger pieces are drawn first. Type will be used to set the color
   const int Neutral = 0;
   const int Void = 1;
   const int Tension = 2;
   const int Compression = 3;

   std::vector<std::pair<CComPtr<IGeneralSectionSlice>, int>> vPieces;
   vPieces.reserve(nSlices);

   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      Float64 fgStress, bgStress;
      slice->get_ForegroundStress(&fgStress);
      slice->get_BackgroundStress(&bgStress);

      Float64 stress = fgStress - bgStress;
      stress = IsZero(stress) ? 0 : stress;

      CComPtr<IStressStrain> fgMaterial, bgMaterial;
      slice->get_ForegroundMaterial(&fgMaterial);
      slice->get_BackgroundMaterial(&bgMaterial);

      if (fgMaterial == nullptr && bgMaterial != nullptr)
      {
         // this is a void
         // save the slice index and go to the next slice
         vPieces.push_back(std::make_pair(slice, Void));
         continue;
      }

      if (stress < 0)
      {
         vPieces.push_back(std::make_pair(slice, Compression));
      }
      else if (0 < stress)
      {
         vPieces.push_back(std::make_pair(slice, Tension));
      }
      else
      {
         vPieces.push_back(std::make_pair(slice, Neutral));
      }
   }

   auto sort_by_area = [](auto& pieceA, auto& pieceB)
   {
      Float64 areaA, areaB;
      pieceA.first->get_Area(&areaA);
      pieceB.first->get_Area(&areaB);
      return areaB < areaA;
   };

   std::sort(std::begin(vPieces), std::end(vPieces), sort_by_area);

   // draw the pieces/slices
   for (auto& pieceInfo : vPieces)
   {
      switch (pieceInfo.second)
      {
      case Neutral:
         pDC->SelectObject(&columnBrush);
         break;

      case Void:
         pDC->SelectObject(&voidBrush);
         break;

      case Compression:
         pDC->SelectObject(&compressionBrush);
         break;

      case Tension:
         pDC->SelectObject(&tensionBrush);
         break;
      }

      CComPtr<IGeneralSectionSlice> slice = pieceInfo.first;
      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   //
   // draw the strain profile
   //
   CPen pen(PS_SOLID, 1, BLACK);
   pDC->SelectObject(&pen);

   Float64 top, bottom, left, right;
   bbox->get_Top(&top);
   bbox->get_Bottom(&bottom);
   bbox->get_Left(&left);
   bbox->get_Right(&right);

   CPoint p;

   CComPtr<IPlane3d> strain_plane;
   solution->get_StrainPlane(&strain_plane);

   Float64 eTop, eBottom; // strain top and bottom
   strain_plane->GetZ(0, top, &eTop);
   strain_plane->GetZ(0, bottom, &eBottom);

   // capture top/bottom strain as a string before the scaling factor is applied
   CString str_eTop;
   str_eTop.Format(_T("%f"), eTop);

   CString str_eBottom;
   str_eBottom.Format(_T("%f"), eBottom);

   Float64 strain = Max(fabs(eBottom), fabs(eTop));
   Float64 scale = (wx / 4) / strain;
   eTop *= scale;
   eBottom *= scale;
   strain *= scale;

   mapper.SetDeviceExt(strainRect.Width(), strainRect.Height());
   mapper.SetDeviceOrg(strainRect.left + strainRect.Width() / 2, strainRect.top);

   // vertical line (zero strain line)
   mapper.WPtoDP(0, top, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(0, bottom, &p.x, &p.y);
   pDC->LineTo(p);
   pDC->SetTextAlign(TA_TOP | TA_CENTER);
   pDC->TextOut(p.x, p.y, _T("C <-- 0.0 --> T"));

   // draw strain at CG of each slice
   CPen initialStrainPen(PS_SOLID, 3, ORANGE);
   CPen incrementalStrainPen(PS_SOLID, 3, GRAY);
   CPen totalStrainPen(PS_SOLID, 3, BLUE);
   CRect rect(0, 0, 5, 5);
   for (auto& pieceInfo : vPieces)
   {
      CComPtr<IPoint2d> cg;
      pieceInfo.first->get_CG(&cg);
      Float64 Ycg;
      cg->get_Y(&Ycg);

      Float64 initial_strain, incremental_strain, total_strain;
      pieceInfo.first->get_InitialStrain(&initial_strain);
      pieceInfo.first->get_IncrementalStrain(&incremental_strain);
      pieceInfo.first->get_TotalStrain(&total_strain);
      
      pDC->SelectObject(&initialStrainPen);
      mapper.WPtoDP(0, Ycg, &p.x, &p.y);
      pDC->MoveTo(p.x, p.y);
      mapper.WPtoDP(initial_strain*scale, Ycg, &p.x, &p.y);
      rect.MoveToXY(p);
      pDC->Ellipse(&rect);

      pDC->SelectObject(&incrementalStrainPen);
      mapper.WPtoDP(incremental_strain* scale, Ycg, &p.x, &p.y);
      rect.MoveToXY(p);
      pDC->Ellipse(&rect);

      pDC->SelectObject(&totalStrainPen);
      mapper.WPtoDP(total_strain* scale, Ycg, &p.x, &p.y);
      rect.MoveToXY(p);
      pDC->Ellipse(&rect);
   }
   pDC->SelectObject(&pen);

   //
   // Draw stresses and resultant forces
   //
   mapper.SetDeviceExt(stressRect.Width(), stressRect.Height());
   mapper.SetDeviceOrg(stressRect.left + stressRect.Width() / 2, stressRect.top);

   Float64 T;
   solution->get_TensionResultant(&T);
   //mapper.SetWorldExt(T, wy);

   // vertical line (zero stress line)
   mapper.WPtoDP(0, top, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(0, bottom, &p.x, &p.y);
   pDC->LineTo(p);
   pDC->SetTextAlign(TA_TOP | TA_CENTER);
   pDC->TextOut(p.x, p.y, _T("C <-- 0.0 --> T"));

   Float64 minStress = Float64_Max;
   Float64 maxStress = -Float64_Max;
   for (auto& pieceInfo : vPieces)
   {
      Float64 fgStress, bgStress;
      pieceInfo.first->get_ForegroundStress(&fgStress);
      pieceInfo.first->get_BackgroundStress(&bgStress);
      Float64 stress = fgStress - bgStress;

      CComPtr<IShape> shape;
      pieceInfo.first->get_Shape(&shape);
      CComQIPtr<IGenericShape> generic_shape(shape);
      if (generic_shape) stress = 0;

      minStress = Min(stress, minStress);
      maxStress = Max(stress, maxStress);
   }

   Float64 stress_range = maxStress - minStress;
   scale = (wx / 2) / stress_range;
   //mapper.SetWorldExt(stress_range*scale, wy);

   for (auto& pieceInfo : vPieces)
   {
      switch (pieceInfo.second)
      {
      case Neutral:
         pDC->SelectObject(&columnBrush);
         break;

      case Void:
         pDC->SelectObject(&voidBrush);
         break;

      case Compression:
         pDC->SelectObject(&compressionBrush);
         break;

      case Tension:
         pDC->SelectObject(&tensionBrush);
         break;
      }

      CComPtr<IGeneralSectionSlice> slice = pieceInfo.first;
      CComPtr<IPoint2d> cg;
      slice->get_CG(&cg);
      Float64 Ycg;
      cg->get_Y(&Ycg);

      Float64 fgStress, bgStress;
      slice->get_ForegroundStress(&fgStress);
      slice->get_BackgroundStress(&bgStress);
      Float64 stress = fgStress - bgStress;

      stress *= scale;

      mapper.WPtoDP(stress, Ycg, &p.x, &p.y);
      pDC->MoveTo(p);
      mapper.WPtoDP(0, Ycg, &p.x, &p.y);
      pDC->LineTo(p);
   }

   // Draw the compression resultant
   CPen cPen(PS_SOLID, 5, COMPRESSION_COLOR);
   pDC->SelectObject(&cPen);

   CComPtr<IPoint2d> pntC;
   solution->get_CompressionResultantLocation(&pntC);
   Float64 yc;
   pntC->get_Y(&yc);

   Float64 C;
   solution->get_CompressionResultant(&C);
   mapper.WPtoDP(strain, yc, &p.x, &p.y);
   pDC->MoveTo(p);
   CPoint pntCompression = p;
   mapper.WPtoDP(0, yc, &p.x, &p.y);
   pDC->LineTo(p);

   // arrow head
   pDC->MoveTo(p.x + 5, p.y - 5);
   pDC->LineTo(p);
   pDC->MoveTo(p.x + 5, p.y + 5);
   pDC->LineTo(p);

   mapper.WPtoDP(strain / 2, yc, &p.x, &p.y);
   CString strC;
   strC.Format(_T("%s"), FormatDimension(C, pDispUnits->GeneralForce));
   pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
   pDC->TextOut(p.x, p.y, strC);

   // Draw the tension resultant
   CPen tPen(PS_SOLID, 5, TENSION_COLOR);
   pDC->SelectObject(&tPen);

   CComPtr<IPoint2d> pntT;
   solution->get_TensionResultantLocation(&pntT);
   Float64 yt;
   pntT->get_Y(&yt);

   mapper.WPtoDP(0, yt, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(strain, yt, &p.x, &p.y);
   pDC->LineTo(p);
   CPoint pntTension = p;

   // arrow head
   pDC->MoveTo(p.x - 5, p.y - 5);
   pDC->LineTo(p);
   pDC->MoveTo(p.x - 5, p.y + 5);
   pDC->LineTo(p);

   mapper.WPtoDP(strain / 2, yt, &p.x, &p.y);
   CString strT;
   strT.Format(_T("%s"), FormatDimension(T, pDispUnits->GeneralForce));
   pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
   pDC->TextOut(p.x, p.y, strT);

   pDC->SelectObject(pOldPen);
   pDC->MoveTo(p.x,pntCompression.y);
   pDC->LineTo(p.x,pntTension.y);

   CString strMomentArm;
   strMomentArm.Format(_T("%s"), FormatDimension(yc - yt, pDispUnits->ComponentDim));
   pDC->TextOut(p.x, (pntTension.y + pntCompression.y) / 2, strMomentArm);

   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);
   pDC->SelectObject(pOldFont);
}

void CRCCapacityResultsView::DrawSlice(IShape* pShape, CDC* pDC, grlibPointMapper& mapper) const
{
   CComPtr<IPoint2dCollection> objPoints;
   pShape->get_PolyPoints(&objPoints);

   CollectionIndexType nPoints;
   objPoints->get_Count(&nPoints);

   if (nPoints < 3)
   {
      CComPtr<IPoint2d> pnt;
      objPoints->get_Item(0, &pnt);

      Float64 x, y;
      pnt->Location(&x, &y);

      LONG dx, dy;
      mapper.WPtoDP(x, y, &dx, &dy);

      CRect box(CPoint(dx, dy), CSize(0, 0));
      const int r = 5;
      box.top -= r;
      box.bottom += r;
      box.left -= r;
      box.right += r;

      //pDC->Rectangle(box);
      pDC->Ellipse(box);
   }
   else
   {
      CPoint* points = new CPoint[nPoints];
      for (CollectionIndexType pntIdx = 0; pntIdx < nPoints; pntIdx++)
      {
         CComPtr<IPoint2d> point;
         objPoints->get_Item(pntIdx, &point);
         Float64 x, y;
         point->get_X(&x);
         point->get_Y(&y);

         LONG dx, dy;
         mapper.WPtoDP(x, y, &dx, &dy);

         points[pntIdx] = CPoint(dx, dy);
      }

      pDC->Polygon(points, (int)nPoints);

      delete[] points;
   }
}
