///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
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

#include "BilinearConcreteModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//void DumpSolution(IGeneralSection* section, IMomentCapacitySolution* solution)
//{
//   USES_CONVERSION;
//
//   std::ostringstream os;
//   
//   CComPtr<IGeneralSectionSolution> general_solution;
//   solution->get_GeneralSectionSolution(&general_solution);
//
//   IndexType nSlices;
//   general_solution->get_SliceCount(&nSlices);
//   os << "Element, Material, Top, Bottom, Ycg, Area, initial strain, incremental strain, total strain, stress, force, moment" << std::endl;
//   TRACE(os.str().c_str());
//
//   for (IndexType sliceIdx = 0; sliceIdx < nSlices; sliceIdx++)
//   {
//      CComPtr<IGeneralSectionSlice> slice;
//      general_solution->get_Slice(sliceIdx, &slice);
//      Float64 area;
//      CComPtr<IPoint2d> cg;
//      Float64 initial_strain;
//      Float64 incremental_strain;
//      Float64 total_strain;
//      Float64 fgStress;
//      slice->get_Area(&area);
//      slice->get_CG(&cg);
//      slice->get_InitialStrain(&initial_strain);
//      slice->get_IncrementalStrain(&incremental_strain);
//      slice->get_TotalStrain(&total_strain);
//      slice->get_ForegroundStress(&fgStress);
//
//      CComPtr<IStressStrain> ss;
//      slice->get_ForegroundMaterial(&ss);
//      CComBSTR bstrMaterial;
//      ss->get_Name(&bstrMaterial);
//
//      Float64 y;
//      cg->get_Y(&y);
//
//      CComPtr<IShape> shape;
//      slice->get_Shape(&shape);
//      CComPtr<IRect2d> rect;
//      shape->get_BoundingBox(&rect);
//      Float64 top, bottom;
//      rect->get_Top(&top);
//      rect->get_Bottom(&bottom);
//
//      IndexType shapeIdx;
//      slice->get_ShapeIndex(&shapeIdx);
//      CComBSTR bstrName;
//      section->get_Name(shapeIdx, &bstrName);
//
//      std::ostringstream os;
//      os << OLE2A(bstrName) << ", " << OLE2A(bstrMaterial) << ", " << top << ", " << bottom << ", " << y << ", " << area << ", " << initial_strain << ", " << incremental_strain << ", " << total_strain << ", " << fgStress << ", " << area * fgStress << ", " << area * fgStress * y << std::endl;
//      TRACE(os.str().c_str());
//   }
//}

// CRCCapacityDoc

IMPLEMENT_DYNCREATE(CRCCapacityDoc, CBEToolboxDoc)

