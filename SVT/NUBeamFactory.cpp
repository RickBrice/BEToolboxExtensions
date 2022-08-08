#include "stdafx.h"

#include "NUBeamFactory.h"
#include "Helpers.h"

static Float64 gs_NUBeamDimensions[][13] = {
   { 2.56, 1.75, 35.4 - 15.11, 5.5, 5.3, 7.9, 7.9, 2, 2, 5.9, 48.2, 38.4, 0 }, //NU900
   { 2.56, 1.75, 43.3 - 15.11, 5.5, 5.3, 7.9, 7.9, 2, 2, 5.9, 48.2, 38.4, 0 }, //NU1100
   { 2.56, 1.75, 53.1 - 15.11, 5.5, 5.3, 7.9, 7.9, 2, 2, 5.9, 48.2, 38.4, 0 }, //NU1350
   { 2.56, 1.75, 63.0 - 15.11, 5.5, 5.3, 7.9, 7.9, 2, 2, 5.9, 48.2, 38.4, 0 }, //NU1600
   { 2.56, 1.75, 70.9 - 15.11, 5.5, 5.3, 7.9, 7.9, 2, 2, 5.9, 48.2, 38.4, 0 }, //NU1800
   { 2.56, 1.75, 78.7 - 15.11, 5.5, 5.3, 7.9, 7.9, 2, 2, 5.9, 48.2, 38.4, 0 }, //NU2000
};


void NUBeamFactory::CreateBeam(NUBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)NUBeamType::NU900 <= (int)type && (int)type <= (int)NUBeamType::NU2000)
   {
      int i = (int)type - (int)NUBeamType::NU900;
      CComPtr<INUBeam> beam;
      beam.CoCreateInstance(CLSID_NUBeam);
      using namespace _NUBeam;

      Float64 d1, d2, d3, d4, d5, r1, r2, r3, r4, t, w1, w2, c1;
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][R1], CComBSTR(_T("in")), &r1);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][R2], CComBSTR(_T("in")), &r2);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][R3], CComBSTR(_T("in")), &r3);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][R4], CComBSTR(_T("in")), &r4);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][T], CComBSTR(_T("in")),  &t);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_NUBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);

      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_R1(r1);
      beam->put_R2(r2);
      beam->put_R3(r3);
      beam->put_R4(r4);
      beam->put_T( t );
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_C1(c1);

      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


std::unique_ptr<WBFL::Geometry::Shape> NUBeamFactory::CreateBeam(NUBeamType type)
{
   std::unique_ptr<WBFL::Geometry::NUBeam> beam;

   if ((int)NUBeamType::NU900 <= (int)type && (int)type <= (int)NUBeamType::NU2000)
   {
      int i = (int)type - (int)NUBeamType::NU900;
      beam = std::make_unique<WBFL::Geometry::NUBeam>();

      using namespace _NUBeam;

      Float64 d1, d2, d3, d4, d5, r1, r2, r3, r4, t, w1, w2, c1;
      d1 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
      d2 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
      d3 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
      d4 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
      d5 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
      r1 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][R1], WBFL::Units::Measure::Inch);
      r2 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][R2], WBFL::Units::Measure::Inch);
      r3 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][R3], WBFL::Units::Measure::Inch);
      r4 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][R4], WBFL::Units::Measure::Inch);
      t  = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][T], WBFL::Units::Measure::Inch);
      w1 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
      w2 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
      c1 = WBFL::Units::ConvertToSysUnits(gs_NUBeamDimensions[i][C1], WBFL::Units::Measure::Inch);

      beam->SetD1(d1);
      beam->SetD2(d2);
      beam->SetD3(d3);
      beam->SetD4(d4);
      beam->SetD5(d5);
      beam->SetR1(r1);
      beam->SetR2(r2);
      beam->SetR3(r3);
      beam->SetR4(r4);
      beam->SetT(t);
      beam->SetW1(w1);
      beam->SetW2(w2);
      beam->SetC1(c1);
   }
   return beam;
}

static std::_tstring gs_NUnames[] = {
   _T("NU900"),
   _T("NU1100"),
   _T("NU1350"),
   _T("NU1600"),
   _T("NU1800"),
   _T("NU2000"),
};

LPCTSTR NUBeamFactory::GetName(NUBeamType type)
{
   return gs_NUnames[(int)type].c_str();
}

int NUBeamFactory::GetApproxMethods(NUBeamType type)
{
   return AM_J1 | AM_J2;
}

Float64 NUBeamFactory::GetJApprox1(NUBeamType type, IUnitConvert* pConvert)
{
   int i = (int)type - (int)NUBeamType::NU900;
   return ComputeJApprox_NU(i, pConvert, gs_NUBeamDimensions);
}

Float64 NUBeamFactory::GetJApprox1(NUBeamType type)
{
   int i = (int)type - (int)NUBeamType::NU900;
   return ComputeJApprox_NU(i, gs_NUBeamDimensions);
}
