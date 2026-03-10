#include "stdafx.h"

#include "FloridaBeamFactory.h"
#include "Helpers.h"

#include <MathEx.h>

static Float64 gs_FIBeamDimensions[][12] = {
   // D1     D2     D3   D4     D5     H    T     R    W1     W2    W3      C1
   { 3.5,   1.5,   3.5,   7,   7.5,   36,   7,   15,   17,   3.5,   38,   0.75}, // FIB 36
   { 3.5,   1.5,   3.5,   7,   7.5,   45,   7,   15,   17,   3.5,   38,   0.75}, // FIB 45
   { 3.5,   1.5,   3.5,   7,   7.5,   54,   7,   15,   17,   3.5,   38,   0.75}, // FIB 54
   { 3.5,   1.5,   3.5,   7,   7.5,   63,   7,   15,   17,   3.5,   38,   0.75}, // FIB 63
   { 3.5,   1.5,   3.5,   7,   7.5,   72,   7,   15,   17,   3.5,   38,   0.75}, // FIB 72
   { 3.5,   1.5,   3.5,   7,   7.5,   78,   7,   15,   17,   3.5,   38,   0.75}, // FIB 78
   { 3.5,   1.5,   3.5,   7,   7.5,   84,   7,   15,   17,   3.5,   38,   0.75}, // FIB 84
   { 3.5,   1.5,   3.5,   7,   7.5,   96,   7,   15,   17,   3.5,   38,   0.75}, // FIB 93
   { 3.5,   1.5,   3.5,   7,   7.5,  102,   8,   15,   17,   3.5,   39,   0.75 }, // FIB 102
};

static Float64 gs_FUBeamDimensions[][14] = {
   { 0.75,   48,   10,   3,   1,   7,   21,   56,    94,   3,   0.5,   8.5,   16,   1.9375 }, // FUB 48
   { 0.75,   54,   10,   3,   1,   7,   21,   56,    97,   3,   0.5,   8.5,   16,   1.9375 }, // FUB 54
   { 0.75,   63,   10,   3,   1,   7,   21,   56, 101.5,   3,   0.5,   8.5,   16,   1.9375 }, // FUB 63
   { 0.75,   72,   10,   3,   1,   7,   21,   56,   106,   3,   0.5,   8.5,   16,   1.9375 }, // FUB 72
};


