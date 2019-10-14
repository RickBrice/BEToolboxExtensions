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

int NCBeamFactory::GetApproxMethods(NCBeamType type)
{
   return AM_J1 | AM_J2;
}

Float64 NCBeamFactory::GetJApprox1(NCBeamType type)
{
   int i = (int)type - (int)NCBeamType::BT63;
   return ComputeJApprox_IBeam(i, gs_NCBeamDimensions);
}