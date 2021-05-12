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

// DlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include <EAF\EAFApp.h>
#include "SVTDlgBar.h"
#include "SVTToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSVTDlgBar

IMPLEMENT_DYNAMIC(CSVTDlgBar, CDialogBar)

CSVTDlgBar::CSVTDlgBar()
{
}

CSVTDlgBar::~CSVTDlgBar()
{
}


BEGIN_MESSAGE_MAP(CSVTDlgBar, CDialogBar)
END_MESSAGE_MAP()


// CM3CDlgBar message handlers

BOOL CSVTDlgBar::Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID)
{
   BOOL bResult = CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID);
   if (bResult)
   {
      FillMeshSizeList();
      FillTypeList();
      UpdateGirderList();
   }

   return bResult;
}

BOOL CSVTDlgBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
   return CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
}

void CSVTDlgBar::FillTypeList()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_TYPE);
   CSVTToolDoc* pDoc = (CSVTToolDoc*)EAFGetDocument();
   IndexType nTypes = pDoc->GetTypeCount();
   for (IndexType typeIdx = 0; typeIdx < nTypes; typeIdx++)
   {
      LPCTSTR name = pDoc->GetTypeName(typeIdx);
      pCB->AddString(name);
   }
   pCB->SetCurSel(0);
}

void CSVTDlgBar::FillMeshSizeList()
{
   CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_MESH_SIZE);
   pCB->SetItemData(pCB->AddString(_T("1/2\"")), 2);
   pCB->SetItemData(pCB->AddString(_T("1/4\"")), 4);
   pCB->SetItemData(pCB->AddString(_T("1/8\"")), 8);
   pCB->SetItemData(pCB->AddString(_T("1/16\"")), 16);
   pCB->SetItemData(pCB->AddString(_T("1/32\"")), 32);
   pCB->SetItemData(pCB->AddString(_T("1/64\"")), 64);
   pCB->SetCurSel(2); // default to 1/8" because this is the study mesh size
}

void CSVTDlgBar::UpdateGirderList()
{
   CComboBox* pcbType = (CComboBox*)GetDlgItem(IDC_TYPE);
   IndexType typeIdx = (IndexType)(pcbType->GetCurSel());
   if (typeIdx == INVALID_INDEX)
   {
      typeIdx = 0;
   }

   CSVTToolDoc* pDoc = (CSVTToolDoc*)EAFGetDocument();
   IndexType nBeams = pDoc->GetBeamCount(typeIdx);

   CComboBox* pcbGirders = (CComboBox*)GetDlgItem(IDC_GIRDERS);
   pcbGirders->ResetContent();
   for (IndexType beamIdx = 0; beamIdx < nBeams; beamIdx++)
   {
      LPCTSTR beam = pDoc->GetBeamName(typeIdx, beamIdx);
      pcbGirders->AddString(beam);
   }
   pcbGirders->SetCurSel(0);
}

void CSVTDlgBar::GetGirder(IndexType& typeIdx, IndexType& beamIdx)
{
   CComboBox* pcbType = (CComboBox*)GetDlgItem(IDC_TYPE);
   typeIdx = (IndexType)(pcbType->GetCurSel());

   CComboBox* pcbGirders = (CComboBox*)GetDlgItem(IDC_GIRDERS);
   beamIdx = (IndexType)(pcbGirders->GetCurSel());
}

Float64 CSVTDlgBar::GetMaxElementSize()
{
   CComboBox* pcbSize = (CComboBox*)GetDlgItem(IDC_MESH_SIZE);
   int curSel = pcbSize->GetCurSel();
   int base = (int)pcbSize->GetItemData(curSel);
   return 1.0 / base;
}

void CSVTDlgBar::DoDataExchange(CDataExchange* pDX)
{
   CDialogBar::DoDataExchange(pDX);
}