void FloridaBeamFactory::CreateBeam(FloridaBeamType type, IUnitConvert* pConvert, IShape** ppShape)
{
   if ((int)FloridaBeamType::FIB36 <= (int)type && (int)type <= (int)FloridaBeamType::FIB102)
   {
      int i = (int)type - (int)FloridaBeamType::FIB36;

      using namespace _FIBBeam;

      Float64 d1, d2, d3, d4, d5, h, t, r, w1, w2, w3, c1;
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][H], CComBSTR(_T("in")), &h);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][T], CComBSTR(_T("in")), &t);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][R], CComBSTR(_T("in")), &r);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_FIBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);

      CComPtr<IPolyShape> poly_shape;
      poly_shape.CoCreateInstance(CLSID_PolyShape);

      const long nSpaces = 100; // number of spaces used to approximate the curved fillets
      Float64 cx, cy;           // center of arc
      Float64 delta;            // sweep angle of arc
      Float64 startAngle;       // start angle for generating points along arc

                                // Start at the lower left corner of the shape
      if (IsZero(c1))
      {
         poly_shape->AddPoint(-w3 / 2, 0.0);
      }
      else
      {
         poly_shape->AddPoint(-w3 / 2 + c1, 0.0);
         poly_shape->AddPoint(-w3 / 2, c1);
      }

      poly_shape->AddPoint(-w3 / 2, d4);

      // compute angle of bottom flange (measured from vertical)
      delta = atan2((w3 - t) / 2, d5);

      // generate bottom left flange-web fillet
      cx = -t / 2 - r;
      cy = d5 + d4 + r*tan(delta / 2);
      startAngle = TWO_PI - delta;
      GenerateFillet(poly_shape, cx, cy, r, startAngle, delta, nSpaces);

      // going around the top flange
      poly_shape->AddPoint(-t / 2, h - d1 - d2 - d3);
      poly_shape->AddPoint(-t / 2 - w2, h - d1 - d2);
      poly_shape->AddPoint(-t / 2 - w2 - w1, h - d1);
      poly_shape->AddPoint(-t / 2 - w2 - w1, h);
      poly_shape->AddPoint(t / 2 + w2 + w1, h);
      poly_shape->AddPoint(t / 2 + w2 + w1, h - d1);
      poly_shape->AddPoint(t / 2 + w2, h - d1 - d2);
      poly_shape->AddPoint(t / 2, h - d1 - d2 - d3);

      // compute angle of bottom flange (measured from vertical)
      delta = atan2((w3 - t) / 2, d5);

      // generate bottom right flange-web fillet
      cx = t / 2 + r;
      cy = d5 + d4 + r*tan(delta / 2);
      startAngle = M_PI;
      GenerateFillet(poly_shape, cx, cy, r, startAngle, delta, nSpaces);

      poly_shape->AddPoint(w3 / 2, d4);

      // bottom right point
      if (IsZero(c1))
      {
         poly_shape->AddPoint(w3 / 2, 0.0);
      }
      else
      {
         poly_shape->AddPoint(w3 / 2, c1);
         poly_shape->AddPoint(w3 / 2 - c1, 0.0);
      }

      // rotate if necessary
      CComQIPtr<IXYPosition> pPosition(poly_shape);
      CComPtr<IPoint2d> origin;
      origin.CoCreateInstance(CLSID_Point2d);
      origin->Move(0, 0);

      CComPtr<IPoint2d> hook_point;
      hook_point.CoCreateInstance(CLSID_Point2d);
      hook_point->Move(0, 0);
      pPosition->MoveEx(origin, hook_point);

      poly_shape.QueryInterface(ppShape);
   }
   else if ((int)FloridaBeamType::FUB48 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      int i = (int)type - (int)FloridaBeamType::FUB48;

      CComPtr<IUBeam2> beam;
      beam.CoCreateInstance(CLSID_UBeam2);
      using namespace _UBeam2; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

      Float64 c1, d1, d2, d3, d4, d5, d6, w1, w2, w3, w4, w5, w6, w7;
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][C1], CComBSTR(_T("in")), &c1);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][D1], CComBSTR(_T("in")), &d1);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][D2], CComBSTR(_T("in")), &d2);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][D3], CComBSTR(_T("in")), &d3);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][D4], CComBSTR(_T("in")), &d4);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][D5], CComBSTR(_T("in")), &d5);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][D6], CComBSTR(_T("in")), &d6);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][W1], CComBSTR(_T("in")), &w1);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][W2], CComBSTR(_T("in")), &w2);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][W3], CComBSTR(_T("in")), &w3);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][W4], CComBSTR(_T("in")), &w4);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][W5], CComBSTR(_T("in")), &w5);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][W6], CComBSTR(_T("in")), &w6);
      pConvert->ConvertToBaseUnits(gs_FUBeamDimensions[i][W7], CComBSTR(_T("in")), &w7);

      beam->put_C1(c1);
      beam->put_D1(d1);
      beam->put_D2(d2);
      beam->put_D3(d3);
      beam->put_D4(d4);
      beam->put_D5(d5);
      beam->put_D6(d6);
      beam->put_W1(w1);
      beam->put_W2(w2);
      beam->put_W3(w3);
      beam->put_W4(w4);
      beam->put_W5(w5);
      beam->put_W6(w6);
      beam->put_W7(w7);


      beam->QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}

