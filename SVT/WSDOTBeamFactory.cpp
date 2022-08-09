#include "stdafx.h"

#include "WSDOTBeamFactory.h"
#include "Helpers.h"

static Float64 gs_WSDOTBeamDimensions[][14] = {
   { 1,     3.5,     1.5,   0,   5,   2,   0,   30,   6,   6,   4.5,   0,     7,   0 }, // W42G
   { 1,       5,       2,   0,   6,   3,   0,   34,   6,   6,     7,   0,   9.5,   0 }, // W50G
   { 1,       5,       2,   0,   6,   3,   0,   42,   6,   6,   9.5,   0,   9.5,   0 }, // W58G
   { 1,   2.875,   2.625,   2,   6,   3,   0,   57,   6,   6,  16.5,   2,   9.5,   0 }, // W74G
   { 1,3,3,3,5.125,4.5,3,14.375,6.125,6.125,18.4375,3,13.125,3 }, // WF36G
   { 1,3,3,3,5.125,4.5,3,20.375,6.125,6.125,18.4375,3,13.125,3 }, // WF42G
   { 1,3,3,3,5.125,4.5,3,28.375,6.125,6.125,18.4375,3,13.125,3 }, // WF50G
   { 1,3,3,3,5.125,4.5,3,36.375,6.125,6.125,18.4375,3,13.125,3 }, // WF58G
   { 1,3,3,3,5.125,4.5,3,44.375,6.125,6.125,18.4375,3,13.125,3 }, // WF66G
   { 1,3,3,3,5.125,4.5,3,52.375,6.125,6.125,18.4375,3,13.125,3 }, // WF74G
   { 1,3,3,3,5.125,4.5,3,61.000,6.125,6.125,18.4375,3,13.125,3 }, // WF83G
   { 1,3,3,3,5.125,4.5,3,72.875,6.125,6.125,18.4375,3,13.125,3 }, // WF95G
   { 1,3,3,3,5.125,4.5,3,78.375,6.125,6.125,18.4375,3,13.125,3 }, // WF100G
};

static Float64 gs_WSDOTModifiedBeamDimensions[][15] = {
   { 1,3,3,3,3,4.5,5.125,100,6.125,6.125,6,18.4375,3,3,13.125 } // WF100G_Modified
};


static Float64 gs_WSDOTUBeamDimensions[][13] =
{
   //    D1     D2  D3  D4  D5 D6 D7 T W1 W2 W3 W4 W5
   { 54,   6,   6,   0,   0,   0,   0,   7,   48,   63.42857143,   12,   0,   0   }, // U54G4
   { 54,   6,   6,   0,   0,   0,   0,   7,   60,   75.42857143,   12,   0,   0   }, // U54G5
   { 54,   6,   6,   0,   0,   0,   0,   7,   72,   87.42857143,   24,   0,   0   }, // U54G6
   { 66,   6,   6,   0,   0,   0,   0,   7,   48,   66.85714286,   12,   0,   0   }, // U66G4
   { 66,   6,   6,   0,   0,   0,   0,   7,   60,   78.85714286,   12,   0,   0   }, // U66G5
   { 66,   6,   6,   0,   0,   0,   0,   7,   72,   90.85714286,   24,   0,   0   }, // U66G6
   { 78,   6,   6,   0,   0,   0,   0,   7,   48,   70.28571429,   12,   0,   0   }, // U78G4
   { 78,   6,   6,   0,   0,   0,   0,   7,   60,   82.28571429,   12,   0,   0   }, // U78G5
   { 78,   6,   6,   0,   0,   0,   0,   7,   72,   94.28571429,   24,   0,   0   }, // U78G6
   { 60,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   48,   73.428571,   12,   3,   5   }, // UF60G4
   { 60,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   60,   85.428571,   12,   3,   5   }, // UF60G5
   { 60,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   72,   97.428571,   24,   3,   5   }, // UF60G6
   { 72,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   48,   76.857143,   12,   3,   5   }, // UF72G4
   { 72,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   60,   88.857143,   12,   3,   5   }, // UF72G5
   { 72,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   72,   100.85714,   24,   3,   5   }, // UF72G6
   { 84,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   48,   80.285714,   12,   3,   5   }, // UF84G4
   { 84,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   60,   92.285714,   12,   3,   5   }, // UF84G5
   { 84,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   72,  104.285714,   12,   3,   5   }, // UF84G6

};

