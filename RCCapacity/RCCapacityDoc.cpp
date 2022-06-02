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

// RCCapacityDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "RCCapacityDoc.h"
#include "RCCapacityChildFrame.h"
#include "BEToolboxStatusBar.h"
#include "RCCapacity.hh"

#include "RCCapacityChapterBuilder.h"

#include "..\SVT\AASHTOBeamFactory.h"
#include "..\SVT\CDOTBeamFactory.h"
#include "..\SVT\CTBeamFactory.h"
#include "..\SVT\FloridaBeamFactory.h"
#include "..\SVT\ILBeamFactory.h"
#include "..\SVT\MNBeamFactory.h"
#include "..\SVT\NUBeamFactory.h"
#include "..\SVT\NEBeamFactory.h"
#include "..\SVT\NorthCarolinaBeamFactory.h"
#include "..\SVT\OhioBeamFactory.h"
#include "..\SVT\OregonBeamFactory.h"
#include "..\SVT\TxDOTBeamFactory.h"
#include "..\SVT\VirginiaBeamFactory.h"
#include "..\SVT\WSDOTBeamFactory.h"
#include "..\SVT\Helpers.h"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <EAF\EAFHelp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DumpSolution(IGeneralSection* section, IMomentCapacitySolution* solution)
{
   USES_CONVERSION;

   std::ostringstream os;
   
   CComPtr<IGeneralSectionSolution> general_solution;
   solution->get_GeneralSectionSolution(&general_solution);

   IndexType nSlices;
   general_solution->get_SliceCount(&nSlices);
   os << "Element, Material, Top, Bottom, Ycg, Area, initial strain, incremental strain, total strain, stress, force, moment" << std::endl;
   TRACE(os.str().c_str());

   for (IndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
   {
      CComPtr<IGeneralSectionSlice> slice;
      general_solution->get_Slice(sliceIdx, &slice);
      Float64 area;
      CComPtr<IPoint2d> cg;
      Float64 initial_strain;
      Float64 incremental_strain;
      Float64 total_strain;
      Float64 fgStress;
      slice->get_Area(&area);
      slice->get_CG(&cg);
      slice->get_InitialStrain(&initial_strain);
      slice->get_IncrementalStrain(&incremental_strain);
      slice->get_TotalStrain(&total_strain);
      slice->get_ForegroundStress(&fgStress);

      CComPtr<IStressStrain> ss;
      slice->get_ForegroundMaterial(&ss);
      CComBSTR bstrMaterial;
      ss->get_Name(&bstrMaterial);

      Float64 y;
      cg->get_Y(&y);

      CComPtr<IShape> shape;
      slice->get_Shape(&shape);
      CComPtr<IRect2d> rect;
      shape->get_BoundingBox(&rect);
      Float64 top, bottom;
      rect->get_Top(&top);
      rect->get_Bottom(&bottom);

      IndexType shapeIdx;
      slice->get_ShapeIndex(&shapeIdx);
      CComBSTR bstrName;
      section->get_Name(shapeIdx, &bstrName);

      std::ostringstream os;
      os << OLE2A(bstrName) << ", " << OLE2A(bstrMaterial) << ", " << top << ", " << bottom << ", " << y << ", " << area << ", " << initial_strain << ", " << incremental_strain << ", " << total_strain << ", " << fgStress << ", " << area * fgStress << ", " << area * fgStress * y << std::endl;
      TRACE(os.str().c_str());
   }
}

// CRCCapacityDoc

IMPLEMENT_DYNCREATE(CRCCapacityDoc, CBEToolboxDoc)

CRCCapacityDoc::CRCCapacityDoc()
{
   m_BeamFactories.push_back(std::make_pair(_T("AASHTO"), std::make_unique<CAASHTOBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("California"), std::make_unique<CCTBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Colorado"), std::make_unique<CCDOTBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Florida"), std::make_unique<CFloridaBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Illinios"), std::make_unique<CILBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Minnesota"), std::make_unique<CMNBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Nebraska"), std::make_unique<CNUBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("New England"), std::make_unique<CNEBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("North Carolina"), std::make_unique<CNCBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Ohio"), std::make_unique<COhioBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Oregon"), std::make_unique<COregonBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Texas"), std::make_unique<CTxDOTBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Virginia"), std::make_unique<CVirginiaBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Washington"), std::make_unique<CWSDOTBeamFactory>()));

   EnableUIHints(FALSE); // not using UIHints feature

   m_UnitServer.CoCreateInstance(CLSID_UnitServer);
   m_UnitServer->get_UnitConvert(&m_UnitConvert);

   m_Solver.CoCreateInstance(CLSID_MomentCapacitySolver);
   m_Solver->put_Slices(50);
   m_Solver->put_SliceGrowthFactor(1);
   m_Solver->put_AxialTolerance(0.0001);

   m_bUpdateModel = true;
   m_bUpdateSolution = true;;


   // when report printing is implemented, we'll need a title page... this commented out code shows how to do it
   //std::shared_ptr<CTitlePageBuilder> pTitlePageBuilder(std::make_shared<CM3CTitlePageBuilder>());
   std::unique_ptr<CReportBuilder> pRptBuilder(std::make_unique<CReportBuilder>(_T("Analysis Results")));
   //pRptBuilder->AddTitlePageBuilder(pTitlePageBuilder);
   pRptBuilder->AddChapterBuilder(std::shared_ptr<CChapterBuilder>(new CRCCapacityChapterBuilder(this)));
   m_RptMgr.AddReportBuilder(pRptBuilder.release());
}

