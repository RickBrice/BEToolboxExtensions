///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2025  Washington State Department of Transportation
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
#include "M3CDoc.h"

class CM3CMaterialDetailsChapterBuilder :
   public WBFL::Reporting::ChapterBuilder
{
public:
   CM3CMaterialDetailsChapterBuilder(CM3CDoc* pDoc);
   ~CM3CMaterialDetailsChapterBuilder(void);

   virtual LPCTSTR GetName() const override;
   virtual Uint16 GetMaxLevel() const override;
   virtual rptChapter* Build(const std::shared_ptr<const WBFL::Reporting::ReportSpecification>& pRptSpec,Uint16 level) const override;
   virtual bool Select() const override { return true; }
   virtual std::unique_ptr<WBFL::Reporting::ChapterBuilder> Clone() const override;

private:
   CM3CDoc* m_pDoc;

   void BuildConcreteModel(rptChapter* pChapter) const;
   void BuildRebarModel(rptChapter* pChapter) const;
   void BuildStrandModel(rptChapter* pChapter) const;
};
