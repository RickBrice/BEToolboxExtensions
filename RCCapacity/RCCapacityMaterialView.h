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


// CRCCapacityView view
#include <EAF\EAFView.h>
#include <EAF\EAFUtilities.h>
#include "RCCapacityChildFrame.h"
#include "RCCapacityDoc.h"
#include <array>

class CMaterialViewController
{
public:
   virtual LPCTSTR GetTitle() const = 0;
   virtual const WBFL::Materials::StressStrainModel& GetMaterial() const = 0;
   virtual void GetSign(Float64* pX, Float64* pY) const = 0;
   virtual std::vector<Float64> GetSpecialPoints() const { return std::vector<Float64>(); }
};

class CDeckMaterialViewController : public CMaterialViewController
{
public:
   virtual LPCTSTR GetTitle() const override { return _T("Deck Concrete"); }
   virtual const WBFL::Materials::StressStrainModel& GetMaterial() const override
   {
      CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
      return pDoc->GetMaterial(Deck);
   }
   virtual void GetSign(Float64* pX, Float64* pY) const override { *pX = -1.0; *pY = -1.0; }
};

class CGirderMaterialViewController : public CMaterialViewController
{
public:
   virtual LPCTSTR GetTitle() const override { return _T("Girder Concrete"); }
   virtual const WBFL::Materials::StressStrainModel& GetMaterial() const override
   {
      CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
      return pDoc->GetMaterial(Girder);
   }
   virtual void GetSign(Float64* pX, Float64* pY) const override { *pX = -1.0; *pY = -1.0; }

   virtual std::vector<Float64> GetSpecialPoints() const override
   {
      CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
      if (pDoc->GetModelData().concreteType == Conventional)
         return __super::GetSpecialPoints();
      else
      {
         const auto& modelData = pDoc->GetModelData();

         std::vector<Float64> vPoints;
         vPoints.push_back(-1.0 * fabs(modelData.ecu));
         Float64 fc = WBFL::Units::ConvertFromSysUnits(modelData.fcBeam, WBFL::Units::Measure::KSI);
         Float64 Ec = 2500.0 * pow(fc, 0.33); // KSI

         Float64 alpha = 0.85;
         Float64 ecp = -1.0 * alpha * fc/ Ec;
         vPoints.push_back(ecp);

         Float64 gamma = 0.85;
         Float64 ftcr = WBFL::Units::ConvertFromSysUnits(modelData.ftcr, WBFL::Units::Measure::KSI);
         Float64 etcr = gamma * ftcr / Ec;
         vPoints.push_back(etcr);

         vPoints.push_back(modelData.etloc);
         return vPoints;
      }
   }
};

class CRebarMaterialViewController : public CMaterialViewController
{
public:
   virtual LPCTSTR GetTitle() const override { return _T("Rebar"); }
   virtual const WBFL::Materials::StressStrainModel& GetMaterial() const override
   {
      CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
      return pDoc->GetMaterial(Rebar);
   }
   virtual void GetSign(Float64* pX, Float64* pY) const override { *pX = 1.0; *pY = 1.0; }
   virtual std::vector<Float64> GetSpecialPoints() const override
   {
      CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
      const auto& modelData = pDoc->GetModelData();

      Float64 fy = WBFL::Materials::Rebar::GetYieldStrength(modelData.RebarType, modelData.RebarGrade);
      Float64 Es = WBFL::Materials::Rebar::GetE(modelData.RebarType, modelData.RebarGrade);

      std::vector<Float64> vPoints;
      vPoints.push_back(fy / Es);
      vPoints.push_back(-fy / Es);
      return vPoints;
   }
};

class CStrandMaterialViewController : public CMaterialViewController
{
public:
   virtual LPCTSTR GetTitle() const override { return _T("Strand"); }
   virtual const WBFL::Materials::StressStrainModel& GetMaterial() const override
   {
      CRCCapacityDoc* pDoc = (CRCCapacityDoc*)EAFGetDocument();
      return pDoc->GetMaterial(Strand);
   }
   virtual void GetSign(Float64* pX, Float64* pY) const override { *pX = 1.0; *pY = 1.0; }
};

class CRCCapacityMaterialView : public CEAFView
{
	DECLARE_DYNCREATE(CRCCapacityMaterialView)

protected:
   CRCCapacityMaterialView();           // protected constructor used by dynamic creation
	virtual ~CRCCapacityMaterialView();

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   CComboBox m_cbMaterial;
   CFont   m_fnEdit;

   WBFL::Units::ScalarData m_Scalar;

   CDeckMaterialViewController m_DeckController;
   CGirderMaterialViewController m_GirderController;
   CRebarMaterialViewController m_RebarController;
   CStrandMaterialViewController m_StrandController;
   CMaterialViewController* m_pViewController;
   std::array<CMaterialViewController*, 4> m_ViewControllers{ &m_DeckController,&m_GirderController,&m_RebarController,&m_StrandController };

   CRCCapacityChildFrame* m_pFrame;
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnMaterialChanged();

   void OnDraw(CDC* pDC) override;
   CRect GetDrawingRect();

	DECLARE_MESSAGE_MAP()

public:
   virtual void OnInitialUpdate() override;
   virtual void OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint) override;
   afx_msg void OnSize(UINT nType, int cx, int cy);
};


