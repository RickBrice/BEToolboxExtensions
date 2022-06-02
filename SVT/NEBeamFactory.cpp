#include "stdafx.h"

#include "NEBeamFactory.h"
#include "Helpers.h"

static Float64 gs_NEBeamDimensions[][13] = {
   //  D1      D2       D3      D4      D5      R1      R2      R3     R4      T       W1       W2      C1
   { 3.35,   1.97,   21.45,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 39
   { 3.35,   1.97,   29.32,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 47
   { 3.35,   1.97,   37.20,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 55
   { 3.35,   1.97,   45.07,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 63
   { 3.35,   1.97,   52.95,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 71
   { 3.35,   1.97,   60.82,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 79
   { 3.35,   1.97,   64.76,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 83
   { 3.35,   1.97,   68.69,   3.94,   8.66,   7.87,   7.87,   0.79,  3.94,  7.09,   47.24,   31.89,   0.79}, // NEBT 87
};


void NEBeamFactory::CreateBeam(NEBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)NEBeamType::NEBT39 <= (int)type && (int)type < (int)NEBeamType::nSections)
   {
      int i = (int)type - (int)NEBeamType::NEBT39;
      CComPtr<INUBeam> beam;
      beam.CoCreateInstance(CLSID_NUBeam);
      using namespace _NUBeam;

      Float64 d1, d2, d3, d4, d5, r1, r2, r3, r4, t, w1, w2, c1;
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][D1],CComBSTR(_T("in")),  &d1);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][R1], CComBSTR(_T("in")), &r1);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][R2], CComBSTR(_T("in")), &r2);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][R3], CComBSTR(_T("in")), &r3);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][R4], CComBSTR(_T("in")), &r4);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][T], CComBSTR(_T("in")),  &t);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_NEBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);

      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_R1(r1);
      beam->put_R2(r2);
      beam->put_R3(r3);
      beam->put_R4(r4);
      beam->put_T( t);
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


static std::_tstring gs_NEnames[] = {
   _T("NEBT 39"),
   _T("NEBT 47"),
   _T("NEBT 55"),
   _T("NEBT 63"),
   _T("NEBT 71"),
   _T("NEBT 79"),
   _T("NEBT 83"),
   _T("NEBT 87"),
};

LPCTSTR NEBeamFactory::GetName(NEBeamType type)
{
   return gs_NEnames[(int)type].c_str();
}

int NEBeamFactory::GetApproxMethods(NEBeamType type)
{
   return AM_J1 | AM_J2;
}

Float64 NEBeamFactory::GetJApprox1(NEBeamType type,IUnitConvert* pConvert)
{
   int i = (int)type - (int)NEBeamType::NEBT39;
   return ComputeJApprox_NU(i, pConvert, gs_NEBeamDimensions);
}
