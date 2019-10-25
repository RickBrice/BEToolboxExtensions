// Solver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <array>

#include <fstream>
#include <chrono>

#include <MathEx.h>

#include "..\Helpers.h"

#include "..\FDMeshGenerator.h"
#include "..\PrandtlMembrane.h"

#include "..\ValidationShapeFactory.h"
#include "..\AASHTOBeamFactory.h"
#include "..\CDOTBeamFactory.h"
#include "..\WSDOTBeamFactory.h"
#include "..\CTBeamFactory.h"
#include "..\MNBeamFactory.h"
#include "..\NUBeamFactory.h"
#include "..\OhioBeamFactory.h"
#include "..\OregonBeamFactory.h"
#include "..\ILBeamFactory.h"
#include "..\TxDOTBeamFactory.h"
#include "..\VirginiaBeamFactory.h"
#include "..\NorthCarolinaBeamFactory.h"
#include "..\NEBeamFactory.h"
#include "..\FloridaBeamFactory.h"

#include <initguid.h>
#include <WBFLGeometry_i.c>
///////////////////////////


#include <future>

#define D_MIN 0.125 // inch

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


template < typename T, class FACTORY >
Results ComputeJ(T type)
{
   CComPtr<IShape> shape;
   FACTORY::CreateBeam(type, &shape);

   Float64 d_min = D_MIN;
   FDMeshGenerator mesh_generator(d_min, d_min);

   UniformFDMesh mesh;
   mesh_generator.GenerateMesh(shape, mesh);

   //mesh.Dump(std::cout);

   PrandtlMembrane membrane;
   Float64 J = membrane.ComputeJ(mesh);

   Results r;
   r.Shape = shape;
   r.J = J;
   r.nElements = mesh.GetElementCount();
   r.nInteriorNodes = mesh.GetInteriorNodeCount();
   r.ApproxArea = (mesh.HasSymmetry() ? 2 : 1)*r.nElements * mesh.GetElementArea();

   shape->get_ShapeProperties(&r.Props);

   r.ApproxMethods = FACTORY::GetApproxMethods(type);
   r.Japprox1 = 0;
   r.Japprox2 = 0;

   if (r.ApproxMethods & AM_J1)
   {
      r.Japprox1 = FACTORY::GetJApprox1(type);
   }

   if (r.ApproxMethods & AM_J2)
   {
      r.Japprox2 = GetJApprox2(r.Props);
   }

   return r;
}

template<typename T,class FACTORY>
void Beams(TCHAR* strAgency)
{
   _tprintf(_T("%s\n"), strAgency);
   _tprintf(_T("Name,Area (in2),Yt (in),Yb (in),Ix (in4),Iy (in4),J (in4),Amesh (in4),Amesh/A,J1 (in4),J1/J,J2 (in4),J2/J,Number of Equations,Solution Time (ms)\n"));
   for (int i = 0; i < (int)T::nSections; i++)
   {
      T type = (T)i;

      std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
      Results results = ComputeJ<T, FACTORY>(type);
      std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
      std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      Float64 A, Yt, Yb, Ix, Iy;
      results.Props->get_Area(&A);
      results.Props->get_Ytop(&Yt);
      results.Props->get_Ybottom(&Yb);
      results.Props->get_Ixx(&Ix);
      results.Props->get_Iyy(&Iy);

      _tprintf(_T("%s,%f,%f,%f,%f,%f,%f,%f,%f"),
         FACTORY::GetName(type),
         A, Yt, Yb, Ix, Iy,
         results.J,
         results.ApproxArea, results.ApproxArea / A);
      
      if (results.ApproxMethods & AM_J1)
      {
         _tprintf(_T(",%f,%f"), results.Japprox1, results.Japprox1 / results.J);
      }
      else
      {
         _tprintf(_T(",-,-"));
      }

      if (results.ApproxMethods & AM_J2)
      {
         _tprintf(_T(",%f,%f"), results.Japprox2, results.Japprox2 / results.J);
      }
      else
      {
         _tprintf(_T(",-,-"));
      }
      
      _tprintf(_T(",%zd,%lld\n"), results.nInteriorNodes, duration.count());
   }
   _tprintf(_T("\n"));
}


template<typename T, class FACTORY>
void BDMTable(TCHAR* strAgency)
{
   _tprintf(_T("%s\n"), strAgency);
   _tprintf(_T("Name,Depth (in),Area (in2),Yb (in),Ix (in4),Iy (in4),J (in4),Wt (k/ft),VS (in)\n"));
   for (int i = 0; i < (int)T::nSections; i++)
   {
      T type = (T)i;

      std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
      Results results = ComputeJ<T, FACTORY>(type);
      std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
      std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      Float64 A, Yt, Yb, Ix, Iy;
      results.Props->get_Area(&A);
      results.Props->get_Ytop(&Yt);
      results.Props->get_Ybottom(&Yb);
      results.Props->get_Ixx(&Ix);
      results.Props->get_Iyy(&Iy);

      Float64 H = Yt + Yb;
      Float64 w = A*0.165 / 144;

      Float64 P;
      results.Shape->get_Perimeter(&P);
      Float64 VS = A / P;

      _tprintf(_T("%s,%f,%f,%f,%f,%f,%f,%f,%f\n"),
         FACTORY::GetName(type),
         H, A, Yb, Ix, Iy, results.J, w, VS);
   }
   _tprintf(_T("\n"));
}

long main()
{
   ::CoInitialize(nullptr);
   {
      Beams<ValidationShapeType, ValidationShapeFactory>(_T("Validation"));
      Beams<AASHTOBeamType, AASHTOBeamFactory>(_T("AASHTO"));
      Beams<CTBeamType, CTBeamFactory>(_T("California"));
      Beams<CDOTBeamType, CDOTBeamFactory>(_T("Colorado"));
      Beams<FloridaBeamType, FloridaBeamFactory>(_T("Florida"));
      Beams<ILBeamType, ILBeamFactory>(_T("Illinois"));
      Beams<MNBeamType, MNBeamFactory>(_T("Minnesota"));
      Beams<NUBeamType, NUBeamFactory>(_T("Nebraska"));
      Beams<NEBeamType, NEBeamFactory>(_T("New England"));
      Beams<NCBeamType, NCBeamFactory>(_T("North Carolina"));
      Beams<OhioBeamType, OhioBeamFactory>(_T("Ohio"));
      Beams<OregonBeamType, OregonBeamFactory>(_T("Oregon"));
      Beams<TxDOTBeamType, TxDOTBeamFactory>(_T("Texas"));
      Beams<VirginiaBeamType, VirginiaBeamFactory>(_T("Virginia"));
      Beams<WSDOTBeamType, WSDOTBeamFactory>(_T("Washington"));

      // Lists values for WSDOT BDM girder properties table
      //BDMTable<WSDOTBeamType, WSDOTBeamFactory>(_T("Washington"));
   }
   ::CoUninitialize();

   return 0;
}
