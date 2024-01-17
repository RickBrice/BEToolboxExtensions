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

#pragma once

// CSVTToolView3D view
#include "SVTToolChildFrame.h"
#include "SVTToolViewer3D.h"

interface IPoint2dCollection;
interface IPoint3dCollection;

class CSVTToolView3D : public CView
{
	DECLARE_DYNCREATE(CSVTToolView3D)

protected:
   CSVTToolView3D();           // protected constructor used by dynamic creation
	virtual ~CSVTToolView3D();

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   virtual void OnDraw(CDC* pDC) override;      // overridden to draw this view

   CSVTToolChildFrame* m_pFrame;
   CSVTToolViewer3D* m_pViewer;

   SoGroup* m_pGirder;
   SoGroup* m_pGrid;
   SoGroup* m_pMembrane;

   void BuildGirderShape();
   void BuildGrid();
   void BuildMembrane();
#if defined USE_COM_GEOMETRY
   SoGroup* BuildFacet(IPoint2dCollection* pPoints, const std::tuple<Float64, Float64, Float64>& color);
#else
   SoGroup* BuildFacet(const std::vector<WBFL::Geometry::Point2d>& points, const std::tuple<Float64, Float64, Float64>& color);
#endif

	DECLARE_MESSAGE_MAP()
public:
   virtual void OnInitialUpdate() override;
   virtual void OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint) override;
   afx_msg void OnSize(UINT nType, int cx, int cy);
};


