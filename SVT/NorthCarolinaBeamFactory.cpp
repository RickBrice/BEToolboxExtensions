#include "stdafx.h"

#include "NorthCarolinaBeamFactory.h"
#include "Helpers.h"

static Float64 gs_NCBeamDimensions[][14] = {
   //  C1    D1    D2   D3    D4    D5    D6     D7    T1   T2    W1   W2    W3   W4
   {    0,  3.5,    2,   2, 6.25, 4.25,    0,    45,    7,   7,   16,   2,  9.5,   0 }, // BT63
   {    0,  3.5,    2,   2, 6.25, 4.25,    0,    54,    7,   7,   16,   2,  9.5,   0 }, // BT72
};


void NCBeamFactory::CreateBeam(NCBeamType type, IShape** ppShape)
{
   if ((int)NCBeamType::BT63 <= (int)type && (int)type < (int)NCBeamType::nSections)
   {
      int i = (int)type - (int)NCBeamType::BT63;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_NCBeamDimensions[i][C1]);
      beam->put_D1(gs_NCBeamDimensions[i][D1]);
      beam->put_D2(gs_NCBeamDimensions[i][D2]);
      beam->put_D3(gs_NCBeamDimensions[i][D3]);
      beam->put_D4(gs_NCBeamDimensions[i][D4]);
      beam->put_D5(gs_NCBeamDimensions[i][D5]);
      beam->put_D6(gs_NCBeamDimensions[i][D6]);
      beam->put_D7(gs_NCBeamDimensions[i][D7]);
      beam->put_T1(gs_NCBeamDimensions[i][T1]);
      beam->put_T2(gs_NCBeamDimensions[i][T2]);
      beam->put_W1(gs_NCBeamDimensions[i][W1]);
      beam->put_W2(gs_NCBeamDimensions[i][W2]);
      beam->put_W3(gs_NCBeamDimensions[i][W3]);
      beam->put_W4(gs_NCBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_NCnames[] = {
   _T("BT63"),
   _T("BT72"),
};

LPCTSTR NCBeamFactory::GetName(NCBeamType type)
{
   return gs_NCnames[(int)type].c_str();
}

Float64 NCBeamFactory::GetJApprox(NCBeamType type)
{
   int i = (int)type - (int)NCBeamType::BT63;
   using namespace IBeam;

   Float64 d1 = gs_NCBeamDimensions[i][D1];
   Float64 d2 = gs_NCBeamDimensions[i][D2];
   Float64 d3 = gs_NCBeamDimensions[i][D3];
   Float64 d4 = gs_NCBeamDimensions[i][D4];
   Float64 d5 = gs_NCBeamDimensions[i][D5];
   Float64 d6 = gs_NCBeamDimensions[i][D6];
   Float64 d7 = gs_NCBeamDimensions[i][D7];
   Float64 t1 = gs_NCBeamDimensions[i][T1];
   Float64 t2 = gs_NCBeamDimensions[i][T2];
   Float64 w1 = gs_NCBeamDimensions[i][W1];
   Float64 w2 = gs_NCBeamDimensions[i][W2];
   Float64 w3 = gs_NCBeamDimensions[i][W3];
   Float64 w4 = gs_NCBeamDimensions[i][W4];

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