std::unique_ptr<WBFL::Geometry::Shape> FloridaBeamFactory::CreateBeam(FloridaBeamType type)
{
   std::unique_ptr<WBFL::Geometry::Shape> beam;

   if ((int)FloridaBeamType::FIB36 <= (int)type && (int)type <= (int)FloridaBeamType::FIB102)
   {
      int i = (int)type - (int)FloridaBeamType::FIB36;

      auto pbeam = std::make_unique<WBFL::Geometry::FIBeam>();

      using namespace _FIBBeam;

      Float64 d1, d2, d3, d4, d5, h, t, r, w1, w2, w3, c1;
      d1 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][D1], WBFL::Units::Measure::Inch);
      d2 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][D2], WBFL::Units::Measure::Inch);
      d3 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][D3], WBFL::Units::Measure::Inch);
      d4 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][D4], WBFL::Units::Measure::Inch);
      d5 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][D5], WBFL::Units::Measure::Inch);
      h = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][H], WBFL::Units::Measure::Inch);
      t  = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][T], WBFL::Units::Measure::Inch);
      r  = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][R], WBFL::Units::Measure::Inch);
      w1 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][W1], WBFL::Units::Measure::Inch);
      w2 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][W2], WBFL::Units::Measure::Inch);
      w3 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][W3], WBFL::Units::Measure::Inch);
      c1 = WBFL::Units::ConvertToSysUnits(gs_FIBeamDimensions[i][C1], WBFL::Units::Measure::Inch);

      pbeam->SetD1(d1);
      pbeam->SetD2(d2);
      pbeam->SetD3(d3);
      pbeam->SetD4(d4);
      pbeam->SetD5(d5);
      pbeam->SetH(h);
      pbeam->SetT(t);
      pbeam->SetR(r);
      pbeam->SetW1(w1);
      pbeam->SetW2(w2);
      pbeam->SetW3(w3);
      pbeam->SetC1(c1);

      beam = std::move(pbeam);
   }
   else if ((int)FloridaBeamType::FUB48 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      int i = (int)type - (int)FloridaBeamType::FUB48;

      auto u_beam = std::make_unique<WBFL::Geometry::UBeam2>();

      using namespace _UBeam2; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

      Float64 c1, d1, d2, d3, d4, d5, d6, w1, w2, w3, w4, w5, w6, w7;
      c1 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][C1], WBFL::Units::Measure::KSI);
      d1 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][D1], WBFL::Units::Measure::KSI);
      d2 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][D2], WBFL::Units::Measure::KSI);
      d3 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][D3], WBFL::Units::Measure::KSI);
      d4 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][D4], WBFL::Units::Measure::KSI);
      d5 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][D5], WBFL::Units::Measure::KSI);
      d6 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][D6], WBFL::Units::Measure::KSI);
      w1 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][W1], WBFL::Units::Measure::KSI);
      w2 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][W2], WBFL::Units::Measure::KSI);
      w3 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][W3], WBFL::Units::Measure::KSI);
      w4 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][W4], WBFL::Units::Measure::KSI);
      w5 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][W5], WBFL::Units::Measure::KSI);
      w6 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][W6], WBFL::Units::Measure::KSI);
      w7 = WBFL::Units::ConvertToSysUnits(gs_FUBeamDimensions[i][W7], WBFL::Units::Measure::KSI);

      u_beam->SetC1(c1);
      u_beam->SetD1(d1);
      u_beam->SetD2(d2);
      u_beam->SetD3(d3);
      u_beam->SetD4(d4);
      u_beam->SetD5(d5);
      u_beam->SetD6(d6);
      u_beam->SetW1(w1);
      u_beam->SetW2(w2);
      u_beam->SetW3(w3);
      u_beam->SetW4(w4);
      u_beam->SetW5(w5);
      u_beam->SetW6(w6);
      u_beam->SetW7(w7);

      beam = std::move(u_beam);
   }
   return beam;
}

void FloridaBeamFactory::GenerateFillet(IPolyShape* pShape, Float64 cx, Float64 cy, Float64 r, Float64 startAngle, Float64 delta, long nSpaces)
{
   if (!IsZero(r))
   {
      Float64 dAngle = delta / nSpaces;
      for (long i = 0; i <= nSpaces; i++)
      {
         Float64 x = cx + r*cos(startAngle + i*dAngle);
         Float64 y = cy + r*sin(startAngle + i*dAngle);

         pShape->AddPoint(x, y);
      }
   }
   else
   {
      // No radius 
      Float64 x = cx;
      Float64 y = cy;
      pShape->AddPoint(x, y);
   }
}

