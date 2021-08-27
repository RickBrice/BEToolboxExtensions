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

// GirderDimensionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "GirderDimensionsDlg.h"


// CGirderDimensionsDlg dialog

IMPLEMENT_DYNAMIC(CGirderDimensionsDlg, CDialog)

CGirderDimensionsDlg::CGirderDimensionsDlg(BeamType beamType, const std::array<Float64, 14>& values, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GIRDER_DIMENSIONS, pParent), m_Values(values)
{
	m_BeamType = beamType;
}

CGirderDimensionsDlg::~CGirderDimensionsDlg()
{
}

void CGirderDimensionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_MetaFileStatic(pDX, IDC_PICTURE, m_GirderPicture, AfxGetInstanceHandle(), m_BeamType == WideFlange ? _T("WideFlange") : _T("NU"), _T("Metafile"), EMF_FIT);

	CEAFApp* pApp = EAFGetApp();
	const unitmgtIndirectMeasure* pDisplayUnits = pApp->GetDisplayUnits();

	DDX_UnitValueAndTag(pDX, IDC_VALUE_1,  IDC_UNIT_1,  m_Values[0], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_2,  IDC_UNIT_2,  m_Values[1], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_3,  IDC_UNIT_3,  m_Values[2], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_4,  IDC_UNIT_4,  m_Values[3], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_5,  IDC_UNIT_5,  m_Values[4], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_6,  IDC_UNIT_6,  m_Values[5], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_7,  IDC_UNIT_7,  m_Values[6], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_8,  IDC_UNIT_8,  m_Values[7], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_9,  IDC_UNIT_9,  m_Values[8], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_10, IDC_UNIT_10, m_Values[9], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_11, IDC_UNIT_11, m_Values[10], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_12, IDC_UNIT_12, m_Values[11], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_13, IDC_UNIT_13, m_Values[12], pDisplayUnits->ComponentDim);
	DDX_UnitValueAndTag(pDX, IDC_VALUE_14, IDC_UNIT_14, m_Values[13], pDisplayUnits->ComponentDim);
}


BEGIN_MESSAGE_MAP(CGirderDimensionsDlg, CDialog)
END_MESSAGE_MAP()


// CGirderDimensionsDlg message handlers


BOOL CGirderDimensionsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if (m_BeamType == WideFlange)
	{
		// this order much match the enum in the IBeam namespace
		GetDlgItem(IDC_LABEL_1)->SetWindowText(_T("C1"));
		GetDlgItem(IDC_LABEL_2)->SetWindowText(_T("D1"));
		GetDlgItem(IDC_LABEL_3)->SetWindowText(_T("D2"));
		GetDlgItem(IDC_LABEL_4)->SetWindowText(_T("D3"));
		GetDlgItem(IDC_LABEL_5)->SetWindowText(_T("D4"));
		GetDlgItem(IDC_LABEL_6)->SetWindowText(_T("D5"));
		GetDlgItem(IDC_LABEL_7)->SetWindowText(_T("D6"));
		GetDlgItem(IDC_LABEL_8)->SetWindowText(_T("D7"));
		GetDlgItem(IDC_LABEL_9)->SetWindowText(_T("T1"));
		GetDlgItem(IDC_LABEL_10)->SetWindowText(_T("T2"));
		GetDlgItem(IDC_LABEL_11)->SetWindowText(_T("W1"));
		GetDlgItem(IDC_LABEL_12)->SetWindowText(_T("W2"));
		GetDlgItem(IDC_LABEL_13)->SetWindowText(_T("W3"));
		GetDlgItem(IDC_LABEL_14)->SetWindowText(_T("W4"));
	}
	else
	{
		// this order much match the enum in the _NUBeam namespace
		GetDlgItem(IDC_LABEL_1)->SetWindowText(_T("D1"));
		GetDlgItem(IDC_LABEL_2)->SetWindowText(_T("D2"));
		GetDlgItem(IDC_LABEL_3)->SetWindowText(_T("D3"));
		GetDlgItem(IDC_LABEL_4)->SetWindowText(_T("D4"));
		GetDlgItem(IDC_LABEL_5)->SetWindowText(_T("D5"));
		GetDlgItem(IDC_LABEL_6)->SetWindowText(_T("R1"));
		GetDlgItem(IDC_LABEL_7)->SetWindowText(_T("R2"));
		GetDlgItem(IDC_LABEL_8)->SetWindowText(_T("R3"));
		GetDlgItem(IDC_LABEL_9)->SetWindowText(_T("R4"));
		GetDlgItem(IDC_LABEL_10)->SetWindowText(_T("T"));
		GetDlgItem(IDC_LABEL_11)->SetWindowText(_T("W1"));
		GetDlgItem(IDC_LABEL_12)->SetWindowText(_T("W2"));
		GetDlgItem(IDC_LABEL_13)->SetWindowText(_T("C1"));
		
		GetDlgItem(IDC_LABEL_14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VALUE_14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_UNIT_14)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
