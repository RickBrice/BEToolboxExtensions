#include "stdafx.h"

#include "OregonBeamFactory.h"
#include "Helpers.h"

static Float64 gs_OregonBeamDimensions[][14] = {
   //  C1    D1    D2   D3   D4    D5    D6     D7    T1   T2    W1   W2    W3   W4
   {    1, 6.25, 0.75,   2,   6,    3,    0,    33,    6,   6,    7,   2,    9,   0 }, // BI51
   {    1, 6.25, 0.75,   2,   6,    3,    0,    45,    6,   6,    7,   2,    9,   0 }, // BI63
   {    1, 6.25, 0.75,   2,   6,    3,    0,    57,    6,   6,    7,   2,    9,   0 }, // BI75

   {    1,    2,    2,   2,   6,    3,    0,    33,    6,   6,   19,   2,    9,   0 }, // BT48
   {    1,    2,    2,   2,   6,    3,    0,    45,    6,   6,   19,   2,    9,   0 }, // BT60
   {    1,    2,    2,   2,   6,    3,    0,    57,    6,   6,   19,   2,    9,   0 }, // BT72
   {    1,    2,    2,   2,   6,    3,    0,    69,    6,   6,   19,   2,    9,   0 }, // BT84
   
   {    1,  2.5,    2,   3,   6,    4,    0,  72.5,    6,   6,   24,   3,   12,   0 }, // BT90
   {    1,  2.5,    2,   3,   6,    4,    0,  78.5,    6,   6,   24,   3,   12,   0 }, // BT96
};


void OregonBeamFactory::CreateBeam(OregonBeamType type, IShape** ppShape)
{
   if ((int)OregonBeamType::BI51 <= (int)type && (int)type < (int)OregonBeamType::nSections)
   {
      int i = (int)type - (int)OregonBeamType::BI51;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_OregonBeamDimensions[i][C1]);
      beam->put_D1(gs_OregonBeamDimensions[i][D1]);
      beam->put_D2(gs_OregonBeamDimensions[i][D2]);
      beam->put_D3(gs_OregonBeamDimensions[i][D3]);
      beam->put_D4(gs_OregonBeamDimensions[i][D4]);
      beam->put_D5(gs_OregonBeamDimensions[i][D5]);
      beam->put_D6(gs_OregonBeamDimensions[i][D6]);
      beam->put_D7(gs_OregonBeamDimensions[i][D7]);
      beam->put_T1(gs_OregonBeamDimensions[i][T1]);
      beam->put_T2(gs_OregonBeamDimensions[i][T2]);
      beam->put_W1(gs_OregonBeamDimensions[i][W1]);
      beam->put_W2(gs_OregonBeamDimensions[i][W2]);
      beam->put_W3(gs_OregonBeamDimensions[i][W3]);
      beam->put_W4(gs_OregonBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_Oregonnames[] = {
   _T("BI51"),
   _T("BI63"),
   _T("BI75"),
   _T("BT48"),
   _T("BT60"),
   _T("BT72"),
   _T("BT84"),
   _T("BT90"),
   _T("BT96"),
};

LPCTSTR OregonBeamFactory::GetName(OregonBeamType type)
{
   return gs_Oregonnames[(int)type].c_str();
}
