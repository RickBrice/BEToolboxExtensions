#include "stdafx.h"

#include "CTBeamFactory.h"
#include "Helpers.h"

static Float64 gs_CTNUBeamDimensions[][13] = {
   { 3.9375, 3.9375, 27.375, 5.875, 7.875, 7.875, 7.875, 0.1, 0.1, 7.875, 47.25, 29.5, 0 }, //BT49
   { 3.9375, 3.9375, 33.5, 5.875, 7.875, 7.875, 7.875, 0.1, 0.1, 7.875, 47.25, 29.5, 0 }, //BT55
   { 3.9375, 3.9375, 39.375, 5.875, 7.875, 7.875, 7.875, 0.1, 0.1, 7.875, 47.25, 29.5, 0 }, //BT61
   { 3.9375, 3.9375, 45.276, 5.875, 7.875, 7.875, 7.875, 0.1, 0.1, 7.875, 47.25, 29.5, 0 }, //BT67
   { 3.9375, 3.9375, 51.25, 5.875, 7.875, 7.875, 7.875, 0.1, 0.1, 7.875, 47.25, 29.5, 0 }, //BT73
   { 3.9375, 3.9375, 57.125, 5.875, 7.875, 7.875, 7.875, 0.1, 0.1, 7.875, 47.25, 29.5, 0 }, //BT79
   { 3.9375, 3.9375, 63, 5.875, 7.875, 7.875, 7.875, 0.1, 0.1, 7.875, 47.25, 29.5, 0 }, //BT85
   { 3, 2, 30, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF48
   { 3, 2, 36, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF54
   { 3, 2, 42, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF60
   { 3, 2, 48, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF66
   { 3, 2, 54, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF72
   { 3, 2, 60, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF78
   { 3, 2, 66, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF84
   { 3, 2, 72, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF90
   { 3, 2, 78, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF96
   { 3, 2, 84, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF102
   { 3, 2, 90, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF108
   { 3, 2, 96, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF114
   { 3, 2, 102, 6.375, 6.625, 10, 10, 2.5, 2.5, 6.5, 48, 43, 0.75 }, //WF120
};

static Float64 gs_CTIBeamDimensions[][14] = {
   { 0, 3, 6, 0, 6, 6, 0, 15, 7, 7, 6, 0, 6, 0 }, //I36
   { 0, 3, 6, 0, 6, 6, 0, 21, 7, 7, 6, 0, 6, 0 }, //I42
   { 0, 3, 6, 0, 6, 6, 0, 27, 7, 7, 6, 0, 6, 0 }, //I48
   { 0, 3, 6, 0, 6, 6, 0, 33, 7, 7, 6, 0, 6, 0 }, //I54
   { 0, 3, 6, 0, 6, 6, 0, 39, 7, 7, 6, 0, 6, 0 }, //I60
   { 0, 3, 6, 0, 6, 6, 0, 45, 7, 7, 6, 0, 6, 0 }, //I66,
};

static Float64 gs_CTUBeamDimensions[][13] =
{
   { 55.125, 6.875, 3, 0, 11.75, 6.875, 4.875, 7.875, 59, 86.5625, 3, 5.1, 2.9375 }, //Tub55
   { 61, 6.875, 3, 0, 11.75, 6.875, 4.875, 7.875, 59, 89.5, 3, 5.1, 2.9375 }, //Tub61
   { 66.875, 6.875, 3, 0, 11.75, 6.875, 4.875, 7.875, 59, 92.4375, 3, 5.1, 2.9375 }, //Tub67
   { 72.785, 6.875, 3, 0, 11.75, 6.875, 4.875, 7.875, 59, 95.4325, 3, 5.1, 2.9375 }, //Tub73
   { 78.75, 6.875, 3, 0, 11.75, 6.875, 4.875, 7.875, 59, 98.375, 3, 5.1, 2.9375 }, //Tub79
   { 84.625, 6.875, 3, 0, 11.75, 6.875, 4.875, 7.875, 59, 101.3125, 3, 5.1, 2.9375 }, //Tub85
};

void CTBeamFactory::CreateBeam(CTBeamType type, IShape** ppShape)
{
   if ((int)CTBeamType::BT49 <= (int)type && (int)type <= (int)CTBeamType::WF120)
   {
      int i = (int)type - (int)CTBeamType::BT49;
      CComPtr<INUBeam> beam;
      beam.CoCreateInstance(CLSID_NUBeam);
      using namespace _NUBeam;

      beam->put_D1(gs_CTNUBeamDimensions[i][D1]);
      beam->put_D2(gs_CTNUBeamDimensions[i][D2]);
      beam->put_D3(gs_CTNUBeamDimensions[i][D3]);
      beam->put_D4(gs_CTNUBeamDimensions[i][D4]);
      beam->put_D5(gs_CTNUBeamDimensions[i][D5]);
      beam->put_R1(gs_CTNUBeamDimensions[i][R1]);
      beam->put_R2(gs_CTNUBeamDimensions[i][R2]);
      beam->put_R3(gs_CTNUBeamDimensions[i][R3]);
      beam->put_R4(gs_CTNUBeamDimensions[i][R4]);
      beam->put_T(gs_CTNUBeamDimensions[i][T]);
      beam->put_W1(gs_CTNUBeamDimensions[i][W1]);
      beam->put_W2(gs_CTNUBeamDimensions[i][W2]);
      beam->put_C1(gs_CTNUBeamDimensions[i][C1]);

      beam.QueryInterface(ppShape);
   }
   else if ((int)CTBeamType::I36 <= (int)type && (int)type <= (int)CTBeamType::I66)
   {
      int i = (int)type - (int)CTBeamType::I36;
      CComPtr<IPrecastBeam> beam;
      beam.CoCreateInstance(CLSID_PrecastBeam);

      using namespace IBeam; // this is so we don't have to use the name space below (eg IBeam::C1, IBeam::D2...)

      beam->put_C1(gs_CTIBeamDimensions[i][C1]);
      beam->put_D1(gs_CTIBeamDimensions[i][D1]);
      beam->put_D2(gs_CTIBeamDimensions[i][D2]);
      beam->put_D3(gs_CTIBeamDimensions[i][D3]);
      beam->put_D4(gs_CTIBeamDimensions[i][D4]);
      beam->put_D5(gs_CTIBeamDimensions[i][D5]);
      beam->put_D6(gs_CTIBeamDimensions[i][D6]);
      beam->put_D7(gs_CTIBeamDimensions[i][D7]);
      beam->put_T1(gs_CTIBeamDimensions[i][T1]);
      beam->put_T2(gs_CTIBeamDimensions[i][T2]);
      beam->put_W1(gs_CTIBeamDimensions[i][W1]);
      beam->put_W2(gs_CTIBeamDimensions[i][W2]);
      beam->put_W3(gs_CTIBeamDimensions[i][W3]);
      beam->put_W4(gs_CTIBeamDimensions[i][W4]);

      beam.QueryInterface(ppShape);
   }
   //else if ((int)Tub55 <= (int)type && (int)type <= (int)Tub85)
   //{
   //   int i = (int)type - (int)Tub55;

   //   CComPtr<IUBeam> beam;
   //   beam.CoCreateInstance(CLSID_UBeam);
   //   using namespace _UBeam; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

   //   beam->put_D1(gs_CTUBeamDimensions[i][D1]);
   //   beam->put_D2(gs_CTUBeamDimensions[i][D2]);
   //   beam->put_D3(gs_CTUBeamDimensions[i][D3]);
   //   beam->put_D4(gs_CTUBeamDimensions[i][D4]);
   //   beam->put_D5(gs_CTUBeamDimensions[i][D5]);
   //   beam->put_D6(gs_CTUBeamDimensions[i][D6]);
   //   beam->put_D7(gs_CTUBeamDimensions[i][D7]);
   //   beam->put_T(gs_CTUBeamDimensions[i][T]);
   //   beam->put_W1(gs_CTUBeamDimensions[i][W1]);
   //   beam->put_W2(gs_CTUBeamDimensions[i][W2]);
   //   beam->put_W3(gs_CTUBeamDimensions[i][W3]);
   //   beam->put_W4(gs_CTUBeamDimensions[i][W4]);
   //   beam->put_W5(gs_CTUBeamDimensions[i][W5]);

   //   beam->QueryInterface(ppShape);
   //}
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_CTnames[] = {
   _T("BT49"),
   _T("BT55"),
   _T("BT61"),
   _T("BT67"),
   _T("BT73"),
   _T("BT79"),
   _T("BT85"),
   _T("WF48"),
   _T("WF54"),
   _T("WF60"),
   _T("WF66"),
   _T("WF72"),
   _T("WF78"),
   _T("WF84"),
   _T("WF90"),
   _T("WF96"),
   _T("WF102"),
   _T("WF108"),
   _T("WF114"),
   _T("WF120"),
   _T("I36"),
   _T("I42"),
   _T("I48"),
   _T("I54"),
   _T("I60"),
   _T("I66"),
   _T("Tub55"),
   _T("Tub61"),
   _T("Tub67"),
   _T("Tub73"),
   _T("Tub79"),
   _T("Tub85"),
};

LPCTSTR CTBeamFactory::GetName(CTBeamType type)
{
   return gs_CTnames[(int)type].c_str();
}
