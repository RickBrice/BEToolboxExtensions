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

// RCCapacitySectionView.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "RCCapacityInputView.h"
#include "RCCapacityDoc.h"
#include "RCCapacityMainView.h"
#include "GirderDimensionsDLg.h"
#include <GraphicsLib\GraphicsLib.h>

#include <EAF\EAFHints.h>
#include <MfcTools\CustomDDX.h>

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRCCapacityInputView

IMPLEMENT_DYNCREATE(CRCCapacityInputView, CFormView)

CRCCapacityInputView::CRCCapacityInputView() :
   CFormView(IDD_RCSECTION)
{
   m_pSectionView = nullptr;
}

CRCCapacityInputView::~CRCCapacityInputView()
{
}

BEGIN_MESSAGE_MAP(CRCCapacityInputView, CFormView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CRCCapacityResultsView::OnFilePrint)
   ON_CBN_SELCHANGE(IDC_TYPE, OnTypeChanged)
   ON_CBN_SELCHANGE(IDC_GIRDERS, OnGirderChanged)
   ON_BN_CLICKED(IDC_HAS_SLAB, &CRCCapacityInputView::OnBnClickedHasSlab)
   ON_BN_CLICKED(IDC_ADD_REBAR, OnAddRebar)
   ON_BN_CLICKED(IDC_REMOVE_REBAR, OnRemoveRebar)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE_REBAR, OnUpdateRemoveRebar)
   ON_BN_CLICKED(IDC_ADD_STRANDS, OnAddStrands)
   ON_BN_CLICKED(IDC_REMOVE_STRANDS, OnRemoveStrands)
   ON_UPDATE_COMMAND_UI(IDC_REMOVE_STRANDS, OnUpdateRemoveStrands)
   ON_BN_CLICKED(IDC_CONVENTIONAL, OnConcreteType)
   ON_BN_CLICKED(IDC_UHPC, OnConcreteType)
   ON_CBN_SELCHANGE(IDC_STRAND_TYPE, OnStrandTypeChanged)
   ON_CBN_SELCHANGE(IDC_BEAM_TYPE, &CRCCapacityInputView::OnBeamTypeChanged)
   ON_BN_CLICKED(IDC_EDIT_DIMENSIONS, OnEditDimensions)
END_MESSAGE_MAP()

// CRCCapacityResultsView diagnostics

#ifdef _DEBUG
void CRCCapacityInputView::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
   //__super::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacityInputView::Dump(CDumpContext& dc) const
{
   __super::Dump(dc);
}
#endif
#endif //_DEBUG


BOOL CRCCapacityInputView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CRCCapacityInputView::OnInitialUpdate()
{
   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
   m_ModelData = pDoc->GetModelData();

   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
   VERIFY(m_RebarGrid.SubclassDlgItem(IDC_REBAR, this));
   m_RebarGrid.CustomInit();

   VERIFY(m_StrandGrid.SubclassDlgItem(IDC_STRANDS, this));
   m_StrandGrid.CustomInit();

   FillBeamType();
   FillTypeList();
   UpdateGirderList();
   FillStrandSizeList();
   FillStrandTypeList();

   __super::OnInitialUpdate();

   OnBeamTypeChanged();
   OnBnClickedHasSlab();
   OnConcreteType();
   OnStrandTypeChanged();
}

void CRCCapacityInputView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactivateView)
{
   if (!bActivate)
   {
      // view is being deactivated - get the data and set it onto the model
      UpdateData(TRUE);
      CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
      pDoc->SetModelData(m_ModelData);
   }
   else
   {
      OnBeamTypeChanged();
      OnBnClickedHasSlab();
      OnConcreteType();
      OnStrandTypeChanged();
   }

   __super::OnActivateView(bActivate, pActivateView, pDeactivateView);
}

void CRCCapacityInputView::GetRebarType(matRebar::Type& type, matRebar::Grade& grade)
{
   CDataExchange dx(this, TRUE);
   DDX_RebarMaterial(&dx, IDC_REBAR_TYPE, type, grade);
}

BOOL CRCCapacityInputView::HasSlab()
{
   return IsDlgButtonChecked(IDC_HAS_SLAB);
}

