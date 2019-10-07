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
   r.J = J;
   r.nElements = mesh.GetElementCount();
   r.nInteriorNodes = mesh.GetInteriorNodeCount();
   r.ApproxArea = (mesh.HasSymmetry() ? 2 : 1)*r.nElements * mesh.GetElementArea();

   shape->get_ShapeProperties(&r.Props);

   r.Japprox1 = FACTORY::GetJApprox(type);

   Float64 A, Ix, Iy;
   r.Props->get_Area(&A);
   r.Props->get_Ixx(&Ix);
   r.Props->get_Iyy(&Iy);

   Float64 Ip = Ix + Iy;
   r.Japprox2 = A*A*A*A / (40.0*Ip);

   return r;
}

template<typename T,class FACTORY>
void Beams(TCHAR* strAgency)
{
   _tprintf(_T("%s\n"), strAgency);
   _tprintf(_T("Name,Amesh (in4),Area (in2),Amesh/A,Yt (in),Yb (in),Ix (in4),Iy (in4),J (in4),J1 (in4),J1/J,J2 (in4),J2/J,Number of Equations,Solution Time (ms)\n"));
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

      _tprintf(_T("%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%zd,%lld\n"), 
         FACTORY::GetName(type), results.ApproxArea, 
         A, results.ApproxArea / A, Yt, Yb, Ix, Iy,
         results.J, 
         results.Japprox1, results.Japprox1 / results.J, 
         results.Japprox2, results.Japprox2 / results.J,
         results.nInteriorNodes, duration.count());
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
      Beams<ILBeamType, ILBeamFactory>(_T("Illinois"));
      Beams<MNBeamType, MNBeamFactory>(_T("Minnesota"));
      Beams<NUBeamType, NUBeamFactory>(_T("Nebraska"));
      Beams<NCBeamType, NCBeamFactory>(_T("North Carolina"));
      Beams<OhioBeamType, OhioBeamFactory>(_T("Ohio"));
      Beams<OregonBeamType, OregonBeamFactory>(_T("Oregon"));
      Beams<TxDOTBeamType, TxDOTBeamFactory>(_T("Texas"));
      Beams<VirginiaBeamType, VirginiaBeamFactory>(_T("Virginia"));
      Beams<WSDOTBeamType, WSDOTBeamFactory>(_T("Washington"));
   }
   ::CoUninitialize();

   return 0;
}