CRCCapacityDoc::~CRCCapacityDoc()
{
}


BEGIN_MESSAGE_MAP(CRCCapacityDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CRCCapacityDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_RCCAPACITY);
}


// CRCCapacityDoc diagnostics

#ifdef _DEBUG
void CRCCapacityDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacityDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CRCCapacityDoc::Init()
{
   if (!CBEToolboxDoc::Init())
   {
      return FALSE;
   }

   m_pSlabShape.CoCreateInstance(CLSID_Rect);
   m_pCompositeShape.CoCreateInstance(CLSID_CompositeShape);
   CComQIPtr<IShape> slab_shape(m_pSlabShape);
   m_pCompositeShape->AddShape(slab_shape, VARIANT_FALSE);

   m_BeamFactories[0].second->CreateBeam(0, m_UnitConvert, &m_pGirderShape);
   m_pCompositeShape->AddShape(m_pGirderShape, VARIANT_FALSE);


   return TRUE;
}

void CRCCapacityDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CRCCapacityDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("RCCapacity"),1.0);
   if ( FAILED(hr) )
      return hr;

   pStrSave->put_Property(_T("BeamType"), CComVariant(m_ModelData.beamType));
   if (m_ModelData.beamType == Predefined)
   {
      pStrSave->put_Property(_T("Catalog"), CComVariant(m_ModelData.typeIdx));
      pStrSave->put_Property(_T("Beam"), CComVariant(m_ModelData.beamIdx));
   }
   else
   {
      int i = 0;
      for (auto value : m_ModelData.dimensions)
      {
         CString str;
         str.Format(_T("Value%d"), i++);
         pStrSave->put_Property(str, CComVariant(value));
      }
   }

   pStrSave->put_Property(_T("BeamConcreteType"), CComVariant(m_ModelData.concreteType));
   pStrSave->put_Property(_T("Beam_fc"), CComVariant(m_ModelData.fcBeam));
   pStrSave->put_Property(_T("Beam_Ec"), CComVariant(m_ModelData.EcBeam));
   if (m_ModelData.concreteType == UHPC)
   {
      pStrSave->put_Property(_T("ecu"), CComVariant(m_ModelData.ecu));
      pStrSave->put_Property(_T("ftcr"), CComVariant(m_ModelData.ftcr));
      pStrSave->put_Property(_T("ftloc"), CComVariant(m_ModelData.ftloc));
      pStrSave->put_Property(_T("etloc"), CComVariant(m_ModelData.etloc));
   }
   pStrSave->put_Property(_T("Beam_ft"), CComVariant(m_ModelData.ftBeam));
   pStrSave->put_Property(_T("Beam_fb"), CComVariant(m_ModelData.fbBeam));

   pStrSave->put_Property(_T("HasDeck"), CComVariant(m_ModelData.bHasDeck));
   if (m_ModelData.bHasDeck)
   {
      pStrSave->put_Property(_T("DeckWidth"), CComVariant(m_ModelData.DeckWidth));
      pStrSave->put_Property(_T("DeckThickness"), CComVariant(m_ModelData.DeckThickness));
      pStrSave->put_Property(_T("Deck_fc"), CComVariant(m_ModelData.fcDeck));
      pStrSave->put_Property(_T("Deck_Ec"), CComVariant(m_ModelData.EcDeck));
      pStrSave->put_Property(_T("Deck_ft"), CComVariant(m_ModelData.ftDeck));
      pStrSave->put_Property(_T("Deck_fb"), CComVariant(m_ModelData.fbDeck));
   }

   pStrSave->BeginUnit(_T("Rebar"), 1.0);
   pStrSave->put_Property(_T("RebarType"), CComVariant(m_ModelData.RebarType));
   pStrSave->put_Property(_T("RebarGrade"), CComVariant(m_ModelData.RebarGrade));
   for (auto& rebar : m_ModelData.Rebar)
   {
      pStrSave->BeginUnit(_T("RebarData"), 1.0);
      pStrSave->put_Property(_T("Size"), CComVariant(rebar.size));
      pStrSave->put_Property(_T("nBars"), CComVariant(rebar.nBars));
      pStrSave->put_Property(_T("Spacing"), CComVariant(rebar.spacing));
      pStrSave->put_Property(_T("Location"), CComVariant(rebar.location));
      pStrSave->put_Property(_T("Measure"), CComVariant(rebar.measure));
      pStrSave->EndUnit(); // RebarData
   }
   pStrSave->EndUnit();

   pStrSave->BeginUnit(_T("Strands"), 1.0);
   pStrSave->put_Property(_T("StrandSize"), CComVariant(m_ModelData.StrandSize));
   pStrSave->put_Property(_T("StrandType"), CComVariant(m_ModelData.StrandType));
   if (m_ModelData.StrandType == Other)
   {
      pStrSave->put_Property(_T("A"), CComVariant(m_ModelData.A));
      pStrSave->put_Property(_T("B"), CComVariant(m_ModelData.B));
      pStrSave->put_Property(_T("C"), CComVariant(m_ModelData.C));
      pStrSave->put_Property(_T("Eps"), CComVariant(m_ModelData.Eps));
      pStrSave->put_Property(_T("fpu"), CComVariant(m_ModelData.fpu));
      pStrSave->put_Property(_T("esu"), CComVariant(m_ModelData.esu));
   }

   for (auto& strand : m_ModelData.Strands)
   {
      pStrSave->BeginUnit(_T("StrandData"), 1.0);
      pStrSave->put_Property(_T("nStrands"), CComVariant(strand.nStrands));
      pStrSave->put_Property(_T("Spacing"), CComVariant(strand.spacing));
      pStrSave->put_Property(_T("Location"), CComVariant(strand.location));
      pStrSave->put_Property(_T("Measure"), CComVariant(strand.measure));
      pStrSave->put_Property(_T("fpe"), CComVariant(strand.fpe));
      pStrSave->EndUnit(); // StrandData
   }
   pStrSave->EndUnit(); // Strands

   hr = pStrSave->EndUnit(); // RCCapacity
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CRCCapacityDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("RCCapacity"));
   if ( FAILED(hr) )
      return hr;

   CComVariant var;
   var.vt = VT_I8;
   pStrLoad->get_Property(_T("BeamType"), &var); m_ModelData.beamType = (BeamType)(var.lVal);
   if (m_ModelData.beamType == Predefined)
   {
      var.vt = VT_INDEX;
      pStrLoad->get_Property(_T("Catalog"), &var); m_ModelData.typeIdx = VARIANT2INDEX(var);
      pStrLoad->get_Property(_T("Beam"), &var); m_ModelData.beamIdx = VARIANT2INDEX(var);
   }
   else
   {
      var.vt = VT_R8;
      for (int i = 0; i < m_ModelData.dimensions.size(); i++)
      {
         CString str;
         str.Format(_T("Value%d"), i);
         pStrLoad->get_Property(str, &var); m_ModelData.dimensions[i] = var.dblVal;
      }
   }

   var.vt = VT_I8;
   pStrLoad->get_Property(_T("BeamConcreteType"), &var); m_ModelData.concreteType = (ConcreteType)var.lVal;
   var.vt = VT_R8;
   pStrLoad->get_Property(_T("Beam_fc"), &var); m_ModelData.fcBeam = var.dblVal;
   pStrLoad->get_Property(_T("Beam_Ec"), &var); m_ModelData.EcBeam = var.dblVal;
   if (m_ModelData.concreteType == UHPC)
   {
      pStrLoad->get_Property(_T("ecu"), &var); m_ModelData.ecu = var.dblVal;
      pStrLoad->get_Property(_T("ftcr"), &var); m_ModelData.ftcr = var.dblVal;
      pStrLoad->get_Property(_T("ftloc"), &var); m_ModelData.ftloc = var.dblVal;
      pStrLoad->get_Property(_T("etloc"), &var); m_ModelData.etloc = var.dblVal;
   }
   pStrLoad->get_Property(_T("Beam_ft"), &var); m_ModelData.ftBeam = var.dblVal;
   pStrLoad->get_Property(_T("Beam_fb"), &var); m_ModelData.fbBeam = var.dblVal;

   var.vt = VT_I8;
   pStrLoad->get_Property(_T("HasDeck"), &var); m_ModelData.bHasDeck = (BOOL)var.lVal;
   if (m_ModelData.bHasDeck)
   {
      var.vt = VT_R8;
      pStrLoad->get_Property(_T("DeckWidth"), &var); m_ModelData.DeckWidth = var.dblVal;
      pStrLoad->get_Property(_T("DeckThickness"), &var); m_ModelData.DeckThickness = var.dblVal;
      pStrLoad->get_Property(_T("Deck_fc"), &var); m_ModelData.fcDeck = var.dblVal;
      pStrLoad->get_Property(_T("Deck_Ec"), &var); m_ModelData.EcDeck = var.dblVal;
      pStrLoad->get_Property(_T("Deck_ft"), &var); m_ModelData.ftDeck = var.dblVal;
      pStrLoad->get_Property(_T("Deck_fb"), &var); m_ModelData.fbDeck = var.dblVal;
   }

   pStrLoad->BeginUnit(_T("Rebar"));
   var.vt = VT_I8;
   pStrLoad->get_Property(_T("RebarType"), &var); m_ModelData.RebarType = (matRebar::Type)var.lVal;
   pStrLoad->get_Property(_T("RebarGrade"), &var); m_ModelData.RebarGrade = (matRebar::Grade)var.lVal;
   while(SUCCEEDED(pStrLoad->BeginUnit(_T("RebarData"))))
   {
      RebarData rebar;
      var.vt = VT_I8;
      pStrLoad->get_Property(_T("Size"), &var); rebar.size = (matRebar::Size)var.lVal;
      var.vt = VT_INDEX;
      pStrLoad->get_Property(_T("nBars"), &var); rebar.nBars = VARIANT2INDEX(var);
      var.vt = VT_R8;
      pStrLoad->get_Property(_T("Spacing"), &var); rebar.spacing = var.dblVal;
      pStrLoad->get_Property(_T("Location"), &var); rebar.location = var.dblVal;
      var.vt = VT_I8;
      pStrLoad->get_Property(_T("Measure"), &var); rebar.measure = (Measure)var.lVal;
      pStrLoad->EndUnit(); // RebarData
      m_ModelData.Rebar.emplace_back(rebar);
   }
   pStrLoad->EndUnit(); //Rebar

   pStrLoad->BeginUnit(_T("Strands"));
   var.vt = VT_I8;
   pStrLoad->get_Property(_T("StrandSize"), &var); m_ModelData.StrandSize = (StrandSize)var.lVal;
   pStrLoad->get_Property(_T("StrandType"), &var); m_ModelData.StrandType = (StrandType)var.lVal;
   if (m_ModelData.StrandType == Other)
   {
      var.vt = VT_R8;
      pStrLoad->get_Property(_T("A"), &var); m_ModelData.A = var.dblVal;
      pStrLoad->get_Property(_T("B"), &var); m_ModelData.B = var.dblVal;
      pStrLoad->get_Property(_T("C"), &var); m_ModelData.C = var.dblVal;
      pStrLoad->get_Property(_T("Eps"), &var); m_ModelData.Eps = var.dblVal;
      pStrLoad->get_Property(_T("fpu"), &var); m_ModelData.fpu = var.dblVal;
      pStrLoad->get_Property(_T("esu"), &var); m_ModelData.esu = var.dblVal;
   }

   while(SUCCEEDED(pStrLoad->BeginUnit(_T("StrandData"))))
   {
      StrandData strand;
      var.vt = VT_INDEX;
      pStrLoad->get_Property(_T("nStrands"), &var); strand.nStrands = VARIANT2INDEX(var);
      var.vt = VT_R8;
      pStrLoad->get_Property(_T("Spacing"), &var); strand.spacing = var.dblVal;
      pStrLoad->get_Property(_T("Location"), &var); strand.location = var.dblVal;
      var.vt = VT_I8;
      pStrLoad->get_Property(_T("Measure"), &var); strand.measure = (Measure)var.lVal;
      var.vt = VT_R8;
      pStrLoad->get_Property(_T("fpe"), &var); strand.fpe = var.dblVal;
      pStrLoad->EndUnit(); // StrandData
      m_ModelData.Strands.emplace_back(strand);
   }
   pStrLoad->EndUnit(); // Strands

   hr = pStrLoad->EndUnit(); // RCCapacity
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

void CRCCapacityDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::LoadDocumentSettings();
}

void CRCCapacityDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::SaveDocumentSettings();
}

CString CRCCapacityDoc::GetDocumentationRootLocation()
{
   CEAFApp* pApp = EAFGetApp();
   return pApp->GetDocumentationRootLocation();
}

CString CRCCapacityDoc::GetDocumentationSetName()
{
   // Return the name of your documentation set
   return _T("RCCapacityDocs");
   //return CBEToolboxDoc::GetDocumentationSetName(); // this returns BEToolbox
}

CString CRCCapacityDoc::GetDocumentationURL()
{
   return GetDocumentationRootLocation();
}

CString CRCCapacityDoc::GetToolbarSectionName()
{
   // This is the name that is used in the registry to
   // save/load the toolbar state
   return _T("RCCapacity");
}

UINT CRCCapacityDoc::GetToolbarID()
{
   // return the resource ID for this tool's toolbar
   return IDR_RCCAPACITY;
}

void CRCCapacityDoc::LoadToolbarResource()
{
   // Set the module state to this DLL before the BEToolbox application
   // loads the toolbar resource (identified by the return value of GetToolbarID())
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::LoadToolbarResource();
}

IndexType CRCCapacityDoc::GetTypeCount() const
{
   return (IndexType)m_BeamFactories.size();
}

