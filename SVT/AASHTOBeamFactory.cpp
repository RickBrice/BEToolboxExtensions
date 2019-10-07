#include "stdafx.h"

#include "AASHTOBeamFactory.h"
#include "Helpers.h"

static Float64 gs_AASHTOBeamDimensions[][14] = {
   { 0, 4, 3,   0, 5,   5, 0, 11, 6, 6,   3, 0,   5, 0 }, // TypeI
   { 0, 6, 3,   0, 6,   6, 0, 15, 6, 6,   3, 0,   6, 0 }, // TypeII
   { 0, 7, 4.5, 0, 7, 7.5, 0, 19, 7, 7, 4.5, 0, 7.5, 0 }, // TypeIII
   { 0, 8, 6,   0, 8,   9, 0, 23, 8, 8,   6, 0,   9, 0 }, // TypeIV
   { 0, 5, 3,   4, 8,  10, 0, 33, 8, 8,  13, 4,  10, 0 }, // TypeV
   { 0, 5, 3,   4, 8,  10, 0, 42, 8, 8,  13, 4,  10, 0 }, // TypeVI
};


void AASHTOBeamFactory::CreateBeam(AASHTOBeamType type, IShape** ppShape)
{
   if ((int)AASHTOBeamType::TypeI <= (int)type && (int)type <= (int)AASHTOBeamType::TypeVI)
   {
      int i = (int)type - (int)AASHTOBeamType::TypeI;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_AASHTOBeamDimensions[i][C1]);
      beam->put_D1(gs_AASHTOBeamDimensions[i][D1]);
      beam->put_D2(gs_AASHTOBeamDimensions[i][D2]);
      beam->put_D3(gs_AASHTOBeamDimensions[i][D3]);
      beam->put_D4(gs_AASHTOBeamDimensions[i][D4]);
      beam->put_D5(gs_AASHTOBeamDimensions[i][D5]);
      beam->put_D6(gs_AASHTOBeamDimensions[i][D6]);
      beam->put_D7(gs_AASHTOBeamDimensions[i][D7]);
      beam->put_T1(gs_AASHTOBeamDimensions[i][T1]);
      beam->put_T2(gs_AASHTOBeamDimensions[i][T2]);
      beam->put_W1(gs_AASHTOBeamDimensions[i][W1]);
      beam->put_W2(gs_AASHTOBeamDimensions[i][W2]);
      beam->put_W3(gs_AASHTOBeamDimensions[i][W3]);
      beam->put_W4(gs_AASHTOBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_AASHTOnames[] = {
   _T("Type I"),
   _T("Type II"),
   _T("Type III"),
   _T("Type IV"),
   _T("Type V"),
   _T("Type VI"),
};

LPCTSTR AASHTOBeamFactory::GetName(AASHTOBeamType type)
{
   return gs_AASHTOnames[(int)type].c_str();
}

Float64 AASHTOBeamFactory::GetJApprox(AASHTOBeamType type)
{
   int i = (int)type - (int)AASHTOBeamType::TypeI;
   using namespace IBeam;

   Float64 d1 = gs_AASHTOBeamDimensions[i][D1];
   Float64 d2 = gs_AASHTOBeamDimensions[i][D2];
   Float64 d3 = gs_AASHTOBeamDimensions[i][D3];
   Float64 d4 = gs_AASHTOBeamDimensions[i][D4];
   Float64 d5 = gs_AASHTOBeamDimensions[i][D5];
   Float64 d6 = gs_AASHTOBeamDimensions[i][D6];
   Float64 d7 = gs_AASHTOBeamDimensions[i][D7];
   Float64 t1 = gs_AASHTOBeamDimensions[i][T1];
   Float64 t2 = gs_AASHTOBeamDimensions[i][T2];
   Float64 w1 = gs_AASHTOBeamDimensions[i][W1];
   Float64 w2 = gs_AASHTOBeamDimensions[i][W2];
   Float64 w3 = gs_AASHTOBeamDimensions[i][W3];
   Float64 w4 = gs_AASHTOBeamDimensions[i][W4];

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
