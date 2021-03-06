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


void FloridaBeamFactory::CreateBeam(FloridaBeamType type, IShape** ppShape)
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
      Float64 h  = gs_FIBeamDimensions[i][H];
      Float64 t  = gs_FIBeamDimensions[i][T];
      Float64 r  = gs_FIBeamDimensions[i][R];
      Float64 w1 = gs_FIBeamDimensions[i][W1];
      Float64 w2 = gs_FIBeamDimensions[i][W2];
      Float64 w3 = gs_FIBeamDimensions[i][W3];
      Float64 c1 = gs_FIBeamDimensions[i][C1];

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

      beam->put_C1(gs_FUBeamDimensions[i][C1]);
      beam->put_D1(gs_FUBeamDimensions[i][D1]);
      beam->put_D2(gs_FUBeamDimensions[i][D2]);
      beam->put_D3(gs_FUBeamDimensions[i][D3]);
      beam->put_D4(gs_FUBeamDimensions[i][D4]);
      beam->put_D5(gs_FUBeamDimensions[i][D5]);
      beam->put_D6(gs_FUBeamDimensions[i][D6]);
      beam->put_W1(gs_FUBeamDimensions[i][W1]);
      beam->put_W2(gs_FUBeamDimensions[i][W2]);
      beam->put_W3(gs_FUBeamDimensions[i][W3]);
      beam->put_W4(gs_FUBeamDimensions[i][W4]);
      beam->put_W5(gs_FUBeamDimensions[i][W5]);
      beam->put_W6(gs_FUBeamDimensions[i][W6]);
      beam->put_W7(gs_FUBeamDimensions[i][W7]);


      beam->QueryInterface(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
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
      return AM_J1 | AM_J2;
   }
   else if ((int)FloridaBeamType::FUB48 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      return AM_J1;
   }

   return AM_NONE;
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
      Float64 t = 0.5*(d1 + (d1 + d2));
      Float64 t_top = t;
      Float64 J = b*t*t*t; // top flange

      b = w3;
      t = 0.5*(d4 + (d4 + d5));
      Float64 t_bot = t;
      J += b*t*t*t; // bottom flange

      J += (h - t_top - t_bot)*tw*tw*tw; // web, full depth

      J *= 1. / 3.;

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
