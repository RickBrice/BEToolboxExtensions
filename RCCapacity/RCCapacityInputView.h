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

#pragma once


// CRCCapacityInputView view
#include "RCCapacityChildFrame.h"
#include "RCCapacityDoc.h"
#include "RCCapacitySectionView.h"
#include "RebarGrid.h"
#include "StrandGrid.h"

interface IShape;
interface IGeneralSectionSolution;
class grlibPointMapper;

class CRCCapacityInputView : public CFormView
{
	DECLARE_DYNCREATE(CRCCapacityInputView)

protected:
   CRCCapacityInputView();           // protected constructor used by dynamic creation
	virtual ~CRCCapacityInputView();

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

	CRCCapacitySectionView* m_pSectionView;

	CRebarMaterialComboBox m_cbRebar;
	CRebarGrid m_RebarGrid;
	CStrandGrid m_StrandGrid;
	ModelData m_ModelData;

	friend CRebarGrid;
	friend CStrandGrid;

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL) override;
	virtual void OnInitialUpdate() override;
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactivateView) override;
	virtual void DoDataExchange(CDataExchange* pDX) override;

	afx_msg void OnTypeChanged();
	afx_msg void OnGirderChanged();
	afx_msg void OnBnClickedHasSlab();
	afx_msg void OnAddRebar();
	afx_msg void OnRemoveRebar();
	afx_msg void OnUpdateRemoveRebar(CCmdUI* pCmdUI);
	afx_msg void OnAddStrands();
	afx_msg void OnRemoveStrands();
	afx_msg void OnUpdateRemoveStrands(CCmdUI* pCmdUI);
	afx_msg void OnConcreteType();
	afx_msg void OnStrandTypeChanged();
	afx_msg void OnBeamTypeChanged();
	afx_msg void OnEditDimensions();

	void FillBeamType();
	void FillTypeList();
	void UpdateGirderList();
	void FillStrandSizeList();
	void FillStrandTypeList();

public:
	void SetSectionView(CRCCapacitySectionView* pView) { m_pSectionView = pView; }
	void GetGirder(IndexType& typeIdx, IndexType& beamIdx);
	void GetRebarType(matRebar::Type& type, matRebar::Grade& grade);
	BOOL HasSlab();
	const ModelData& GetModelData() { UpdateData(TRUE); return m_ModelData; };
};


