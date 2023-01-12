///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2023  Washington State Department of Transportation
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

// SVTToolDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "SVTToolDoc.h"
#include "SVTToolChildFrame.h"
#include "BEToolboxStatusBar.h"
#include "SVTTool.hh"

#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <EAF\EAFHelp.h>

#include "ValidationShapeFactory.h"
#include "AASHTOBeamFactory.h"
#include "CDOTBeamFactory.h"
#include "CTBeamFactory.h"
#include "FloridaBeamFactory.h"
#include "ILBeamFactory.h"
#include "MNBeamFactory.h"
#include "NUBeamFactory.h"
#include "NEBeamFactory.h"
#include "NorthCarolinaBeamFactory.h"
#include "OhioBeamFactory.h"
#include "OregonBeamFactory.h"
#include "TxDOTBeamFactory.h"
#include "VirginiaBeamFactory.h"
#include "WSDOTBeamFactory.h"

#include <EngTools/PrandtlMembraneSolver.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CSVTToolDoc

IMPLEMENT_DYNCREATE(CSVTToolDoc, CBEToolboxDoc)

CSVTToolDoc::CSVTToolDoc() : CBEToolboxDoc()
{
   m_BeamFactories.push_back(std::make_pair(_T("Validation Shapes"), std::make_unique<CValidationShapeFactory>()));
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

#if defined USE_COM_GEOMETRY
   m_UnitServer.CoCreateInstance(CLSID_UnitServer);
   m_UnitServer->SetSystemUnits(CComBSTR("12kslug"), CComBSTR("in"), CComBSTR("sec"), CComBSTR("F"), CComBSTR("deg"));
   m_UnitServer->get_UnitConvert(&m_UnitConvert);
#else
   WBFL::Units::System::SetSystemUnits(WBFL::Units::Measure::_12KSlug, WBFL::Units::Measure::Inch, WBFL::Units::Measure::Second, WBFL::Units::Measure::Fahrenheit, WBFL::Units::Measure::Degree);
#endif

   m_bComputed = false;
}

CSVTToolDoc::~CSVTToolDoc()
{
}


BEGIN_MESSAGE_MAP(CSVTToolDoc, CBEToolboxDoc)
   ON_COMMAND(ID_HELP_FINDER, OnHelpFinder)
END_MESSAGE_MAP()

void CSVTToolDoc::OnHelpFinder()
{
   EAFHelp(EAFGetDocument()->GetDocumentationSetName(),IDH_SVTTOOL);
}


// CSVTToolDoc diagnostics

#ifdef _DEBUG
void CSVTToolDoc::AssertValid() const
{
	CBEToolboxDoc::AssertValid();
}

#ifndef _WIN32_WCE
void CSVTToolDoc::Dump(CDumpContext& dc) const
{
	CBEToolboxDoc::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CSVTToolDoc::Init()
{
   if (!CBEToolboxDoc::Init())
   {
      return FALSE;
   }

   // initialize with some data
   m_Dmax = 0.125; // inch

   SetGirder(0, 0);

   return TRUE;
}

void CSVTToolDoc::OnCloseDocument()
{
   CBEToolboxDoc::OnCloseDocument();
}

HRESULT CSVTToolDoc::WriteTheDocument(IStructuredSave* pStrSave)
{
   HRESULT hr = pStrSave->BeginUnit(_T("SVTTool"),1.0);
   if ( FAILED(hr) )
      return hr;

   // save that data for this tool here

   hr = pStrSave->EndUnit(); // SVTTool
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

HRESULT CSVTToolDoc::LoadTheDocument(IStructuredLoad* pStrLoad)
{
   HRESULT hr = pStrLoad->BeginUnit(_T("SVTTool"));
   if ( FAILED(hr) )
      return hr;

   // load that data for this tool here
   
   hr = pStrLoad->EndUnit(); // SVTTool
   if ( FAILED(hr) )
      return hr;

   return S_OK;
}

void CSVTToolDoc::LoadDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::LoadDocumentSettings();
}

void CSVTToolDoc::SaveDocumentSettings()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::SaveDocumentSettings();
}

CString CSVTToolDoc::GetDocumentationRootLocation()
{
   // clearly a dummy URL, but you get the idea... this is the
   // URL where your documentation is located
   // be sure to include the last '/'
   return _T("http://www.somewhere.com/Documentation/");

   //CEAFApp* pApp = EAFGetApp();
   //return pApp->GetDocumentationRootLocation();
}

