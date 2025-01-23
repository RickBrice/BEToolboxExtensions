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

#pragma once

#include <BEToolboxDoc.h>
#include <WBFLGeometry.h>
#include <EngTools/UniformFDMesh.h>
#include "AbstractBeamFactory.h"
#include "Helpers.h"

class CSVTToolDoc : public CBEToolboxDoc
{
	DECLARE_DYNCREATE(CSVTToolDoc)

public:
   CSVTToolDoc();
	virtual ~CSVTToolDoc();

   virtual CString GetToolbarSectionName() override;

   std::vector<LPCTSTR> GetTypes() const;
   std::vector<LPCTSTR> GetBeams(LPCTSTR type) const;

#if defined USE_COM_GEOMETRY
   CComPtr<IUnitConvert>& GetUnitConvert() { return m_UnitConvert; }
#endif

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

   IndexType GetTypeCount() const;
   LPCTSTR GetTypeName(IndexType typeIdx) const;
   IndexType GetBeamCount(IndexType typeIdx) const;
   LPCTSTR GetBeamName(IndexType typeIdx, IndexType beamIdx) const;

   void SetGirder(IndexType typeIdx, IndexType beamIdx);

   void SetMaxElementSize(Float64 dMax);
   Float64 GetMaxElementSize() const;

#if defined USE_COM_GEOMETRY
   void GetShape(IShape** ppShape);
#else
   const std::unique_ptr<WBFL::Geometry::Shape>& GetShape() const;
#endif

#if defined USE_COM_GEOMETRY
   const Results& GetTorsionalConstant() const;
#else
   const Results2& GetTorsionalConstant() const;
#endif

#if defined USE_COM_GEOMETRY
   std::vector<CComPtr<IRectangle>> GetMesh() const;
#else
   std::vector<WBFL::Geometry::Rectangle> GetMesh() const;
#endif

   bool IsComputed() const { return m_bComputed; }

   // over-ride default behavior by destroying column
   virtual void OnCloseDocument() override;

protected:
   virtual UINT GetToolbarID() override;
   virtual void LoadToolbarResource() override;

private:
#if defined USE_COM_GEOMETRY
   CComPtr<IShape> m_pShape;
#else
   std::unique_ptr<WBFL::Geometry::Shape> m_Shape;
#endif
   mutable bool m_bComputed;

#if defined USE_COM_GEOMETRY
   CComPtr<IUnitServer> m_UnitServer;
   CComPtr<IUnitConvert> m_UnitConvert;
#endif

#if defined USE_COM_GEOMETRY
   mutable Results m_Results;
#else
   mutable Results2 m_Results;
#endif

   IndexType m_TypeIdx, m_BeamIdx;

   std::vector<std::pair<std::_tstring, std::unique_ptr<CAbstractBeamFactory>>> m_BeamFactories;

   Float64 m_Dmax;
};
