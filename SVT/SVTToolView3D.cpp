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

// SVTToolView3D.cpp : implementation file
//

// Using Open Inventor by Coin3d (http://www.coin3d.org/)
// 1) Download and unzip the Coin3d source for Windows (http://www.coin3d.org/lib/coin/releases/3.1.3/source-archive-zip)
// 2) Download and unzip the SoWin library (http://www.coin3d.org/lib/sowin)
// 3) Define the environment variable COINDIR=C:\Coin3d
// 4) Put the \Coin3d\Lib on your path
// 5) Build Coin3d with the "Install" project
// 6) Build SoWin with the "Install" project
// You should now be able to compile this project


#include "stdafx.h"
#include "SVTToolView3D.h"
#include "SVTToolDoc.h"

#include <EAF\EAFHints.h>

#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoTriangleStripSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSVTToolView3D

IMPLEMENT_DYNCREATE(CSVTToolView3D, CView)

CSVTToolView3D::CSVTToolView3D()
{
   m_pViewer = nullptr;
}

CSVTToolView3D::~CSVTToolView3D()
{
   if (m_pViewer != nullptr)
      delete m_pViewer;
}

BEGIN_MESSAGE_MAP(CSVTToolView3D, CView)
   //ON_COMMAND(ID_FILE_PRINT_DIRECT,&CSVTToolView::OnFilePrint)
   ON_WM_SIZE()
END_MESSAGE_MAP()

// CSVTToolView3D diagnostics

#ifdef _DEBUG
void CSVTToolView3D::AssertValid() const
{
//   __super::AssertValid();
}

#ifndef _WIN32_WCE
void CSVTToolView3D::Dump(CDumpContext& dc) const
{
   __super::Dump(dc);
}
#endif
#endif //_DEBUG

// CSVTToolView3D message handlers
void CSVTToolView3D::OnInitialUpdate()
{
   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();

   m_pFrame = (CSVTToolChildFrame*)GetParentFrame();

   m_pViewer = new CSVTToolViewer3D(m_hWnd);
   SoGroup* root = new SoGroup;

   // start with an orthograph camera
   SoOrthographicCamera* camera = new SoOrthographicCamera;
   //SoPerspectiveCamera* camera = new SoPerspectiveCamera;
   root->addChild(camera);

   SoEnvironment* environment = new SoEnvironment;
   environment->ambientIntensity = 1.0f;
   environment->ambientColor.setValue(SbColor(0.7f, 0.7f, 0.7f));
   root->addChild(environment);

   m_pGirder = new SoGroup;
   root->addChild(m_pGirder);

   m_pGrid = new SoGroup;
   root->addChild(m_pGrid);

   m_pMembrane = new SoGroup;
   root->addChild(m_pMembrane);
   
   m_pViewer->setSceneGraph(root);
   camera->viewAll(root, m_pViewer->getViewportRegion());

   __super::OnInitialUpdate();
}

void CSVTToolView3D::OnUpdate(CView* pSender,LPARAM lHint,CObject* pHint)
{
   __super::OnUpdate(pSender, lHint, pHint);

   // this basically clears the scene
   SoGroup* root = (SoGroup*)m_pViewer->getSceneGraph();
   SoGroup* girder = new SoGroup;
   root->replaceChild(m_pGirder, girder);
   m_pGirder = girder;

   root->addChild(new SoSeparator());
   
   SoGroup* grid = new SoGroup;
   root->replaceChild(m_pGrid, grid);
   m_pGrid = grid;

   root->addChild(new SoSeparator());

   SoGroup* membrane = new SoGroup;
   root->replaceChild(m_pMembrane, membrane);
   m_pMembrane = membrane;

   BuildGirderShape();  
   BuildGrid();
   BuildMembrane();
}

void CSVTToolView3D::OnSize(UINT nType, int cx, int cy)
{
   __super::OnSize(nType, cx, cy);
}

void CSVTToolView3D::OnDraw(CDC* pDC)
{
   __super::OnDraw(pDC);
}

void CSVTToolView3D::BuildGirderShape()
{
   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();
   CComPtr<IShape> pShape;
   pDoc->GetShape(&pShape);

   CComPtr<IPoint2dCollection> shape_points;
   pShape->get_PolyPoints(&shape_points);

   SoGroup* group = BuildFacet(shape_points,std::tuple<Float64,Float64,Float64>(1,0,0));

   SoGroup* root = (SoGroup*)m_pViewer->getSceneGraph();
   root->replaceChild(m_pGirder, group);
   m_pGirder = group;
}

void CSVTToolView3D::BuildGrid()
{
   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();

   SoGroup* group = new SoGroup;

   std::vector<CComPtr<IRectangle>> vMesh = pDoc->GetMesh();
   for (auto mesh_element : vMesh)
   {
      CComQIPtr<IShape> shape(mesh_element);
      CComPtr<IShape> clone;
      shape->Clone(&clone);
      CComQIPtr<IRectangle> rect(clone);
      CComPtr<IPoint2d> pnt;
      rect->get_HookPoint(&pnt);
      Float64 X;
      pnt->get_X(&X);
      pnt->put_X(-X);
      CComPtr<IPoint2dCollection> poly_points;
      clone->get_PolyPoints(&poly_points);

      SoGroup* facet = BuildFacet(poly_points,std::tuple<Float64,Float64,Float64>(0,0,1));
      group->addChild(facet);
   }

   SoGroup* root = (SoGroup*)m_pViewer->getSceneGraph();
   root->replaceChild(m_pGrid, group);
   m_pGrid = group;
}

