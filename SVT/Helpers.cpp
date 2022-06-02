///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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
#include <thread>
#include <MathEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static IndexType gs_hardware_threads = std::thread::hardware_concurrency();

void GetThreadParameters(IndexType nItems, IndexType& nWorkerThreads, IndexType& nItemsPerThread)
{
   IndexType min_per_thread = 10000;
   IndexType max_threads = (nItems + min_per_thread - 1) / min_per_thread;
   IndexType nThreads = min(gs_hardware_threads != 0 ? gs_hardware_threads : 2, max_threads);
   nWorkerThreads = nThreads - 1;
   nItemsPerThread = nItems / nThreads;

   // this makes everything run in the main thread
   //nWorkerThreads = 0;
   //nItemsPerThread = nItems;
}

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

   pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in4")), &J);

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

   pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in4")), &J);

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

   pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in4")), &J);

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

   pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in4")), &J);

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

   pConvert->ConvertToBaseUnits(J, CComBSTR(_T("in^4")), &J);

   return J;
}
