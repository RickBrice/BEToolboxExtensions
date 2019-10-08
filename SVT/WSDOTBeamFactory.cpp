#include "stdafx.h"

#include "WSDOTBeamFactory.h"
#include "Helpers.h"

static Float64 gs_WSDOTBeamDimensions[][14] = {
   { 1,3,3,3,5.125,4.5,3,14.375,6.125,6.125,18.4375,3,13.125,3 },// WF36G
   { 1,3,3,3,5.125,4.5,3,20.375,6.125,6.125,18.4375,3,13.125,3 },// WF42G
   { 1,3,3,3,5.125,4.5,3,28.375,6.125,6.125,18.4375,3,13.125,3 },// WF50G
   { 1,3,3,3,5.125,4.5,3,36.375,6.125,6.125,18.4375,3,13.125,3 },// WF58G
   { 1,3,3,3,5.125,4.5,3,44.375,6.125,6.125,18.4375,3,13.125,3 },// WF66G
   { 1,3,3,3,5.125,4.5,3,52.375,6.125,6.125,18.4375,3,13.125,3 },// WF74G
   { 1,3,3,3,5.125,4.5,3,61.000,6.125,6.125,18.4375,3,13.125,3 },// WF83G
   { 1,3,3,3,5.125,4.5,3,72.875,6.125,6.125,18.4375,3,13.125,3 },// WF95G
   { 1,3,3,3,5.125,4.5,3,78.375,6.125,6.125,18.4375,3,13.125,3 },// WF100G
};


void WSDOTBeamFactory::CreateBeam(WSDOTBeamType type, IShape** ppShape)
{
   if ((int)WSDOTBeamType::WF36G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::WF36G;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      beam->put_C1(gs_WSDOTBeamDimensions[i][C1]);
      beam->put_D1(gs_WSDOTBeamDimensions[i][D1]);
      beam->put_D2(gs_WSDOTBeamDimensions[i][D2]);
      beam->put_D3(gs_WSDOTBeamDimensions[i][D3]);
      beam->put_D4(gs_WSDOTBeamDimensions[i][D4]);
      beam->put_D5(gs_WSDOTBeamDimensions[i][D5]);
      beam->put_D6(gs_WSDOTBeamDimensions[i][D6]);
      beam->put_D7(gs_WSDOTBeamDimensions[i][D7]);
      beam->put_T1(gs_WSDOTBeamDimensions[i][T1]);
      beam->put_T2(gs_WSDOTBeamDimensions[i][T2]);
      beam->put_W1(gs_WSDOTBeamDimensions[i][W1]);
      beam->put_W2(gs_WSDOTBeamDimensions[i][W2]);
      beam->put_W3(gs_WSDOTBeamDimensions[i][W3]);
      beam->put_W4(gs_WSDOTBeamDimensions[i][W4]);


      beam.QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_WSDOTnames[] = {
   _T("WF36G"),
   _T("WF42G"),
   _T("WF50G"),
   _T("WF58G"),
   _T("WF66G"),
   _T("WF74G"),
   _T("WF83G"),
   _T("WF95G"),
   _T("WF100G"),
};

LPCTSTR WSDOTBeamFactory::GetName(WSDOTBeamType type)
{
   return gs_WSDOTnames[(int)type].c_str();
}

Float64 WSDOTBeamFactory::GetJApprox(WSDOTBeamType type)
{
   int i = (int)type - (int)WSDOTBeamType::WF36G;
   return ComputeJApprox_IBeam(i, gs_WSDOTBeamDimensions);
}