void CSVTToolView3D::BuildMembrane()
{
   CSVTToolDoc* pDoc = (CSVTToolDoc*)GetDocument();
   if (pDoc->IsComputed())
   {
      SoGroup* group = new SoGroup;

      std::vector<CComPtr<IRectangle>> vMesh = pDoc->GetMesh();
      Float64* pNodeValues = pDoc->GetNodeOrdinates();
      const auto* pFDMesh = pDoc->GetFDMesh();
      IndexType nElements = pFDMesh->GetElementCount();
      ATLASSERT(vMesh.size() == nElements);
      Float64 min_value = 0;
      Float64 max_value = 0;
      IndexType nNodes = pFDMesh->GetInteriorNodeCount();
      for (IndexType nodeIdx = 0; nodeIdx < nNodes; nodeIdx++)
      {
         max_value = Max(max_value, pNodeValues[nodeIdx]);
      }

      SbVec3f* verticies = new SbVec3f[4 * nElements];
      long* indicies = new long[5 * nElements];
      SbColor* colors = new SbColor[nElements];

      for (IndexType elementIdx = 0; elementIdx < nElements; elementIdx++)
      {
         CComQIPtr<IShape> shape(vMesh[elementIdx]);
         CComPtr<IPoint2dCollection> points;
         shape->get_PolyPoints(&points);

         IndexType nPoints;
         points->get_Count(&nPoints);

         const auto* pElement = pFDMesh->GetElement(elementIdx);
         Float64 avg_value = 0;
         for (IndexType i = 0; i < 4; i++)
         {
            CComPtr<IPoint2d> pnt;
            points->get_Item(i, &pnt);

            Float64 x, y;
            pnt->Location(&x, &y);

            Float64 z = pElement->Node[i] == INVALID_INDEX ? 0 : pNodeValues[pElement->Node[i]];
            avg_value += z;

            verticies[4 * elementIdx + i].setValue((float)x, (float)y, (float)sqrt(z));
            indicies[5 * elementIdx + i] = (long)(4 * elementIdx + i);
         }
         indicies[5 * elementIdx + 4] = SO_END_FACE_INDEX;
         avg_value /= 4;

         auto c = GetColor(min_value, max_value, avg_value);
         colors[elementIdx].setValue((float)std::get<0>(c), (float)std::get<1>(c), (float)std::get<2>(c));
      } // next mesh element

      SoMaterial* material = new SoMaterial;
      //material->ambientColor.setValues(0, (int)nElements, colors);
      material->diffuseColor.setValues(0, (int)nElements, colors);
      //material->emissiveColor.setValues(0, (int)nElements, colors);
      group->addChild(material);

      SoMaterialBinding* binding = new SoMaterialBinding;
      binding->value = SoMaterialBinding::PER_FACE_INDEXED;
      group->addChild(binding);

      SoDrawStyle* style = new SoDrawStyle;
      style->style = SoDrawStyle::FILLED;
      group->addChild(style);

      SoShapeHints* hints = new SoShapeHints;
      hints->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
      hints->faceType = SoShapeHints::UNKNOWN_FACE_TYPE;
      hints->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
      group->addChild(hints);

      SoCoordinate3* coordinates = new SoCoordinate3;
      coordinates->point.setValues(0, (int)(4 * nElements), verticies);
      group->addChild(coordinates);

      SoIndexedFaceSet* face_set = new SoIndexedFaceSet;
      face_set->coordIndex.setValues(0, (int)(5 * nElements), (int32_t*)indicies);
      group->addChild(face_set);

      SoGroup* root = (SoGroup*)m_pViewer->getSceneGraph();
      root->replaceChild(m_pMembrane, group);
      m_pMembrane = group;
   } // if results computed
}

SoGroup* CSVTToolView3D::BuildFacet(IPoint2dCollection* pPoints,const std::tuple<Float64,Float64,Float64>& color)
{
   SoGroup* group = new SoGroup;

   SoMaterial* material = new SoMaterial;
   material->emissiveColor.setValue((float)std::get<0>(color),(float)std::get<1>(color),(float)std::get<2>(color));
   group->addChild(material);

   SoDrawStyle* style = new SoDrawStyle;
   style->style = SoDrawStyle::LINES;
   group->addChild(style);

   IndexType nPoints;
   pPoints->get_Count(&nPoints);
   SbVec3f* pVerticies = new SbVec3f[nPoints];
   for (IndexType i = 0; i < nPoints; i++)
   {
      CComPtr<IPoint2d> pnt;
      pPoints->get_Item(i, &pnt);
      Float64 x, y;
      pnt->Location(&x, &y);
      pVerticies[i].setValue((float)x, (float)y, 0.0);
   }

   SoCoordinate3* coords = new SoCoordinate3;
   coords->point.setValues(0, (int)nPoints, pVerticies);
   group->addChild(coords);

   int* pIndicies = new int[nPoints + 1];
   for (int i = 0; i < nPoints; i++)
   {
      pIndicies[i] = i;
   }

   pIndicies[nPoints] = SO_END_FACE_INDEX;

   SoIndexedFaceSet* face_set = new SoIndexedFaceSet;
   face_set->coordIndex.setValues(0, (int)(nPoints + 1), (int32_t*)pIndicies);
   group->addChild(face_set);

   return group;
}