LPCTSTR CRCCapacityDoc::GetTypeName(IndexType typeIdx) const
{
   return m_BeamFactories[typeIdx].first.c_str();
}

IndexType CRCCapacityDoc::GetBeamCount(IndexType typeIdx) const
{
   return m_BeamFactories[typeIdx].second->GetBeamCount();
}

LPCTSTR CRCCapacityDoc::GetBeamName(IndexType typeIdx, IndexType beamIdx) const
{
   return m_BeamFactories[typeIdx].second->GetBeamName(beamIdx);
}

void CRCCapacityDoc::GetBeamShape(const ModelData& modelData, IShape** ppShape) const
{
   if (modelData.beamType == Predefined)
   {
      m_BeamFactories[modelData.typeIdx].second->CreateBeam(modelData.beamIdx, m_UnitConvert, ppShape);
   }
   else if (modelData.beamType == WideFlange)
   {
      CComPtr<IPrecastBeam> beam;
      CreateBeam(modelData.dimensions, &beam);
      beam.QueryInterface(ppShape);
   }
   else if (modelData.beamType == NU)
   {
      CComPtr<INUBeam> beam;
      CreateBeam(modelData.dimensions, &beam);
      beam.QueryInterface(ppShape);
   }
   else
   {
      ATLASSERT(false);
   }
}

Float64 CRCCapacityDoc::GetStrandDiameter(StrandSize size) const
{
   Float64 dia;
   switch (size)
   {
   case Strand_050: dia = ::ConvertToSysUnits(0.50, unitMeasure::Inch); break;
   case Strand_052: dia = ::ConvertToSysUnits(0.52, unitMeasure::Inch); break;
   case Strand_060: dia = ::ConvertToSysUnits(0.60, unitMeasure::Inch); break;
   case Strand_062: dia = ::ConvertToSysUnits(0.62, unitMeasure::Inch); break;
   case Strand_070: dia = ::ConvertToSysUnits(0.70, unitMeasure::Inch); break;
   default: ATLASSERT(false);
   }
   return dia;
}

