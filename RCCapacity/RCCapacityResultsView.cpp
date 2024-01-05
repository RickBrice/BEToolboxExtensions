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
#include <MFCTools\Format.h>

#include <EAF\EAFHints.h>
#include <Graphing/GraphXY.h>

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
   auto solution = pDoc->GetCapacity(Parabolic);

   if (solution == nullptr)
      return;

   const auto* general_solution = solution->GetGeneralSectionSolution();

   CFont font;
   font.Attach(GetStockObject(DEFAULT_GUI_FONT));
   CFont* pOldFont = pDC->SelectObject(&font);

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   IndexType nSlices = general_solution->GetSliceCount();

   // determine the bounding box
   WBFL::Geometry::Rect2d bbox;
   for (IndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      const auto* slice = general_solution->GetSlice(sliceIdx);
      const auto& shape = slice->GetShape();

      if (sliceIdx == 0)
      {
         bbox = shape->GetBoundingBox();
      }
      else
      {
         bbox.Union(shape->GetBoundingBox());
      }
   }

   auto wx = bbox.Width();
   auto wy = bbox.Height();

   // set up coordinate mapping to draw cross section
   WBFL::Graphing::PointMapper mapper;
   mapper.SetMappingMode(WBFL::Graphing::PointMapper::MapMode::Isotropic);
   mapper.SetWorldExt(wx, wy);

   Float64 orgY = bbox.Top();
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

   std::vector<std::pair<const WBFL::RCSection::GeneralSectionSlice*, int>> vPieces;
   vPieces.reserve(nSlices);

   for (IndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      const auto* slice = general_solution->GetSlice(sliceIdx);
      auto fgStress = slice->GetForegroundStress();
      auto bgStress = slice->GetBackgroundStress();

      Float64 stress = fgStress - bgStress;
      stress = IsZero(stress) ? 0 : stress;

      auto fgMaterial = slice->GetForegroundMaterial();
      auto bgMaterial = slice->GetBackgroundMaterial();

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
      auto areaA = pieceA.first->GetArea();
      auto areaB = pieceB.first->GetArea();
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

      const auto* slice = pieceInfo.first;
      const auto& shape = slice->GetShape();

      DrawSlice(*shape, pDC, mapper);
   }

   //
   // draw the strain profile
   //
   CPen pen(PS_SOLID, 1, BLACK);
   pDC->SelectObject(&pen);

   auto top = bbox.Top();
   auto bottom = bbox.Bottom();
   auto left = bbox.Left();
   auto right = bbox.Right();

   CPoint p;

   const auto& strain_plane = solution->GetIncrementalStrainPlane();

   // strain top and bottom
   auto eTop = strain_plane.GetZ(0, top);
   auto eBottom = strain_plane.GetZ(0, bottom);

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
      const auto& cg = pieceInfo.first->GetCentroid();
      auto Ycg = cg.Y();

      auto initial_strain = pieceInfo.first->GetInitialStrain();
      auto incremental_strain = pieceInfo.first->GetIncrementalStrain();
      auto total_strain = pieceInfo.first->GetTotalStrain();
      
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

   auto T = solution->GetTensionResultant();
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
      auto fgStress = pieceInfo.first->GetForegroundStress();
      auto bgStress = pieceInfo.first->GetBackgroundStress();
      auto stress = fgStress - bgStress;

      const auto& shape = pieceInfo.first->GetShape();
      auto generic_shape = std::dynamic_pointer_cast<const WBFL::Geometry::GenericShape>(shape);
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

      const auto* slice = pieceInfo.first;
      const auto& cg = slice->GetCentroid();
      auto Ycg = cg.Y();

      auto fgStress = slice->GetForegroundStress();
      auto bgStress = slice->GetBackgroundStress();
      auto stress = fgStress - bgStress;

      stress *= scale;

      mapper.WPtoDP(stress, Ycg, &p.x, &p.y);
      pDC->MoveTo(p);
      mapper.WPtoDP(0, Ycg, &p.x, &p.y);
      pDC->LineTo(p);
   }

   // Draw the compression resultant
   CPen cPen(PS_SOLID, 5, COMPRESSION_COLOR);
   pDC->SelectObject(&cPen);

   const auto& pntC = solution->GetCompressionResultantLocation();
   auto yc = pntC.Y();

   auto C = solution->GetCompressionResultant();
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

   const auto& pntT = solution->GetTensionResultantLocation();
   auto yt = pntT.Y();

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

void CRCCapacityResultsView::DrawSlice(const WBFL::Geometry::Shape& shape, CDC* pDC, WBFL::Graphing::PointMapper& mapper) const
{
   auto poly_points = shape.GetPolyPoints();

   IndexType nPoints = poly_points.size();

   if (nPoints < 3)
   {
      const auto& pnt = poly_points[0];
      LONG dx, dy;
      mapper.WPtoDP(pnt.X(),pnt.Y(), &dx, &dy);

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
      for (IndexType pntIdx = 0; pntIdx < nPoints; pntIdx++)
      {
         const auto& point = poly_points[pntIdx];

         LONG dx, dy;
         mapper.WPtoDP(point.X(), point.Y(), &dx, &dy);

         points[pntIdx] = CPoint(dx, dy);
      }

      pDC->Polygon(points, (int)nPoints);

      delete[] points;
   }
}
