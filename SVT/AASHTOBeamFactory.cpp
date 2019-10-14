#include "stdafx.h"

#include "AASHTOBeamFactory.h"
#include "Helpers.h"

static Float64 gs_AASHTOBeamDimensions[][14] = {
   //  C1    D1    D2   D3   D4    D5    D6     D7    T1   T2    W1   W2    W3   W4
   {    0,    4,    3,   0,   5,    5,    0,    11,    6,   6,    3,   0,    5,   0 }, // TypeI
   {    0,    6,    3,   0,   6,    6,    0,    15,    6,   6,    3,   0,    6,   0 }, // TypeII
   {    0,    7,  4.5,   0,   7,  7.5,    0,    19,    7,   7,  4.5,   0,  7.5,   0 }, // TypeIII
   {    0,    8,    6,   0,   8,    9,    0,    23,    8,   8,    6,   0,    9,   0 }, // TypeIV
   {    0,    5,    3,   4,   8,   10,    0,    33,    8,   8,   13,   4,   10,   0 }, // TypeV
   {    0,    5,    3,   4,   8,   10,    0,    42,    8,   8,   13,   4,   10,   0 }, // TypeVI
   {    0,  3.5,    2,   2,   6,  4.5,    0,    36,    6,   6,   16,   2,   10,   0 }, // BT54
   {    0,  3.5,    2,   2,   6,  4.5,    0,    45,    6,   6,   16,   2,   10,   0 }, // BT63
   {    0,  3.5,    2,   2,   6,  4.5,    0,    54,    6,   6,   16,   2,   10,   0 }, // BT72
};


void AASHTOBeamFactory::CreateBeam(AASHTOBeamType type, IShape** ppShape)
{
   if ((int)AASHTOBeamType::TypeI <= (int)type && (int)type < (int)AASHTOBeamType::nSections)
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
   _T("BT-54"),
   _T("BT-63"),
   _T("BT-72"),
};

LPCTSTR AASHTOBeamFactory::GetName(AASHTOBeamType type)
{
   return gs_AASHTOnames[(int)type].c_str();
}

int AASHTOBeamFactory::GetApproxMethods(AASHTOBeamType type)
{
   return AM_J1 | AM_J2;
}

Float64 AASHTOBeamFactory::GetJApprox1(AASHTOBeamType type)
{
   int i = (int)type - (int)AASHTOBeamType::TypeI;
   return ComputeJApprox_IBeam(i, gs_AASHTOBeamDimensions);
}
