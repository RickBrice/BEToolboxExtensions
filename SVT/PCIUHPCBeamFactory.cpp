#include "stdafx.h"

#include "PCIUHPCBeamFactory.h"
#include "Helpers.h"

void PCIUHPCBeamFactory::CreateBeam(PCIUHPCBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if (type == PCIUHPCBeamType::PCI_DBT)
   {

   }
   //else if (type == PCIUHPCBeamType::NU_DBT)
   //{
   //}
   else
   {
      *ppShape = nullptr;
   }
   //if ((int)PCIUHPCBeamType::W42G <= (int)type && (int)type <= (int)PCIUHPCBeamType::WF100G_NoTopFlange)
   //{
   //   int i = (int)type - (int)PCIUHPCBeamType::W42G;
   //   CComPtr<IPrecastBeam> beam;
   //   beam.CoCreateInstance(CLSID_PrecastBeam);
   //   using namespace IBeam;

   //   Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);

   //   beam->put_C1(c1);
   //   beam->put_D1(d1);
   //   beam->put_D2(d2);
   //   beam->put_D3(d3);
   //   beam->put_D4(d4);
   //   beam->put_D5(d5);
   //   beam->put_D6(d6);
   //   beam->put_D7(d7);
   //   beam->put_T1(t1);
   //   beam->put_T2(t2);
   //   beam->put_W1(w1);
   //   beam->put_W2(w2);
   //   beam->put_W3(w3);
   //   beam->put_W4(w4);

   //   beam.QueryInterface(ppShape);
   //}
   //else if (PCIUHPCBeamType::WF100G_Modified == type)
   //{
   //   int i = (int)type - (int)PCIUHPCBeamType::WF100G_Modified;
   //   CComPtr<IPrecastBeam2> beam;
   //   beam.CoCreateInstance(CLSID_PrecastBeam2);

   //   using namespace IBeam2;

   //   Float64 c1, d1, d2, d3, d4, d5, d6, h, t1, t2, w1, w2, w3, w4, w5;
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][H], CComBSTR(_T("in")),  &h);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCModifiedBeamDimensions[i][W5], CComBSTR(_T("in")), &w5);

   //   beam->put_C1(c1);
   //   beam->put_D1(d1);
   //   beam->put_D2(d2);
   //   beam->put_D3(d3);
   //   beam->put_D4(d4);
   //   beam->put_D5(d5);
   //   beam->put_D6(d6);
   //   beam->put_H( h );
   //   beam->put_T1(t1);
   //   beam->put_T2(t2);
   //   beam->put_W1(w1);
   //   beam->put_W2(w2);
   //   beam->put_W3(w3);
   //   beam->put_W4(w4);
   //   beam->put_W5(w5);

   //   beam.QueryInterface(ppShape);
   //}
   //else if ((int)PCIUHPCBeamType::U54G4 <= (int)type && (int)type < (int)PCIUHPCBeamType::nSections)
   //{
   //   int i = (int)type - (int)PCIUHPCBeamType::U54G4;

   //   CComPtr<IUBeam> beam;
   //   beam.CoCreateInstance(CLSID_UBeam);
   //   using namespace _UBeam; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

   //   Float64 d1, d2, d3, d4, d5, d6, d7, t, w1, w2, w3, w4, w5;
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][T], CComBSTR(_T("in")),  &t);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);
   //   pConvert->ConvertToBaseUnits(gs_PCIUHPCUBeamDimensions[i][W5], CComBSTR(_T("in")), &w5);

   //   beam->put_D1(d1);
   //   beam->put_D2(d2);
   //   beam->put_D3(d3);
   //   beam->put_D4(d4);
   //   beam->put_D5(d5);
   //   beam->put_D6(d6);
   //   beam->put_D7(d7);
   //   beam->put_T( t );
   //   beam->put_W1(w1);
   //   beam->put_W2(w2);
   //   beam->put_W3(w3);
   //   beam->put_W4(w4);
   //   beam->put_W5(w5);

   //   beam->QueryInterface(ppShape);
   //}
   //else
   //{
   //   *ppShape = nullptr;
   //}
}


