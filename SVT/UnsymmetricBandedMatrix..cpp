///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2021  Washington State Department of Transportation
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
#include "UnsymmetricBandedMatrix.h"
#include "Helpers.h"
#include <vector>
#include <future>

#if defined _DEBUG
#include <MathEx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mathUnsymmetricBandedMatrix::mathUnsymmetricBandedMatrix()
{
   N = 0;
   BW = 0;
   half_band_width = 0;
   ba = nullptr;
   b = nullptr;
}

mathUnsymmetricBandedMatrix::mathUnsymmetricBandedMatrix(IndexType N, IndexType BW) :
   N(N), BW(BW)
{
   ba = nullptr;
   b = nullptr;
   Initialize(N, BW);
}

mathUnsymmetricBandedMatrix::mathUnsymmetricBandedMatrix(IndexType N, IndexType BW, Float64** ba, Float64* b) :
   N(N), BW(BW), ba(ba), b(b)
{
   half_band_width = BW / 2;
}

mathUnsymmetricBandedMatrix::~mathUnsymmetricBandedMatrix()
{
   Clear();
}

void mathUnsymmetricBandedMatrix::Initialize(IndexType n, IndexType bw)
{
   Clear();
   N = n;
   BW = bw;
   half_band_width = BW / 2;

#if defined ROW_STORAGE
   ba = new Float64*[BW];
   for (IndexType i = 0; i < BW; i++)
   {
      ba[i] = new Float64[N];
      memset((void*)ba[i], 0, N * sizeof(Float64));
   }
#else
   ba = new Float64*[N];
   for (IndexType i = 0; i < N; i++)
   {
      ba[i] = new Float64[BW];
      memset((void*)ba[i], 0, BW * sizeof(Float64));
   }
#endif

   b = new Float64[N];
   memset((void*)b, 0, N * sizeof(Float64));
}

IndexType mathUnsymmetricBandedMatrix::GetSize() const
{
   return N;
}

IndexType mathUnsymmetricBandedMatrix::GetBandwidth() const
{
   return BW;
}

void mathUnsymmetricBandedMatrix::SetCoefficient(IndexType i, IndexType j, Float64 aij)
{
   IndexType m, n;
   Full2Condensed(i, j, half_band_width, m, n);
   ba[m][n] = aij;
}

Float64 mathUnsymmetricBandedMatrix::GetCoefficient(IndexType i, IndexType j)
{
   IndexType m, n;
   Full2Condensed(i, j, half_band_width, m, n);
   return ba[m][n];
}

void mathUnsymmetricBandedMatrix::SetB(IndexType i, Float64 bi)
{
   b[i] = bi;
}

Float64 mathUnsymmetricBandedMatrix::GetB(IndexType i) const
{
   return b[i];
}

