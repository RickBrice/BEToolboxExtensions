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


void TxDOTBeamFactory::CreateBeam(TxDOTBeamType type, IShape** ppShape)
{
   if ((int)TxDOTBeamType::Tx28 <= (int)type && (int)type <= (int)TxDOTBeamType::Tx70)
   {
      int i = (int)type - (int)TxDOTBeamType::Tx28;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_TxDOTBeamDimensions[i][C1]);
      beam->put_D1(gs_TxDOTBeamDimensions[i][D1]);
      beam->put_D2(gs_TxDOTBeamDimensions[i][D2]);
      beam->put_D3(gs_TxDOTBeamDimensions[i][D3]);
      beam->put_D4(gs_TxDOTBeamDimensions[i][D4]);
      beam->put_D5(gs_TxDOTBeamDimensions[i][D5]);
      beam->put_D6(gs_TxDOTBeamDimensions[i][D6]);
      beam->put_D7(gs_TxDOTBeamDimensions[i][D7]);
      beam->put_T1(gs_TxDOTBeamDimensions[i][T1]);
      beam->put_T2(gs_TxDOTBeamDimensions[i][T2]);
      beam->put_W1(gs_TxDOTBeamDimensions[i][W1]);
      beam->put_W2(gs_TxDOTBeamDimensions[i][W2]);
      beam->put_W3(gs_TxDOTBeamDimensions[i][W3]);
      beam->put_W4(gs_TxDOTBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else if ((int)TxDOTBeamType::U40 <= (int)type && (int)type <= (int)TxDOTBeamType::U54)
   {
      int i = (int)type - (int)TxDOTBeamType::U40;

      CComPtr<IUBeam2> beam;
      beam.CoCreateInstance(CLSID_UBeam2);
      using namespace _UBeam2; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

      beam->put_C1(gs_TxUBeamDimensions[i][C1]);
      beam->put_D1(gs_TxUBeamDimensions[i][D1]);
      beam->put_D2(gs_TxUBeamDimensions[i][D2]);
      beam->put_D3(gs_TxUBeamDimensions[i][D3]);
      beam->put_D4(gs_TxUBeamDimensions[i][D4]);
      beam->put_D5(gs_TxUBeamDimensions[i][D5]);
      beam->put_D6(gs_TxUBeamDimensions[i][D6]);
      beam->put_W1(gs_TxUBeamDimensions[i][W1]);
      beam->put_W2(gs_TxUBeamDimensions[i][W2]);
      beam->put_W3(gs_TxUBeamDimensions[i][W3]);
      beam->put_W4(gs_TxUBeamDimensions[i][W4]);
      beam->put_W5(gs_TxUBeamDimensions[i][W5]);
      beam->put_W6(gs_TxUBeamDimensions[i][W6]);
      beam->put_W7(gs_TxUBeamDimensions[i][W7]);
      

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

Float64 TxDOTBeamFactory::GetJApprox1(TxDOTBeamType type)
{
   if ((int)TxDOTBeamType::Tx28 <= (int)type && (int)type <= (int)TxDOTBeamType::Tx70)
   {
      int i = (int)type - (int)TxDOTBeamType::Tx28;
      return ComputeJApprox_IBeam(i, gs_TxDOTBeamDimensions);
   }
   else if ((int)TxDOTBeamType::U40 <= (int)type && (int)type <= (int)TxDOTBeamType::U54)
   {
      int i = (int)type - (int)TxDOTBeamType::U40;
      return ComputeJApprox_UBeam2(i, gs_TxUBeamDimensions);
   }
   else
   {
      return -1;
   }
}