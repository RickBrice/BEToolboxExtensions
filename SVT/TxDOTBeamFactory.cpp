#include "stdafx.h"

#include "TxDOTBeamFactory.h"
#include "Helpers.h"

static Float64 gs_TxDOTBeamDimensions[][14] = {
   { 0.75, 3.5,   2, 2, 6.75, 4.75, 3,    6, 7, 7, 12.5, 2, 9.5, 3 },// Tx28
   { 0.75, 3.5,   2, 2, 6.75, 4.75, 3,   12, 7, 7, 12.5, 2, 9.5, 3 },// Tx34
   { 0.75, 3.5,   2, 2, 6.75, 4.75, 3,   18, 7, 7, 12.5, 2, 9.5, 3 },// Tx40
   { 0.75, 3.5,   2, 2, 8.75, 4.75, 3,   22, 7, 7, 12.5, 2, 9.5, 3 },// Tx46
   { 0.75, 3.5,   2, 2, 8.75, 4.75, 3,   30, 7, 7, 12.5, 2, 9.5, 3 },// Tx54
   { 0.75, 3.5, 2.5, 2, 8.75, 4.75, 3, 37.5, 7, 7, 15.5, 2, 9.5, 3 },// Tx62
   { 0.75, 3.5, 2.5, 2, 8.75, 4.75, 3, 45.5, 7, 7, 15.5, 2, 9.5, 3 },// Tx70
};

static Float64 gs_TxUBeamDimensions[][14] = {
   { 0.75,   40,   8.25,   3,   0.875,   5.875,   21.625,   55,   89,   3,   0.375,   8.25,   15.75,   1.75   }, // U40
   { 0.75,   54,   8.25,   3,   0.875,   5.875,   21.625,   55,   96,   3,   0.375,   8.25,   15.75,   1.75   }, // U54
};


void TxDOTBeamFactory::CreateBeam(TxDOTBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)TxDOTBeamType::Tx28 <= (int)type && (int)type <= (int)TxDOTBeamType::Tx70)
   {
      int i = (int)type - (int)TxDOTBeamType::Tx28;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_TxDOTBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);

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
   else if ((int)TxDOTBeamType::U40 <= (int)type && (int)type <= (int)TxDOTBeamType::U54)
   {
      int i = (int)type - (int)TxDOTBeamType::U40;

      CComPtr<IUBeam2> beam;
      beam.CoCreateInstance(CLSID_UBeam2);
      using namespace _UBeam2; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

      Float64 c1, d1, d2, d3, d4, d5, d6, w1, w2, w3, w4, w5, w6, w7;
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][W5], CComBSTR(_T("in")), &w5);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][W6], CComBSTR(_T("in")), &w6);
      pConvert->ConvertToBaseUnits(gs_TxUBeamDimensions[i][W7], CComBSTR(_T("in")), &w7);

      beam->put_C1(c1);
      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_D6(d6);
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_W3(w3);
      beam->put_W4(w4);
      beam->put_W5(w5);
      beam->put_W6(w6);
      beam->put_W7(w7);
      

      beam->QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_TxDOTnames[] = {
   _T("Tx28"),
   _T("Tx34"),
   _T("Tx40"),
   _T("Tx46"),
   _T("Tx54"),
   _T("Tx62"),
   _T("Tx70"),
   _T("U40"),
   _T("U54")
};

LPCTSTR TxDOTBeamFactory::GetName(TxDOTBeamType type)
{
   return gs_TxDOTnames[(int)type].c_str();
}

int TxDOTBeamFactory::GetApproxMethods(TxDOTBeamType type)
{
   if ((int)TxDOTBeamType::Tx28 <= (int)type && (int)type <= (int)TxDOTBeamType::Tx70)
   {
      int i = (int)type - (int)TxDOTBeamType::Tx28;
      return AM_J1 | AM_J2;
   }
   else if ((int)TxDOTBeamType::U40 <= (int)type && (int)type <= (int)TxDOTBeamType::U54)
   {
      int i = (int)type - (int)TxDOTBeamType::U40;
      return AM_J1;
   }
   else
   {
      return AM_NONE;
   }
}

Float64 TxDOTBeamFactory::GetJApprox1(TxDOTBeamType type,IUnitConvert* pConvert)
{
   if ((int)TxDOTBeamType::Tx28 <= (int)type && (int)type <= (int)TxDOTBeamType::Tx70)
   {
      int i = (int)type - (int)TxDOTBeamType::Tx28;
      return ComputeJApprox_IBeam(i, pConvert, gs_TxDOTBeamDimensions);
   }
   else if ((int)TxDOTBeamType::U40 <= (int)type && (int)type <= (int)TxDOTBeamType::U54)
   {
      int i = (int)type - (int)TxDOTBeamType::U40;
      return ComputeJApprox_UBeam2(i, pConvert, gs_TxUBeamDimensions);
   }
   else
   {
      return -1;
   }
}