Float64* mathUnsymmetricBandedMatrix::Solve()
{
   Float64* x = new Float64[N];

   // Gaussian elimination phase
   for (IndexType j = 0; j < N; j++)
   {
      IndexType jmax = min(j + half_band_width, N - 1);
      for (IndexType i = j + 1; i <= jmax; i++)
      {
         IndexType m, n;
         Full2Condensed(i, j, half_band_width, m, n);
         Float64 Aij = ba[m][n];
         Full2Condensed(j, j, half_band_width, m, n);
         Float64 Ajj = ba[m][n];
         ATLASSERT(!IsZero(Ajj));

         Float64 c = Aij / Ajj;
         IndexType kmin = max(j, j < half_band_width ? 0 : j - half_band_width);
         IndexType kmax = min(j + half_band_width, N - 1);

         IndexType nWorkerThreads, nElementsPerThread;
         ::GetThreadParameters(kmax - kmin + 1, nWorkerThreads, nElementsPerThread);
         std::vector<std::future<void>> vFutures;
         IndexType kStart = kmin;
         for (IndexType t = 0; t < nWorkerThreads; t++)
         {
            IndexType kEnd = kStart + nElementsPerThread - 1;
            vFutures.emplace_back(std::async([this, c, i, j, kStart, kEnd] {ReduceRow(c, i, j, kStart, kEnd);}));
            kStart = kEnd + 1;
         }
         ReduceRow(c, i, j, kStart, kmax);
         //for (IndexType k = kmin; k <= kmax; k++)
         //{
         //   IndexType o, p;
         //   Full2Condensed(j, k, half_band_width, o, p);
         //   Float64 c_Ajk = c*ba[o][p];

         //   Full2Condensed(i, k, half_band_width, m, n);
         //   ba[m][n] -= c_Ajk;
         //}
         for (auto& f : vFutures)
         {
            f.get();
         }
         b[i] -= c*b[j];
      }
   }

   // Backsubstitution phase
   IndexType m, n;
   Full2Condensed(N - 1, N - 1, half_band_width, m, n);
   Float64 Ann = ba[m][n];
   x[N - 1] = b[N - 1] / Ann;
   for (IndexType i = N - 2; i >= 0 && i != INVALID_INDEX; i--)
   {
      Float64 sum = 0;
      IndexType kmax = min(i + half_band_width, N - 1);
      for (IndexType j = i + 1; j <= kmax; j++)
      {
         Full2Condensed(i, j, half_band_width, m, n);
         sum += ba[m][n] * x[j];
      }
      Full2Condensed(i, i, half_band_width, m, n);
      Float64 Aii = ba[m][n];
      x[i] = (b[i] - sum) / Aii;
   }

   return x;
}

void mathUnsymmetricBandedMatrix::ReduceRow(Float64 c, IndexType i, IndexType j, IndexType kStart, IndexType kEnd)
{
   for (IndexType k = kStart; k <= kEnd; k++)
   {
      IndexType o, p;
      Full2Condensed(j, k, half_band_width, o, p);
      Float64 c_Ajk = c*ba[o][p];

      IndexType m, n;
      Full2Condensed(i, k, half_band_width, m, n);
      ba[m][n] -= c_Ajk;
   }
}

Float64& mathUnsymmetricBandedMatrix::operator()(IndexType i, IndexType j)
{
   IndexType m, n;
   Full2Condensed(i, j, half_band_width, m, n);
   return ba[m][n];
}

Float64& mathUnsymmetricBandedMatrix::operator[](IndexType i)
{
   return b[i];
}

void mathUnsymmetricBandedMatrix::Clear()
{
   if (b)
   {
      delete[] b;
      b = nullptr;
   }

   if (ba)
   {
#if defined ROW_STORAGE
      for (IndexType i = 0; i < BW; i++)
      {
         delete[] ba[i];
      }
#else
      for (IndexType i = 0; i < N; i++)
      {
         delete[] ba[i];
      }
#endif
      delete[] ba;
      ba = nullptr;
   }
}

void mathUnsymmetricBandedMatrix::Dump(std::ostream& os, bool bFull) const
{
   if (bFull)
      DumpFull(os);
   else
      DumpBanded(os);
}

void mathUnsymmetricBandedMatrix::DumpBanded(std::ostream& os) const
{
   for (IndexType i = 0; i < N; i++)
   {
      os << "[";
      for (IndexType j = 0; j < BW; j++)
      {
#if defined ROW_STORAGE
         std::swap(i, j);
#endif
         os << ba[i][j] << " ";
      }
      os << "]" << std::endl;
   }
}

void mathUnsymmetricBandedMatrix::DumpFull(std::ostream& os) const
{
   for (IndexType i = 0; i < N; i++)
   {
      os << "[";
      for (IndexType j = 0; j < N; j++)
      {
         IndexType m, n;
         Full2Condensed(i, j, half_band_width, m, n);
         Float64 value = (BW < n ? 0 : ba[m][n]);
         os << value << " ";
      }
      os << "]" << std::endl;
   }
}
