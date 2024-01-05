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

rptChapter* CRCCapacityChapterBuilder::Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec, Uint16 level) const
{
   rptChapter* pChapter = new rptChapter;
   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   *pChapter << pPara;

   *pPara << _T("Summary") << rptNewLine;

   pPara = new rptParagraph;
   *pChapter << pPara;

   rptRcTable* table = rptStyleManager::CreateDefaultTable(4);
   *pPara << table << rptNewLine;

   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   (*table)(0, 0) << _T("");
   (*table)(0, 1) << _T("LRFD");
   (*table)(0, 2) << _T("Parabolic Model");
   (*table)(0, 3) << _T("Bilinear Model");
   
   (*table)(1, 0) << COLHDR(_T("Neutral axis depth, c"), rptLengthUnitTag, pDisplayUnits->ComponentDim);
   (*table)(2, 0) << COLHDR(_T("Steel stress, ") << RPT_FPS, rptStressUnitTag, pDisplayUnits->Stress);
   (*table)(3, 0) << COLHDR(_T("Nominal capacity, ") << RPT_MN, rptMomentUnitTag, pDisplayUnits->Moment);
   (*table)(4, 0) << Sub2(symbol(epsilon), _T("t"));
   (*table)(5, 0) << symbol(phi);
   (*table)(6, 0) << COLHDR(RPT_PHI_MN, rptMomentUnitTag, pDisplayUnits->Moment);

   ReportClosedForm(pChapter,table,1);
   ReportStrainCompatibility(pChapter,Parabolic,table,2);
   ReportStrainCompatibility(pChapter, Bilinear, table, 3);
   return pChapter;
}