void WSDOTBeamFactory::CreateBeam(WSDOTBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_WSDOTBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);

      beam->put_C1(c1);
      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_D6(d6);
      beam->put_D7(d7);
      beam->put_T1(t1);
      beam->put_T2(t2);
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_W3(w3);
      beam->put_W4(w4);

      beam.QueryInterface(ppShape);
   }
   else if (WSDOTBeamType::WF100G_Modified == type)
   {
      int i = (int)type - (int)WSDOTBeamType::WF100G_Modified;
      CComPtr<IPrecastBeam2> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam2);

      using namespace IBeam2;

      Float64 c1, d1, d2, d3, d4, d5, d6, h, t1, t2, w1, w2, w3, w4, w5;
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][H], CComBSTR(_T("in")),  &h);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);
      pConvert->ConvertToBaseUnits(gs_WSDOTModifiedBeamDimensions[i][W5], CComBSTR(_T("in")), &w5);

      beam->put_C1(c1);
      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_D6(d6);
      beam->put_H( h );
      beam->put_T1(t1);
      beam->put_T2(t2);
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_W3(w3);
      beam->put_W4(w4);
      beam->put_W5(w5);

      beam.QueryInterface(ppShape);
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;

      CComPtr<IUBeam> beam;
      beam.CoCreateInstance(CLSID_UBeam);
      using namespace _UBeam; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

      Float64 d1, d2, d3, d4, d5, d6, d7, t, w1, w2, w3, w4, w5;
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][T], CComBSTR(_T("in")),  &t);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);
      pConvert->ConvertToBaseUnits(gs_WSDOTUBeamDimensions[i][W5], CComBSTR(_T("in")), &w5);

      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_D6(d6);
      beam->put_D7(d7);
      beam->put_T( t );
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_W3(w3);
      beam->put_W4(w4);
      beam->put_W5(w5);

      beam->QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


std::unique_ptr<WBFL::Geometry::Shape> WSDOTBeamFactory::CreateBeam(WSDOTBeamType type)
{
   std::unique_ptr<WBFL::Geometry::Shape> beam;
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;

      auto pbeam = std::make_unique<WBFL::Geometry::PrecastBeam>();

      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      c1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][C1], WBFL::Units::Measure::Inch);
      d1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
      d2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
      d3 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
      d4 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
      d5 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
      d6 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][D6], WBFL::Units::Measure::Inch);
      d7 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][D7], WBFL::Units::Measure::Inch);
      t1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][T1], WBFL::Units::Measure::Inch);
      t2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][T2], WBFL::Units::Measure::Inch);
      w1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
      w2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
      w3 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][W3], WBFL::Units::Measure::Inch);
      w4 = WBFL::Units::ConvertToSysUnits(gs_WSDOTBeamDimensions[i][W4], WBFL::Units::Measure::Inch);


      MapPrecastBeamDimensions(pbeam, c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4);
      beam = std::move(pbeam);
   }
   else if (WSDOTBeamType::WF100G_Modified == type)
   {
      int i = (int)type - (int)WSDOTBeamType::WF100G_Modified;

      auto pbeam = std::make_unique<WBFL::Geometry::PrecastBeam>();

      using namespace IBeam2;

      Float64 c1, d1, d2, d3, d4, d5, d6, h, t1, t2, w1, w2, w3, w4, w5;
      c1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][C1], WBFL::Units::Measure::Inch);
      d1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
      d2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
      d3 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
      d4 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
      d5 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
      d6 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][D6], WBFL::Units::Measure::Inch);
      h  = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][H], WBFL::Units::Measure::Inch);
      t1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][T1], WBFL::Units::Measure::Inch);
      t2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][T2], WBFL::Units::Measure::Inch);
      w1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
      w2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
      w3 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][W3], WBFL::Units::Measure::Inch);
      w4 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][W4], WBFL::Units::Measure::Inch);
      w5 = WBFL::Units::ConvertToSysUnits(gs_WSDOTModifiedBeamDimensions[i][W5], WBFL::Units::Measure::Inch);

      pbeam->SetC1(c1);
      pbeam->SetD1(d1);
      pbeam->SetD2(d2);
      pbeam->SetD3(d3);
      pbeam->SetD4(d4);
      pbeam->SetD5(d5);
      pbeam->SetD6(d6);
      pbeam->SetHeight(h);
      pbeam->SetT1(t1);
      pbeam->SetT2(t2);
      pbeam->SetW1(w1);
      pbeam->SetW2(w2);
      pbeam->SetW3(w3);
      pbeam->SetW4(w4);
      pbeam->SetW5(w5);

      beam = std::move(pbeam);
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;
      auto u_beam = std::make_unique<WBFL::Geometry::UBeam>();

      using namespace _UBeam; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

      Float64 d1, d2, d3, d4, d5, d6, d7, t, w1, w2, w3, w4, w5;
      d1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][D1],WBFL::Units::Measure::Inch);
      d2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][D2],WBFL::Units::Measure::Inch);
      d3 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][D3],WBFL::Units::Measure::Inch);
      d4 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][D4],WBFL::Units::Measure::Inch);
      d5 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][D5],WBFL::Units::Measure::Inch);
      d6 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][D6],WBFL::Units::Measure::Inch);
      d7 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][D7],WBFL::Units::Measure::Inch);
      t  = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][T], WBFL::Units::Measure::Inch);
      w1 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][W1],WBFL::Units::Measure::Inch);
      w2 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][W2],WBFL::Units::Measure::Inch);
      w3 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][W3],WBFL::Units::Measure::Inch);
      w4 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][W4],WBFL::Units::Measure::Inch);
      w5 = WBFL::Units::ConvertToSysUnits(gs_WSDOTUBeamDimensions[i][W5], WBFL::Units::Measure::Inch);

      u_beam->SetD1(d1);
      u_beam->SetD2(d2);
      u_beam->SetD3(d3);
      u_beam->SetD4(d4);
      u_beam->SetD5(d5);
      u_beam->SetD6(d6);
      u_beam->SetD7(d7);
      u_beam->SetT(t);
      u_beam->SetW1(w1);
      u_beam->SetW2(w2);
      u_beam->SetW3(w3);
      u_beam->SetW4(w4);
      u_beam->SetW5(w5);

      beam = std::move(u_beam);
   }
   return beam;
}

