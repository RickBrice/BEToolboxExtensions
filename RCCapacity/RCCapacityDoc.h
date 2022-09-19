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

#pragma once

#include "BEToolboxDoc.h"
#include <WBFLGeometry.h>
#include <WBFLRCCapacity.h>
#include "..\SVT\AbstractBeamFactory.h"

#include <Materials/Materials.h>
#include <LRFD\Lrfd.h>
#include <ReportManager\ReportManager.h>


typedef enum ElementType
{
   Deck,
   Girder,
   Rebar,
   Strand
} ElementType;

typedef enum Measure
{
   TopGirder, // top of girder downward into the girder
   BottomGirder, // bottom of girder upward into the girder (not into deck)
   TopDeck, // top deck downward into the deck (not into girder)
   BottomDeck // bottom of deck upward into the deck
} Measure;

struct RebarData
{
   WBFL::Materials::Rebar::Size size;
   IndexType nBars;
   Float64 spacing;
   Float64 location;
   Measure measure;

   RebarData() 
   {
      size = WBFL::Materials::Rebar::Size::bs3;
      nBars = 1;
      spacing = WBFL::Units::ConvertToSysUnits(2.0,WBFL::Units::Measure::Inch);
      location = WBFL::Units::ConvertToSysUnits(2.0,WBFL::Units::Measure::Inch);
      measure = BottomGirder;
   }
};

struct StrandData
{
   IndexType nStrands;
   Float64 spacing;
   Float64 location;
   Measure measure;
   Float64 fpe;

   StrandData()
   {
      nStrands = 1;
      spacing = WBFL::Units::ConvertToSysUnits(2.0, WBFL::Units::Measure::Inch);
      location = WBFL::Units::ConvertToSysUnits(2.0, WBFL::Units::Measure::Inch);
      measure = BottomGirder;
      fpe = 0.0;
   }
};

typedef enum ConcreteType
{
   Conventional,
   UHPC
} ConcreteType;

typedef enum StrandSize
{
   Strand_050,
   Strand_052,
   Strand_060,
   Strand_062,
   Strand_070
} StrandSize;

typedef enum StrandType
{
   Grade270_LR,
   Other
} StrandType;

typedef enum BeamType
{
   Predefined,
   WideFlange,
   NU
} BeamType;

struct ModelData
{
   BeamType beamType;
   IndexType typeIdx;
   IndexType beamIdx;
   std::array<Float64, 14> dimensions;

   ConcreteType concreteType;
   Float64 fcBeam;
   Float64 EcBeam;
   Float64 ecu;
   Float64 ftcr;
   Float64 ftloc;
   Float64 etloc;
   Float64 ftBeam;
   Float64 fbBeam;

   BOOL bHasDeck;
   Float64 DeckWidth;
   Float64 DeckThickness;
   Float64 fcDeck;
   Float64 EcDeck;
   Float64 ftDeck;
   Float64 fbDeck;

   WBFL::Materials::Rebar::Type RebarType;
   WBFL::Materials::Rebar::Grade RebarGrade;
   std::vector<RebarData> Rebar;

   StrandSize StrandSize;
   StrandType StrandType;
   std::vector<StrandData> Strands;

   Float64 A, B, C, Eps, fpu, esu;

   ModelData()
   {
      beamType = Predefined;
      typeIdx = 0;
      beamIdx = 0;
      dimensions.fill(0.0);
      concreteType = Conventional;
      fcBeam = WBFL::Units::ConvertToSysUnits(5.0, WBFL::Units::Measure::KSI);
      EcBeam = WBFL::Units::ConvertToSysUnits(2500 * pow(5.0, 0.33), WBFL::Units::Measure::KSI);
      ecu = -0.0035;
      ftcr = WBFL::Units::ConvertToSysUnits(1.2, WBFL::Units::Measure::KSI);
      ftloc = WBFL::Units::ConvertToSysUnits(1.2, WBFL::Units::Measure::KSI);
      etloc = 0.0045;
      ftBeam = 0;
      fbBeam = 0;

      bHasDeck = TRUE;
      DeckWidth = WBFL::Units::ConvertToSysUnits(72.0,WBFL::Units::Measure::Inch);
      DeckThickness = WBFL::Units::ConvertToSysUnits(8.0,WBFL::Units::Measure::Inch);
      fcDeck = WBFL::Units::ConvertToSysUnits(4.0, WBFL::Units::Measure::KSI);
      EcDeck = WBFL::Units::ConvertToSysUnits(2500 * pow(4.0, 0.33), WBFL::Units::Measure::KSI);
      ftDeck = 0;
      fbDeck = 0;

      RebarType = WBFL::Materials::Rebar::Type::A615;
      RebarGrade = WBFL::Materials::Rebar::Grade::Grade60;

      StrandSize = Strand_060;
      StrandType = Grade270_LR;
      Eps = 28500;
      A = 887 / Eps;
      B = 112.4;
      C = 7.36;
      Eps = WBFL::Units::ConvertToSysUnits(Eps, WBFL::Units::Measure::KSI);
      fpu = WBFL::Units::ConvertToSysUnits(270.0, WBFL::Units::Measure::KSI);
      esu = 0.035;
   }
};

class CRCCapacityDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CRCCapacityDoc)

public:
   CRCCapacityDoc();
	virtual ~CRCCapacityDoc();

   virtual CString GetToolbarSectionName() override;

   IndexType GetTypeCount() const;
   LPCTSTR GetTypeName(IndexType typeIdx) const;
   IndexType GetBeamCount(IndexType typeIdx) const;
   LPCTSTR GetBeamName(IndexType typeIdx, IndexType beamIdx) const;
   void GetBeamShape(const ModelData& modelData,IShape** pShape) const;


   template <typename T>
   Float64 GetReinforcementLocation(const T& reinf, Float64 Hg, Float64 ts)
   {
      Float64 y;
      switch (reinf.measure)
      {
      case TopGirder: y = -reinf.location;  break;
      case BottomGirder: y = reinf.location - Hg;  break;
      case TopDeck: y = ts - reinf.location;  break;
      case BottomDeck: y = reinf.location;  break;
      }
      return y;
   }

   Float64 GetStrandDiameter(StrandSize size) const;

   void SetModelData(const ModelData& modelData);
   const ModelData& GetModelData() const;


   void GetShape(IShape** ppShape);
   void GetMaterial(ElementType element, IStressStrain** ppMaterial);
   void GetSection(IGeneralSection** ppSection);
   void GetCapacity(IMomentCapacitySolution** ppSolution);

#ifdef _DEBUG
	virtual void AssertValid() const override;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const override;
#endif
#endif

protected:
   // called when a document is created (New or Open)
   virtual BOOL Init() override; 

   // Called by the framework when the document is to be loaded and saved
   virtual HRESULT WriteTheDocument(IStructuredSave* pStrSave) override;
   virtual HRESULT LoadTheDocument(IStructuredLoad* pStrLoad) override;

   virtual void LoadDocumentSettings() override;
   virtual void SaveDocumentSettings() override;

   virtual CString GetDocumentationRootLocation() override;
   virtual CString GetDocumentationSetName() override;
   virtual CString GetDocumentationURL() override;

   afx_msg void OnHelpFinder();
	DECLARE_MESSAGE_MAP()
public:

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument() override;

protected:
   virtual UINT GetToolbarID() override;
   virtual void LoadToolbarResource() override;

private:
   bool m_bUpdateModel; // state flag indicating if the model needs to be update
   bool m_bUpdateSolution; // state flag indicating if the solution needs to be updated (not valid if model state is invalid)

   ModelData m_ModelData;

   std::vector < std::pair<std::_tstring, std::unique_ptr<CAbstractBeamFactory>>> m_BeamFactories;
   CComPtr<IShape> m_pGirderShape;
   CComPtr<IRectangle> m_pSlabShape;
   CComPtr<ICompositeShape> m_pCompositeShape;
   CComPtr<IGeneralSection> m_pSection;

   CComPtr<IMomentCapacitySolver> m_Solver;
   CComPtr<IMomentCapacitySolution> m_pSolution;

   CComPtr<IStressStrain> m_ssDeck;
   CComPtr<IStressStrain> m_ssGirder;
   CComPtr<IStressStrain> m_ssRebar;
   CComPtr<IStressStrain> m_ssStrand;

   CComPtr<IUnitServer> m_UnitServer;
   CComPtr<IUnitConvert> m_UnitConvert;

   void Update();

   void CreateBeam(const std::array<Float64, 14>& dimensions, IPrecastBeam** ppBeam) const;
   void CreateBeam(const std::array<Float64, 14>& dimensions, INUBeam** ppBeam) const;

};
