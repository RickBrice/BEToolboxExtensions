// Solver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <array>

#include <fstream>
#include <chrono>

#include <MathEx.h>
#include <WBFLUnitServer.h>

#include "..\Helpers.h"

#include <EngTools\PrandtlMembraneSolver.h>
#include <EngTools\PrandtlMembraneSolution.h>
#include <EngTools\UniformFDMesh.h>

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
#include <WBFLUnitServer_i.c>

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
   CComPtr<IUnitServer> unitServer;
   unitServer.CoCreateInstance(CLSID_UnitServer);
   unitServer->SetBaseUnits(CComBSTR("12kslug"), CComBSTR("in"), CComBSTR("sec"), CComBSTR("F"), CComBSTR("deg")); // base units of kip and ksi
   CComPtr<IUnitConvert> unit_convert;
   unitServer->get_UnitConvert(&unit_convert);

   CComPtr<IShape> shape;
   FACTORY::CreateBeam(type, unit_convert, &shape);

   //Float64 d_min = D_MIN;
   //FDMeshGenerator mesh_generator(d_min, d_min);

   //UniformFDMesh mesh;
   //mesh_generator.GenerateMesh(shape, mesh);

   ////mesh.Dump(std::cout);

   //PrandtlMembrane membrane;
   //std::unique_ptr<Float64[]> meshValues;
   //Float64 J = membrane.ComputeJ(mesh,meshValues);

   PrandtlMembraneSolution solution = PrandtlMembraneSolver::Solve(shape, D_MIN, D_MIN);

   Results r;
   r.Shape = shape;
   r.J = solution.GetJ();
   r.nElements = solution.GetFiniteDifferenceMesh()->GetElementCount();
   r.nInteriorNodes = solution.GetFiniteDifferenceMesh()->GetInteriorNodeCount();
   r.ApproxArea = solution.GetApproximateArea();

   shape->get_ShapeProperties(&r.Props);

   r.ApproxMethods = FACTORY::GetApproxMethods(type);
   r.Japprox1 = 0;
   r.Japprox2 = 0;

   if (r.ApproxMethods & AM_J1)
   {
      r.Japprox1 = FACTORY::GetJApprox1(type, unit_convert);
   }

   if (r.ApproxMethods & AM_J2)
   {
      r.Japprox2 = GetJApprox2(r.Props);
   }

   if (r.ApproxMethods & AM_J3)
   {
      r.Japprox3 = FACTORY::GetJApprox3(type,unit_convert);
   }

   return r;
}


template < typename T, class FACTORY >
Results2 ComputeJ2(T type)
{
   WBFL::Units::AutoSystem au;
   WBFL::Units::System::SetBaseUnits(WBFL::Units::Measure::_12KSlug, WBFL::Units::Measure::Inch, WBFL::Units::Measure::Second,WBFL::Units::Measure::Fahrenheit, WBFL::Units::Measure::Degree);

   auto shape = FACTORY::CreateBeam(type);

   //Float64 d_min = D_MIN;
   //FDMeshGenerator mesh_generator(d_min, d_min);

   //UniformFDMesh mesh;
   //mesh_generator.GenerateMesh(shape, mesh);

   ////mesh.Dump(std::cout);

   //PrandtlMembrane membrane;
   //std::unique_ptr<Float64[]> meshValues;
   //Float64 J = membrane.ComputeJ(mesh, meshValues);

   WBFL::EngTools::PrandtlMembraneSolution solution = WBFL::EngTools::PrandtlMembraneSolver::Solve(shape, D_MIN, D_MIN);

   Results2 r;
   r.J = solution.GetJ();
   r.nElements = solution.GetFiniteDifferenceMesh()->GetElementCount();
   r.nInteriorNodes = solution.GetFiniteDifferenceMesh()->GetInteriorNodeCount();
   r.ApproxArea = solution.GetFiniteDifferenceMesh()->GetMeshArea();

   r.Props = shape->GetProperties();
   r.Shape = std::move(shape);

   r.ApproxMethods = FACTORY::GetApproxMethods(type);
   r.Japprox1 = 0;
   r.Japprox2 = 0;
   r.Japprox3 = 0;

   if (r.ApproxMethods & AM_J1)
   {
      r.Japprox1 = FACTORY::GetJApprox1(type);
   }

   if (r.ApproxMethods & AM_J2)
   {
      r.Japprox2 = GetJApprox2(r.Props);
   }

   if (r.ApproxMethods & AM_J3)
   {
      r.Japprox3 = FACTORY::GetJApprox3(type);
   }

   return r;
}

