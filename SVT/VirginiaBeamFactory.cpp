#include "stdafx.h"

#include "VirginiaBeamFactory.h"
#include "Helpers.h"

static Float64 gs_VirginiaBeamDimensions[][14] = {
   //  C1    D1    D2   D3   D4    D5    D6     D7    T1   T2    W1   W2    W3   W4
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,     8,    7,   7,   18,   2,    9, 3.5 }, // PCBT_29
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    16,    7,   7,   18,   2,    9, 3.5 }, // PCBT_37
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    24,    7,   7,   18,   2,    9, 3.5 }, // PCBT_45
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    32,    7,   7,   18,   2,    9, 3.5 }, // PCBT_53
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    40,    7,   7,   18,   2,    9, 3.5 }, // PCBT_61
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    48,    7,   7,   18,   2,    9, 3.5 }, // PCBT_69
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    56,    7,   7,   18,   2,    9, 3.5 }, // PCBT_77
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    64,    7,   7,   18,   2,    9, 3.5 }, // PCBT_85
   { 0.75,    4,  1.5,   2,   7,    3,  3.5,    72,    7,   7,   18,   2,    9, 3.5 }, // PCBT_93
};


void VirginiaBeamFactory::CreateBeam(VirginiaBeamType type, IShape** ppShape)
{
   if ((int)VirginiaBeamType::PCBT_29 <= (int)type && (int)type < (int)VirginiaBeamType::nSections)
   {
      int i = (int)type - (int)VirginiaBeamType::PCBT_29;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_VirginiaBeamDimensions[i][C1]);
      beam->put_D1(gs_VirginiaBeamDimensions[i][D1]);
      beam->put_D2(gs_VirginiaBeamDimensions[i][D2]);
      beam->put_D3(gs_VirginiaBeamDimensions[i][D3]);
      beam->put_D4(gs_VirginiaBeamDimensions[i][D4]);
      beam->put_D5(gs_VirginiaBeamDimensions[i][D5]);
      beam->put_D6(gs_VirginiaBeamDimensions[i][D6]);
      beam->put_D7(gs_VirginiaBeamDimensions[i][D7]);
      beam->put_T1(gs_VirginiaBeamDimensions[i][T1]);
      beam->put_T2(gs_VirginiaBeamDimensions[i][T2]);
      beam->put_W1(gs_VirginiaBeamDimensions[i][W1]);
      beam->put_W2(gs_VirginiaBeamDimensions[i][W2]);
      beam->put_W3(gs_VirginiaBeamDimensions[i][W3]);
      beam->put_W4(gs_VirginiaBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_Virginianames[] = {
   _T("PCBT-29"),
   _T("PCBT-37"),
   _T("PCBT-45"),
   _T("PCBT-53"),
   _T("PCBT-61"),
   _T("PCBT-69"),
   _T("PCBT-77"),
   _T("PCBT-85"),
   _T("PCBT-93"),
};

LPCTSTR VirginiaBeamFactory::GetName(VirginiaBeamType type)
{
   return gs_Virginianames[(int)type].c_str();
}

Float64 VirginiaBeamFactory::GetJApprox(VirginiaBeamType type)
{
   int i = (int)type - (int)VirginiaBeamType::PCBT_29;
   return ComputeJApprox_IBeam(i, gs_VirginiaBeamDimensions);
}