void FloridaBeamFactory::GenerateFillet(std::unique_ptr<WBFL::Geometry::Polygon>& shape, Float64 cx, Float64 cy, Float64 r, Float64 startAngle, Float64 delta, long nSpaces)
{
   if (!IsZero(r))
   {
      Float64 dAngle = delta / nSpaces;
      for (long i = 0; i <= nSpaces; i++)
      {
         Float64 x = cx + r * cos(startAngle + i * dAngle);
         Float64 y = cy + r * sin(startAngle + i * dAngle);

         shape->AddPoint(x, y);
      }
   }
   else
   {
      // No radius 
      Float64 x = cx;
      Float64 y = cy;
      shape->AddPoint(x, y);
   }
}

static std::_tstring gs_FloridaNames[] = {
   _T("FIB-36"),
   _T("FIB-45"),
   _T("FIB-54"),
   _T("FIB-63"),
   _T("FIB-72"),
   _T("FIB-78"),
   _T("FIB-84"),
   _T("FIB-96"),
   _T("FIB-102"),
   _T("FUB-48"), 
   _T("FUB-54"),
   _T("FUB-63"),
   _T("FUB-72")
};

LPCTSTR FloridaBeamFactory::GetName(FloridaBeamType type)
{
   return gs_FloridaNames[(int)type].c_str();
}

int FloridaBeamFactory::GetApproxMethods(FloridaBeamType type)
{
   if ((int)FloridaBeamType::FIB36 <= (int)type && (int)type <= (int)FloridaBeamType::FIB102)
   {
      return AM_J1 | AM_J2 | AM_J3;
   }
   else if ((int)FloridaBeamType::FUB48 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      return AM_J1;
   }

   return AM_NONE;
}

Float64 FloridaBeamFactory::GetJApprox1(FloridaBeamType type,IUnitConvert* pConvert)
{
   if ((int)FloridaBeamType::FIB36 <= (int)type && (int)type <= (int)FloridaBeamType::FIB102)
   {
      int i = (int)type - (int)FloridaBeamType::FIB36;

      using namespace _FIBBeam;

      Float64 d1 = gs_FIBeamDimensions[i][D1];
      Float64 d2 = gs_FIBeamDimensions[i][D2];
      Float64 d3 = gs_FIBeamDimensions[i][D3];
      Float64 d4 = gs_FIBeamDimensions[i][D4];
      Float64 d5 = gs_FIBeamDimensions[i][D5];
      Float64 h = gs_FIBeamDimensions[i][H];
      Float64 tw = gs_FIBeamDimensions[i][T];
      Float64 r = gs_FIBeamDimensions[i][R];
      Float64 w1 = gs_FIBeamDimensions[i][W1];
      Float64 w2 = gs_FIBeamDimensions[i][W2];
      Float64 w3 = gs_FIBeamDimensions[i][W3];
      Float64 c1 = gs_FIBeamDimensions[i][C1];

      Float64 b = 2 * (w1 + w2) + tw;
      Float64 t = 0.5*(d1 + (d1 + d2));
      Float64 t_top = t;
      Float64 J = b*t*t*t; // top flange

      b = w3;
      t = 0.5*(d4 + (d4 + d5));
      Float64 t_bot = t;
      J += b*t*t*t; // bottom flange

      J += (h - t_top - t_bot)*tw*tw*tw; // web, full depth

      J *= 1. / 3.;

      pConvert->ConvertToBaseUnits(J, CComBSTR("in^4"), &J);

      return J;
   }
   else if ((int)FloridaBeamType::FUB48 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      int i = (int)type - (int)FloridaBeamType::FUB48;
      return ComputeJApprox_UBeam2(i, pConvert, gs_FUBeamDimensions);
   }

   ATLASSERT(false); // should never get here
   return -1;

}

