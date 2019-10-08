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


void NEBeamFactory::CreateBeam(NEBeamType type, IShape** ppShape)
{
   if ((int)NEBeamType::NEBT39 <= (int)type && (int)type < (int)NEBeamType::nSections)
   {
      int i = (int)type - (int)NEBeamType::NEBT39;
      CComPtr<INUBeam> beam;
      beam.CoCreateInstance(CLSID_NUBeam);
      using namespace _NUBeam;

      beam->put_D1(gs_NEBeamDimensions[i][D1]);
      beam->put_D2(gs_NEBeamDimensions[i][D2]);
      beam->put_D3(gs_NEBeamDimensions[i][D3]);
      beam->put_D4(gs_NEBeamDimensions[i][D4]);
      beam->put_D5(gs_NEBeamDimensions[i][D5]);
      beam->put_R1(gs_NEBeamDimensions[i][R1]);
      beam->put_R2(gs_NEBeamDimensions[i][R2]);
      beam->put_R3(gs_NEBeamDimensions[i][R3]);
      beam->put_R4(gs_NEBeamDimensions[i][R4]);
      beam->put_T(gs_NEBeamDimensions[i][T]);
      beam->put_W1(gs_NEBeamDimensions[i][W1]);
      beam->put_W2(gs_NEBeamDimensions[i][W2]);
      beam->put_C1(gs_NEBeamDimensions[i][C1]);

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

Float64 NEBeamFactory::GetJApprox(NEBeamType type)
{
   int i = (int)type - (int)NEBeamType::NEBT39;
   using namespace _NUBeam;

   Float64 d1 = gs_NEBeamDimensions[i][D1];
   Float64 d2 = gs_NEBeamDimensions[i][D2];
   Float64 d3 = gs_NEBeamDimensions[i][D3];
   Float64 d4 = gs_NEBeamDimensions[i][D4];
   Float64 d5 = gs_NEBeamDimensions[i][D5];
   Float64 tw = gs_NEBeamDimensions[i][T];
   Float64 w1 = gs_NEBeamDimensions[i][W1];
   Float64 w2 = gs_NEBeamDimensions[i][W2];

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
