#include "stdafx.h"

#include "ILBeamFactory.h"
#include "Helpers.h"
#include <MathEx.h>

static Float64 gs_ILBeamDimensions[][11] = {
   {      5,    4.5,  3.5,  8, 6, 4,  4, 7, 18, 30, 0.75 }, //IL27_1830
   { 6.0625, 6.4375,  1.5, 15, 7, 6, 12, 7, 24, 38, 0.75 }, //IL36_2438
   {      5,    7.5,  1.5, 15, 7, 6, 12, 7, 38, 38, 0.75 }, //IL36_3838
   { 6.0625, 6.4375, 10.5, 15, 7, 6, 12, 7, 24, 38, 0.75 }, //IL45_2438
   {      5,    7.5, 10.5, 15, 7, 6, 12, 7, 38, 38, 0.75 }, //IL45_3838
   { 6.0625, 6.4375, 19.5, 15, 7, 6, 12, 7, 24, 38, 0.75 }, //IL54_2438
   {      5,    7.5, 19.5, 15, 7, 6, 12, 7, 38, 38, 0.75 }, //IL54_3838
   { 6.0625, 6.4375, 28.5, 15, 7, 6, 12, 7, 24, 38, 0.75 }, //IL63_2438
   {      5,    7.5, 28.5, 15, 7, 6, 12, 7, 38, 38, 0.75 }, //IL63_3838
   { 6.0625, 6.4375, 37.5, 15, 7, 6, 12, 7, 24, 38, 0.75 }, //IL72_2438
   {      5,    7.5, 37.5, 15, 7, 6, 12, 7, 38, 38, 0.75 }, //IL72_3838
};

void ILBeamFactory::GetNUDimensions(int i, Float64& d1, Float64& d2, Float64& d3, Float64& d4, Float64& d5, Float64& r1, Float64& r2, Float64& r3, Float64& r4, Float64& t, Float64& w1, Float64& w2, Float64& c1)
{
   // convert IL dimensions to NU dimensions
   using namespace _ILBeam;
   d1 = gs_ILBeamDimensions[i][D1];
   d2 = gs_ILBeamDimensions[i][D2];
   d3 = gs_ILBeamDimensions[i][D3];
   d4 = gs_ILBeamDimensions[i][D4];
   d5 = gs_ILBeamDimensions[i][D5];
   r1 = gs_ILBeamDimensions[i][R1];
   r2 = gs_ILBeamDimensions[i][R2];
   t = gs_ILBeamDimensions[i][T];
   w1 = gs_ILBeamDimensions[i][W1];
   w2 = gs_ILBeamDimensions[i][W2];
   c1 = gs_ILBeamDimensions[i][C1];

   r3 = 0;
   r4 = 0;

   Float64 K = d2;
   Float64 F = 0.5*(w1 - t);
   Float64 L = F - r1;
   Float64 alpha = atan2(K, L);
   Float64 d = sqrt(K*K + L*L);
   Float64 beta = acos(r1 / d);
   Float64 delta = M_PI - alpha - beta;
   Float64 D2 = fabs(F / tan(delta));

   K = d4;
   F = 0.5*(w2 - t);
   L = F - r2;
   alpha = atan2(K, L);
   d = sqrt(K*K + L*L);
   beta = acos(r2 / d);
   delta = M_PI - alpha - beta;
   Float64 D4 = fabs(F / tan(delta));

   Float64 D3 = d2 + d3 + d4 - D2 - D4;

   ATLASSERT(IsEqual(d1 + d2 + d3 + d4 + d5, d1 + D2 + D3 + D4 + d5)); // make sure height didn't change

   d2 = D2;
   d3 = D3;
   d4 = D4;
}

void ILBeamFactory::CreateBeam(ILBeamType type, IShape** ppShape)
{
   if ((int)ILBeamType::IL27_1830 <= (int)type && (int)type <= (int)ILBeamType::IL72_3838)
   {
      int i = (int)type - (int)ILBeamType::IL27_1830;
      CComPtr<INUBeam> beam;
      beam.CoCreateInstance(CLSID_NUBeam);
      using namespace _ILBeam;

      Float64 d1, d2, d3, d4, d5, r1, r2, r3, r4, t, w1, w2, c1;
      GetNUDimensions(i, d1, d2, d3, d4, d5, r1, r2, r3, r4, t, w1, w2, c1);

      // set the dimensions on the NU Girder
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_T(t);
      beam->put_R1(r1);
      beam->put_R2(r2);
      beam->put_R3(r3);
      beam->put_R4(r4);
      beam->put_C1(c1);

      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_ILnames[] = {
   _T("IL27-1830"),
   _T("IL36-2438"),
   _T("IL36-3838"),
   _T("IL45-2438"),
   _T("IL45-3838"),
   _T("IL54-2438"),
   _T("IL54-3838"),
   _T("IL63-2438"),
   _T("IL63-3838"),
   _T("IL72-2438"),
   _T("IL72-3838")
};

LPCTSTR ILBeamFactory::GetName(ILBeamType type)
{
   return gs_ILnames[(int)type].c_str();
}

Float64 ILBeamFactory::GetJApprox(ILBeamType type)
{
   int i = (int)type - (int)ILBeamType::IL27_1830;

   Float64 D1, D2, D3, D4, D5, R1, R2, R3, R4, T, W1, W2, C1;
   GetNUDimensions(i, D1, D2, D3, D4, D5, R1, R2, R3, R4, T, W1, W2, C1);

   Float64 b = W1;
   Float64 t = 0.5*(D1 + (D1 + D2));
   Float64 t_top = t;
   Float64 J = b*t*t*t; // top flange

   b = W2;
   t = 0.5*(D4 + (D4 + D5));
   Float64 t_bot = t;
   J += b*t*t*t; // bottom flange

   Float64 H = D1 + D2 + D3 + D4 + D5 - t_top - t_bot;
   J += H*T*T*T; // web, full depth

   J *= 1. / 3.;

   return J;
}
