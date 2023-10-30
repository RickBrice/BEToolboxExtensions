#include "stdafx.h"

#include "OhioBeamFactory.h"
#include "Helpers.h"

static Float64 gs_OhioBeamDimensions[][14] = {
   //  C1  D1   D2   D3   D4    D5    D6     D7    T1   T2    W1   W2    W3   W4
   { 0.75,  5,   3,   3, 5.5,    7,    2,  10.5,    8,   8, 17.5,   3,   14,   2 }, // WF36-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  16.5,    8,   8, 17.5,   3,   14,   2 }, // WF42-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  22.5,    8,   8, 17.5,   3,   14,   2 }, // WF48-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  28.5,    8,   8, 17.5,   3,   14,   2 }, // WF54-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  34.5,    8,   8, 17.5,   3,   14,   2 }, // WF60-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  40.5,    8,   8, 17.5,   3,   14,   2 }, // WF66-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  46.5,    8,   8, 17.5,   3,   14,   2 }, // WF72-49

   { 0.75,  4,   2,   3,   8,    9,    0,    34,    8,   8,   11,   3,    9,   0 }, // TypeIV_60
   { 0.75,  4,   2,   3,   8,    9,    0,    40,    8,   8,   11,   3,    9,   0 }, // TypeIV_66
   { 0.75,  4,   2,   3,   8,    9,    0,    46,    8,   8,   17,   3,    9,   0 }, // TypeIV_72
};


void OhioBeamFactory::CreateBeam(OhioBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)OhioBeamType::WF36_49 <= (int)type && (int)type < (int)OhioBeamType::nSections)
   {
      int i = (int)type - (int)OhioBeamType::WF36_49;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][C1],CComBSTR(_T("in")),  &c1);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_OhioBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);

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
   else
   {
      *ppShape = nullptr;
   }
}


std::unique_ptr<WBFL::Geometry::Shape> OhioBeamFactory::CreateBeam(OhioBeamType type)
{
   std::unique_ptr<WBFL::Geometry::PrecastBeam> beam;

   if ((int)OhioBeamType::WF36_49 <= (int)type && (int)type < (int)OhioBeamType::nSections)
   {
      int i = (int)type - (int)OhioBeamType::WF36_49;
      beam = std::make_unique<WBFL::Geometry::PrecastBeam>();

      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      c1 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][C1], WBFL::Units::Measure::Inch);
      d1 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
      d2 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
      d3 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
      d4 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
      d5 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
      d6 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][D6], WBFL::Units::Measure::Inch);
      d7 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][D7], WBFL::Units::Measure::Inch);
      t1 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][T1], WBFL::Units::Measure::Inch);
      t2 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][T2], WBFL::Units::Measure::Inch);
      w1 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
      w2 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
      w3 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][W3], WBFL::Units::Measure::Inch);
      w4 = WBFL::Units::ConvertToSysUnits(gs_OhioBeamDimensions[i][W4], WBFL::Units::Measure::Inch);

      MapPrecastBeamDimensions(beam, c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4);
   }
   return beam;
}

static std::_tstring gs_OhioNames[] = {
   _T("WF36-49"),
   _T("WF42-49"),
   _T("WF48-49"),
   _T("WF54-49"),
   _T("WF60-49"),
   _T("WF66-49"),
   _T("WF72-49"),
   _T("Modified Type IV (60\")"),
   _T("Modified Type IV (66\")"),
   _T("Modified Type IV (72\")"),
};

LPCTSTR OhioBeamFactory::GetName(OhioBeamType type)
{
   return gs_OhioNames[(int)type].c_str();
}

int OhioBeamFactory::GetApproxMethods(OhioBeamType type)
{
   return AM_J1 | AM_J2 | AM_J3;
}

Float64 OhioBeamFactory::GetJApprox1(OhioBeamType type, IUnitConvert* pConvert)
{
   int i = (int)type - (int)OhioBeamType::WF36_49;
   return ComputeJApprox_IBeam(i, pConvert, gs_OhioBeamDimensions);
}

Float64 OhioBeamFactory::GetJApprox1(OhioBeamType type)
{
   int i = (int)type - (int)OhioBeamType::WF36_49;
   return ComputeJApprox_IBeam(i, gs_OhioBeamDimensions);
}

Float64 OhioBeamFactory::GetJApprox3(OhioBeamType type)
{
   int i = (int)type - (int)OhioBeamType::WF36_49;
   return ComputeJApprox3_IBeam(i, gs_OhioBeamDimensions);
}
