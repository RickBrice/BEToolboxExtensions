///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2025  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Helpers.h"
#include <MathEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

std::tuple<Float64, Float64, Float64> GetColor(Float64 min, Float64 max, Float64 value)
{
   Float64 ratio = 2 * (value - min) / (max - min);
   Float64 b = Max(0., (1 - ratio));
   Float64 r = Max(0., (ratio - 1));
   Float64 g = 1 - b - r;
   return std::tuple<Float64, Float64, Float64>(r, b, g);
}

Float64 GetJApprox2(IShapeProperties* pProps)
{
   Float64 A, Ix, Iy;
   pProps->get_Area(&A);
   pProps->get_Ixx(&Ix);
   pProps->get_Iyy(&Iy);

   Float64 Ip = Ix + Iy;
   Float64 J = A*A*A*A / (40.0*Ip);
   return J;
}

Float64 GetJApprox2(const WBFL::Geometry::ShapeProperties& props)
{
   Float64 A = props.GetArea();
   Float64 Ix = props.GetIxx();
   Float64 Iy = props.GetIyy();

   Float64 Ip = Ix + Iy;
   Float64 J = A * A * A * A / (40.0 * Ip);
   return J;
}

Float64 ComputeJApprox_IBeam(int i, IUnitConvert* pConvert, const Float64 dimensions[][14])
{
   using namespace IBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 d7 = dimensions[i][D7];
   Float64 t1 = dimensions[i][T1];
   Float64 t2 = dimensions[i][T2];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];

   Float64 b = 2*(w1 + w2) + t1;
   Float64 t = d1 + 0.5*d2;
   Float64 t_top = t;
   Float64 J = b*t*t*t; // top flange

   b = 2*(w3 + w4) + t2;
   t = d4 + 0.5*d5;
   Float64 t_bot = t;
   J += b*t*t*t; // bottom flange

   b = d1 + d2 + d3 + d4 + d5 + d6 + d7 - t_top - t_bot;
   t = 0.5*(t1 + t2);
   J += b*t*t*t; // web

   J *= 1. / 3.;

   VERIFY(SUCCEEDED(pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in^4")), &J)));

   return J;
}

Float64 ComputeJApprox_IBeam2(int i, IUnitConvert* pConvert, const Float64 dimensions[][15])
{
   using namespace IBeam2;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 h = dimensions[i][H];
   Float64 t1 = dimensions[i][T1];
   Float64 t2 = dimensions[i][T2];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];
   Float64 w5 = dimensions[i][W5];

   Float64 b = 2 * (w1 + w2 + w3) + t1;
   Float64 t = d1 + 0.5*d2;
   Float64 t_top = t;
   Float64 J = b*t*t*t; // top flange

   b = 2 * (w4 + w5) + t2;
   t = d6 + 0.5*d5;
   Float64 t_bot = t;
   J += b*t*t*t; // bottom flange

   b = h - t_top - t_bot;
   t = 0.5*(t1 + t2);
   J += b*t*t*t; // web

   J *= 1. / 3.;

   VERIFY(SUCCEEDED(pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in^4")), &J)));

   return J;
}

Float64 ComputeJApprox_NU(int i, IUnitConvert* pConvert, const Float64 dimensions[][13])
{
   using namespace _NUBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 tw = dimensions[i][T];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];

   Float64 b = w1;
   Float64 t = 0.5*(d1 + (d1 + d2));
   Float64 t_top = t;
   Float64 J = b*t*t*t; // top flange

   b = w2;
   t = 0.5*(d4 + (d4 + d5));
   Float64 t_bot = t;
   J += b*t*t*t; // bottom flange

   b = d1 + d2 + d3 + d4 + d5 - t_top - t_bot;
   J += b*tw*tw*tw; // web, full depth

   J *= 1. / 3.;

   VERIFY(SUCCEEDED(pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in^4")), &J)));

   return J;
}

Float64 ComputeJApprox_UBeam(int i, IUnitConvert* pConvert, const Float64 dimensions[][13])
{
   using namespace _UBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 d7 = dimensions[i][D7];
   Float64 t  = dimensions[i][T];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];
   Float64 w5 = dimensions[i][W5];

   Float64 rise = d1 - d4 - d5;
   Float64 run = (w2 - w1) / 2 - w5;
   Float64 slope = (IsZero(run) ? DBL_MAX : rise / run);

   Float64 Jweb = (d1 - d2)*sqrt(slope*slope + 1)/slope*t*t*t;
   Float64 Jflg = w1*d2*d2*d2;

   Float64 J = (2 * Jweb + Jflg)/3.;

   VERIFY(SUCCEEDED(pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in^4")), &J)));

   return J;
}

Float64 ComputeJApprox_UBeam2(int i, IUnitConvert* pConvert, const Float64 dimensions[][14])
{
   CComPtr<IUBeam2> beam;
   beam.CoCreateInstance(CLSID_UBeam2);
   using namespace _UBeam2; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

   beam->put_C1(dimensions[i][C1]);
   beam->put_D1(dimensions[i][D1]);
   beam->put_D2(dimensions[i][D2]);
   beam->put_D3(dimensions[i][D3]);
   beam->put_D4(dimensions[i][D4]);
   beam->put_D5(dimensions[i][D5]);
   beam->put_D6(dimensions[i][D6]);
   beam->put_W1(dimensions[i][W1]);
   beam->put_W2(dimensions[i][W2]);
   beam->put_W3(dimensions[i][W3]);
   beam->put_W4(dimensions[i][W4]);
   beam->put_W5(dimensions[i][W5]);
   beam->put_W6(dimensions[i][W6]);
   beam->put_W7(dimensions[i][W7]);

   Float64 slope;
   beam->get_Slope(0,&slope);

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 w1 = dimensions[i][W1];

   Float64 t;
   beam->get_T(&t);

   Float64 Jweb = (d1 - d2)*sqrt(slope*slope+1)/slope*t*t*t;
   Float64 Jflg = w1*d2*d2*d2;

   Float64 J = (2 * Jweb + Jflg) / 3.;

   VERIFY(SUCCEEDED(pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in^4")), &J)));

   return J;
}


//////////////
Float64 ComputeJApprox_IBeam(int i, const Float64 dimensions[][14])
{
   using namespace IBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 d7 = dimensions[i][D7];
   Float64 t1 = dimensions[i][T1];
   Float64 t2 = dimensions[i][T2];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];

   Float64 b = 2 * (w1 + w2) + t1;
   Float64 t = d1 + 0.5 * d2;
   Float64 t_top = t;
   Float64 J = b * t * t * t; // top flange

   b = 2 * (w3 + w4) + t2;
   t = d4 + 0.5 * d5;
   Float64 t_bot = t;
   J += b * t * t * t; // bottom flange

   b = d1 + d2 + d3 + d4 + d5 + d6 + d7 - t_top - t_bot;
   t = 0.5 * (t1 + t2);
   J += b * t * t * t; // web

   J *= 1. / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox_IBeam2(int i, const Float64 dimensions[][15])
{
   using namespace IBeam2;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 h = dimensions[i][H];
   Float64 t1 = dimensions[i][T1];
   Float64 t2 = dimensions[i][T2];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];
   Float64 w5 = dimensions[i][W5];

   Float64 b = 2 * (w1 + w2 + w3) + t1;
   Float64 t = d1 + 0.5 * d2;
   Float64 t_top = t;
   Float64 J = b * t * t * t; // top flange

   b = 2 * (w4 + w5) + t2;
   t = d6 + 0.5 * d5;
   Float64 t_bot = t;
   J += b * t * t * t; // bottom flange

   b = h - t_top - t_bot;
   t = 0.5 * (t1 + t2);
   J += b * t * t * t; // web

   J *= 1. / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox_NU(int i, const Float64 dimensions[][13])
{
   using namespace _NUBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 tw = dimensions[i][T];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];

   Float64 b = w1;
   Float64 t = 0.5 * (d1 + (d1 + d2));
   Float64 t_top = t;
   Float64 J = b * t * t * t; // top flange

   b = w2;
   t = 0.5 * (d4 + (d4 + d5));
   Float64 t_bot = t;
   J += b * t * t * t; // bottom flange

   b = d1 + d2 + d3 + d4 + d5 - t_top - t_bot;
   J += b * tw * tw * tw; // web, full depth

   J *= 1. / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox_UBeam(int i, const Float64 dimensions[][13])
{
   using namespace _UBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 d7 = dimensions[i][D7];
   Float64 t = dimensions[i][T];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];
   Float64 w5 = dimensions[i][W5];

   Float64 rise = d1 - d4 - d5;
   Float64 run = (w2 - w1) / 2 - w5;
   Float64 slope = (IsZero(run) ? DBL_MAX : rise / run);

   Float64 Jweb = (d1 - d2) * sqrt(slope * slope + 1) / slope * t * t * t;
   Float64 Jflg = w1 * d2 * d2 * d2;

   Float64 J = (2 * Jweb + Jflg) / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox_UBeam2(int i, const Float64 dimensions[][14])
{
   WBFL::Geometry::UBeam2 beam;

   using namespace _UBeam2; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

   beam.SetC1(dimensions[i][C1]);
   beam.SetD1(dimensions[i][D1]);
   beam.SetD2(dimensions[i][D2]);
   beam.SetD3(dimensions[i][D3]);
   beam.SetD4(dimensions[i][D4]);
   beam.SetD5(dimensions[i][D5]);
   beam.SetD6(dimensions[i][D6]);
   beam.SetW1(dimensions[i][W1]);
   beam.SetW2(dimensions[i][W2]);
   beam.SetW3(dimensions[i][W3]);
   beam.SetW4(dimensions[i][W4]);
   beam.SetW5(dimensions[i][W5]);
   beam.SetW6(dimensions[i][W6]);
   beam.SetW7(dimensions[i][W7]);

   Float64 slope = fabs(beam.GetSlope(0));

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 w1 = dimensions[i][W1];

   Float64 t = beam.GetT();

   Float64 Jweb = (d1 - d2) * sqrt(slope * slope + 1) / slope * t * t * t;
   Float64 Jflg = w1 * d2 * d2 * d2;

   Float64 J = (2 * Jweb + Jflg) / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}


//////////////
// This is based on Eq 5 from Eby
Float64 ComputeJApprox3_IBeam(int i, const Float64 dimensions[][14])
{
   using namespace IBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 d7 = dimensions[i][D7];
   Float64 t1 = dimensions[i][T1];
   Float64 t2 = dimensions[i][T2];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];

   Float64 b = 2 * (w1 + w2) + t1;
   Float64 t = d1 + 0.5 * d2;
   Float64 t_top = t;
   Float64 J = b * t * t * t*(1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // top flange

   b = 2 * (w3 + w4) + t2;
   t = d4 + 0.5 * d5;
   Float64 t_bot = t;
   J += b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // bottom flange

   b = d1 + d2 + d3 + d4 + d5 + d6 + d7 - t_top - t_bot;
   t = 0.5 * (t1 + t2);
   J += b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // web

   J *= 1. / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox3_IBeam2(int i, const Float64 dimensions[][15])
{
   using namespace IBeam2;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 h = dimensions[i][H];
   Float64 t1 = dimensions[i][T1];
   Float64 t2 = dimensions[i][T2];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];
   Float64 w5 = dimensions[i][W5];

   Float64 b = 2 * (w1 + w2 + w3) + t1;
   Float64 t = d1 + 0.5 * d2;
   Float64 t_top = t;
   Float64 J = b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // top flange

   b = 2 * (w4 + w5) + t2;
   t = d6 + 0.5 * d5;
   Float64 t_bot = t;
   J += b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // bottom flange

   b = h - t_top - t_bot;
   t = 0.5 * (t1 + t2);
   J += b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // web

   J *= 1. / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox3_NU(int i, const Float64 dimensions[][13])
{
   using namespace _NUBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 tw = dimensions[i][T];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];

   Float64 b = w1;
   Float64 t = 0.5 * (d1 + (d1 + d2));
   Float64 t_top = t;
   Float64 J = b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // top flange

   b = w2;
   t = 0.5 * (d4 + (d4 + d5));
   Float64 t_bot = t;
   J += b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2)); // bottom flange

   b = d1 + d2 + d3 + d4 + d5 - t_top - t_bot;
   J += b * tw * tw * tw * (1.0 - 0.63 * (tw / b) + 0.052 * pow(tw / b, 2)); // web, full depth

   J *= 1. / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox3_UBeam(int i, const Float64 dimensions[][13])
{
   using namespace _UBeam;

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 d3 = dimensions[i][D3];
   Float64 d4 = dimensions[i][D4];
   Float64 d5 = dimensions[i][D5];
   Float64 d6 = dimensions[i][D6];
   Float64 d7 = dimensions[i][D7];
   Float64 t = dimensions[i][T];
   Float64 w1 = dimensions[i][W1];
   Float64 w2 = dimensions[i][W2];
   Float64 w3 = dimensions[i][W3];
   Float64 w4 = dimensions[i][W4];
   Float64 w5 = dimensions[i][W5];

   Float64 rise = d1 - d4 - d5;
   Float64 run = (w2 - w1) / 2 - w5;
   Float64 slope = (IsZero(run) ? DBL_MAX : rise / run);

   Float64 b = (d1 - d2) * sqrt(slope * slope + 1) / slope;
   Float64 Jweb = b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2));
   Float64 Jflg = w1 * d2 * d2 * d2 * (1.0 - 0.63 * (d2 / w1) + 0.052 * pow(d2 / w1, 2));

   Float64 J = (2 * Jweb + Jflg) / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

Float64 ComputeJApprox3_UBeam2(int i, const Float64 dimensions[][14])
{
   WBFL::Geometry::UBeam2 beam;

   using namespace _UBeam2; // this is so we don't have to use the name space below (eg _UBeam::D1, _UBeam::D2...)

   beam.SetC1(dimensions[i][C1]);
   beam.SetD1(dimensions[i][D1]);
   beam.SetD2(dimensions[i][D2]);
   beam.SetD3(dimensions[i][D3]);
   beam.SetD4(dimensions[i][D4]);
   beam.SetD5(dimensions[i][D5]);
   beam.SetD6(dimensions[i][D6]);
   beam.SetW1(dimensions[i][W1]);
   beam.SetW2(dimensions[i][W2]);
   beam.SetW3(dimensions[i][W3]);
   beam.SetW4(dimensions[i][W4]);
   beam.SetW5(dimensions[i][W5]);
   beam.SetW6(dimensions[i][W6]);
   beam.SetW7(dimensions[i][W7]);

   Float64 slope = fabs(beam.GetSlope(0));

   Float64 d1 = dimensions[i][D1];
   Float64 d2 = dimensions[i][D2];
   Float64 w1 = dimensions[i][W1];

   Float64 t = beam.GetT();

   Float64 b = (d1 - d2) * sqrt(slope * slope + 1) / slope;
   Float64 Jweb = b * t * t * t * (1.0 - 0.63 * (t / b) + 0.052 * pow(t / b, 2));
   Float64 Jflg = w1 * d2 * d2 * d2 * (1.0 - 0.63 * (d2 / w1) + 0.052 * pow(d2 / w1, 2));

   Float64 J = (2 * Jweb + Jflg) / 3.;

   J = WBFL::Units::ConvertToSysUnits(J, WBFL::Units::Measure::Inch4);

   return J;
}

void MapPrecastBeamDimensions(std::unique_ptr<WBFL::Geometry::PrecastBeam>& beam, Float64 c1, Float64 d1, Float64 d2, Float64 d3, Float64 d4, Float64 d5, Float64 d6, Float64 d7, Float64 t1, Float64 t2, Float64 w1, Float64 w2, Float64 w3, Float64 w4)
{
   Float64 h = d1 + d2 + d3 + d4 + d5 + d6 + d7;

   beam->SetC1(c1);
   beam->SetD1(d1);
   beam->SetD2(d2);
   beam->SetD3(d3);
   beam->SetD4(d6);
   beam->SetD5(d5);
   beam->SetD6(d4);
   beam->SetHeight(h);
   beam->SetT1(t1);
   beam->SetT2(t2);
   beam->SetW1(0);
   beam->SetW2(w1);
   beam->SetW3(w2);
   beam->SetW4(w4);
   beam->SetW5(w3);
}