template<typename T,class FACTORY>
void Beams(TCHAR* strAgency)
{
   _tprintf(_T("%s\n"), strAgency);
   _tprintf(_T("Name,Area (in2),Yt (in),Yb (in),Ix (in4),Iy (in4),J (in4),Height (in), Width(in) ,Amesh (in4),Amesh/A,J1 (in4),J1/J,J2 (in4),J2/J,J3 (in4),J3/J,Number of Equations,Solution Time (ms)\n"));
   for (int i = 0; i < (int)T::nSections; i++)
   {
      T type = (T)i;

      std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
      Results results = ComputeJ<T, FACTORY>(type);
      std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
      std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      Float64 A, Yt, Yb, Ix, Iy, Xl, Xr;
      results.Props->get_Area(&A);
      results.Props->get_Ytop(&Yt);
      results.Props->get_Ybottom(&Yb);
      results.Props->get_Ixx(&Ix);
      results.Props->get_Iyy(&Iy);
      results.Props->get_Xleft(&Xl);
      results.Props->get_Xright(&Xr);

      Float64 H = Yb + Yt;
      Float64 W = Xl + Xr;
      

      _tprintf(_T("%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f"),
         FACTORY::GetName(type),
         A, Yt, Yb, Ix, Iy,
         results.J,
         H,W,
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

      if (results.ApproxMethods & AM_J3)
      {
         _tprintf(_T(",%f,%f"), results.Japprox3, results.Japprox3 / results.J);
      }
      else
      {
         _tprintf(_T(",-,-"));
      }

      //_tprintf(_T(",%zd,%lld\n"), results.nInteriorNodes, duration.count());
      _tprintf(_T(",%zd\n"), results.nInteriorNodes);
   }
   _tprintf(_T("\n"));
}

template<typename T, class FACTORY>
void Beams2(TCHAR* strAgency)
{
   _tprintf(_T("%s\n"), strAgency);
   _tprintf(_T("Name,Area (in2),Yt (in),Yb (in),Ix (in4),Iy (in4),J (in4),Height (in), Width(in) ,Amesh (in4),Amesh/A,J1 (in4),J1/J,J2 (in4),J2/J,J3 (in4),J3/J,Number of Equations,Solution Time (ms)\n"));
   for (int i = 0; i < (int)T::nSections; i++)
   {
      T type = (T)i;

      std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
      Results2 results = ComputeJ2<T, FACTORY>(type);
      std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
      std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      Float64 A, Yt, Yb, Ix, Iy, Xl, Xr;
      A = results.Props.GetArea();
      Yt = results.Props.GetYtop();
      Yb = results.Props.GetYbottom();
      Ix = results.Props.GetIxx();
      Iy = results.Props.GetIyy();
      Xl = results.Props.GetXleft();
      Xr = results.Props.GetXright();

      Float64 H = Yb + Yt;
      Float64 W = Xl + Xr;


      _tprintf(_T("%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f"),
         FACTORY::GetName(type),
         A, Yt, Yb, Ix, Iy,
         results.J,
         H, W,
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

      if (results.ApproxMethods & AM_J3)
      {
         _tprintf(_T(",%f,%f"), results.Japprox3, results.Japprox3 / results.J);
      }
      else
      {
         _tprintf(_T(",-,-"));
      }

      //_tprintf(_T(",%zd,%lld\n"), results.nInteriorNodes, duration.count());
      _tprintf(_T(",%zd\n"), results.nInteriorNodes);
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
      Results2 results = ComputeJ2<T, FACTORY>(type);
      std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
      std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

      Float64 A, Yt, Yb, Ix, Iy, Xl, Xr;
      A = results.Props.GetArea();
      Yt = results.Props.GetYtop();
      Yb = results.Props.GetYbottom();
      Ix = results.Props.GetIxx();
      Iy = results.Props.GetIyy();
      Xl = results.Props.GetXleft();
      Xr = results.Props.GetXright();

      Float64 H = Yt + Yb;
      Float64 w = A*0.165 / 144;

      Float64 P = results.Shape->GetPerimeter();
      Float64 VS = A / P;

      _tprintf(_T("%s,%f,%f,%f,%f,%f,%f,%f,%f\n"),
         FACTORY::GetName(type),
         H, A, Yb, Ix, Iy, results.J, w, VS);
   }
   _tprintf(_T("\n"));
}

//#define USE_COM

long main()
{
   std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
#if defined USE_COM
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
#else
   Beams2<ValidationShapeType, ValidationShapeFactory>(_T("Validation"));
   Beams2<AASHTOBeamType, AASHTOBeamFactory>(_T("AASHTO"));
   Beams2<CTBeamType, CTBeamFactory>(_T("California"));
   Beams2<CDOTBeamType, CDOTBeamFactory>(_T("Colorado"));
   Beams2<FloridaBeamType, FloridaBeamFactory>(_T("Florida"));
   Beams2<ILBeamType, ILBeamFactory>(_T("Illinois"));
   Beams2<MNBeamType, MNBeamFactory>(_T("Minnesota"));
   Beams2<NUBeamType, NUBeamFactory>(_T("Nebraska"));
   Beams2<NEBeamType, NEBeamFactory>(_T("New England"));
   Beams2<NCBeamType, NCBeamFactory>(_T("North Carolina"));
   Beams2<OhioBeamType, OhioBeamFactory>(_T("Ohio"));
   Beams2<OregonBeamType, OregonBeamFactory>(_T("Oregon"));
   Beams2<TxDOTBeamType, TxDOTBeamFactory>(_T("Texas"));
   Beams2<VirginiaBeamType, VirginiaBeamFactory>(_T("Virginia"));
   Beams2<WSDOTBeamType, WSDOTBeamFactory>(_T("Washington"));

   // Lists values for WSDOT BDM girder properties table
   //BDMTable<WSDOTBeamType, WSDOTBeamFactory>(_T("Washington"));
#endif

   std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
   std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
   std::cout << "Runtime: " << duration.count() << " milli-seconds." << std::endl;

   return 0;
}
