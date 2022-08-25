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

static const COLORREF BACKGROUND_COLOR = WHITE;
static const COLORREF VOID_COLOR = WHITE;
static const COLORREF COMPRESSION_COLOR = RED;
static const COLORREF TENSION_COLOR = BLUE;
static const COLORREF SEGMENT_FILL_COLOR = GRAY;

//////////////////////////////////
CRCCapacityChapterBuilder::CRCCapacityChapterBuilder(CRCCapacityDoc* pDoc)
{
   m_pDoc = pDoc;
}

CRCCapacityChapterBuilder::~CRCCapacityChapterBuilder(void)
{
   std::vector<std::_tstring>::iterator iter;
   for (iter = m_TemporaryImageFiles.begin(); iter != m_TemporaryImageFiles.end(); iter++)
   {
      std::_tstring file = *iter;
      ::DeleteFile(file.c_str());
   }
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
   solution->get_IncrementalStrainPlane(&strain_plane);
   strain_plane->GetY(x, z, &y);

   Float64 c = sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));

   (*pPara) << _T("Depth to neutral axis, c = ") << dist.SetValue(c) << rptNewLine;
   (*pPara) << _T("Compression Resultant, C = ") << force.SetValue(C) << rptNewLine;
   (*pPara) << _T("Depth to Compression Resultant, ") << Sub2(_T("d"), _T("c")) << _T(" = ") << dist.SetValue(dc) << rptNewLine;
   (*pPara) << _T("Tension Resultant, T = ") << force.SetValue(T) << rptNewLine;
   (*pPara) << _T("Depth to Tension Resultant, ") << Sub2(_T("d"), _T("e")) << _T(" = ") << dist.SetValue(de) << rptNewLine;

   // Image
   *pPara << CreateImage(solution, true);


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


rptRcImage* CRCCapacityChapterBuilder::CreateImage(IMomentCapacitySolution* pSolution, bool bPositiveMoment) const
{
   CImage image;
   DrawSection(image, pSolution, bPositiveMoment);

   // get a temporary file name for the image
   TCHAR temp_path[_MAX_PATH];
   TCHAR temp_file[_MAX_PATH];
   bool should_delete = true;

   if (::GetTempPath(_MAX_PATH, temp_path) == 0)
      _tcscpy_s(temp_path, _MAX_PATH, _T("\\")); // Couldn't establish a temp path, just use the root drive.

   //
   // Make sure the temp path actually exists
   // We do this by looking for any file in the directory.  If nothing shows up, then the
   // path doesn't exist. (Well, this isn't exactly true, but its the best I can come up
   // with).
   CFileFind finder;
   BOOL bExist;
   CString path(temp_path);
   if (path[path.GetLength() - 1] != '\\')
   {
      path += _T("\\");
   }

   path += _T("*.*");
   bExist = finder.FindFile(path);
   if (!bExist)
   {
      _tcscpy_s(temp_path, _MAX_PATH, _T("\\"));
   }

   // This creates a file called "temp_file".TMP
   if (::GetTempFileName(temp_path, _T("bsexp_"), 0, temp_file) == 0)
   {
      // We could not get a temp name, so just use this default
      // (Use a tmp extension so it is in the same format as the one
      //  the OS would have created for us)
      _tcscpy_s(temp_file, _MAX_PATH, _T("bsexp.tmp"));
      should_delete = false;
   }

   // Replace the TMP extension with png
   std::_tstring strFilename;
   strFilename.assign(temp_file);
   strFilename.replace(strFilename.end() - 3, strFilename.end(), _T("png"));

   // We don't want the file Windows created for us
   if (should_delete)
      ::DeleteFile(temp_file);

   std::transform(std::begin(strFilename), std::end(strFilename), std::begin(strFilename), [](auto& c) {return toupper(c); });

   // this is a const function so we have to cast away const-ness to save
   // the file name
   CRCCapacityChapterBuilder* pMe = const_cast<CRCCapacityChapterBuilder*>(this);
   pMe->m_TemporaryImageFiles.push_back(strFilename);

   image.Save(strFilename.c_str(), Gdiplus::ImageFormatPNG);

   rptRcImage* pImage = new rptRcImage(strFilename.c_str(), rptRcImage::Baseline);
   return pImage;

}

