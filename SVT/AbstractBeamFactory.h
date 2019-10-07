#pragma once

#include <WBFLGeometry.h>

class CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const = 0;
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const = 0;
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const = 0;
   virtual Float64 GetJApprox(IndexType beamIdx) const = 0; // computes Approx J by AASHTO LRFD C4.6.2.2.1-1
};

template<typename T>
IndexType _GetBeamCount() { return (IndexType)T::nSections;  }

template<typename T,class FACTORY>
LPCTSTR _GetBeamName(IndexType beamIdx)
{
   return FACTORY::GetName((T)beamIdx);
}

template<typename T, class FACTORY>
bool _CreateBeam(IndexType beamIdx, IShape** ppShape)
{
   FACTORY::CreateBeam((T)beamIdx, ppShape);
   return *ppShape != nullptr;
}

template<typename T, class FACTORY>
Float64 _GetJApprox(IndexType beamIdx)
{
   return FACTORY::GetJApprox((T)beamIdx);
}