void CRCCapacityChapterBuilder::ReportStrainCompatibility(rptChapter* pChapter, ConcreteModel concreteModel, rptRcTable* pSummaryTable, ColumnIndexType colIdx) const
{
   USES_CONVERSION;

   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   *pChapter << pPara;

   std::array<std::_tstring, 2> strConcreteModel{ _T("Parabolic"), _T("Bilinear") };

   *pPara << _T("Strain compatibility with ") << strConcreteModel[concreteModel] << _T(" concrete model") << rptNewLine;

   pPara = new rptParagraph;
   *pChapter << pPara;

   auto solution = m_pDoc->GetCapacity(concreteModel);

   if (solution == nullptr || solution->GetGeneralSectionSolution() == nullptr)
   {
      *pPara << _T("Moment capacity could not be computed") << rptNewLine;
      return;
   }

   auto section = m_pDoc->GetSection(concreteModel);


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

   auto Fz = solution->GetFz();
   auto Mx = solution->GetMx();

   (*pPara) << _T("Resultant Force  = ") << symbol(SUM) << _T(" ") << symbol(delta) << _T("F = ") << force.SetValue(Fz) << rptNewLine;
   (*pPara) << _T("Resultant Moment = ") << symbol(SUM) << _T(" ") << symbol(delta) << _T("M = ") << moment.SetValue(-Mx) << _T(" = ") << moment2.SetValue(-Mx) << rptNewLine;

   auto C = solution->GetCompressionResultant();
   auto T = solution->GetTensionResultant();

   Float64 dc = solution->GetDepthToCompressionResultant();
   Float64 de = solution->GetDepthToTensionResultant();
   Float64 moment_arm = de - dc;

   Float64 c = solution->GetDepthToNeutralAxis();

   Float64 Yc = solution->GetExtremeCompressionPoint().Y();

   (*pPara) << _T("Depth to neutral axis, c = ") << dist.SetValue(c) << rptNewLine;
   (*pPara) << _T("Compression Resultant, C = ") << force.SetValue(C) << rptNewLine;
   (*pPara) << _T("Depth to Compression Resultant, ") << Sub2(_T("d"), _T("c")) << _T(" = ") << dist.SetValue(dc) << rptNewLine;
   (*pPara) << _T("Tension Resultant, T = ") << force.SetValue(T) << rptNewLine;
   (*pPara) << _T("Depth to Tension Resultant, ") << Sub2(_T("d"), _T("e")) << _T(" = ") << dist.SetValue(de) << rptNewLine;

   // Image
   rptParagraph* pPara2 = new rptParagraph;
   *pChapter << pPara2;
   *pPara2 << CreateImage(*solution, true, pDisplayUnits);


   rptRcTable* pTable = rptStyleManager::CreateDefaultTable(12, _T(""));
   (*pPara2) << pTable << rptNewLine;

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
   Float64 Aps = 0;
   Float64 Aps_fps = 0.0;

   Float64 dt = 0;

   IndexType nSlices = solution->GetGeneralSectionSolution()->GetSliceCount();
   for (IndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      const auto* slice = solution->GetGeneralSectionSolution()->GetSlice(sliceIdx);

      auto slice_area = slice->GetArea();

      const auto& shape = slice->GetShape();

      const auto& pntTop = shape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter);
      const auto& pntBottom = shape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter);
      auto Yt = pntTop.Y();
      auto Yb = pntBottom.Y();
      Yt *= -1;
      Yb *= -1;

      const auto& pntCG = slice->GetCentroid();
      auto [cgX, cgY] = pntCG.GetLocation();

      Float64 initial_strain = slice->GetInitialStrain();
      Float64 incremental_strain = slice->GetIncrementalStrain();
      Float64 total_strain = slice->GetTotalStrain();

      auto& fgMaterial = slice->GetForegroundMaterial();
      auto [fg, bFgStressSuccess] = fgMaterial->ComputeStress(total_strain);

      auto fgStress = slice->GetForegroundStress();
      auto bgStress = slice->GetBackgroundStress();
      ATLASSERT(IsEqual(fg, fgStress));
      ATLASSERT(IsZero(bgStress)); // we aren't currently using background stress
      auto netStress = fgStress - bgStress;

      auto shapeIdx = slice->GetShapeIndex();
      const auto& name = section->GetName(shapeIdx);

      if (name.starts_with(_T("Strand")))
      {
         Aps += slice_area;
         Aps_fps += slice_area * netStress;
         Float64 dt_ = fabs(Yc - pntCG.Y());
         if (dt < dt_)
         {
            dt = dt_;
         }
      }

      Float64 F = slice_area * netStress;
      Float64 M = F * cgY;


      col = 0;
      (*pTable)(row, col++) << name;
      (*pTable)(row, col++) << sliceIdx + 1;
      (*pTable)(row, col++) << area.SetValue(slice_area);
      (*pTable)(row, col++) << cg.SetValue(Yt);
      (*pTable)(row, col++) << cg.SetValue(Yb);
      (*pTable)(row, col++) << cg.SetValue(-cgY);
      (*pTable)(row, col++) << initial_strain;
      (*pTable)(row, col++) << incremental_strain;
      if (bFgStressSuccess)
      {
         (*pTable)(row, col++) << total_strain;
         (*pTable)(row, col++) << stress.SetValue(netStress);
      }
      else
      {
         (*pTable)(row, col).SetFillBackGroundColor(rptRiStyle::Red);
         (*pTable)(row, col++) << total_strain;
         (*pTable)(row, col).SetFillBackGroundColor(rptRiStyle::Red);
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



   Float64 fps = IsZero(Aps) ? 0.0 : Aps_fps / Aps;

   Float64 et = (dt - c) * 0.003 / c;
   auto ecl = WBFL::LRFD::Rebar::GetCompressionControlledStrainLimit(WBFL::Materials::Rebar::Grade::Grade60);
   auto etl = WBFL::LRFD::Rebar::GetTensionControlledStrainLimit(WBFL::Materials::Rebar::Grade::Grade60);
   auto phi = WBFL::LRFD::ConcreteUtil::GetFlexureCapacityResistanceFactor(et, ecl, etl, 0.75, 1.0);

   stress.ShowUnitTag(true);

   (*pPara) << _T("Average ") << RPT_FPS << _T(" = ") << stress.SetValue(fps) << rptNewLine;
   (*pPara) << Sub2(symbol(epsilon), _T("t")) << _T(" = ") << et << rptNewLine;
   (*pPara) << Sub2(_T("d"), _T("t")) << _T(" = ") << dist.SetValue(dt) << rptNewLine;
   (*pPara) << symbol(phi) << _T(" = ") << phi << rptNewLine;
   (*pPara) << RPT_MU << _T(" = ") << RPT_PHI_MN << _T(" = ") << moment2.SetValue(-phi * Mx) << rptNewLine;

   dist.ShowUnitTag(false);
   stress.ShowUnitTag(false);
   moment2.ShowUnitTag(false);

   (*pSummaryTable)(1, colIdx) << dist.SetValue(c);
   (*pSummaryTable)(2, colIdx) << stress.SetValue(fps);
   (*pSummaryTable)(3, colIdx) << moment2.SetValue(-Mx);
   (*pSummaryTable)(4, colIdx) << et;
   (*pSummaryTable)(5, colIdx) << phi;
   (*pSummaryTable)(6, colIdx) << moment2.SetValue(-phi * Mx);
}

