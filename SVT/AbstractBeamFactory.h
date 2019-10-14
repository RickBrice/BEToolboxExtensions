#pragma once

#include <WBFLGeometry.h>

#define AM_NONE 0x0000
#define AM_J1 0x0001
#define AM_J2 0x0002

class CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const = 0;
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const = 0;
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const = 0;
   virtual int GetApproxMethods(IndexType beamIdx) const = 0; // returns AM_NONE if approximate methods aren't supported otherwise a computation of AM_J1 | AM_J2
   virtual Float64 GetJApprox1(IndexType beamIdx) const = 0; // computes Approx J by AASHTO LRFD C4.6.2.2.1-1
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
int _ApproxMethods(IndexType beamIdx)
{
   return FACTORY::GetApproxMethods((T)beamIdx);
}

template<typename T, class FACTORY>
Float64 _GetJApprox1(IndexType beamIdx)
{
   return FACTORY::GetJApprox1((T)beamIdx);
}