CRCCapacityDoc::CRCCapacityDoc() : CBEToolboxDoc()
{
   m_BeamFactories.push_back(std::make_pair(_T("AASHTO"), std::make_unique<CAASHTOBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("California"), std::make_unique<CCTBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Colorado"), std::make_unique<CCDOTBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Florida"), std::make_unique<CFloridaBeamFactory>()));
   m_BeamFactories.push_back(std::make_pair(_T("Illinois"), std::make_unique<CILBeamFactory>()));
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

   m_Solver.SetSlices(100);
   m_Solver.SetSliceGrowthFactor(1);
   m_Solver.SetTolerance(0.0001);


   // when report printing is implemented, we'll need a title page... this commented out code shows how to do it
   //std::shared_ptr<WBFL::Reporting::TitlePageBuilder> pTitlePageBuilder(std::make_shared<CM3CTitlePageBuilder>());
   std::shared_ptr<WBFL::Reporting::ReportBuilder> pRptBuilder(std::make_shared<WBFL::Reporting::ReportBuilder>(_T("Analysis Results")));
   //pRptBuilder->AddTitlePageBuilder(pTitlePageBuilder);
   pRptBuilder->AddChapterBuilder(std::make_shared<CRCCapacityChapterBuilder>(this));
   GetReportManager()->AddReportBuilder(pRptBuilder);
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

   m_pCompositeShape = std::make_shared<WBFL::Geometry::CompositeShape>();
   
   m_pSlabShape = std::make_shared<WBFL::Geometry::Rectangle>();
   m_pCompositeShape->AddShape(m_pSlabShape);

   m_pGirderShape = std::move(m_BeamFactories[0].second->CreateBeam(0));
   m_pCompositeShape->AddShape(m_pGirderShape);

   //m_pSlabShape.CoCreateInstance(CLSID_Rect);
   //m_pCompositeShape.CoCreateInstance(CLSID_CompositeShape);
   //CComQIPtr<IShape> slab_shape(m_pSlabShape);
   //m_pCompositeShape->AddShape(slab_shape, VARIANT_FALSE);

   //m_BeamFactories[0].second->CreateBeam(0, m_UnitConvert, &m_pGirderShape);
   //m_pCompositeShape->AddShape(m_pGirderShape, VARIANT_FALSE);


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
      pStrSave->BeginUnit(_T("BeamDimensions"), 1.0);
      int i = 0;
      for (auto value : m_ModelData.dimensions)
      {
         CString str;
         str.Format(_T("Value%d"), i++);
         pStrSave->put_Property(str, CComVariant(value));
      }
      pStrSave->EndUnit(); // BeamDimensions
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
      pStrSave->put_Property(_T("HaunchThickness"), CComVariant(m_ModelData.HaunchThickness));
      pStrSave->put_Property(_T("Deck_fc"), CComVariant(m_ModelData.fcDeck));
      pStrSave->put_Property(_T("Deck_Ec"), CComVariant(m_ModelData.EcDeck));
      pStrSave->put_Property(_T("Deck_ft"), CComVariant(m_ModelData.ftDeck));
      pStrSave->put_Property(_T("Deck_fb"), CComVariant(m_ModelData.fbDeck));
   }

   pStrSave->BeginUnit(_T("Rebar"), 1.0);
   pStrSave->put_Property(_T("RebarType"), CComVariant(+m_ModelData.RebarType));
   pStrSave->put_Property(_T("RebarGrade"), CComVariant(+m_ModelData.RebarGrade));
   for (auto& rebar : m_ModelData.Rebar)
   {
      pStrSave->BeginUnit(_T("RebarData"), 1.0);
      pStrSave->put_Property(_T("Size"), CComVariant(+rebar.size));
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
      pStrLoad->BeginUnit(_T("BeamDimensions"));
      for (int i = 0; i < m_ModelData.dimensions.size(); i++)
      {
         CString str;
         str.Format(_T("Value%d"), i);
         pStrLoad->get_Property(str, &var); m_ModelData.dimensions[i] = var.dblVal;
      }
      pStrLoad->EndUnit(); // BeamDimensions
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
      pStrLoad->get_Property(_T("HaunchThickness"), &var); m_ModelData.HaunchThickness = var.dblVal;
      pStrLoad->get_Property(_T("Deck_fc"), &var); m_ModelData.fcDeck = var.dblVal;
      pStrLoad->get_Property(_T("Deck_Ec"), &var); m_ModelData.EcDeck = var.dblVal;
      pStrLoad->get_Property(_T("Deck_ft"), &var); m_ModelData.ftDeck = var.dblVal;
      pStrLoad->get_Property(_T("Deck_fb"), &var); m_ModelData.fbDeck = var.dblVal;
   }

   pStrLoad->BeginUnit(_T("Rebar"));
   var.vt = VT_I8;
   pStrLoad->get_Property(_T("RebarType"), &var); m_ModelData.RebarType = (WBFL::Materials::Rebar::Type)var.lVal;
   pStrLoad->get_Property(_T("RebarGrade"), &var); m_ModelData.RebarGrade = (WBFL::Materials::Rebar::Grade)var.lVal;
   while(SUCCEEDED(pStrLoad->BeginUnit(_T("RebarData"))))
   {
      RebarData rebar;
      var.vt = VT_I8;
      pStrLoad->get_Property(_T("Size"), &var); rebar.size = (WBFL::Materials::Rebar::Size)var.lVal;
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

std::unique_ptr<WBFL::Geometry::Shape> CRCCapacityDoc::CreateBeamShape(const ModelData& modelData) const
{
   if (modelData.beamType == Predefined)
   {
      return m_BeamFactories[modelData.typeIdx].second->CreateBeam(modelData.beamIdx);
   }
   else if (modelData.beamType == WideFlange)
   {
      return CreateBeam(modelData.dimensions);
   }
   else if (modelData.beamType == NU)
   {
      //CComPtr<INUBeam> beam;
      //CreateBeam(modelData.dimensions, &beam);
      //beam.QueryInterface(ppShape);
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
   case Strand_050: dia = WBFL::Units::ConvertToSysUnits(0.50, WBFL::Units::Measure::Inch); break;
   case Strand_052: dia = WBFL::Units::ConvertToSysUnits(0.52, WBFL::Units::Measure::Inch); break;
   case Strand_060: dia = WBFL::Units::ConvertToSysUnits(0.60, WBFL::Units::Measure::Inch); break;
   case Strand_062: dia = WBFL::Units::ConvertToSysUnits(0.62, WBFL::Units::Measure::Inch); break;
   case Strand_070: dia = WBFL::Units::ConvertToSysUnits(0.70, WBFL::Units::Measure::Inch); break;
   default: ATLASSERT(false);
   }
   return dia;
}

void CRCCapacityDoc::SetModelData(const ModelData& modelData)
{
   m_bUpdateModel = { true,true };
   m_bUpdateSolution = { true, true };
   SetModifiedFlag();
   m_ModelData = modelData;
}

const ModelData& CRCCapacityDoc::GetModelData() const
{
   return m_ModelData;
}

const std::shared_ptr<WBFL::Geometry::Shape>& CRCCapacityDoc::GetBeamShape() const
{
   Update(Parabolic);
   return m_pGirderShape;
}

const std::shared_ptr<WBFL::Geometry::Shape>& CRCCapacityDoc::GetSlabShape() const
{
   Update(Parabolic);
   return m_pSlabShape;
}

const WBFL::Geometry::Shape& CRCCapacityDoc::GetShape() const
{
   Update(Parabolic);
   if (m_ModelData.bHasDeck)
      return *m_pCompositeShape;
   else
      return *m_pGirderShape;
}

const WBFL::Materials::StressStrainModel& CRCCapacityDoc::GetMaterial(ElementType element) const
{
   Update(Parabolic);

   std::shared_ptr<WBFL::Materials::StressStrainModel> material;
   switch (element)
   {
   case Deck:  material = m_ssDeck[Parabolic]; break;
   case Girder: material = m_ssGirder[Parabolic]; break;
   case Rebar: material = m_ssRebar; break;
   case Strand: material = m_ssStrand; break;
   default: ATLASSERT(false);
   }

   return *material;
}

std::shared_ptr<const WBFL::RCSection::GeneralSection> CRCCapacityDoc::GetSection(ConcreteModel concreteModel) const
{
   Update(concreteModel);
   return m_pSection[concreteModel];
}

std::shared_ptr<const WBFL::RCSection::MomentCapacitySolution> CRCCapacityDoc::GetCapacity(ConcreteModel concreteModel) const
{
#pragma Reminder("WORKING HERE - Need to cache solution so it doesn't recompute over and over")
   Update(concreteModel);
   if (m_pSection[concreteModel] == nullptr)
   {
      m_pSolution[concreteModel] = nullptr;
   }

   if (m_bUpdateSolution[concreteModel])
   {
      // solution needs to be updated
      m_Solver.SetSection(m_pSection[concreteModel]);

      // assume conventional concrete deck and/or girder
      Float64 ec = -0.003;
      Float64 ei = m_ModelData.ftDeck / m_ModelData.EcDeck;
      if (!m_ModelData.bHasDeck && m_ModelData.concreteType == UHPC)
      {
         // if there is no deck and the girder is UHPC, use the UHPC properties
         ec = -fabs(m_ModelData.ecu);
         ei = m_ModelData.ftBeam / m_ModelData.EcBeam;
      }

      m_pSolution[concreteModel] = m_Solver.Solve(0.00, 0.00, ec - ei, 0.00, WBFL::RCSection::MomentCapacitySolver::SolutionMethod::FixedCompressionStrain);
      if(m_pSolution[concreteModel]->GetGeneralSectionSolution() && m_pSolution[concreteModel]->GetGeneralSectionSolution()->ExceededStrainLimits())
      {
         if (m_ModelData.concreteType == UHPC)
         {
            // this is UHPC so limit tension in the concrete as a first attempt
            m_pSolution[concreteModel] = m_Solver.Solve(0.00, 0.00, m_ModelData.etloc - m_ModelData.fbBeam/m_ModelData.EcBeam, 0.0, WBFL::RCSection::MomentCapacitySolver::SolutionMethod::FixedTensionStrain);
            //DumpSolution(m_pSection, *ppSolution);
         }

         // reinforcement failed (strain too high)
         if (m_pSolution[concreteModel]->GetGeneralSectionSolution()->ExceededStrainLimits())
         {
            Float64 Eps = WBFL::Units::ConvertToSysUnits(28500, WBFL::Units::Measure::KSI); // carbon-steel
            Float64 esu = 0.035; // carbon-steel, 3.5% strain limit
            if (m_ModelData.StrandType == Other)
            {
               Eps = m_ModelData.Eps;
               esu = m_ModelData.esu;
            }

            // height of girder/section
            auto bbox = m_pGirderShape->GetBoundingBox();
            auto Hg = bbox.Height();

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
            m_pSolution[concreteModel] = m_Solver.Solve(0.00, 0.00, esu - ei_ps, Min(y_strand,y_rebar), WBFL::RCSection::MomentCapacitySolver::SolutionMethod::FixedStrain);
            ATLASSERT(m_pSolution[concreteModel]->GetGeneralSectionSolution()->ExceededStrainLimits() == false);
         }
      }

      m_bUpdateSolution[concreteModel] = false; // solution is up to date
   }

   return m_pSolution[concreteModel];
}

const WBFL::RCSection::RCBeam& CRCCapacityDoc::GetRCCBeam() const
{
   Update(Parabolic);
   return m_Beam;
}

WBFL::RCSection::RCSolution CRCCapacityDoc::GetRCCCapacity() const
{
   Update(Parabolic);
   return WBFL::RCSection::RCSolver::Solve(m_Beam);
}

std::unique_ptr<WBFL::Geometry::PrecastBeam> CRCCapacityDoc::CreateBeam(const std::array<Float64,14>& dimensions) const
{
   auto beam = std::make_unique<WBFL::Geometry::PrecastBeam>();

   using namespace IBeam;
   beam->SetC1(dimensions[C1]);
   beam->SetD1(dimensions[D1]);
   beam->SetD2(dimensions[D2]);
   beam->SetD3(dimensions[D3]);
   beam->SetD4(dimensions[D4]);
   beam->SetD5(dimensions[D5]);
   beam->SetD6(dimensions[D6]);
   beam->SetT1(dimensions[T1]);
   beam->SetT2(dimensions[T2]);
   beam->SetW1(dimensions[W1]);
   beam->SetW2(dimensions[W2]);
   beam->SetW3(dimensions[W3]);
   beam->SetW4(dimensions[W4]);

   double h = dimensions[D7] + beam->GetD1() + beam->GetD2() + beam->GetD3() + beam->GetD4() + beam->GetD5() + beam->GetD6();
   beam->SetHeight(h);

   return beam;
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

void CRCCapacityDoc::Update(ConcreteModel concreteModel) const
{
   if (m_bUpdateModel[concreteModel] == false)
      return; // model is up to date

   m_pGirderShape.reset();
   m_pSlabShape.reset();
   m_pSection[concreteModel].reset();

   WBFL::Geometry::Point2d origin(0, 0);

   if (m_ModelData.beamType == Predefined)
   {
      m_pGirderShape = std::move(m_BeamFactories[m_ModelData.typeIdx].second->CreateBeam(m_ModelData.beamIdx));
   }
   else if (m_ModelData.beamType == WideFlange)
   {
      m_pGirderShape = std::move(CreateBeam(m_ModelData.dimensions));
   }
   else if (m_ModelData.beamType == NU)
   {
      //CComPtr<INUBeam> beam;
      //CreateBeam(m_ModelData.dimensions, &beam);
      //beam.QueryInterface(&m_pGirderShape);
   }
   else
   {
      ATLASSERT(false); // is there a new beam type
   }

   m_pGirderShape->SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter, origin);
   m_pCompositeShape->AddShape(m_pGirderShape);

   if (m_ModelData.bHasDeck)
   {
      if (0.0 < m_ModelData.HaunchThickness)
      {
         auto flanged_beam = std::dynamic_pointer_cast<WBFL::Geometry::FlangedBeam>(m_pGirderShape);
         auto wtf = flanged_beam->GetTopWidth();
         WBFL::Geometry::Rectangle haunch;
         haunch.SetHeight(m_ModelData.HaunchThickness);
         haunch.SetWidth(wtf);
         haunch.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter, origin);

         WBFL::Geometry::Rectangle slab;
         slab.SetHeight(m_ModelData.DeckThickness);
         slab.SetWidth(m_ModelData.DeckWidth);
         slab.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter, haunch.GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter));

         auto deck = std::make_shared<WBFL::Geometry::CompositeShape>();
         deck->AddShape(haunch);
         deck->AddShape(slab);

         m_pCompositeShape->AddShape(deck);
         m_pSlabShape = deck;
      }
      else
      {
         WBFL::Geometry::Rectangle slab;
         slab.SetHeight(m_ModelData.DeckThickness);
         slab.SetWidth(m_ModelData.DeckWidth);
         slab.SetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter, origin);

         m_pSlabShape = m_pCompositeShape->AddShape(slab);
      }
   }

   m_pSection[concreteModel] = std::make_shared<WBFL::RCSection::GeneralSection>();

   //
   // materials
   //

   // deck
   if(concreteModel == ConcreteModel::Parabolic)
      m_ssDeck[concreteModel] = std::make_shared<WBFL::Materials::UnconfinedConcreteModel>(_T("Deck"), m_ModelData.fcDeck);
   else
      m_ssDeck[concreteModel] = std::make_shared<BilinearConcreteModel>(_T("Deck"), m_ModelData.fcDeck, m_ModelData.EcDeck, 0.85);

   // girder
   if (m_ModelData.concreteType == Conventional)
   {
      if (concreteModel == ConcreteModel::Parabolic)
         m_ssGirder[concreteModel] = std::make_shared<WBFL::Materials::UnconfinedConcreteModel>(_T("Girder"), m_ModelData.fcBeam);
      else
         m_ssGirder[concreteModel] = std::make_shared<BilinearConcreteModel>(_T("Girder"), m_ModelData.fcBeam, m_ModelData.EcBeam, 0.85);
   }
   else
   {
      auto uhpc = std::make_shared<WBFL::Materials::UHPCModel>(_T("Girder"));
      uhpc->SetFc(m_ModelData.fcBeam);
      uhpc->SetCompressiveStrainLimit(-1.0 * fabs(m_ModelData.ecu));
      uhpc->SetFtcr(m_ModelData.ftcr);
      uhpc->SetFtloc(m_ModelData.ftloc);
      uhpc->Set_etloc(m_ModelData.etloc);

      m_ssGirder[concreteModel] = uhpc;
   }

   // rebar
   auto rebar = std::make_shared<WBFL::Materials::RebarModel>(_T("Rebar"));

   const auto* pRebar = WBFL::LRFD::RebarPool::GetInstance()->GetRebar(m_ModelData.RebarType, m_ModelData.RebarGrade, WBFL::Materials::Rebar::Size::bs3); // use any size, we just want the material properties
   Float64 fy = pRebar->GetYieldStrength();
   Float64 Es = pRebar->GetE();
   Float64 e = pRebar->GetElongation();
   rebar->SetProperties(fy, Es, e);
   m_ssRebar = std::move(rebar);

   // strand
   if(m_ModelData.StrandType == Grade270_LR)
   {
      // use the pre-built power formula
      auto strand = std::make_unique<WBFL::Materials::PSPowerFormulaModel>(_T("Strand"));
      strand->SetStrandGrade(WBFL::Materials::StrandGrade::Grade270);
      strand->SetStrandType(WBFL::Materials::StrandType::LowRelaxation);

      m_ssStrand = std::move(strand);
   }
   else
   {
      // use the generalized form of the stress-strain model
      auto strand = std::make_shared<WBFL::Materials::RambergOsgoodModel>(_T("Strand"));
      strand->SetModelParameters(m_ModelData.A, m_ModelData.B, m_ModelData.C, m_ModelData.Eps, m_ModelData.fpu, -1.0, m_ModelData.esu);
      // note: strand doesn't really fail in compression so make the compression strain limit (minStrain) -1
      // to not exceed the strain limit during analysis
      m_ssStrand = strand;
   }


   // Create the general section
   if (m_ModelData.bHasDeck)
   {
      Float64 e_top = m_ModelData.ftDeck / m_ModelData.EcDeck;
      Float64 e_bot = m_ModelData.fbDeck / m_ModelData.EcDeck;

      WBFL::Geometry::Point3d p1(-1000.0, m_ModelData.DeckThickness, e_top);
      WBFL::Geometry::Point3d p2(1000.0, m_ModelData.DeckThickness, e_top);
      WBFL::Geometry::Point3d p3(0, 0, e_bot);
      auto initial_strain = std::make_shared<WBFL::Geometry::Plane3d>(p1, p2, p3);

      m_pSection[concreteModel]->AddShape(_T("Deck"), m_pSlabShape, m_ssDeck[concreteModel], std::shared_ptr<WBFL::Materials::StressStrainModel>(), initial_strain, 1.0);
   }

   auto bbox = m_pGirderShape->GetBoundingBox();
   Float64 Hg = bbox.Height();
   Float64 e_top = m_ModelData.ftBeam / m_ModelData.EcBeam;
   Float64 e_bot = m_ModelData.fbBeam / m_ModelData.EcBeam;
   WBFL::Geometry::Point3d p1(-1000.0, m_ModelData.DeckThickness, e_top);
   WBFL::Geometry::Point3d p2(1000.0, m_ModelData.DeckThickness, e_top);
   WBFL::Geometry::Point3d p3(0, -Hg, e_bot);
   auto initial_strain = std::make_shared<WBFL::Geometry::Plane3d>(p1, p2, p3);
   m_pSection[concreteModel]->AddShape(_T("Girder"),m_pGirderShape, m_ssGirder[concreteModel], std::shared_ptr<WBFL::Materials::StressStrainModel>(), initial_strain, 1.0, true);

   Float64 Y_top = bbox.Top();

   if (m_ModelData.bHasDeck)
      Y_top = m_pSlabShape->GetBoundingBox().Top();

   Float64 As = 0.0;
   Float64 As_ds = 0.0;
   Float64 ds_min = 0.0;

   int i = 1;
   for (const auto& rebar_data : m_ModelData.Rebar)
   {
      const auto* pRebar = WBFL::LRFD::RebarPool::GetInstance()->GetRebar(m_ModelData.RebarType, m_ModelData.RebarGrade, rebar_data.size);
      Float64 area = pRebar->GetNominalArea();
      area *= rebar_data.nBars;

      WBFL::Geometry::GenericShape bar_shape;
      bar_shape.SetArea(area);

      WBFL::Geometry::Point2d cg;

      switch (rebar_data.measure)
      {
      case TopGirder:
         {
         auto tc = m_pGirderShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter);
         cg.Y() = tc.Y() - rebar_data.location;
         }
         break;
      case BottomGirder:
         {
         auto bc = m_pGirderShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter);
         cg.Y() = bc.Y() + rebar_data.location;
         }
         break;
      case TopDeck:
         {
         auto tc = m_pSlabShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter);
         cg.Y() = tc.Y() - rebar_data.location;
         }
         break;
      case BottomDeck:
         {
         auto bc = m_pSlabShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter);
         cg.Y() = bc.Y() + rebar_data.location;
         }
         break;
      default:
         ATLASSERT(false);
      }

      bar_shape.SetCentroid(cg);

      As += area;
      As_ds += area * cg.Y();

      ds_min = min(ds_min, cg.Y());

      std::_tostringstream os;
      os << _T("Rebar Layer ") << i++ << std::endl;
      m_pSection[concreteModel]->AddShape(os.str().c_str(), bar_shape, m_ssRebar, std::shared_ptr<WBFL::Materials::StressStrainModel>(), std::shared_ptr<WBFL::Geometry::Plane3d>(), 1.0);
   } // next rebar

   std::map<StrandSize, WBFL::Materials::PsStrand::Size> strandSizeMap;
   strandSizeMap.insert(std::make_pair(Strand_050, WBFL::Materials::PsStrand::Size::D1270));
   strandSizeMap.insert(std::make_pair(Strand_052, WBFL::Materials::PsStrand::Size::D1320));
   strandSizeMap.insert(std::make_pair(Strand_060, WBFL::Materials::PsStrand::Size::D1524));
   strandSizeMap.insert(std::make_pair(Strand_062, WBFL::Materials::PsStrand::Size::D1575));
   strandSizeMap.insert(std::make_pair(Strand_070, WBFL::Materials::PsStrand::Size::D1778));
   Float64 Eps = m_ssStrand->GetModulusOfElasticity();

   Float64 Aps = 0.0;
   Float64 Aps_dps = 0.0;
   Float64 dps_min = 0.0;

   i = 1;
   for (const auto& strand_data : m_ModelData.Strands)
   {
      // get the nominal strand area - it doesn't matter what the grade and production type are, we just want the nominal area
      WBFL::Materials::PsStrand::Size strand_size = strandSizeMap.find(m_ModelData.StrandSize)->second;
      const auto* pStrand = WBFL::LRFD::StrandPool::GetInstance()->GetStrand(
         WBFL::Materials::PsStrand::Grade::Gr1860, 
         WBFL::Materials::PsStrand::Type::LowRelaxation, 
         WBFL::Materials::PsStrand::Coating::None, 
         strand_size);
      Float64 area = pStrand->GetNominalArea();
      area *= strand_data.nStrands;

      WBFL::Geometry::GenericShape strand_shape;
      strand_shape.SetArea(area);

      WBFL::Geometry::Point2d cg;
      switch (strand_data.measure)
      {
      case TopGirder:
      {
         auto tc = m_pGirderShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter);
         cg.Y() = tc.Y() - strand_data.location;
      }
      break;
      case BottomGirder:
      {
         auto bc = m_pGirderShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter);
         cg.Y() = bc.Y() + strand_data.location;
      }
      break;
      case TopDeck:
      {
         auto tc = m_pSlabShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::TopCenter);
         cg.Y() = tc.Y() - strand_data.location;
      }
      break;
      case BottomDeck:
      {
         auto bc = m_pSlabShape->GetLocatorPoint(WBFL::Geometry::Shape::LocatorPoint::BottomCenter);
         cg.Y() = bc.Y() + strand_data.location;
      }
      break;
      default:
         ATLASSERT(false);
      }

      strand_shape.SetCentroid(cg);

      Aps += area;
      Aps_dps += area * cg.Y();

      dps_min = min(dps_min, cg.Y());

      Float64 eps = strand_data.fpe / Eps;
      auto initial_strain = std::make_shared<WBFL::Geometry::Plane3d>(eps);

      std::_tostringstream os;
      os << _T("Strand Layer ") << i++ << std::endl;
      m_pSection[concreteModel]->AddShape(os.str().c_str(), strand_shape, m_ssStrand, std::shared_ptr<WBFL::Materials::StressStrainModel>(), initial_strain, 1.0);
   } // next strand

   Float64 ds = IsZero(As) ? 0.0 : Y_top -1.0*(As_ds / As);
   Float64 ds_max = Y_top - ds_min;

   Float64 dps = IsZero(Aps) ? 0.0 : Y_top -1.0*(Aps_dps / Aps);
   Float64 dps_max = Y_top - dps_min;

   Float64 fpu = WBFL::Units::ConvertToSysUnits(270.0, WBFL::Units::Measure::KSI);
   Float64 fpy = 0.9 * fpu;
   Float64 hf = m_ModelData.DeckThickness;
   Float64 b = m_ModelData.DeckWidth;
   auto flanged_beam = std::dynamic_pointer_cast<WBFL::Geometry::FlangedBeam>(m_pGirderShape);
   Float64 bw = flanged_beam->GetMinWebWidth();

   m_Beam.Init(m_ModelData.fcBeam, m_ModelData.fcDeck, fpy, fpu, fy, Aps, As, hf, b, bw, ds, ds_max, dps, dps_max);

   m_bUpdateModel[concreteModel] = false; // model is up to date
}
