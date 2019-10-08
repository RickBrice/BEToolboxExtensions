#include "stdafx.h"

#include "FloridaBeamFactory.h"
#include "Helpers.h"

#include <MathEx.h>

static Float64 gs_FIBBeamDimensions[][12] = {
   // D1     D2     D3   D4     D5     H    T     R    W1     W2    W3      C1
   { 3.5,   1.5,   3.5,   7,   7.5,   36,   7,   15,   17,   3.5,   38,   0.75}, // FIB 36
   { 3.5,   1.5,   3.5,   7,   7.5,   45,   7,   15,   17,   3.5,   38,   0.75}, // FIB 45
   { 3.5,   1.5,   3.5,   7,   7.5,   54,   7,   15,   17,   3.5,   38,   0.75}, // FIB 54
   { 3.5,   1.5,   3.5,   7,   7.5,   63,   7,   15,   17,   3.5,   38,   0.75}, // FIB 63
   { 3.5,   1.5,   3.5,   7,   7.5,   72,   7,   15,   17,   3.5,   38,   0.75}, // FIB 72
   { 3.5,   1.5,   3.5,   7,   7.5,   78,   7,   15,   17,   3.5,   38,   0.75}, // FIB 78
   { 3.5,   1.5,   3.5,   7,   7.5,   84,   7,   15,   17,   3.5,   38,   0.75}, // FIB 84
   { 3.5,   1.5,   3.5,   7,   7.5,   96,   7,   15,   17,   3.5,   38,   0.75}, // FIB 93
};


void FloridaBeamFactory::CreateBeam(FloridaBeamType type, IShape** ppShape)
{
   if ((int)FloridaBeamType::FIB36 <= (int)type && (int)type < (int)FloridaBeamType::nSections)
   {
      int i = (int)type - (int)FloridaBeamType::FIB36;

      using namespace _FIBBeam;

      Float64 d1 = gs_FIBBeamDimensions[i][D1];
      Float64 d2 = gs_FIBBeamDimensions[i][D2];
      Float64 d3 = gs_FIBBeamDimensions[i][D3];
      Float64 d4 = gs_FIBBeamDimensions[i][D4];
      Float64 d5 = gs_FIBBeamDimensions[i][D5];
      Float64 h  = gs_FIBBeamDimensions[i][H];
      Float64 t  = gs_FIBBeamDimensions[i][T];
      Float64 r  = gs_FIBBeamDimensions[i][R];
      Float64 w1 = gs_FIBBeamDimensions[i][W1];
      Float64 w2 = gs_FIBBeamDimensions[i][W2];
      Float64 w3 = gs_FIBBeamDimensions[i][W3];
      Float64 c1 = gs_FIBBeamDimensions[i][C1];

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
   _T("FIB 36"),
   _T("FIB 45"),
   _T("FIB 54"),
   _T("FIB 63"),
   _T("FIB 72"),
   _T("FIB 78"),
   _T("FIB 84"),
   _T("FIB 96"),
};

LPCTSTR FloridaBeamFactory::GetName(FloridaBeamType type)
{
   return gs_FloridaNames[(int)type].c_str();
}

Float64 FloridaBeamFactory::GetJApprox(FloridaBeamType type)
{
   int i = (int)type - (int)FloridaBeamType::FIB36;

   using namespace _FIBBeam;

   Float64 d1 = gs_FIBBeamDimensions[i][D1];
   Float64 d2 = gs_FIBBeamDimensions[i][D2];
   Float64 d3 = gs_FIBBeamDimensions[i][D3];
   Float64 d4 = gs_FIBBeamDimensions[i][D4];
   Float64 d5 = gs_FIBBeamDimensions[i][D5];
   Float64 h = gs_FIBBeamDimensions[i][H];
   Float64 tw = gs_FIBBeamDimensions[i][T];
   Float64 r = gs_FIBBeamDimensions[i][R];
   Float64 w1 = gs_FIBBeamDimensions[i][W1];
   Float64 w2 = gs_FIBBeamDimensions[i][W2];
   Float64 w3 = gs_FIBBeamDimensions[i][W3];
   Float64 c1 = gs_FIBBeamDimensions[i][C1];

   Float64 b = 2*(w1 + w2) + tw;
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