void CRCCapacityDoc::SetModelData(const ModelData& modelData)
{
   m_bUpdateModel = true;
   m_bUpdateSolution = true;
   SetModifiedFlag();
   m_ModelData = modelData;
}

const ModelData& CRCCapacityDoc::GetModelData() const
{
   return m_ModelData;
}

void CRCCapacityDoc::GetShape(IShape** ppShape)
{
   Update();
   if (m_ModelData.bHasDeck)
      m_pCompositeShape.QueryInterface(ppShape);
   else
      m_pGirderShape.CopyTo(ppShape);
}

void CRCCapacityDoc::GetMaterial(ElementType element, IStressStrain** ppMaterial)
{
   Update();
   switch (element)
   {
   case Deck:  m_ssDeck.CopyTo(ppMaterial);  break;
   case Girder: m_ssGirder.CopyTo(ppMaterial); break;
   case Rebar: m_ssRebar.CopyTo(ppMaterial); break;
   case Strand: m_ssStrand.CopyTo(ppMaterial); break;
   default: ATLASSERT(false);
   }
}

void CRCCapacityDoc::GetSection(IGeneralSection** ppSection)
{
   Update();
   m_pSection.CopyTo(ppSection);
}

void CRCCapacityDoc::GetCapacity(IMomentCapacitySolution** ppSolution)
{
#pragma Reminder("WORKING HERE - Need to cache solution so it doesn't recompute over and over")
   Update();
   if (m_pSection == nullptr)
   {
      *ppSolution = nullptr;
   }

   if (m_bUpdateSolution)
   {
      // solution needs to be updated
      m_Solver->putref_Section(m_pSection);

      // assume conventional concrete deck and/or girder
      Float64 ec = -0.003;
      Float64 ei = m_ModelData.ftDeck / m_ModelData.EcDeck;
      if (!m_ModelData.bHasDeck && m_ModelData.concreteType == UHPC)
      {
         // if there is no deck and the girder is UHPC, use the UHPC properties
         ec = -fabs(m_ModelData.ecu);
         ei = m_ModelData.ftBeam / m_ModelData.EcBeam;
      }

      // NOTE: The solver will re-use and existing solution object if provided one. This saves the overhead of allocation a new solution object.
      // m_pSolution is of type CComPtr<> and an assert fires on &m_pSolution if the underlying "p" pointer is not null. The assert note for the
      // CComPtr<>::operator& says to take the address of p if you intend to pass in an object... this is what we are doing
      HRESULT hr = m_Solver->Solve(0.00, 0.00, ec - ei, 0.0, smFixedCompressionStrain, &m_pSolution.p);
      if (hr == RC_E_MATERIALFAILURE)
      {
         if (m_ModelData.concreteType == UHPC)
         {
            // this is UHPC so limit tension in the concrete as a first attempt
            hr = m_Solver->Solve(0.00, 0.00, m_ModelData.etloc - m_ModelData.fbBeam/m_ModelData.EcBeam, 0.0, smFixedTensionStrain, &m_pSolution.p);
            //DumpSolution(m_pSection, *ppSolution);
         }

         // reinforcement failed (strain too high)
         if (hr == RC_E_MATERIALFAILURE)
         {
            Float64 Eps = ::ConvertToSysUnits(28500, unitMeasure::KSI); // carbon-steel
            Float64 esu = 0.035; // carbon-steel, 3.5% strain limit
            if (m_ModelData.StrandType == Other)
            {
               Eps = m_ModelData.Eps;
               esu = m_ModelData.esu;
            }

            // height of girder/section
            Float64 Hg;
            CComPtr<IRect2d> bbox;
            m_pGirderShape->get_BoundingBox(&bbox);
            bbox->get_Height(&Hg);

            // find reinforcement that is closest to the bottom of the girder
            Float64 y_rebar = Float64_Max;
            for (const auto& rebar : m_ModelData.Rebar)
            {
               y_rebar = Min(y_rebar, GetReinforcementLocation<RebarData>(rebar, Hg, m_ModelData.DeckThickness));
            }

            Float64 y_strand = Float64_Max;
            for (const auto& strand : m_ModelData.Strands)
            {
               y_strand = Min(y_strand, GetReinforcementLocation<StrandData>(strand, Hg, m_ModelData.DeckThickness));
            }

            Float64 ei_ps = 0;
            if (0 < m_ModelData.Strands.size() && y_strand < y_rebar )
            {
               // if the reinforcement is a strand we need the initial strain
               ei_ps = m_ModelData.Strands.back().fpe / Eps;
            }

            // need to deduct the initial strain from the target strain to cause the solver to seek the correct incremental strain
            hr = m_Solver->Solve(0.00, 0.00, esu - ei_ps, Min(y_strand,y_rebar), smFixedStrain, &m_pSolution.p);
            ATLASSERT(hr != RC_E_MATERIALFAILURE);
         }
      }

      m_bUpdateSolution = false; // solution is up to date
   }

   m_pSolution.CopyTo(ppSolution);
}

