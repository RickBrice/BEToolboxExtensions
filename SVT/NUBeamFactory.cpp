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


void NUBeamFactory::CreateBeam(NUBeamType type, IShape** ppShape)
{
   if ((int)NUBeamType::NU900 <= (int)type && (int)type <= (int)NUBeamType::NU2000)
   {
      int i = (int)type - (int)NUBeamType::NU900;
      CComPtr<INUBeam> beam;
      beam.CoCreateInstance(CLSID_NUBeam);
      using namespace _NUBeam;

      beam->put_D1(gs_NUBeamDimensions[i][D1]);
      beam->put_D2(gs_NUBeamDimensions[i][D2]);
      beam->put_D3(gs_NUBeamDimensions[i][D3]);
      beam->put_D4(gs_NUBeamDimensions[i][D4]);
      beam->put_D5(gs_NUBeamDimensions[i][D5]);
      beam->put_R1(gs_NUBeamDimensions[i][R1]);
      beam->put_R2(gs_NUBeamDimensions[i][R2]);
      beam->put_R3(gs_NUBeamDimensions[i][R3]);
      beam->put_R4(gs_NUBeamDimensions[i][R4]);
      beam->put_T(gs_NUBeamDimensions[i][T]);
      beam->put_W1(gs_NUBeamDimensions[i][W1]);
      beam->put_W2(gs_NUBeamDimensions[i][W2]);
      beam->put_C1(gs_NUBeamDimensions[i][C1]);

      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
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

Float64 NUBeamFactory::GetJApprox(NUBeamType type)
{
   int i = (int)type - (int)NUBeamType::NU900;
   using namespace _NUBeam;

   Float64 d1 = gs_NUBeamDimensions[i][D1];
   Float64 d2 = gs_NUBeamDimensions[i][D2];
   Float64 d3 = gs_NUBeamDimensions[i][D3];
   Float64 d4 = gs_NUBeamDimensions[i][D4];
   Float64 d5 = gs_NUBeamDimensions[i][D5];
   Float64 tw = gs_NUBeamDimensions[i][T];
   Float64 w1 = gs_NUBeamDimensions[i][W1];
   Float64 w2 = gs_NUBeamDimensions[i][W2];

   Float64 b = (w1 - tw) / 2;
   Float64 t = 0.5*(d1 + (d1 + d2));
   Float64 J = 2 * b*t*t*t; // top flange, left and right

   b = (w2 - tw) / 2;
   t = 0.5*(d4 + (d4 + d5));
   J += 2 * b*t*t*t; // bottom flange, left and right

   b = d1 + d2 + d3 + d4 + d5;
   J += b*tw*tw*tw; // web, full depth

   J *= 1. / 3.;

   return J;
}