void CRCCapacityInputView::DoDataExchange(CDataExchange* pDX)
{
   __super::DoDataExchange(pDX);

   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

   DDX_Control(pDX, IDC_REBAR_TYPE, m_cbRebar);

   DDX_RebarMaterial(pDX, IDC_REBAR_TYPE, m_ModelData.RebarType, m_ModelData.RebarGrade);

   DDX_CBEnum(pDX, IDC_BEAM_TYPE, m_ModelData.beamType);
   DDX_CBIndex(pDX, IDC_TYPE, m_ModelData.typeIdx);
   DDX_CBIndex(pDX, IDC_GIRDERS, m_ModelData.beamIdx);
   DDX_RadioEnum(pDX, IDC_CONVENTIONAL, m_ModelData.concreteType);
   DDX_UnitValueAndTag(pDX, IDC_FC_BEAM, IDC_FC_BEAM_UNIT, m_ModelData.fcBeam, pDisplayUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_EC_BEAM, IDC_EC_BEAM_UNIT, m_ModelData.EcBeam, pDisplayUnits->Stress);
   DDX_Text(pDX, IDC_ECU, m_ModelData.ecu);
   DDX_UnitValueAndTag(pDX, IDC_FTCR, IDC_FTCR_UNIT, m_ModelData.ftcr, pDisplayUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_FTLOC, IDC_FTLOC_UNIT, m_ModelData.ftloc, pDisplayUnits->Stress);
   DDX_Text(pDX, IDC_ETLOC, m_ModelData.etloc);
   DDX_UnitValueAndTag(pDX, IDC_FTOP_BEAM, IDC_FTOP_BEAM_UNIT, m_ModelData.ftBeam, pDisplayUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_FBOTTOM_BEAM, IDC_FBOTTOM_BEAM_UNIT, m_ModelData.fbBeam, pDisplayUnits->Stress);


   DDX_Check(pDX, IDC_HAS_SLAB, m_ModelData.bHasDeck);
   DDX_UnitValueAndTag(pDX, IDC_WIDTH, IDC_WIDTH_UNIT, m_ModelData.DeckWidth, pDisplayUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_DEPTH, IDC_DEPTH_UNIT, m_ModelData.DeckThickness, pDisplayUnits->ComponentDim);
   DDX_UnitValueAndTag(pDX, IDC_FC_DECK, IDC_FC_DECK_UNIT, m_ModelData.fcDeck, pDisplayUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_EC_DECK, IDC_EC_DECK_UNIT, m_ModelData.EcDeck, pDisplayUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_FTOP_DECK, IDC_FTOP_DECK_UNIT, m_ModelData.ftDeck, pDisplayUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_FBOTTOM_DECK, IDC_FBOTTOM_DECK_UNIT, m_ModelData.fbDeck, pDisplayUnits->Stress);

   m_RebarGrid.DoDataExchange(pDX);

   DDX_CBEnum(pDX, IDC_STRAND_SIZE, m_ModelData.StrandSize);
   DDX_CBEnum(pDX, IDC_STRAND_TYPE, m_ModelData.StrandType);
   DDX_Text(pDX, IDC_A, m_ModelData.A);
   DDX_Text(pDX, IDC_B, m_ModelData.B);
   DDX_Text(pDX, IDC_C, m_ModelData.C);
   DDX_UnitValueAndTag(pDX, IDC_EPS, IDC_EPS_UNIT, m_ModelData.Eps, pDisplayUnits->Stress);
   DDX_UnitValueAndTag(pDX, IDC_FPU, IDC_FPU_UNIT, m_ModelData.fpu, pDisplayUnits->Stress);
   DDX_Text(pDX, IDC_ESU, m_ModelData.esu);
   m_StrandGrid.DoDataExchange(pDX);
}

void CRCCapacityInputView::FillBeamType()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_BEAM_TYPE);
   int idx = pCB->AddString(_T("Predefined"));
   pCB->SetItemData(idx, (DWORD_PTR)Predefined);
   idx = pCB->AddString(_T("Wide Flange"));
   pCB->SetItemData(idx, (DWORD_PTR)WideFlange);
   idx = pCB->AddString(_T("NU"));
   pCB->SetItemData(idx, (DWORD_PTR)NU);
}

void CRCCapacityInputView::FillTypeList()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_TYPE);
   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
   IndexType nTypes = pDoc->GetTypeCount();
   for (IndexType typeIdx = 0; typeIdx < nTypes; typeIdx++)
   {
      LPCTSTR name = pDoc->GetTypeName(typeIdx);
      pCB->AddString(name);
   }
   pCB->SetCurSel(m_ModelData.typeIdx);
}

void CRCCapacityInputView::FillStrandSizeList()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_STRAND_SIZE);
   pCB->AddString(_T("0.50\""));
   pCB->AddString(_T("0.52\""));
   pCB->AddString(_T("0.60\""));
   pCB->AddString(_T("0.62\""));
   pCB->AddString(_T("0.70\""));
   pCB->SetCurSel(0);
}

void CRCCapacityInputView::FillStrandTypeList()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_STRAND_TYPE);
   pCB->AddString(_T("Grade 270 Low Relaxation"));
   pCB->AddString(_T("Other"));
   pCB->SetCurSel(0);
}

void CRCCapacityInputView::UpdateGirderList()
{
   CComboBox* pcbType = (CComboBox*)GetDlgItem(IDC_TYPE);
   IndexType typeIdx = (IndexType)(pcbType->GetCurSel());
   if (typeIdx == INVALID_INDEX)
   {
      typeIdx = 0;
   }

   CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
   IndexType nBeams = pDoc->GetBeamCount(typeIdx);

   CComboBox* pcbGirders = (CComboBox*)GetDlgItem(IDC_GIRDERS);
   pcbGirders->ResetContent();
   for (IndexType beamIdx = 0; beamIdx < nBeams; beamIdx++)
   {
      LPCTSTR beam = pDoc->GetBeamName(typeIdx, beamIdx);
      pcbGirders->AddString(beam);
   }
   pcbGirders->SetCurSel(m_ModelData.beamIdx);
}

