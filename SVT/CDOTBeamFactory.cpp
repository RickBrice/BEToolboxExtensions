#include "stdafx.h"

#include "CDOTBeamFactory.h"
#include "Helpers.h"

static Float64 gs_CDOTBeamDimensions[][14] = {
   //  C1    D1    D2   D3     D4    D5    D6      D7    T1   T2    W1   W2     W3   W4
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3,  6.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 30
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 12.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 36
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 21.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 45
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 30.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 54
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 39.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 63
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 48.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 72
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 57.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 81
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 66.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 90
};


void CDOTBeamFactory::CreateBeam(CDOTBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)CDOTBeamType::CBT_30 <= (int)type && (int)type < (int)CDOTBeamType::nSections)
   {
      int i = (int)type - (int)CDOTBeamType::CBT_30;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_CDOTBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);

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


std::unique_ptr<WBFL::Geometry::Shape> CDOTBeamFactory::CreateBeam(CDOTBeamType type)
{
   std::unique_ptr<WBFL::Geometry::PrecastBeam> beam;

   if ((int)CDOTBeamType::CBT_30 <= (int)type && (int)type < (int)CDOTBeamType::nSections)
   {
      int i = (int)type - (int)CDOTBeamType::CBT_30;

      beam = std::make_unique<WBFL::Geometry::PrecastBeam>();

      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      c1 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][C1], WBFL::Units::Measure::Inch);
      d1 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
      d2 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
      d3 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
      d4 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
      d5 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
      d6 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][D6], WBFL::Units::Measure::Inch);
      d7 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][D7], WBFL::Units::Measure::Inch);
      t1 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][T1], WBFL::Units::Measure::Inch);
      t2 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][T2], WBFL::Units::Measure::Inch);
      w1 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
      w2 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
      w3 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][W3], WBFL::Units::Measure::Inch);
      w4 = WBFL::Units::ConvertToSysUnits(gs_CDOTBeamDimensions[i][W4], WBFL::Units::Measure::Inch);


      MapPrecastBeamDimensions(beam, c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4);
   }

   return beam;
}

static std::_tstring gs_CDOTnames[] = {
   _T("CBT 30"),
   _T("CBT 36"),
   _T("CBT 45"),
   _T("CBT 54"),
   _T("CBT 63"),
   _T("CBT 72"),
   _T("CBT 81"),
   _T("CBT 90"),
};

LPCTSTR CDOTBeamFactory::GetName(CDOTBeamType type)
{
   return gs_CDOTnames[(int)type].c_str();
}

int CDOTBeamFactory::GetApproxMethods(CDOTBeamType type)
{
   return AM_J1 | AM_J2;
}

Float64 CDOTBeamFactory::GetJApprox1(CDOTBeamType type,IUnitConvert* pConvert)
{
   int i = (int)type - (int)CDOTBeamType::CBT_30;
   return ComputeJApprox_IBeam(i, pConvert, gs_CDOTBeamDimensions);
}

Float64 CDOTBeamFactory::GetJApprox1(CDOTBeamType type)
{
   int i = (int)type - (int)CDOTBeamType::CBT_30;
   return ComputeJApprox_IBeam(i, gs_CDOTBeamDimensions);
}

Float64 CDOTBeamFactory::GetJApprox3(CDOTBeamType type)
{
   int i = (int)type - (int)CDOTBeamType::CBT_30;
   return ComputeJApprox3_IBeam(i, gs_CDOTBeamDimensions);
}
