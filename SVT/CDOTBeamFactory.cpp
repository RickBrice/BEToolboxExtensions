#include "stdafx.h"

#include "CDOTBeamFactory.h"
#include "Helpers.h"

static Float64 gs_CDOTBeamDimensions[][14] = {
   //  C1    D1    D2   D3     D4    D5    D6      D7    T1   T2    W1   W2     W3   W4
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3,  6.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 30
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 12.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 36
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 21.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 45
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 30.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 54
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 39.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 63
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 48.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 72
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 57.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 81
   { 0.75, 3.75,    3,   3, 5.875,  4.5,    3, 66.875,    7,   7, 18.5,   3, 13.25,   3 }, // CBT 90
};


void CDOTBeamFactory::CreateBeam(CDOTBeamType type, IShape** ppShape)
{
   if ((int)CDOTBeamType::CBT_30 <= (int)type && (int)type < (int)CDOTBeamType::nSections)
   {
      int i = (int)type - (int)CDOTBeamType::CBT_30;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_CDOTBeamDimensions[i][C1]);
      beam->put_D1(gs_CDOTBeamDimensions[i][D1]);
      beam->put_D2(gs_CDOTBeamDimensions[i][D2]);
      beam->put_D3(gs_CDOTBeamDimensions[i][D3]);
      beam->put_D4(gs_CDOTBeamDimensions[i][D4]);
      beam->put_D5(gs_CDOTBeamDimensions[i][D5]);
      beam->put_D6(gs_CDOTBeamDimensions[i][D6]);
      beam->put_D7(gs_CDOTBeamDimensions[i][D7]);
      beam->put_T1(gs_CDOTBeamDimensions[i][T1]);
      beam->put_T2(gs_CDOTBeamDimensions[i][T2]);
      beam->put_W1(gs_CDOTBeamDimensions[i][W1]);
      beam->put_W2(gs_CDOTBeamDimensions[i][W2]);
      beam->put_W3(gs_CDOTBeamDimensions[i][W3]);
      beam->put_W4(gs_CDOTBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_CDOTnames[] = {
   _T("CBT 30"),
   _T("CBT 36"),
   _T("CBT 45"),
   _T("CBT 54"),
   _T("CBT 63"),
   _T("CBT 72"),
   _T("CBT 81"),
   _T("CBT 90"),
};

LPCTSTR CDOTBeamFactory::GetName(CDOTBeamType type)
{
   return gs_CDOTnames[(int)type].c_str();
}

int CDOTBeamFactory::GetApproxMethods(CDOTBeamType type)
{
   return AM_J1 | AM_J2;
}

Float64 CDOTBeamFactory::GetJApprox1(CDOTBeamType type)
{
   int i = (int)type - (int)CDOTBeamType::CBT_30;
   return ComputeJApprox_IBeam(i, gs_CDOTBeamDimensions);
}