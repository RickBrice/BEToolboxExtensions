#include "stdafx.h"

#include "WSDOTBeamFactory.h"
#include "Helpers.h"

static Float64 gs_WSDOTBeamDimensions[][14] = {
   { 1,     3.5,     1.5,   0,   5,   2,   0,   30,   6,   6,   4.5,   0,     7,   0 }, // W42G
   { 1,       5,       2,   0,   6,   3,   0,   34,   6,   6,     7,   0,   9.5,   0 }, // W50G
   { 1,       5,       2,   0,   6,   3,   0,   42,   6,   6,   9.5,   0,   9.5,   0 }, // W58G
   { 1,   2.875,   2.625,   2,   6,   3,   0,   57,   6,   6,  16.5,   2,   9.5,   0 }, // W74G
   { 1,3,3,3,5.125,4.5,3,14.375,6.125,6.125,18.4375,3,13.125,3 }, // WF36G
   { 1,3,3,3,5.125,4.5,3,20.375,6.125,6.125,18.4375,3,13.125,3 }, // WF42G
   { 1,3,3,3,5.125,4.5,3,28.375,6.125,6.125,18.4375,3,13.125,3 }, // WF50G
   { 1,3,3,3,5.125,4.5,3,36.375,6.125,6.125,18.4375,3,13.125,3 }, // WF58G
   { 1,3,3,3,5.125,4.5,3,44.375,6.125,6.125,18.4375,3,13.125,3 }, // WF66G
   { 1,3,3,3,5.125,4.5,3,52.375,6.125,6.125,18.4375,3,13.125,3 }, // WF74G
   { 1,3,3,3,5.125,4.5,3,61.000,6.125,6.125,18.4375,3,13.125,3 }, // WF83G
   { 1,3,3,3,5.125,4.5,3,72.875,6.125,6.125,18.4375,3,13.125,3 }, // WF95G
   { 1,3,3,3,5.125,4.5,3,78.375,6.125,6.125,18.4375,3,13.125,3 }, // WF100G
};


static Float64 gs_WSDOTUBeamDimensions[][13] =
{
   //    D1     D2  D3  D4  D5 D6 D7 T W1 W2 W3 W4 W5
   { 54,   6,   6,   0,   0,   0,   0,   7,   48,   63.42857143,   12,   0,   0   }, // U54G4
   { 54,   6,   6,   0,   0,   0,   0,   7,   60,   75.42857143,   12,   0,   0   }, // U54G5
   { 54,   6,   6,   0,   0,   0,   0,   7,   72,   87.42857143,   24,   0,   0   }, // U54G6
   { 66,   6,   6,   0,   0,   0,   0,   7,   48,   66.85714286,   12,   0,   0   }, // U66G4
   { 66,   6,   6,   0,   0,   0,   0,   7,   60,   78.85714286,   12,   0,   0   }, // U66G5
   { 66,   6,   6,   0,   0,   0,   0,   7,   72,   90.85714286,   24,   0,   0   }, // U66G6
   { 78,   6,   6,   0,   0,   0,   0,   7,   48,   70.28571429,   12,   0,   0   }, // U78G4
   { 78,   6,   6,   0,   0,   0,   0,   7,   60,   82.28571429,   12,   0,   0   }, // U78G5
   { 78,   6,   6,   0,   0,   0,   0,   7,   72,   94.28571429,   24,   0,   0   }, // U78G6
   { 60,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   48,   73.428571,   12,   3,   5   }, // UF60G4
   { 60,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   60,   85.428571,   12,   3,   5   }, // UF60G5
   { 60,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   72,   97.428571,   24,   3,   5   }, // UF60G6
   { 72,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   48,   76.857143,   12,   3,   5   }, // UF72G4
   { 72,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   60,   88.857143,   12,   3,   5   }, // UF72G5
   { 72,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   72,   100.85714,   24,   3,   5   }, // UF72G6
   { 84,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   48,   80.285714,   12,   3,   5   }, // UF84G4
   { 84,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   60,   92.285714,   12,   3,   5   }, // UF84G5
   { 84,   6,   6,   4.5,   1.5,   4.5,   1.5,   7,   72,   92.285714,   12,   3,   5   }, // UF84G6

};

void WSDOTBeamFactory::CreateBeam(WSDOTBeamType type, IShape** ppShape)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
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
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;

      CComPtr<IUBeam> beam;
      beam.CoCreateInstance(CLSID_UBeam);
      using namespace _UBeam; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

      beam->put_D1(gs_WSDOTUBeamDimensions[i][D1]);
      beam->put_D2(gs_WSDOTUBeamDimensions[i][D2]);
      beam->put_D3(gs_WSDOTUBeamDimensions[i][D3]);
      beam->put_D4(gs_WSDOTUBeamDimensions[i][D4]);
      beam->put_D5(gs_WSDOTUBeamDimensions[i][D5]);
      beam->put_D6(gs_WSDOTUBeamDimensions[i][D6]);
      beam->put_D7(gs_WSDOTUBeamDimensions[i][D7]);
      beam->put_T(gs_WSDOTUBeamDimensions[i][T]);
      beam->put_W1(gs_WSDOTUBeamDimensions[i][W1]);
      beam->put_W2(gs_WSDOTUBeamDimensions[i][W2]);
      beam->put_W3(gs_WSDOTUBeamDimensions[i][W3]);
      beam->put_W4(gs_WSDOTUBeamDimensions[i][W4]);
      beam->put_W5(gs_WSDOTUBeamDimensions[i][W5]);

      beam->QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_WSDOTnames[] = {
   _T("W42G"),
   _T("W50G"),
   _T("W58G"),
   _T("W74G"),
   _T("WF36G"),
   _T("WF42G"),
   _T("WF50G"),
   _T("WF58G"),
   _T("WF66G"),
   _T("WF74G"),
   _T("WF83G"),
   _T("WF95G"),
   _T("WF100G"),
   _T("U54G4"),
   _T("U54G5"),
   _T("U54G6"),
   _T("U66G4"),
   _T("U66G5"),
   _T("U66G6"),
   _T("U78G4"),
   _T("U78G5"),
   _T("U78G6"),
   _T("UF60G4"),
   _T("UF60G5"),
   _T("UF60G6"),
   _T("UF72G4"),
   _T("UF72G5"),
   _T("UF72G6"),
   _T("UF84G4"),
   _T("UF84G5"),
   _T("UF84G6"),
};

LPCTSTR WSDOTBeamFactory::GetName(WSDOTBeamType type)
{
   return gs_WSDOTnames[(int)type].c_str();
}

int WSDOTBeamFactory::GetApproxMethods(WSDOTBeamType type)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
      return AM_J1 | AM_J2;
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;
      return AM_J1;
   }
   return AM_NONE;
}

Float64 WSDOTBeamFactory::GetJApprox1(WSDOTBeamType type)
{
   if ((int)WSDOTBeamType::W42G <= (int)type && (int)type <= (int)WSDOTBeamType::WF100G)
   {
      int i = (int)type - (int)WSDOTBeamType::W42G;
      return ComputeJApprox_IBeam(i, gs_WSDOTBeamDimensions);
   }
   else if ((int)WSDOTBeamType::U54G4 <= (int)type && (int)type < (int)WSDOTBeamType::nSections)
   {
      int i = (int)type - (int)WSDOTBeamType::U54G4;
      return ComputeJApprox_UBeam(i, gs_WSDOTUBeamDimensions);
   }
   return -1;
}