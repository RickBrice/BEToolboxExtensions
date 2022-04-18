///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2022  Washington State Department of Transportation
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


// CRCCapacitySectionView view
#include <DManip\DManip.h>


class CRCCapacityInputView;

class CRCCapacitySectionView : public CDisplayView
{
	DECLARE_DYNCREATE(CRCCapacitySectionView)

protected:
   CRCCapacitySectionView();           // protected constructor used by dynamic creation
	virtual ~CRCCapacitySectionView();

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   CRCCapacityInputView* m_pInputView;

	DECLARE_MESSAGE_MAP()

public:
   void SetInputView(CRCCapacityInputView* pView) { m_pInputView = pView; }
   virtual void OnInitialUpdate() override;
   virtual void OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint) override;
   afx_msg void OnSize(UINT nType, int cx, int cy);

   void Update(); // called from InputView
};


