///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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
#include "RCCapacityChapterBuilder.h"
#include "RCCapacityReportSpecification.h"
#include "RCCapacityDoc.h"
#include <Reporter\Reporter.h>

#include <WBFLRCCapacity.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////
CRCCapacityChapterBuilder::CRCCapacityChapterBuilder(CRCCapacityDoc* pDoc)
{
   m_pDoc = pDoc;
}

CRCCapacityChapterBuilder::~CRCCapacityChapterBuilder(void)
{

}

LPCTSTR CRCCapacityChapterBuilder::GetName() const
{
   return _T("Analysis Details");
}

Uint16 CRCCapacityChapterBuilder::GetMaxLevel() const
{
   return 1;
}

rptChapter* CRCCapacityChapterBuilder::Build(CReportSpecification* pRptSpec, Uint16 level) const
{
   USES_CONVERSION;

   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara = new rptParagraph;
   *pChapter << pPara;

   CComPtr<IMomentCapacitySolution> solution;
   m_pDoc->GetCapacity(&solution);

   if (solution == nullptr)
   {
      *pPara << _T("Moment capacity could not be computed") << rptNewLine;
      return pChapter;
   }

   CComPtr<IGeneralSectionSolution> general_solution;
   solution->get_GeneralSectionSolution(&general_solution);

   if (general_solution == nullptr)
   {
      *pPara << _T("Moment capacity could not be computed") << rptNewLine;
      return pChapter;
   }

   CComPtr<IGeneralSection> section;
   m_pDoc->GetSection(&section);


   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   INIT_UV_PROTOTYPE(rptAreaUnitValue, area, pDisplayUnits->Area, false);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, cg, pDisplayUnits->ComponentDim, false);
   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, false);
   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment, pDisplayUnits->SmallMoment, true);
   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment2, pDisplayUnits->Moment, true);
   INIT_UV_PROTOTYPE(rptForceUnitValue, force, pDisplayUnits->GeneralForce, true);
   INIT_UV_PROTOTYPE(rptLengthUnitValue, dist, pDisplayUnits->ComponentDim, true);

   force.ShowUnitTag(true);
   moment.ShowUnitTag(true);

   Float64 Fz, Mx;
   solution->get_Fz(&Fz);
   solution->get_Mx(&Mx);

   (*pPara) << _T("Resultant Force  = ") << symbol(SUM) << _T(" ") << symbol(delta) << _T("F = ") << force.SetValue(Fz) << rptNewLine;
   (*pPara) << _T("Resultant Moment = ") << symbol(SUM) << _T(" ") << symbol(delta) << _T("M = ") << moment.SetValue(-Mx) << _T(" = ") << moment2.SetValue(-Mx) << rptNewLine;

   Float64 C, T;
   solution->get_CompressionResultant(&C);
   solution->get_TensionResultant(&T);

   CComPtr<IPoint2d> pntC, pntT;
   solution->get_CompressionResultantLocation(&pntC);
   solution->get_TensionResultantLocation(&pntT);

   Float64 Yc, Yt;
   pntC->get_Y(&Yc);
   pntT->get_Y(&Yt);

   CComPtr<IShape> shape;
   section->get_Shape(0, &shape);
   CComQIPtr<IXYPosition> position(shape);
   CComPtr<IPoint2d> pntTC;
   position->get_LocatorPoint(lpTopCenter, &pntTC);

   Float64 x1, y1;
   pntTC->Location(&x1, &y1);

   Float64 x2, y2;
   pntC->Location(&x2, &y2);

   Float64 moment_arm = fabs(Mx / T);
   Float64 dc = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
   Float64 de = dc + moment_arm;

   Float64 x, y, z;
   x = 0;
   y = 0;
   z = 0;
   CComPtr<IPlane3d> strain_plane;
   solution->get_StrainPlane(&strain_plane);
   strain_plane->GetY(x, z, &y);

   Float64 c = sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));

   (*pPara) << _T("Depth to neutral axis, c = ") << dist.SetValue(c) << rptNewLine;
   (*pPara) << _T("Compression Resultant, C = ") << force.SetValue(C) << rptNewLine;
   (*pPara) << _T("Depth to Compression Resultant, ") << Sub2(_T("d"), _T("c")) << _T(" = ") << dist.SetValue(dc) << rptNewLine;
   (*pPara) << _T("Tension Resultant, T = ") << force.SetValue(T) << rptNewLine;
   (*pPara) << _T("Depth to Tension Resultant, ") << Sub2(_T("d"), _T("e")) << _T(" = ") << dist.SetValue(de) << rptNewLine;


   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(12, _T(""));
   (*pPara) << pTable << rptNewLine;

   ColumnIndexType col = 0;
   (*pTable)(0, col++) << _T("Piece");
   (*pTable)(0, col++) << _T("Slice");
   (*pTable)(0, col++) << COLHDR(_T("Area"), rptAreaUnitTag, pDisplayUnits->Area);
   (*pTable)(0, col++) << COLHDR(Sub2(_T("Y"), _T("top")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   (*pTable)(0, col++) << COLHDR(Sub2(_T("Y"), _T("bot")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   (*pTable)(0, col++) << COLHDR(Sub2(_T("Y"), _T("cg")), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   (*pTable)(0, col++) << _T("Initial") << rptNewLine << _T("Strain");
   (*pTable)(0, col++) << _T("Incremental") << rptNewLine << _T("Strain");
   (*pTable)(0, col++) << _T("Total") << rptNewLine << _T("Strain");
   (*pTable)(0, col++) << COLHDR(_T("Stress"), rptStressUnitTag, pDisplayUnits->Stress);
   (*pTable)(0, col++) << COLHDR(symbol(delta) << _T("F =") << rptNewLine << _T("(Area)(Stress)"), rptForceUnitTag, pDisplayUnits->GeneralForce);
   (*pTable)(0, col++) << COLHDR(symbol(delta) << _T("M =") << rptNewLine << _T("(") << symbol(delta) << _T("F") << _T(")(") << Sub2(_T("Y"), _T("cg")) << _T(")"), rptMomentUnitTag, pDisplayUnits->SmallMoment);
   force.ShowUnitTag(false);
   moment.ShowUnitTag(false);


   RowIndexType row = pTable->GetNumberOfHeaderRows();
#if defined _DEBUG
   Float64 sum_force = 0;
   Float64 sum_moment = 0;
#endif
   CollectionIndexType nSlices;
   general_solution->get_SliceCount(&nSlices);
   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      Float64 slice_area;
      slice->get_Area(&slice_area);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);
      CComQIPtr<IXYPosition> position(shape);
      CComPtr<IPoint2d> pntTop, pntBottom;
      position->get_LocatorPoint(lpTopCenter, &pntTop);
      position->get_LocatorPoint(lpBottomCenter, &pntBottom);
      Float64 Yt, Yb;
      pntTop->get_Y(&Yt);
      pntBottom->get_Y(&Yb);
      Yt *= -1;
      Yb *= -1;

      CComPtr<IPoint2d> pntCG;
      slice->get_CG(&pntCG);

      Float64 cgX, cgY;
      pntCG->Location(&cgX, &cgY);

      Float64 initial_strain;
      slice->get_InitialStrain(&initial_strain);

      Float64 incremental_strain;
      slice->get_IncrementalStrain(&incremental_strain);

      Float64 total_strain;
      slice->get_TotalStrain(&total_strain);

      CComPtr<IStressStrain> fgMaterial;
      slice->get_ForegroundMaterial(&fgMaterial);
      Float64 fg;
      HRESULT hr_stress = fgMaterial->ComputeStress(total_strain,&fg);

      Float64 fgStress, bgStress, netStress;
      slice->get_ForegroundStress(&fgStress);
      slice->get_BackgroundStress(&bgStress);
      ATLASSERT(IsEqual(fg, fgStress));
      ATLASSERT(IsZero(bgStress)); // we aren't currently using background stress

      netStress = fgStress - bgStress;

      Float64 F = slice_area * netStress;
      Float64 M = F * cgY;

      IndexType shapeIdx;
      slice->get_ShapeIndex(&shapeIdx);

      CComBSTR bstrName;
      section->get_Name(shapeIdx, &bstrName);

      col = 0;
      (*pTable)(row, col++) << OLE2T(bstrName);
      (*pTable)(row, col++) << sliceIdx + 1;
      (*pTable)(row, col++) << area.SetValue(slice_area);
      (*pTable)(row, col++) << cg.SetValue(Yt);
      (*pTable)(row, col++) << cg.SetValue(Yb);
      (*pTable)(row, col++) << cg.SetValue(-cgY);
      (*pTable)(row, col++) << initial_strain;
      (*pTable)(row, col++) << incremental_strain;
      if (hr_stress == S_FALSE)
      {
         (*pTable)(row, col).SetFillBackGroundColor(rptRiStyle::Red);
         (*pTable)(row, col++) << total_strain;
         (*pTable)(row, col).SetFillBackGroundColor(rptRiStyle::Red);
         (*pTable)(row, col++) << stress.SetValue(netStress);
      }
      else
      {
         (*pTable)(row, col++) << total_strain;
         (*pTable)(row, col++) << stress.SetValue(netStress);
      }
      (*pTable)(row, col++) << force.SetValue(F);
      (*pTable)(row, col++) << moment.SetValue(-M);

#if defined _DEBUG
      sum_force += F;
      sum_moment += M;
#endif

      row++;
   }

   ATLASSERT(IsEqual(sum_force, Fz));
   ATLASSERT(IsEqual(sum_moment, Mx));

   return pChapter;
}

CChapterBuilder* CRCCapacityChapterBuilder::Clone() const
{
   return new CRCCapacityChapterBuilder(m_pDoc);
}
