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

#include "stdafx.h"
#include "..\resource.h"

#include "RCCapacityTabView.h"
#include "RCCapacityMainView.h"
#include "RCCapacityMaterialView.h"
#include "RCCapacityResultsView.h"
#include "RCCapacityReportView.h"
#include "RCCapacityInputView.h"

#include <EAF\EAFHints.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CRCCapacityTabView

IMPLEMENT_DYNCREATE(CRCCapacityTabView, CTabView)

CRCCapacityTabView::CRCCapacityTabView()
{
}

CRCCapacityTabView::~CRCCapacityTabView()
{
}

BEGIN_MESSAGE_MAP(CRCCapacityTabView, CTabView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CRCCapacityTabView::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacityTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPGStableTabView message handlers

int CRCCapacityTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   int result = CTabView::OnCreate(lpCreateStruct);

   CMFCTabCtrl& tabCtrl = GetTabControl();
   tabCtrl.SetActiveTabBoldFont();

   AddView(RUNTIME_CLASS(CRCCapacityMainView), _T("Section"));
   AddView(RUNTIME_CLASS(CRCCapacityMaterialView), _T("Material Models"));
   //AddView(RUNTIME_CLASS(CRCCapacityResultsView), _T("Results"));
   AddView(RUNTIME_CLASS(CRCCapacityReportView), _T("Report"));

   SetActiveView(0);

   return result;
}

void CRCCapacityTabView::OnInitialUpdate()
{
   __super::OnInitialUpdate();
   m_pFrame = (CRCCapacityChildFrame*)GetParentFrame();
   CDocument* pDoc = GetDocument();
   POSITION pos = pDoc->GetFirstViewPosition();
   while (pos != NULL)
   {
      CView* pView = pDoc->GetNextView(pos);
      if (pView->IsKindOf(RUNTIME_CLASS(CRCCapacityInputView)))
      {
         m_pFrame->SetActiveView(pView);
         pos = NULL;
      }
   }
}

void CRCCapacityTabView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   __super::OnUpdate(pSender, lHint, pHint);
}