Float64 FloridaBeamFactory::GetJApprox1(FloridaBeamType type)
{
   if ((int)FloridaBeamType::FIB36 <= (int)type && (int)type <= (int)FloridaBeamType::FIB102)
   {
      int i = (int)type - (int)FloridaBeamType::FIB36;

      using namespace _FIBBeam;

      Float64 d1 = gs_FIBeamDimensions[i][D1];
      Float64 d2 = gs_FIBeamDimensions[i][D2];
      Float64 d3 = gs_FIBeamDimensions[i][D3];
      Float64 d4 = gs_FIBeamDimensions[i][D4];
      Float64 d5 = gs_FIBeamDimensions[i][D5];
      Float64 h = gs_FIBeamDimensions[i][H];
      Float64 tw = gs_FIBeamDimensions[i][T];
      Float64 r = gs_FIBeamDimensions[i][R];
      Float64 w1 = gs_FIBeamDimensions[i][W1];
      Float64 w2 = gs_FIBeamDimensions[i][W2];
      Float64 w3 = gs_FIBeamDimensions[i][W3];
      Float64 c1 = gs_FIBeamDimensions[i][C1];

      Float64 b = 2 * (w1 + w2) + tw;
      Float64 t = 0.5 * (d1 + (d1 + d2));
      Float64 t_top = t;
      Float64 J = b * t * t * t; // top flange

      b = w3;
      t = 0.5 * (d4 + (d4 + d5));
      Float64 t_bot = t;
      J += b * t * t * t; // bottom flange

      J += (h - t_top - t_bot) * tw * tw * tw; // web, full depth

      J *= 1. / 3.;

      J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

      return J;
   }
   else if ((int)FloridaBeamType::FUB48 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      int i = (int)type - (int)FloridaBeamType::FUB48;
      return ComputeJApprox_UBeam2(i, gs_FUBeamDimensions);
   }

   ATLASSERT(false); // should never get here
   return -1;

}

Float64 FloridaBeamFactory::GetJApprox3(FloridaBeamType type)
{
   if ((int)FloridaBeamType::FIB36 <= (int)type && (int)type <= (int)FloridaBeamType::FIB102)
   {
      int i = (int)type - (int)FloridaBeamType::FIB36;

      using namespace _FIBBeam;

      Float64 d1 = gs_FIBeamDimensions[i][D1];
      Float64 d2 = gs_FIBeamDimensions[i][D2];
      Float64 d3 = gs_FIBeamDimensions[i][D3];
      Float64 d4 = gs_FIBeamDimensions[i][D4];
      Float64 d5 = gs_FIBeamDimensions[i][D5];
      Float64 h = gs_FIBeamDimensions[i][H];
      Float64 tw = gs_FIBeamDimensions[i][T];
      Float64 r = gs_FIBeamDimensions[i][R];
      Float64 w1 = gs_FIBeamDimensions[i][W1];
      Float64 w2 = gs_FIBeamDimensions[i][W2];
      Float64 w3 = gs_FIBeamDimensions[i][W3];
      Float64 c1 = gs_FIBeamDimensions[i][C1];

      Float64 b = 2 * (w1 + w2) + tw;
      Float64 t = 0.5 * (d1 + (d1 + d2));
      Float64 t_top = t;
      Float64 J = b * t * t * t; // top flange

      b = w3;
      t = 0.5 * (d4 + (d4 + d5));
      Float64 t_bot = t;
      J += b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // bottom flange

      b = (h - t_top - t_bot);
      J += b * tw * tw * tw * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // web, full depth

      J *= 1. / 3.;

      J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

      return J;
   }
   else if ((int)FloridaBeamType::FUB48 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      int i = (int)type - (int)FloridaBeamType::FUB48;
      return ComputeJApprox3_UBeam2(i, gs_FUBeamDimensions);
   }

   ATLASSERT(false); // should never get here
   return -1;

}
