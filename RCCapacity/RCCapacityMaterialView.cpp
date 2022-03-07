///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2021  Washington State Department of Transportation
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

// RCCapacityMaterialView.cpp : implementation file
//

#include "stdafx.h"
#include "RCCapacityMaterialView.h"
#include "RCCapacityDoc.h"
#include "BEToolboxColors.h"
#include <array>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRCCapacityMaterialView

#define IDC_MATERIAL 1

IMPLEMENT_DYNCREATE(CRCCapacityMaterialView, CEAFView)

CRCCapacityMaterialView::CRCCapacityMaterialView()
{
   m_Scalar.Width = 8;
   m_Scalar.Precision = 0;
   m_Scalar.Format = sysNumericFormatTool::Fixed;

   m_pViewController = m_ViewControllers[0];
}

CRCCapacityMaterialView::~CRCCapacityMaterialView()
{
}

BEGIN_MESSAGE_MAP(CRCCapacityMaterialView, CEAFView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CRCCapacityMaterialView::OnFilePrint)
   ON_WM_SIZE()
   ON_WM_CREATE()
   ON_CBN_SELCHANGE(IDC_MATERIAL,OnMaterialChanged)
END_MESSAGE_MAP()

// CRCCapacityMaterialView diagnostics

#ifdef _DEBUG
void CRCCapacityMaterialView::AssertValid() const
{
   __super::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacityMaterialView::Dump(CDumpContext& dc) const
{
   __super::Dump(dc);
}
#endif
#endif //_DEBUG


// CRCCapacityMaterialView message handlers
void CRCCapacityMaterialView::OnInitialUpdate()
{
   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)GetDocument();

   m_pFrame = (CRCCapacityChildFrame*)GetParentFrame();

   __super::OnInitialUpdate();
}

void CRCCapacityMaterialView::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   __super::OnUpdate(pSender, lHint, pHint);
}

void CRCCapacityMaterialView::OnSize(UINT nType, int cx, int cy)
{
   __super::OnSize(nType, cx, cy);
}

int CRCCapacityMaterialView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   int result = __super::OnCreate(lpCreateStruct);

   m_fnEdit.Attach(GetStockObject(DEFAULT_GUI_FONT));

   VERIFY(m_cbMaterial.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 200, 100), this, IDC_MATERIAL));
   m_cbMaterial.SetFont(&m_fnEdit);

   for (auto* controller : m_ViewControllers)
   {
      m_cbMaterial.AddString(controller->GetTitle());
   }
   m_cbMaterial.SetCurSel(0);

   return result;
}

CRect CRCCapacityMaterialView::GetDrawingRect()
{
   CRect rect;
   GetClientRect(&rect);
   return rect;
}

void CRCCapacityMaterialView::OnDraw(CDC* pDC)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   std::unique_ptr<arvPhysicalConverter> pStrainFormat = std::make_unique<ScalarTool>(m_Scalar);
   std::unique_ptr<arvPhysicalConverter> pStressFormat = std::make_unique<StressTool>(pDispUnits->Stress);
   grGraphXY graph(*pStrainFormat, *pStressFormat);

   graph.SetTitle(m_pViewController->GetTitle());
   //graph.SetSubtitle(_T("GraphSubtitle"));
   graph.SetXAxisTitle(_T("Strain x 1000"));
   CString strYAxisTitle;
   strYAxisTitle.Format(_T("Stress (%s)"), pDispUnits->Stress.UnitOfMeasure.UnitTag().c_str());
   graph.SetYAxisTitle(strYAxisTitle);

   graph.SetClientAreaColor(GRAPH_BACKGROUND);
   graph.SetGridPenStyle(GRAPH_GRID_PEN_STYLE, GRAPH_GRID_PEN_WEIGHT, GRAPH_GRID_COLOR);

   CComPtr<IStressStrain> ss;
   m_pViewController->GetMaterial(&ss);

   Float64 minStrain, maxStrain;
   ss->StrainLimits(&minStrain, &maxStrain);
   if (0.99 < maxStrain)
      maxStrain = 0;

   if (minStrain < -0.99)
      minStrain = 0;

   std::vector<Float64> vStrain = linspace(minStrain, maxStrain, 30);
   std::vector<Float64> vSpecial = m_pViewController->GetSpecialPoints();
   vStrain.insert(std::end(vStrain), std::begin(vSpecial), std::end(vSpecial));
   std::sort(std::begin(vStrain), std::end(vStrain));
   vStrain.erase(std::unique(std::begin(vStrain), std::end(vStrain)), std::end(vStrain));

   Float64 signX(1.0), signY(1.0);
   m_pViewController->GetSign(&signX, &signY);

   IndexType idx = graph.CreateDataSeries(_T(""), PS_SOLID, 1, BLUE);
   for (auto strain : vStrain)
   {
      Float64 stress;
      ss->ComputeStress(strain, &stress);
      stress = ::ConvertFromSysUnits(stress, pDispUnits->Stress.UnitOfMeasure);
      GraphPoint point(signX * strain * 1000, signY * stress);
      graph.AddPoint(idx, point);
   }


   CRect rect = GetDrawingRect();
   graph.SetOutputRect(rect);
   graph.Draw(pDC->GetSafeHdc());
}

void CRCCapacityMaterialView::OnMaterialChanged()
{
   int curSel = m_cbMaterial.GetCurSel();
   curSel = (curSel == CB_ERR ? 0 : curSel);
   m_pViewController = m_ViewControllers[curSel];

   Invalidate();
   UpdateWindow();
}