std::unique_ptr<WBFL::Geometry::Shape> PCIUHPCBeamFactory::CreateBeam(PCIUHPCBeamType type)
{
   std::unique_ptr<WBFL::Geometry::Shape> beam;
   std::shared_ptr<WBFL::Geometry::Point2d> hookPnt = std::make_shared<WBFL::Geometry::Point2d>(0.0, 0.0);
   if (type == PCIUHPCBeamType::PCI_DBT)
   {
      auto pbeam = std::make_unique<WBFL::Geometry::PCIDeckedIBeam>(
         26.0, // w1
         41.0, // w2
         3.0, // w3
         1.0, // w4
         0.5, // w5
         38.0, // w6
         108.0, // h
         8.0, // d1, 
         2.5, // d2
         0.5, // d3
         0.5, // d4
         2.5, // d5
         8.0, // d6
         2.5, // d7
         11.0, // d8
         5.25, // d9, 
         4.0, // t
         9.0, // r1
         13.0, // r2
         2.0, // r3
         1.0, // c1
         hookPnt
      );
      beam = std::move(pbeam);
   }
   //else if (type == PCIUHPCBeamType::NU_DBT)
   //{
   //}
   else
   {
   }

   return beam;
   //if ((int)PCIUHPCBeamType::W42G <= (int)type && (int)type <= (int)PCIUHPCBeamType::WF100G_NoTopFlange)
   //{
   //   int i = (int)type - (int)PCIUHPCBeamType::W42G;

   //   auto pbeam = std::make_unique<WBFL::Geometry::PrecastBeam>();

   //   using namespace IBeam;

   //   Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
   //   c1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][C1], WBFL::Units::Measure::Inch);
   //   d1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
   //   d2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
   //   d3 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
   //   d4 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
   //   d5 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
   //   d6 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][D6], WBFL::Units::Measure::Inch);
   //   d7 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][D7], WBFL::Units::Measure::Inch);
   //   t1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][T1], WBFL::Units::Measure::Inch);
   //   t2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][T2], WBFL::Units::Measure::Inch);
   //   w1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
   //   w2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
   //   w3 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][W3], WBFL::Units::Measure::Inch);
   //   w4 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCBeamDimensions[i][W4], WBFL::Units::Measure::Inch);


   //   MapPrecastBeamDimensions(pbeam, c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4);
   //   beam = std::move(pbeam);
   //}
   //else if (PCIUHPCBeamType::WF100G_Modified == type)
   //{
   //   int i = (int)type - (int)PCIUHPCBeamType::WF100G_Modified;

   //   auto pbeam = std::make_unique<WBFL::Geometry::PrecastBeam>();

   //   using namespace IBeam2;

   //   Float64 c1, d1, d2, d3, d4, d5, d6, h, t1, t2, w1, w2, w3, w4, w5;
   //   c1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][C1], WBFL::Units::Measure::Inch);
   //   d1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
   //   d2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
   //   d3 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
   //   d4 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
   //   d5 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
   //   d6 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][D6], WBFL::Units::Measure::Inch);
   //   h  = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][H], WBFL::Units::Measure::Inch);
   //   t1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][T1], WBFL::Units::Measure::Inch);
   //   t2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][T2], WBFL::Units::Measure::Inch);
   //   w1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
   //   w2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
   //   w3 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][W3], WBFL::Units::Measure::Inch);
   //   w4 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][W4], WBFL::Units::Measure::Inch);
   //   w5 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCModifiedBeamDimensions[i][W5], WBFL::Units::Measure::Inch);

   //   pbeam->SetC1(c1);
   //   pbeam->SetD1(d1);
   //   pbeam->SetD2(d2);
   //   pbeam->SetD3(d3);
   //   pbeam->SetD4(d4);
   //   pbeam->SetD5(d5);
   //   pbeam->SetD6(d6);
   //   pbeam->SetHeight(h);
   //   pbeam->SetT1(t1);
   //   pbeam->SetT2(t2);
   //   pbeam->SetW1(w1);
   //   pbeam->SetW2(w2);
   //   pbeam->SetW3(w3);
   //   pbeam->SetW4(w4);
   //   pbeam->SetW5(w5);

   //   beam = std::move(pbeam);
   //}
   //else if ((int)PCIUHPCBeamType::U54G4 <= (int)type && (int)type < (int)PCIUHPCBeamType::nSections)
   //{
   //   int i = (int)type - (int)PCIUHPCBeamType::U54G4;
   //   auto u_beam = std::make_unique<WBFL::Geometry::UBeam>();

   //   using namespace _UBeam; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

   //   Float64 d1, d2, d3, d4, d5, d6, d7, t, w1, w2, w3, w4, w5;
   //   d1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][D1],WBFL::Units::Measure::Inch);
   //   d2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][D2],WBFL::Units::Measure::Inch);
   //   d3 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][D3],WBFL::Units::Measure::Inch);
   //   d4 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][D4],WBFL::Units::Measure::Inch);
   //   d5 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][D5],WBFL::Units::Measure::Inch);
   //   d6 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][D6],WBFL::Units::Measure::Inch);
   //   d7 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][D7],WBFL::Units::Measure::Inch);
   //   t  = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][T], WBFL::Units::Measure::Inch);
   //   w1 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][W1],WBFL::Units::Measure::Inch);
   //   w2 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][W2],WBFL::Units::Measure::Inch);
   //   w3 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][W3],WBFL::Units::Measure::Inch);
   //   w4 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][W4],WBFL::Units::Measure::Inch);
   //   w5 = WBFL::Units::ConvertToSysUnits(gs_PCIUHPCUBeamDimensions[i][W5], WBFL::Units::Measure::Inch);

   //   u_beam->SetD1(d1);
   //   u_beam->SetD2(d2);
   //   u_beam->SetD3(d3);
   //   u_beam->SetD4(d4);
   //   u_beam->SetD5(d5);
   //   u_beam->SetD6(d6);
   //   u_beam->SetD7(d7);
   //   u_beam->SetT(t);
   //   u_beam->SetW1(w1);
   //   u_beam->SetW2(w2);
   //   u_beam->SetW3(w3);
   //   u_beam->SetW4(w4);
   //   u_beam->SetW5(w5);

   //   beam = std::move(u_beam);
   //}
   return beam;
}

static std::_tstring gs_PCIUHPCnames[] = {
   _T("PCI"),
   _T("NU"),
};

LPCTSTR PCIUHPCBeamFactory::GetName(PCIUHPCBeamType type)
{
   return gs_PCIUHPCnames[(int)type].c_str();
}

int PCIUHPCBeamFactory::GetApproxMethods(PCIUHPCBeamType type)
{
   return AM_NONE;
}

Float64 PCIUHPCBeamFactory::GetJApprox1(PCIUHPCBeamType type,IUnitConvert* pConvert)
{
   return -1;
}

Float64 PCIUHPCBeamFactory::GetJApprox1(PCIUHPCBeamType type)
{
   return -1;
}

Float64 PCIUHPCBeamFactory::GetJApprox3(PCIUHPCBeamType type)
{
   return -1;
}