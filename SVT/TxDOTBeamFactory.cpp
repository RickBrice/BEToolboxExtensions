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
};

LPCTSTR TxDOTBeamFactory::GetName(TxDOTBeamType type)
{
   return gs_TxDOTnames[(int)type].c_str();
}

Float64 TxDOTBeamFactory::GetJApprox(TxDOTBeamType type)
{
   int i = (int)type - (int)TxDOTBeamType::Tx28;
   using namespace IBeam;

   Float64 d1 = gs_TxDOTBeamDimensions[i][D1];
   Float64 d2 = gs_TxDOTBeamDimensions[i][D2];
   Float64 d3 = gs_TxDOTBeamDimensions[i][D3];
   Float64 d4 = gs_TxDOTBeamDimensions[i][D4];
   Float64 d5 = gs_TxDOTBeamDimensions[i][D5];
   Float64 d6 = gs_TxDOTBeamDimensions[i][D6];
   Float64 d7 = gs_TxDOTBeamDimensions[i][D7];
   Float64 t1 = gs_TxDOTBeamDimensions[i][T1];
   Float64 t2 = gs_TxDOTBeamDimensions[i][T2];
   Float64 w1 = gs_TxDOTBeamDimensions[i][W1];
   Float64 w2 = gs_TxDOTBeamDimensions[i][W2];
   Float64 w3 = gs_TxDOTBeamDimensions[i][W3];
   Float64 w4 = gs_TxDOTBeamDimensions[i][W4];

   Float64 b = w1 + w2;
   Float64 t = 0.5*(d1 + (d1 + d2));
   Float64 J = 2 * b*t*t*t; // top flange, left and right

   b = w3 + w4;
   t = 0.5*(d4 + (d4 + d5));
   J += 2 * b*t*t*t; // bottom flange, left and right

   b = d1 + d2 + d3 + d4 + d5 + d6 + d7;
   t = 0.5*(t1 + t2);
   J += b*t*t*t; // web, full depth

   J *= 1. / 3.;

   return J;
}