void CRCCapacityChapterBuilder::ReportClosedForm(rptChapter* pChapter, rptRcTable* pSummaryTable, ColumnIndexType col) const
{
   rptParagraph* pPara = new rptParagraph(rptStyleManager::GetHeadingStyle());
   *pChapter << pPara;
   (*pPara) << _T("LRFD 5.6.3") << rptNewLine;

   pPara = new rptParagraph;
   *pChapter << pPara;

   auto cap = m_pDoc->GetRCCCapacity();
   const auto& beam = m_pDoc->GetRCCBeam();
   
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   INIT_UV_PROTOTYPE(rptLengthUnitValue, cg, pDisplayUnits->ComponentDim, false);
   INIT_UV_PROTOTYPE(rptStressUnitValue, stress, pDisplayUnits->Stress, false);
   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment, pDisplayUnits->SmallMoment, true);
   INIT_UV_PROTOTYPE(rptMomentUnitValue, moment2, pDisplayUnits->Moment, true);

   cg.ShowUnitTag(true);
   moment.ShowUnitTag(true);
   moment2.ShowUnitTag(true);
   stress.ShowUnitTag(true);

   (*pPara) << Sub2(_T("d"),_T("ps")) << _T(" = ") << cg.SetValue(beam.GetDps()) << rptNewLine;
   (*pPara) << Sub2(_T("d"), _T("t")) << _T(" = ") << cg.SetValue(beam.GetDpsMax()) << rptNewLine;
   (*pPara) << Sub2(_T("h"),_T("f")) << _T(" = ") << cg.SetValue(beam.GetHf()) << rptNewLine;
   (*pPara) << RPT_ALPHA1 << _T(" = ") << cap.GetAlpha1() << rptNewLine;
   (*pPara) << RPT_BETA1 << _T(" = ") << cap.GetBeta1() << rptNewLine;
   (*pPara) << _T("c = ") << cg.SetValue(cap.GetC()) << rptNewLine;
   (*pPara) << RPT_FPS << _T(" = ") << stress.SetValue(cap.GetFps()) << rptNewLine;
   (*pPara) << Sub2(symbol(epsilon),_T("t")) << _T(" = ") << cap.Get_et() << rptNewLine;
   (*pPara) << RPT_MN << _T(" = ") << moment.SetValue(cap.GetMn()) << _T(" = ") << moment2.SetValue(cap.GetMn()) << rptNewLine;

   // strain limits for prestress are the same as grade 60 rebar
   auto ecl = WBFL::LRFD::Rebar::GetCompressionControlledStrainLimit(WBFL::Materials::Rebar::Grade::Grade60);
   auto etl = WBFL::LRFD::Rebar::GetTensionControlledStrainLimit(WBFL::Materials::Rebar::Grade::Grade60);
   auto phi = WBFL::LRFD::ConcreteUtil::GetFlexureCapacityResistanceFactor(cap.Get_et(), ecl, etl, 0.75, 1.0);
   (*pPara) << symbol(phi) << _T(" = ") << phi << rptNewLine;
   (*pPara) << RPT_MU << _T(" = ") << RPT_PHI_MN << _T(" = ") << moment2.SetValue(phi * cap.GetMn()) << rptNewLine;

   cg.ShowUnitTag(false);
   moment2.ShowUnitTag(false);
   stress.ShowUnitTag(false);
   (*pSummaryTable)(1, col) << cg.SetValue(cap.GetC());
   (*pSummaryTable)(2, col) << stress.SetValue(cap.GetFps());
   (*pSummaryTable)(3, col) << moment2.SetValue(cap.GetMn());
   (*pSummaryTable)(4, col) << cap.Get_et();
   (*pSummaryTable)(5, col) << phi;
   (*pSummaryTable)(6, col) << moment2.SetValue(phi * cap.GetMn());
}

std::unique_ptr<WBFL::Reporting::ChapterBuilder> CRCCapacityChapterBuilder::Clone() const
{
   return std::make_unique<CRCCapacityChapterBuilder>(m_pDoc);
}


