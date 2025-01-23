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

// RCCapacityReportView.cpp : implementation file
//

#include "stdafx.h"
#include "RCCapacityDoc.h"
#include "RCCapacityReportView.h"
#include "RCCapacityChildFrame.h"
#include "RCCapacityReportSpecification.h"
#include <MFCTools\Format.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRCCapacityReportView

IMPLEMENT_DYNCREATE(CRCCapacityReportView, CEAFReportView)


CRCCapacityReportView::CRCCapacityReportView()
{

}

CRCCapacityReportView::~CRCCapacityReportView()
{
}

BEGIN_MESSAGE_MAP(CRCCapacityReportView, CEAFReportView)
   ON_COMMAND(ID_FILE_PRINT_DIRECT,&CEAFReportView::OnFilePrint)
END_MESSAGE_MAP()

// CRCCapacityReportView diagnostics

#ifdef _DEBUG
void CRCCapacityReportView::AssertValid() const
{
	CEAFReportView::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacityReportView::Dump(CDumpContext& dc) const
{
	CEAFReportView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRCCapacityReportView message handlers

void CRCCapacityReportView::OnInitialUpdate()
{
   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)GetDocument();

   auto names = pDoc->GetReportManager()->GetReportNames();
   std::_tstring strName = GetReportName();
   IndexType idx = 0;
   for (const auto& name : names)
   {
      if (strName == name)
      {
         break;
      }
      idx++;
   }

   CEAFReportViewCreationData data;
   data.m_RptIdx = idx;
   data.m_bPromptForSpec = false;
   data.m_pReportBuilderMgr = pDoc->GetReportManager();
   CEAFDocTemplate* pDocTemplate = (CEAFDocTemplate*)pDoc->GetDocTemplate();
   pDocTemplate->SetViewCreationData((void*)&data);

   CEAFReportView::OnInitialUpdate();
}

void CRCCapacityReportView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactivateView)
{
   __super::OnActivateView(bActivate, pActivateView, pDeactivateView);
   if (bActivate)
   {
      RefreshReport();
   }
}