void CRCCapacityInputView::GetGirder(IndexType& typeIdx, IndexType& beamIdx)
{
   CComboBox* pcbType = (CComboBox*)GetDlgItem(IDC_TYPE);
   typeIdx = (IndexType)(pcbType->GetCurSel());

   CComboBox* pcbGirders = (CComboBox*)GetDlgItem(IDC_GIRDERS);
   beamIdx = (IndexType)(pcbGirders->GetCurSel());
}


void CRCCapacityInputView::OnTypeChanged()
{
   UpdateGirderList();
   OnGirderChanged();
}

void CRCCapacityInputView::OnGirderChanged()
{
   if(m_pSectionView) m_pSectionView->Update();
}


void CRCCapacityInputView::OnBnClickedHasSlab()
{
   // TODO: Add your control notification handler code here
   BOOL bEnable = IsDlgButtonChecked(IDC_HAS_SLAB);
   GetDlgItem(IDC_WIDTH)->EnableWindow(bEnable);
   GetDlgItem(IDC_DEPTH)->EnableWindow(bEnable);

   if (m_pSectionView) m_pSectionView->Update();
}

void CRCCapacityInputView::OnConcreteType()
{
   int show = GetCheckedRadioButton(IDC_CONVENTIONAL, IDC_UHPC) == IDC_CONVENTIONAL ? SW_HIDE : SW_SHOW;
   GetDlgItem(IDC_ECU_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_ECU)->ShowWindow(show);
   GetDlgItem(IDC_FTCR_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_FTCR)->ShowWindow(show);
   GetDlgItem(IDC_FTCR_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_FTLOC_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_FTLOC)->ShowWindow(show);
   GetDlgItem(IDC_FTLOC_UNIT)->ShowWindow(show);
   GetDlgItem(IDC_ETLOC_LABEL)->ShowWindow(show);
   GetDlgItem(IDC_ETLOC)->ShowWindow(show);
}

void CRCCapacityInputView::OnAddRebar()
{
   m_RebarGrid.AddRow();
   if (m_pSectionView) m_pSectionView->Update();
}

void CRCCapacityInputView::OnRemoveRebar()
{
   m_RebarGrid.RemoveSelectedRows();
   if (m_pSectionView) m_pSectionView->Update();
}

void CRCCapacityInputView::OnUpdateRemoveRebar(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_RebarGrid.AreRowsSelected());
}

void CRCCapacityInputView::OnAddStrands()
{
   m_StrandGrid.AddRow();
   if (m_pSectionView) m_pSectionView->Update();
}

void CRCCapacityInputView::OnRemoveStrands()
{
   m_StrandGrid.RemoveSelectedRows();
   if (m_pSectionView) m_pSectionView->Update();
}

void CRCCapacityInputView::OnUpdateRemoveStrands(CCmdUI* pCmdUI)
{
   pCmdUI->Enable(m_StrandGrid.AreRowsSelected());
}

void CRCCapacityInputView::OnStrandTypeChanged()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_STRAND_TYPE);
   int nCmdShow = ((StrandType)pCB->GetCurSel() == Other) ? SW_SHOW : SW_HIDE;
   GetDlgItem(IDC_A)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_B)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_C)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_EPS)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_EPS_UNIT)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_FPU)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_FPU_UNIT)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_ESU)->ShowWindow(nCmdShow);

   GetDlgItem(IDC_A_LABEL)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_B_LABEL)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_C_LABEL)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_EPS_LABEL)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_FPU_LABEL)->ShowWindow(nCmdShow);
   GetDlgItem(IDC_ESU_LABEL)->ShowWindow(nCmdShow);

   GetDlgItem(IDC_STAINLESS_HINT)->ShowWindow(nCmdShow);
}

void CRCCapacityInputView::OnBeamTypeChanged()
{
    // TODO: Add your control notification handler code here
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_BEAM_TYPE);
   int curSel = pCB->GetCurSel();
   BeamType beamType = (BeamType)(pCB->GetItemData(curSel));
   GetDlgItem(IDC_TYPE)->ShowWindow(beamType == Predefined ? SW_SHOW : SW_HIDE);
   GetDlgItem(IDC_GIRDERS)->ShowWindow(beamType == Predefined ? SW_SHOW : SW_HIDE);

   GetDlgItem(IDC_EDIT_DIMENSIONS)->ShowWindow(beamType == Predefined ? SW_HIDE : SW_SHOW);
}

void CRCCapacityInputView::OnEditDimensions()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_BEAM_TYPE);
   int curSel = pCB->GetCurSel();
   BeamType beamType = (BeamType)(pCB->GetItemData(curSel));

   CGirderDimensionsDlg dlg(beamType,m_ModelData.dimensions);
   if (dlg.DoModal() == IDOK)
   {
      m_ModelData.dimensions = dlg.m_Values;
   }
}