rptRcImage* CRCCapacityChapterBuilder::CreateImage(const WBFL::RCSection::MomentCapacitySolution& solution, bool bPositiveMoment,const WBFL::Units::IndirectMeasure* pDisplayUnits) const
{
   CImage image;
   DrawSection(image, solution, bPositiveMoment, pDisplayUnits);

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

void CRCCapacityChapterBuilder::DrawSection(CImage& image, const WBFL::RCSection::MomentCapacitySolution& solution, bool bPositiveMoment,const WBFL::Units::IndirectMeasure* pDisplayUnits) const
{
   const auto* general_solution = solution.GetGeneralSectionSolution();

   auto nSlices = general_solution->GetSliceCount();

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

   // select image size
   auto wx = bbox.Width();
   auto wy = bbox.Height();
   Float64 aspect_ratio = wy / wx;

   int beam_width = 400;
   int beam_height = (int)(beam_width * aspect_ratio);
   image.Create(3 * beam_width, beam_height, 32);


   // set up coordinate mapping
   Float64 mirror_factor = (bPositiveMoment ? 1 : -1); // if neg moment, mirror x and y (so the result is right side up)
   WBFL::Graphing::PointMapper mapper;
   mapper.SetMappingMode(WBFL::Graphing::PointMapper::MapMode::Isotropic);
   mapper.SetWorldExt(mirror_factor * wx, mirror_factor * wy);

   Float64 orgY;
   if (bPositiveMoment)
      orgY = bbox.Top();
   else
      orgY = bbox.Bottom();

   mapper.SetWorldOrg(0, orgY);

   CRect clientRect(CPoint(0, 0), CSize(image.GetWidth(), image.GetHeight()));
   CRect sectionRect;
   sectionRect.top = clientRect.top + 10;
   sectionRect.bottom = clientRect.bottom - 10;
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

   // Fill image background (otherwise it will be black)
   CDC* pDC = CDC::FromHandle(image.GetDC());
   CRect background(CPoint(0, 0), CSize(image.GetWidth(), image.GetHeight()));
   CBrush bgBrush(BACKGROUND_COLOR);
   CPen bgPen(PS_SOLID, 1, BACKGROUND_COLOR);
   CBrush* pOldBrush = pDC->SelectObject(&bgBrush);
   CPen* pOldPen = pDC->SelectObject(&bgPen);
   pDC->Rectangle(background);


   CFont font;
   font.Attach(GetStockObject(DEFAULT_GUI_FONT));
   CFont* pOldFont = pDC->SelectObject(&font);

   pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
   pDC->TextOut(sectionRect.CenterPoint().x, sectionRect.top, _T("Discretized Section: Red = Compression, Blue = Tension"));
   pDC->TextOut(strainRect.CenterPoint().x, strainRect.top, _T("Strains: Orange = initial strain, Gray = change in strain, Blue = total strain"));
   pDC->TextOut(stressRect.CenterPoint().x, stressRect.top, _T("Stress profile and resultant forces"));

   mapper.SetDeviceExt(sectionRect.Width(), sectionRect.Height());
   mapper.SetDeviceOrg(sectionRect.CenterPoint().x, sectionRect.top);

   // draw each slice
   CPen girderPen(PS_SOLID, 1, WHITESMOKE);
   CBrush girderBrush(SEGMENT_FILL_COLOR);
   CBrush voidBrush(VOID_COLOR);
   CBrush tensionBrush(TENSION_COLOR);
   CBrush compressionBrush(COMPRESSION_COLOR);

   pOldPen = pDC->SelectObject(&girderPen);
   pOldBrush = pDC->SelectObject(&girderBrush);

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
         pDC->SelectObject(&girderBrush);
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

   const auto& strain_plane = solution.GetIncrementalStrainPlane();

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
      mapper.WPtoDP(initial_strain * scale, Ycg, &p.x, &p.y);
      rect.MoveToXY(p);
      pDC->Ellipse(&rect);

      pDC->SelectObject(&incrementalStrainPen);
      mapper.WPtoDP(incremental_strain * scale, Ycg, &p.x, &p.y);
      rect.MoveToXY(p);
      pDC->Ellipse(&rect);

      pDC->SelectObject(&totalStrainPen);
      mapper.WPtoDP(total_strain * scale, Ycg, &p.x, &p.y);
      rect.MoveToXY(p);
      pDC->Ellipse(&rect);
   }
   pDC->SelectObject(&pen);

   //
   // Draw stresses and resultant forces
   //
   mapper.SetDeviceExt(stressRect.Width(), stressRect.Height());
   mapper.SetDeviceOrg(stressRect.left + stressRect.Width() / 2, stressRect.top);

   auto T = solution.GetTensionResultant();
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
   Float64 true_maxStress = -Float64_Max;
   for (auto& pieceInfo : vPieces)
   {
      auto fgStress = pieceInfo.first->GetForegroundStress();
      auto bgStress = pieceInfo.first->GetBackgroundStress();
      auto stress = fgStress - bgStress;

      true_maxStress = Max(stress, true_maxStress);

      const auto& shape = pieceInfo.first->GetShape();
      auto generic_shape = std::dynamic_pointer_cast<const WBFL::Geometry::GenericShape>(shape);
      if (generic_shape) stress = 0;

      minStress = Min(stress, minStress);
      maxStress = Max(stress, maxStress);
   }

   Float64 stress_range = maxStress - minStress;
   scale = (wx / 2) / stress_range;
   //mapper.SetWorldExt(stress_range*scale, wy);

   bool bMin = false;
   bool bMax = false;
   for (auto& pieceInfo : vPieces)
   {
      switch (pieceInfo.second)
      {
      case Neutral:
         pDC->SelectObject(&girderBrush);
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

      CPoint start;
      mapper.WPtoDP(0, Ycg, &start.x, &start.y);
      pDC->MoveTo(start);
      CPoint end;
      mapper.WPtoDP(scale * stress, Ycg, &end.x, &end.y);
      pDC->LineTo(end);

      if (IsEqual(stress, true_maxStress) and !bMax)
      {
         CString strStress;
         strStress.Format(_T("%s"), FormatDimension(stress, pDisplayUnits->Stress));
         pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
         pDC->TextOut(start.x, start.y, strStress);
         bMax = true;
      }

      if (IsEqual(stress, minStress) and !bMin)
      {
         CString strStress;
         strStress.Format(_T("%s"), FormatDimension(stress, pDisplayUnits->Stress));
         pDC->SetTextAlign(TA_LEFT | TA_BOTTOM);
         pDC->TextOut(end.x, end.y, strStress);
         bMin = true;
      }
   }

   // Draw the compression resultant
   CPen cPen(PS_SOLID, 5, COMPRESSION_COLOR);
   pDC->SelectObject(&cPen);

   const auto& pntC = solution.GetCompressionResultantLocation();
   auto yc = pntC.Y();

   auto C = solution.GetCompressionResultant();
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
   strC.Format(_T("%s"), FormatDimension(C, pDisplayUnits->GeneralForce));
   pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
   pDC->TextOut(p.x, p.y, strC);

   // Draw the tension resultant
   CPen tPen(PS_SOLID, 5, TENSION_COLOR);
   pDC->SelectObject(&tPen);

   const auto& pntT = solution.GetTensionResultantLocation();
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
   strT.Format(_T("%s"), FormatDimension(T, pDisplayUnits->GeneralForce));
   pDC->SetTextAlign(TA_CENTER | TA_BOTTOM);
   pDC->TextOut(p.x, p.y, strT);

   pDC->SelectObject(&pen);
   pDC->MoveTo(p.x, pntCompression.y);
   pDC->LineTo(p.x, pntTension.y);

   CString strMomentArm;
   strMomentArm.Format(_T("%s"), FormatDimension(yc - yt, pDisplayUnits->ComponentDim));
   pDC->TextOut(p.x, (pntTension.y + pntCompression.y) / 2, strMomentArm);

   pDC->SelectObject(pOldBrush);
   pDC->SelectObject(pOldPen);
   pDC->SelectObject(pOldFont);

   image.ReleaseDC();
}

void CRCCapacityChapterBuilder::DrawSlice(const WBFL::Geometry::Shape& shape, CDC* pDC, WBFL::Graphing::PointMapper& mapper) const
{
   auto poly_points = shape.GetPolyPoints();

   IndexType nPoints = poly_points.size();

   if (nPoints < 3)
   {
      const auto& pnt = poly_points[0];
      auto [x, y] = pnt.GetLocation();

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

      for (IndexType pntIdx = 0; pntIdx < nPoints; pntIdx++)
      {
         const auto& point = poly_points[pntIdx];
         auto [x, y] = point.GetLocation();

         LONG dx, dy;
         mapper.WPtoDP(x, y, &dx, &dy);

         points[pntIdx] = CPoint(dx, dy);
      }

      pDC->Polygon(points, (int)nPoints);

      delete[] points;
   }
}