///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2024  Washington State Department of Transportation
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


// CRCCapacityReportView view

#include <EAF\EAFReportView.h>

class CRCCapacityReportView : public CEAFReportView
{
   DECLARE_DYNCREATE(CRCCapacityReportView)

protected:
   CRCCapacityReportView();           // protected constructor used by dynamic creation
	virtual ~CRCCapacityReportView();

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

   virtual BOOL CanEditReport() override { return FALSE; }

   LPCTSTR GetReportName() { return _T("Analysis Results"); }

   virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactivateView) override;

protected:
	DECLARE_MESSAGE_MAP()

public:
   virtual void OnInitialUpdate() override;
};