void CRCCapacityChapterBuilder::DrawSection(CImage& image, IMomentCapacitySolution* pSolution, bool bPositiveMoment) const
{
   CComPtr<IGeneralSectionSolution> general_solution;
   pSolution->get_GeneralSectionSolution(&general_solution);
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

   // select image size
   Float64 wx, wy;
   bbox->get_Width(&wx);
   bbox->get_Height(&wy);
   Float64 aspect_ratio = wx / wy;

   int base_dimension = 400;
   Float64 width_scale = 1.5;
   if (aspect_ratio < 1)
   {
      // section is taller than it is wide
      image.Create(int(width_scale * aspect_ratio * base_dimension) + 15, base_dimension + 15, 32);
   }
   else
   {
      // section is wider than it is tall
      image.Create(int(width_scale * base_dimension) + 15, int(base_dimension / aspect_ratio) + 15, 32);
   }


   // set up coordinate mapping
   Float64 mirror_factor = (bPositiveMoment ? 1 : -1); // if neg moment, mirror x and y (so the result is right side up)
   WBFL::Graphing::PointMapper mapper;
   mapper.SetMappingMode(WBFL::Graphing::PointMapper::MapMode::Isotropic);
   mapper.SetWorldExt(mirror_factor * width_scale * wx, mirror_factor * wy);

   Float64 orgY;
   if (bPositiveMoment)
      bbox->get_Top(&orgY);
   else
      bbox->get_Bottom(&orgY);

   mapper.SetWorldOrg(0, orgY);

   mapper.SetDeviceExt(image.GetWidth(), image.GetHeight());
   mapper.SetDeviceOrg(int(image.GetWidth() / (width_scale * 2)) + 2, 2);

   // Fill image background (otherwise it will be black)
   CDC* pDC = CDC::FromHandle(image.GetDC());
   CRect rect(CPoint(0, 0), CSize(image.GetWidth(), image.GetHeight()));
   CBrush bgBrush(BACKGROUND_COLOR);
   CPen bgPen(PS_SOLID, 1, BACKGROUND_COLOR);
   CBrush* pOldBrush = pDC->SelectObject(&bgBrush);
   CPen* pOldPen = pDC->SelectObject(&bgPen);
   pDC->Rectangle(rect);
   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);

   // draw each slice
   CPen girderPen(PS_SOLID, 1, WHITESMOKE);
   CBrush girderBrush(SEGMENT_FILL_COLOR);
   CBrush voidBrush(VOID_COLOR);
   CBrush tensionBrush(TENSION_COLOR);
   CBrush compressionBrush(COMPRESSION_COLOR);

   pOldPen = pDC->SelectObject(&girderPen);

   std::vector<CollectionIndexType> voidIndices;        // contains slice index for void slices
   std::vector<CollectionIndexType> neutralIndices;     // contains slice index for neutral slices
   std::vector<CollectionIndexType> tensionIndices;     // contains slice index for tension slices
   std::vector<CollectionIndexType> compressionIndices; // contains slice index for compression slices

   for (CollectionIndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      Float64 fgStress, bgStress;
      slice->get_ForegroundStress(&fgStress);
      slice->get_BackgroundStress(&bgStress);

      Float64 stress = fgStress - bgStress;

      CComPtr<IStressStrain> fgMaterial, bgMaterial;
      slice->get_ForegroundMaterial(&fgMaterial);
      slice->get_BackgroundMaterial(&bgMaterial);

      if (fgMaterial == nullptr && bgMaterial != nullptr)
      {
         // this is a void
         // save the slice index and go to the next slice
         voidIndices.push_back(sliceIdx);
         continue;
      }

      if (stress < 0)
      {
         compressionIndices.push_back(sliceIdx);
      }
      else if (0 < stress)
      {
         tensionIndices.push_back(sliceIdx);
      }
      else
      {
         neutralIndices.push_back(sliceIdx);
      }
   }

   std::vector<CollectionIndexType>::iterator iter;
   // draw neutral slices first
   pOldBrush = pDC->SelectObject(&girderBrush);
   for (iter = neutralIndices.begin(); iter != neutralIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   // draw compression slices
   pDC->SelectObject(&compressionBrush);
   for (iter = compressionIndices.begin(); iter != compressionIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   // draw the voids on top of the foreground shape
   pDC->SelectObject(&voidBrush);
   for (iter = voidIndices.begin(); iter != voidIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   // draw tension slices
   pDC->SelectObject(&tensionBrush);
   CPen tensionPen(PS_SOLID, 1, TENSION_COLOR);
   pDC->SelectObject(&tensionPen);
   for (iter = tensionIndices.begin(); iter != tensionIndices.end(); iter++)
   {
      CollectionIndexType sliceIdx = *iter;
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);

      DrawSlice(shape, pDC, mapper);
   }

   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);

   // draw the strain profile
   CPen pen(PS_SOLID, 1, BLACK);
   pDC->SelectObject(&pen);

   Float64 top, bottom, left, right;
   bbox->get_Top(&top);
   bbox->get_Bottom(&bottom);
   bbox->get_Left(&left);
   bbox->get_Right(&right);

   //// Draw Y = 0 line
   CPoint p;
   //mapper.WPtoDP(left,0,&p.x,&p.y);
   //pDC->MoveTo(p);
   //mapper.WPtoDP(right,0,&p.x,&p.y);
   //pDC->LineTo(p);

   CComPtr<IPlane3d> strain_plane;
   pSolution->get_IncrementalStrainPlane(&strain_plane);

   Float64 eTop, eBottom; // strain top and bottom
   strain_plane->GetZ(0, mirror_factor * top, &eTop);
   strain_plane->GetZ(0, mirror_factor * bottom, &eBottom);

   // scale strains so that they plot with the same
   // aspect ratio as the section
   Float64 strain = Max(fabs(eBottom), fabs(eTop));
   Float64 scale = (wx / 4) / strain;
   eTop *= scale;
   eBottom *= scale;
   strain *= scale;

   mapper.SetDeviceOrg(3 * image.GetWidth() / 4 + 2, 2);

   if (!bPositiveMoment)
   {
      std::swap(top, bottom);
      std::swap(eTop, eBottom);
   }

   // negate the mirror factor so the strain plane draws correctly
   mapper.GetWorldExt(&wx, &wy);
   wx *= mirror_factor;
   mapper.SetWorldExt(wx, wy);

   mapper.WPtoDP(0, top, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(eTop, top, &p.x, &p.y);
   pDC->LineTo(p);
   mapper.WPtoDP(eBottom, bottom, &p.x, &p.y);
   pDC->LineTo(p);
   mapper.WPtoDP(0, bottom, &p.x, &p.y);
   pDC->LineTo(p);
   mapper.WPtoDP(0, top, &p.x, &p.y);
   pDC->LineTo(p);

   // Draw the compression resultant
   CPen cPen(PS_SOLID, 5, COMPRESSION_COLOR);
   pDC->SelectObject(&cPen);

   CComPtr<IPoint2d> pntC;
   pSolution->get_CompressionResultantLocation(&pntC);
   Float64 y;
   pntC->get_Y(&y);

   if (!bPositiveMoment)
   {
      y *= -1;
   }

   mapper.WPtoDP(strain, y, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(0, y, &p.x, &p.y);
   pDC->LineTo(p);

   // arrow head
   pDC->MoveTo(p.x + 5, p.y - 5);
   pDC->LineTo(p);
   pDC->MoveTo(p.x + 5, p.y + 5);
   pDC->LineTo(p);

   // Draw the tension resultant
   CPen tPen(PS_SOLID, 5, TENSION_COLOR);
   pDC->SelectObject(&tPen);

   CComPtr<IPoint2d> pntT;
   pSolution->get_TensionResultantLocation(&pntT);
   pntT->get_Y(&y);

   if (!bPositiveMoment)
      y *= -1;

   mapper.WPtoDP(0, y, &p.x, &p.y);
   pDC->MoveTo(p);
   mapper.WPtoDP(strain, y, &p.x, &p.y);
   pDC->LineTo(p);


   // arrow head
   pDC->MoveTo(p.x - 5, p.y - 5);
   pDC->LineTo(p);
   pDC->MoveTo(p.x - 5, p.y + 5);
   pDC->LineTo(p);

   pDC->SelectObject(pOldPen);

   image.ReleaseDC();
}

void CRCCapacityChapterBuilder::DrawSlice(IShape* pShape, CDC* pDC, WBFL::Graphing::PointMapper& mapper) const
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
      box.top -= 2;
      box.bottom += 2;
      box.left -= 2;
      box.right += 2;

      pDC->Rectangle(box);
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