CString CSVTToolDoc::GetDocumentationSetName()
{
   // Return the name of your documentation set
   return _T("ExampleDocs");
   //return CBEToolboxDoc::GetDocumentationSetName(); // this returns BEToolbox
}

CString CSVTToolDoc::GetDocumentationURL()
{
   // Return the URL for documentation for the application
   // Called by GetDocumenentLocation to form the complete
   // documentation URL. The typical format for documentation URLs is:
   // For an Online source
   //     GetDocumentationRootLocation() + m_pszExeName + major.minor version number
   // For a local source
   //     GetDocumentationRootLocation() + m_pszExename

   return GetDocumentationRootLocation();
}

CString CSVTToolDoc::GetToolbarSectionName()
{
   // This is the name that is used in the registry to
   // save/load the toolbar state
   return _T("SVTTool");
}

UINT CSVTToolDoc::GetToolbarID()
{
   // return the resource ID for this tool's toolbar
   return IDR_SVT;
}

void CSVTToolDoc::LoadToolbarResource()
{
   // Set the module state to this DLL before the BEToolbox application
   // loads the toolbar resource (identified by the return value of GetToolbarID())
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CBEToolboxDoc::LoadToolbarResource();
}

IndexType CSVTToolDoc::GetTypeCount() const
{
   return (IndexType)m_BeamFactories.size();
}

LPCTSTR CSVTToolDoc::GetTypeName(IndexType typeIdx) const
{
   return m_BeamFactories[typeIdx].first.c_str();
}

IndexType CSVTToolDoc::GetBeamCount(IndexType typeIdx) const
{
   return m_BeamFactories[typeIdx].second->GetBeamCount();
}

LPCTSTR CSVTToolDoc::GetBeamName(IndexType typeIdx, IndexType beamIdx) const
{
   return m_BeamFactories[typeIdx].second->GetBeamName(beamIdx);
}

void CSVTToolDoc::SetGirder(IndexType typeIdx,IndexType beamIdx)
{
   m_bComputed = false;
   m_TypeIdx = typeIdx;
   m_BeamIdx = beamIdx;
#if defined USE_COM_GEOMETRY
   m_pShape.Release();
   m_BeamFactories[typeIdx].second->CreateBeam(beamIdx, m_UnitConvert, &m_pShape);
#else
   m_Shape = m_BeamFactories[typeIdx].second->CreateBeam(beamIdx);
#endif
}

#if defined USE_COM_GEOMETRY
void CSVTToolDoc::GetShape(IShape** ppShape)
{
   m_pShape.CopyTo(ppShape);
}
#else
const std::unique_ptr<WBFL::Geometry::Shape>& CSVTToolDoc::GetShape() const
{
   return m_Shape;
}
#endif

void CSVTToolDoc::SetMaxElementSize(Float64 dMax)
{
   m_bComputed = false;
   m_Dmax = dMax;
}

Float64 CSVTToolDoc::GetMaxElementSize() const
{
   return m_Dmax;
}