static std::_tstring gs_WSDOTnames[] = {
   _T("W42G"),
   _T("W50G"),
   _T("W58G"),
   _T("W74G"),
   _T("WF36G"),
   _T("WF42G"),
   _T("WF50G"),
   _T("WF58G"),
   _T("WF66G"),
   _T("WF74G"),
   _T("WF83G"),
   _T("WF95G"),
   _T("WF100G"),
   _T("WF100G_modified"),
   _T("U54G4"),
   _T("U54G5"),
   _T("U54G6"),
   _T("U66G4"),
   _T("U66G5"),
   _T("U66G6"),
   _T("U78G4"),
   _T("U78G5"),
   _T("U78G6"),
   _T("UF60G4"),
   _T("UF60G5"),
   _T("UF60G6"),
   _T("UF72G4"),
   _T("UF72G5"),
   _T("UF72G6"),
   _T("UF84G4"),
   _T("UF84G5"),
   _T("UF84G6"),
};

LPCTSTR WSDOTBeamFactory::GetName(WSDOTBeamType type)
{
   return gs_WSDOTnames[(int)type].c_str();
}

int WSDOTBeamFactory::GetApproxMethods(WSDOTBeamType type)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
      return AM_J1 | AM_J2;
   }
   else if (type == WSDOTBeamType::WF100G_Modified)
   {
      int i = (int)type - (int)WSDOTBeamType::WF100G_Modified;
      return AM_J1 | AM_J2;
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;
      return AM_J1;
   }
   return AM_NONE;
}

Float64 WSDOTBeamFactory::GetJApprox1(WSDOTBeamType type,IUnitConvert* pConvert)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
      return ComputeJApprox_IBeam(i, pConvert, gs_WSDOTBeamDimensions);
   }
   else if (type == WSDOTBeamType::WF100G_Modified)
   {
      int i = (int)type - (int)WSDOTBeamType::WF100G_Modified;
      return ComputeJApprox_IBeam2(i, pConvert, gs_WSDOTModifiedBeamDimensions);
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;
      return ComputeJApprox_UBeam(i, pConvert, gs_WSDOTUBeamDimensions);
   }
   return -1;
}

Float64 WSDOTBeamFactory::GetJApprox1(WSDOTBeamType type)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
      return ComputeJApprox_IBeam(i, gs_WSDOTBeamDimensions);
   }
   else if (type == WSDOTBeamType::WF100G_Modified)
   {
      int i = (int)type - (int)WSDOTBeamType::WF100G_Modified;
      return ComputeJApprox_IBeam2(i, gs_WSDOTModifiedBeamDimensions);
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;
      return ComputeJApprox_UBeam(i, gs_WSDOTUBeamDimensions);
   }
   return -1;
}

Float64 WSDOTBeamFactory::GetJApprox3(WSDOTBeamType type)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
      return ComputeJApprox3_IBeam(i, gs_WSDOTBeamDimensions);
   }
   else if (type == WSDOTBeamType::WF100G_Modified)
   {
      int i = (int)type - (int)WSDOTBeamType::WF100G_Modified;
      return ComputeJApprox3_IBeam2(i, gs_WSDOTModifiedBeamDimensions);
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;
      return ComputeJApprox3_UBeam(i, gs_WSDOTUBeamDimensions);
   }
   return -1;
}