void CRCCapacityDoc::CreateBeam(const std::array<Float64,14>& dimensions,IPrecastBeam** ppBeam) const
{
   CComPtr<IPrecastBeam> beam;
   beam.CoCreateInstance(CLSID_PrecastBeam);
   using namespace IBeam;
   beam->put_C1(dimensions[C1]);
   beam->put_D1(dimensions[D1]);
   beam->put_D2(dimensions[D2]);
   beam->put_D3(dimensions[D3]);
   beam->put_D4(dimensions[D4]);
   beam->put_D5(dimensions[D5]);
   beam->put_D6(dimensions[D6]);
   beam->put_D7(dimensions[D7]);
   beam->put_T1(dimensions[T1]);
   beam->put_T2(dimensions[T2]);
   beam->put_W1(dimensions[W1]);
   beam->put_W2(dimensions[W2]);
   beam->put_W3(dimensions[W3]);
   beam->put_W4(dimensions[W4]);

   beam.QueryInterface(ppBeam);
}

void CRCCapacityDoc::CreateBeam(const std::array<Float64, 14>& dimensions, INUBeam** ppBeam) const
{
   CComPtr<INUBeam> beam;
   beam.CoCreateInstance(CLSID_NUBeam);
   using namespace _NUBeam;

   beam->put_D1(dimensions[D1]);
   beam->put_D2(dimensions[D2]);
   beam->put_D3(dimensions[D3]);
   beam->put_D4(dimensions[D4]);
   beam->put_D5(dimensions[D5]);
   beam->put_R1(dimensions[R1]);
   beam->put_R2(dimensions[R2]);
   beam->put_R3(dimensions[R3]);
   beam->put_R4(dimensions[R4]);
   beam->put_T(dimensions[T]);
   beam->put_W1(dimensions[W1]);
   beam->put_W2(dimensions[W2]);
   beam->put_C1(dimensions[C1]);

   beam.QueryInterface(ppBeam);
}

