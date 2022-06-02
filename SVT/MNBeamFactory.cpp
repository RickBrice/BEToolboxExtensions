#include "stdafx.h"

#include "MNBeamFactory.h"
#include "Helpers.h"


static Float64 gs_MNIBeamDimensions[][14] = {
   //  C1    D1    D2   D3     D4    D5    D6      D7    T1   T2    W1   W2     W3   W4
   { 0.75,    6,  1.5,   2,   7.5,  3.5,    0,    6.5,    6,   6,   10,   2,    10,   0 }, //27M
   { 0.75,    6,  1.5,   2,   7.5,  3.5,    0,   15.5,    6,   6,   10,   2,    10,   0 }, //36M
};

static Float64 gs_MNNUBeamDimensions[][13] = {
   //    D1     D2        D3      D4   D5  R1  R2    R3 R4    T  W1  W2    C1
   {      5,  1.625, 12.4375, 5.3125, 5.5,  6,  6,    1, 2, 6.5, 34, 39, 0.75 }, // MH30
   {      5,  1.625, 17.4375, 5.3125, 5.5,  6,  6,    1, 2, 6.5, 34, 39, 0.75 }, // MH35
   {      5,  1.625, 22.4375, 5.3125, 5.5,  6,  6,    1, 2, 6.5, 34, 39, 0.75 }, // MH40
   { 3.8125, 1.6875,    26.5,    5.5, 7.5,  8,  8,    0, 2, 6.5, 34, 30, 0.75 }, // MN45
   { 3.8125, 1.6875,    35.5,    5.5, 7.5,  8,  8,    0, 2, 6.5, 34, 30, 0.75 }, // MN54
   { 3.8125, 1.6875,    44.5,    5.5, 7.5,  8,  8,    0, 2, 6.5, 34, 30, 0.75 }, // MN63
   {    3.5, 2.5625, 62.8125,  7.625, 5.5,  8,  8, 0.75, 2, 6.5, 34, 30, 0.75 }, // 82MW
   {    3.5, 2.5625, 76.8125,  7.625, 5.5,  8,  8, 0.75, 2, 6.5, 34, 30, 0.75 }, // 96MW
};

void MNBeamFactory::CreateBeam(MNBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)MNBeamType::M27 <= (int)type && (int)type <= (int)MNBeamType::M36)
   {
      int i = (int)type - (int)MNBeamType::M27;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);

      using namespace IBeam; // this is so we don't have to use the name space below (eg IBeam::C1, IBeam::D2...)

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_MNIBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);

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
   else if ((int)MNBeamType::MH30 <= (int)type && (int)type < (int)MNBeamType::nSections)
   {
      int i = (int)type - (int)MNBeamType::MH30;
      CComPtr<INUBeam> beam;
      beam.CoCreateInstance(CLSID_NUBeam);
      using namespace _NUBeam;

      Float64 d1, d2, d3, d4, d5, r1, r2, r3, r4, t, w1, w2, c1;
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][R1], CComBSTR(_T("in")), &r1);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][R2], CComBSTR(_T("in")), &r2);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][R3], CComBSTR(_T("in")), &r3);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][R4], CComBSTR(_T("in")), &r4);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][T], CComBSTR(_T("in")),  &t);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_MNNUBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);

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


static std::_tstring gs_MNnames[] = {
   _T("27M"),
   _T("36M"),
   _T("30MH"),
   _T("35MH"),
   _T("40MH"),
   _T("MN45"),
   _T("MN54"),
   _T("MN63"),
   _T("MW82"),
   _T("MW96"),
};

LPCTSTR MNBeamFactory::GetName(MNBeamType type)
{
   return gs_MNnames[(int)type].c_str();
}

int MNBeamFactory::GetApproxMethods(MNBeamType type)
{
   return AM_J1 | AM_J2;
}

Float64 MNBeamFactory::GetJApprox1(MNBeamType type, IUnitConvert* pConvert)
{
   if ((int)MNBeamType::M27 <= (int)type && (int)type <= (int)MNBeamType::M36)
   {
      int i = (int)type - (int)MNBeamType::M27;
      return ComputeJApprox_IBeam(i, pConvert, gs_MNIBeamDimensions);
   }
   else if ((int)MNBeamType::MH30 <= (int)type && (int)type < (int)MNBeamType::nSections)
   {
      int i = (int)type - (int)MNBeamType::MH30;
      return ComputeJApprox_NU(i, pConvert, gs_MNNUBeamDimensions);
   }

   ATLASSERT(false); // should never get here
   return -1;
}
