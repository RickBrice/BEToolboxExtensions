#include "stdafx.h"

#include "OhioBeamFactory.h"
#include "Helpers.h"

static Float64 gs_OhioBeamDimensions[][14] = {
   //  C1  D1   D2   D3   D4    D5    D6     D7    T1   T2    W1   W2    W3   W4
   { 0.75,  5,   3,   3, 5.5,    7,    2,  10.5,    8,   8, 17.5,   3,   14,   2 }, // WF36-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  16.5,    8,   8, 17.5,   3,   14,   2 }, // WF42-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  22.5,    8,   8, 17.5,   3,   14,   2 }, // WF48-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  28.5,    8,   8, 17.5,   3,   14,   2 }, // WF54-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  34.5,    8,   8, 17.5,   3,   14,   2 }, // WF60-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  40.5,    8,   8, 17.5,   3,   14,   2 }, // WF66-49
   { 0.75,  5,   3,   3, 5.5,    7,    2,  46.5,    8,   8, 17.5,   3,   14,   2 }, // WF72-49

   { 0.75,  4,   2,   3,   8,    9,    0,    34,    8,   8,   11,   3,    9,   0 }, // TypeIV_60
   { 0.75,  4,   2,   3,   8,    9,    0,    40,    8,   8,   11,   3,    9,   0 }, // TypeIV_66
   { 0.75,  4,   2,   3,   8,    9,    0,    46,    8,   8,   11,   3,    9,   0 }, // TypeIV_72
};


void OhioBeamFactory::CreateBeam(OhioBeamType type, IShape** ppShape)
{
   if ((int)OhioBeamType::WF36_49 <= (int)type && (int)type < (int)OhioBeamType::nSections)
   {
      int i = (int)type - (int)OhioBeamType::WF36_49;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_OhioBeamDimensions[i][C1]);
      beam->put_D1(gs_OhioBeamDimensions[i][D1]);
      beam->put_D2(gs_OhioBeamDimensions[i][D2]);
      beam->put_D3(gs_OhioBeamDimensions[i][D3]);
      beam->put_D4(gs_OhioBeamDimensions[i][D4]);
      beam->put_D5(gs_OhioBeamDimensions[i][D5]);
      beam->put_D6(gs_OhioBeamDimensions[i][D6]);
      beam->put_D7(gs_OhioBeamDimensions[i][D7]);
      beam->put_T1(gs_OhioBeamDimensions[i][T1]);
      beam->put_T2(gs_OhioBeamDimensions[i][T2]);
      beam->put_W1(gs_OhioBeamDimensions[i][W1]);
      beam->put_W2(gs_OhioBeamDimensions[i][W2]);
      beam->put_W3(gs_OhioBeamDimensions[i][W3]);
      beam->put_W4(gs_OhioBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_Ohionames[] = {
   _T("WF36-49"),
   _T("WF42-49"),
   _T("WF48-49"),
   _T("WF54-49"),
   _T("WF60-49"),
   _T("WF66-49"),
   _T("WF72-49"),
   _T("Modified Type IV (60\")"),
   _T("Modified Type IV (66\")"),
   _T("Modified Type IV (72\")"),
};

LPCTSTR OhioBeamFactory::GetName(OhioBeamType type)
{
   return gs_Ohionames[(int)type].c_str();
}

Float64 OhioBeamFactory::GetJApprox(OhioBeamType type)
{
   int i = (int)type - (int)OhioBeamType::WF36_49;
   using namespace IBeam;

   Float64 d1 = gs_OhioBeamDimensions[i][D1];
   Float64 d2 = gs_OhioBeamDimensions[i][D2];
   Float64 d3 = gs_OhioBeamDimensions[i][D3];
   Float64 d4 = gs_OhioBeamDimensions[i][D4];
   Float64 d5 = gs_OhioBeamDimensions[i][D5];
   Float64 d6 = gs_OhioBeamDimensions[i][D6];
   Float64 d7 = gs_OhioBeamDimensions[i][D7];
   Float64 t1 = gs_OhioBeamDimensions[i][T1];
   Float64 t2 = gs_OhioBeamDimensions[i][T2];
   Float64 w1 = gs_OhioBeamDimensions[i][W1];
   Float64 w2 = gs_OhioBeamDimensions[i][W2];
   Float64 w3 = gs_OhioBeamDimensions[i][W3];
   Float64 w4 = gs_OhioBeamDimensions[i][W4];

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