void CRCCapacityDoc::Update()
{
   if (m_bUpdateModel == false)
      return; // model is up to date

   m_pSlabShape.Release();
   m_pGirderShape.Release();
   m_pSection.Release();

   CComPtr<IPoint2d> origin;
   origin.CoCreateInstance(CLSID_Point2d);

   if (m_ModelData.beamType == Predefined)
   {
      m_BeamFactories[m_ModelData.typeIdx].second->CreateBeam(m_ModelData.beamIdx, m_UnitConvert, &m_pGirderShape);
   }
   else if (m_ModelData.beamType == WideFlange)
   {
      CComPtr<IPrecastBeam> beam;
      CreateBeam(m_ModelData.dimensions,&beam);
      beam.QueryInterface(&m_pGirderShape);
   }
   else if (m_ModelData.beamType == NU)
   {
      CComPtr<INUBeam> beam;
      CreateBeam(m_ModelData.dimensions, &beam);
      beam.QueryInterface(&m_pGirderShape);
   }
   else
   {
      ATLASSERT(false); // is there a new beam type
   }

   CComPtr<ICompositeShapeItem> girder_item;
   m_pCompositeShape->get_Item(1, &girder_item);
   girder_item->putref_Shape(m_pGirderShape);

   CComQIPtr<IXYPosition> position(m_pGirderShape);
   position->put_LocatorPoint(lpTopCenter, origin);

   if (m_ModelData.bHasDeck)
   {
      m_pSlabShape.CoCreateInstance(CLSID_Rect);
      m_pSlabShape->put_Height(m_ModelData.DeckThickness);
      m_pSlabShape->put_Width(m_ModelData.DeckWidth);
      position.Release();
      m_pSlabShape->get_XYPosition(&position);
      position->put_LocatorPoint(lpBottomCenter, origin);

      CComPtr<ICompositeShapeItem> slab_item;
      m_pCompositeShape->get_Item(0, &slab_item);
      CComQIPtr<IShape> slab_shape(m_pSlabShape);
      slab_item->putref_Shape(slab_shape);
   }

   m_pSection.CoCreateInstance(CLSID_GeneralSection);

   //
   // materials
   //

   // deck
   CComPtr<IUnconfinedConcrete> deck_concrete;
   deck_concrete.CoCreateInstance(CLSID_UnconfinedConcrete);
   CComQIPtr<ISupportUnitServer> sus(deck_concrete);
   sus->putref_UnitServer(m_UnitServer);
   deck_concrete->put_fc(m_ModelData.fcDeck);
   CComQIPtr<IStressStrain> ss_deck_concrete(deck_concrete);
   m_ssDeck = ss_deck_concrete;

   // girder
   CComPtr<IStressStrain> ss_girder_concrete;
   if (m_ModelData.concreteType == Conventional)
   {
      CComPtr<IUnconfinedConcrete> girder_concrete;
      girder_concrete.CoCreateInstance(CLSID_UnconfinedConcrete);
      sus.Release();
      girder_concrete.QueryInterface(&sus);
      sus->putref_UnitServer(m_UnitServer);
      girder_concrete->put_fc(m_ModelData.fcBeam);
      girder_concrete.QueryInterface(&ss_girder_concrete);
   }
   else
   {
      CComPtr<IUHPConcrete> girder_concrete;
      girder_concrete.CoCreateInstance(CLSID_UHPConcrete);
      sus.Release();
      girder_concrete.QueryInterface(&sus);
      sus->putref_UnitServer(m_UnitServer);
      girder_concrete->put_fc(m_ModelData.fcBeam);
      girder_concrete->put_ecu(-1.0*fabs(m_ModelData.ecu));
      girder_concrete->put_ftcr(m_ModelData.ftcr);
      girder_concrete->put_ftloc(m_ModelData.ftloc);
      girder_concrete->put_etloc(m_ModelData.etloc);
      girder_concrete.QueryInterface(&ss_girder_concrete);
   }
   m_ssGirder = ss_girder_concrete;

   // rebar
   CComPtr<IRebarModel> rebar;
   rebar.CoCreateInstance(CLSID_RebarModel);
   const matRebar* pRebar = lrfdRebarPool::GetInstance()->GetRebar(m_ModelData.RebarType, m_ModelData.RebarGrade, matRebar::bs3); // use any size, we just want the material properties
   Float64 fy = pRebar->GetYieldStrength();
   Float64 Es = pRebar->GetE();
   Float64 e = pRebar->GetElongation();
   rebar->Init(fy,Es,e);
   CComQIPtr<IStressStrain> ss_rebar(rebar);
   m_ssRebar = ss_rebar;

   // strand
   CComPtr<IStressStrain> ss_strand;
   if(m_ModelData.StrandType == Grade270_LR)
   {
      // use the pre-built power formula
      CComPtr<IPowerFormula> strand;
      strand.CoCreateInstance(CLSID_PSPowerFormula);
      strand->put_Grade(sgtGrade270);
      strand->put_ProductionMethod(pmtLowRelaxation);

      sus.Release();
      strand.QueryInterface(&sus);
      sus->putref_UnitServer(m_UnitServer);

      strand.QueryInterface(&ss_strand);
   }
   else
   {
      // use the generalized form of the stress-strain model
      CComPtr<IRambergOsgoodModel> strand;
      strand.CoCreateInstance(CLSID_RambergOsgoodModel);
      strand->Init(m_ModelData.A, m_ModelData.B, m_ModelData.C, m_ModelData.Eps, m_ModelData.fpu, -1.0, m_ModelData.esu);
      strand.QueryInterface(&ss_strand);

      // note: strand doesn't really fail in compression so make the compression strain limit (minStrain) -1
      // to not exceed the strain limit during analysis
   }
   m_ssStrand = ss_strand;


   // Create the general section
   if (m_ModelData.bHasDeck)
   {
      CComPtr<IPlane3d> initialStrain;
      initialStrain.CoCreateInstance(CLSID_Plane3d);
      Float64 e_top = m_ModelData.ftDeck / m_ModelData.EcDeck;
      Float64 e_bot = m_ModelData.fbDeck / m_ModelData.EcDeck;
      CComPtr<IPoint3d> p1, p2, p3;
      p1.CoCreateInstance(CLSID_Point3d);
      p2.CoCreateInstance(CLSID_Point3d);
      p3.CoCreateInstance(CLSID_Point3d);
      p1->Move(-1000, m_ModelData.DeckThickness, e_top);
      p2->Move( 1000, m_ModelData.DeckThickness, e_top);
      p3->Move(0, 0, e_bot);
      initialStrain->ThroughPoints(p1, p2, p3);
      CComQIPtr<IShape> slab_shape(m_pSlabShape);
      m_pSection->AddShape(CComBSTR("Deck"),slab_shape, ss_deck_concrete, nullptr, initialStrain, 1.0, VARIANT_FALSE);
   }

   CComPtr<IRect2d> bbox;
   m_pGirderShape->get_BoundingBox(&bbox);
   Float64 Hg;
   bbox->get_Height(&Hg);
   CComPtr<IPlane3d> initialStrain;
   initialStrain.CoCreateInstance(CLSID_Plane3d);
   Float64 e_top = m_ModelData.ftBeam / m_ModelData.EcBeam;
   Float64 e_bot = m_ModelData.fbBeam / m_ModelData.EcBeam;
   CComPtr<IPoint3d> p1, p2, p3;
   p1.CoCreateInstance(CLSID_Point3d);
   p2.CoCreateInstance(CLSID_Point3d);
   p3.CoCreateInstance(CLSID_Point3d);
   p1->Move(-1000, 0, e_top);
   p2->Move(1000, 0, e_top);
   p3->Move(0, -Hg, e_bot);
   initialStrain->ThroughPoints(p1, p2, p3);
   m_pSection->AddShape(CComBSTR("Girder"),m_pGirderShape, ss_girder_concrete, nullptr, initialStrain, 1.0, VARIANT_TRUE);

   int i = 1;
   for (const auto& rebar_data : m_ModelData.Rebar)
   {
      const matRebar* pRebar = lrfdRebarPool::GetInstance()->GetRebar(m_ModelData.RebarType, m_ModelData.RebarGrade, rebar_data.size);
      Float64 area = pRebar->GetNominalArea();
      area *= rebar_data.nBars;
      CComPtr<IGenericShape> bar_shape;
      bar_shape.CoCreateInstance(CLSID_GenericShape);
      bar_shape->put_Area(area);

      CComPtr<IPoint2d> cg;
      cg.CoCreateInstance(CLSID_Point2d);
      switch (rebar_data.measure)
      {
      case TopGirder:
         {
            CComQIPtr<IXYPosition> position(m_pGirderShape);
            CComPtr<IPoint2d> pntTC;
            position->get_LocatorPoint(lpTopCenter, &pntTC);
            Float64 Y;
            pntTC->get_Y(&Y);
            cg->put_Y(Y - rebar_data.location);
         }
         break;
      case BottomGirder:
         {
            CComQIPtr<IXYPosition> position(m_pGirderShape);
            CComPtr<IPoint2d> pntBC;
            position->get_LocatorPoint(lpBottomCenter, &pntBC);
            Float64 Y;
            pntBC->get_Y(&Y);
            cg->put_Y(Y + rebar_data.location);
         }
         break;
      case TopDeck:
         {
            CComQIPtr<IXYPosition> position(m_pSlabShape);
            CComPtr<IPoint2d> pntTC;
            position->get_LocatorPoint(lpTopCenter, &pntTC);
            Float64 Y;
            pntTC->get_Y(&Y);
            cg->put_Y(Y - rebar_data.location);
         }
         break;
      case BottomDeck:
         {
            CComQIPtr<IXYPosition> position(m_pSlabShape);
            CComPtr<IPoint2d> pntBC;
            position->get_LocatorPoint(lpBottomCenter, &pntBC);
            Float64 Y;
            pntBC->get_Y(&Y);
            cg->put_Y(Y + rebar_data.location);
         }
         break;
      default:
         ATLASSERT(false);
      }

      bar_shape->putref_Centroid(cg);

      CString strName;
      strName.Format(_T("Rebar Layer %d"), i++);

      CComQIPtr<IShape> shape(bar_shape);
      m_pSection->AddShape(CComBSTR(strName),shape, ss_rebar, nullptr, nullptr, 1.0, VARIANT_FALSE);
   } // next rebar

   std::map<StrandSize, matPsStrand::Size> strandSizeMap;
   strandSizeMap.insert(std::make_pair(Strand_050, matPsStrand::D1270));
   strandSizeMap.insert(std::make_pair(Strand_052, matPsStrand::D1320));
   strandSizeMap.insert(std::make_pair(Strand_060, matPsStrand::D1524));
   strandSizeMap.insert(std::make_pair(Strand_062, matPsStrand::D1575));
   strandSizeMap.insert(std::make_pair(Strand_070, matPsStrand::D1778));
   Float64 Eps;
   ss_strand->get_ModulusOfElasticity(&Eps);
   i = 1;
   for (const auto& strand_data : m_ModelData.Strands)
   {
      // get the nominal strand area - it doesn't matter what the grade and production type are, we just want the nominal area
      matPsStrand::Size strand_size = strandSizeMap.find(m_ModelData.StrandSize)->second;
      const matPsStrand* pStrand = lrfdStrandPool::GetInstance()->GetStrand(matPsStrand::Gr1860, matPsStrand::LowRelaxation, matPsStrand::None, strand_size);
      Float64 area = pStrand->GetNominalArea();
      area *= strand_data.nStrands;
      CComPtr<IGenericShape> strand_shape;
      strand_shape.CoCreateInstance(CLSID_GenericShape);
      strand_shape->put_Area(area);

      CComPtr<IPoint2d> cg;
      cg.CoCreateInstance(CLSID_Point2d);
      switch (strand_data.measure)
      {
      case TopGirder:
      {
         CComQIPtr<IXYPosition> position(m_pGirderShape);
         CComPtr<IPoint2d> pntTC;
         position->get_LocatorPoint(lpTopCenter, &pntTC);
         Float64 Y;
         pntTC->get_Y(&Y);
         cg->put_Y(Y - strand_data.location);
      }
      break;
      case BottomGirder:
      {
         CComQIPtr<IXYPosition> position(m_pGirderShape);
         CComPtr<IPoint2d> pntBC;
         position->get_LocatorPoint(lpBottomCenter, &pntBC);
         Float64 Y;
         pntBC->get_Y(&Y);
         cg->put_Y(Y + strand_data.location);
      }
      break;
      case TopDeck:
      {
         CComQIPtr<IXYPosition> position(m_pSlabShape);
         CComPtr<IPoint2d> pntTC;
         position->get_LocatorPoint(lpTopCenter, &pntTC);
         Float64 Y;
         pntTC->get_Y(&Y);
         cg->put_Y(Y - strand_data.location);
      }
      break;
      case BottomDeck:
      {
         CComQIPtr<IXYPosition> position(m_pSlabShape);
         CComPtr<IPoint2d> pntBC;
         position->get_LocatorPoint(lpBottomCenter, &pntBC);
         Float64 Y;
         pntBC->get_Y(&Y);
         cg->put_Y(Y + strand_data.location);
      }
      break;
      default:
         ATLASSERT(false);
      }

      strand_shape->putref_Centroid(cg);

      CString strName;
      strName.Format(_T("Strand Layer %d"), i++);

      CComQIPtr<IShape> shape(strand_shape);
      Float64 eps = strand_data.fpe / Eps;
      CComPtr<IPlane3d> initialStrain;
      initialStrain.CoCreateInstance(CLSID_Plane3d);
      initialStrain->ThroughAltitude(eps);
      m_pSection->AddShape(CComBSTR(strName), shape, ss_strand, nullptr, initialStrain, 1.0, VARIANT_FALSE);
   } // next strand

   m_bUpdateModel = false; // model is up to date
}
