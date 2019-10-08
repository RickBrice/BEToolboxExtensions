///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2019  Washington State Department of Transportation
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

   Float64 b = 2*(w1 + w2) + 0.5*(t1+t2);
   Float64 t = 0.5*(d1 + (d1 + d2));
   Float64 t_top = t;
   Float64 J = b*t*t*t; // top flange

   b = 2*(w3 + w4) + 0.5*(t1+t2);
   t = 0.5*(d4 + (d4 + d5));
   Float64 t_bot = t;
   J += b*t*t*t; // bottom flange

   b = d1 + d2 + d3 + d4 + d5 + d6 + d7 - t_top - t_bot;
   t = 0.5*(t1 + t2);
   J += b*t*t*t; // web

   J *= 1. / 3.;

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

   return J;
}