#if defined USE_COM_GEOMETRY
const Results& CSVTToolDoc::GetTorsionalConstant() const
{
   if (m_bComputed)
      return m_Results;

   m_Results.solution = PrandtlMembraneSolver::Solve(m_Shape, m_Dmax, m_Dmax);
   m_Results.J = m_Results.solution.GetJ();
   m_Results.nElements = m_Results.solution.GetFiniteDifferenceMesh()->GetElementCount();
   m_Results.nInteriorNodes = m_Results.solution.GetFiniteDifferenceMesh()->GetInteriorNodeCount();
   m_Results.ApproxArea = m_Results.solution.GetApproximateArea();

   m_pShape->get_ShapeProperties(&r.Props);

   m_Results.ApproxMethods = m_BeamFactories[m_TypeIdx].second->GetApproxMethods(m_BeamIdx);
   m_Results.Japprox1 = 0;
   m_Results.Japprox2 = 0;
   if (m_Results.ApproxMethods & AM_J1)
   {
      m_Results.Japprox1 = m_BeamFactories[m_TypeIdx].second->GetJApprox1(m_BeamIdx, m_UnitConvert);
   }

   if (m_Results.ApproxMethods & AM_J2)
   {
      m_Results.Japprox2 = GetJApprox2(m_Results.Props);
   }

   m_bComputed = true;

   return m_Results;
}
#else
const Results2& CSVTToolDoc::GetTorsionalConstant() const
{
   if (m_bComputed)
      return m_Results;

   m_Results.solution = WBFL::EngTools::PrandtlMembraneSolver::Solve(m_Shape, m_Dmax, m_Dmax);
   m_Results.J = m_Results.solution.GetJ();
   m_Results.solution.GetMaxSlope(&m_Results.MaxSlope, &m_Results.MaxSlopeElementIdx);
   m_Results.Tmax_per_T = m_Results.solution.GetTmaxPerUnitTorque();
   m_Results.nElements = m_Results.solution.GetFiniteDifferenceMesh()->GetElementCount();
   m_Results.nInteriorNodes = m_Results.solution.GetFiniteDifferenceMesh()->GetInteriorNodeCount();
   m_Results.ApproxArea = m_Results.solution.GetFiniteDifferenceMesh()->GetMeshArea();

   m_Results.Props = m_Shape->GetProperties();

   m_Results.ApproxMethods = m_BeamFactories[m_TypeIdx].second->GetApproxMethods(m_BeamIdx);
   m_Results.Japprox1 = 0;
   m_Results.Japprox2 = 0;
   if (m_Results.ApproxMethods & AM_J1)
   {
      m_Results.Japprox1 = m_BeamFactories[m_TypeIdx].second->GetJApprox1(m_BeamIdx);
   }

   if (m_Results.ApproxMethods & AM_J2)
   {
      m_Results.Japprox2 = GetJApprox2(m_Results.Props);
   }

   m_bComputed = true;

   return m_Results;
}
#endif

#if defined USE_COM_GEOMETRY
std::vector<CComPtr<IRectangle>> CSVTToolDoc::GetMesh() const
{
   CComPtr<IRect2d> bbox;
   m_pShape->get_BoundingBox(&bbox);
   CComPtr<IPoint2d> pntTL;
   bbox->get_TopLeft(&pntTL);
   Float64 tlx, tly;
   pntTL->Location(&tlx, &tly);

   const Results& results = GetTorsionalConstant();
   auto& mesh = results.solution.GetFiniteDifferenceMesh();

   Float64 dx, dy;
   mesh->GetElementSize(&dx, &dy);

   std::vector<CComPtr<IRectangle>> mesh_shapes;
   mesh_shapes.reserve(mesh->GetElementCount());

   auto nRows = mesh->GetElementRowCount();
   for (auto row = 0; row < nRows; row++)
   {
      Float64 cy = tly - row*dy -  dy/2;

      IndexType gridStartIdx, firstElementIdx, lastElementIdx;
      m_pmeshMesh->GetElementRange(row, &gridStartIdx, &firstElementIdx, &lastElementIdx);
      Float64 x = gridStartIdx * dx;
      for (auto elementIdx = firstElementIdx; elementIdx <= lastElementIdx; elementIdx++)
      {
         Float64 cx = tlx + (gridStartIdx + elementIdx - firstElementIdx)*dx + dx/2;

         CComPtr<IRectangle> rect;
         rect.CoCreateInstance(CLSID_Rect);
         rect->put_Width(dx);
         rect->put_Height(dy);
         CComPtr<IPoint2d> pntHook;
         rect->get_HookPoint(&pntHook);
         pntHook->Move(cx, cy);

         mesh_shapes.push_back(rect);
      }
   }

   return mesh_shapes;
}
#else
std::vector<WBFL::Geometry::Rectangle> CSVTToolDoc::GetMesh() const
{
   auto bbox = m_Shape->GetBoundingBox();
   Float64 tlx, tly;
   bbox.TopLeft().GetLocation(&tlx, &tly);

   const Results2& results = GetTorsionalConstant();
   auto& mesh = results.solution.GetFiniteDifferenceMesh();

   std::vector<WBFL::Geometry::Rectangle> mesh_shapes;
   mesh_shapes.reserve(mesh->GetElementCount());

   auto nRows = mesh->GetElementRowCount();
   for (auto row = 0; row < nRows; row++)
   {
      IndexType gridStartIdx, firstElementIdx, lastElementIdx;
      mesh->GetElementRange(row, &gridStartIdx, &firstElementIdx, &lastElementIdx);
      for (auto elementIdx = firstElementIdx; elementIdx <= lastElementIdx; elementIdx++)
      {
         auto shape = results.solution.GetMeshElement(elementIdx);
         shape.GetHookPoint()->Offset(tlx, tly);
         mesh_shapes.emplace_back(shape);
      }
   }

   return mesh_shapes;
}
#endif
