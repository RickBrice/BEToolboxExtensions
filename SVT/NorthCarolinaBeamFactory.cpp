#include "stdafx.h"

#include "NorthCarolinaBeamFactory.h"
#include "Helpers.h"

static Float64 gs_NCBeamDimensions[][14] = {
   //  C1    D1    D2   D3    D4    D5    D6     D7    T1   T2    W1   W2    W3   W4
   {    0,  3.5,    2,   2, 6.25, 4.25,    0,    45,    7,   7,   16,   2,  9.5,   0 }, // BT63
   {    0,  3.5,    2,   2, 6.25, 4.25,    0,    54,    7,   7,   16,   2,  9.5,   0 }, // BT72
};


void NCBeamFactory::CreateBeam(NCBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)NCBeamType::BT63 <= (int)type && (int)type < (int)NCBeamType::nSections)
   {
      int i = (int)type - (int)NCBeamType::BT63;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);
      using namespace IBeam;

      Float64 c1, d1, d2, d3, d4, d5, d6, d7, t1, t2, w1, w2, w3, w4;
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][D7], CComBSTR(_T("in")), &d7);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][T1], CComBSTR(_T("in")), &t1);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][T2], CComBSTR(_T("in")), &t2);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_NCBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);

      beam->put_C1(c1);
      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_D6(d6);
      beam->put_D7(d7);
      beam->put_T1(t1);
      beam->put_T2(t2);
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_W3(w3);
      beam->put_W4(w4);


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

Float64 NCBeamFactory::GetJApprox1(NCBeamType type,IUnitConvert* pConvert)
{
   int i = (int)type - (int)NCBeamType::BT63;
   return ComputeJApprox_IBeam(i, pConvert, gs_NCBeamDimensions);
}