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

#pragma once
#include <ReportManager\ChapterBuilder.h>
#include <Graphing/PointMapper.h>
#include <atlimage.h>

#include "RCCapacityDoc.h"

class CRCCapacityDoc;
class rptRcImage;

namespace WBFL
{
   namespace RCSection
   {
      class MomentCapacitySolution;
   }

   namespace Geometry
   {
      class Shape;
   }
}

class CRCCapacityChapterBuilder :
   public WBFL::Reporting::ChapterBuilder
{
public:
   CRCCapacityChapterBuilder(CRCCapacityDoc* pDoc);
   ~CRCCapacityChapterBuilder(void);

   virtual LPCTSTR GetName() const override;
   virtual Uint16 GetMaxLevel() const override;
   virtual rptChapter* Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec,Uint16 level) const override;
   virtual bool Select() const override { return true; }
   virtual std::unique_ptr<WBFL::Reporting::ChapterBuilder> Clone() const override;

private:
   void ReportStrainCompatibility(rptChapter* pChapter,ConcreteModel concreteModel, rptRcTable* pSummaryTable, ColumnIndexType col) const;
   void ReportClosedForm(rptChapter* pChapter, rptRcTable* pSummaryTable, ColumnIndexType col) const;

   CRCCapacityDoc* m_pDoc;

   rptRcImage* CreateImage(const WBFL::RCSection::MomentCapacitySolution& solution, bool bPositiveMoment, const WBFL::Units::IndirectMeasure* pDisplayUnits) const;
   void DrawSection(CImage& image, const WBFL::RCSection::MomentCapacitySolution& solution, bool bPositiveMoment, const WBFL::Units::IndirectMeasure* pDisplayUnits) const;
   void DrawSlice(const WBFL::Geometry::Shape& shape, CDC* pDC, WBFL::Graphing::PointMapper& mapper) const;

   // This is a list of temporary files that were created on the fly
   // Delete them in the destructor
   std::vector<std::_tstring> m_TemporaryImageFiles;
};
