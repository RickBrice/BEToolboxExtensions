#pragma once


#include <WBFLGeometry.h>
#include <WBFLUnitServer.h>

#include <GeomModel/GeomModel.h>
#include <Units/Units.h>

#define AM_NONE 0x0000
#define AM_J1 0x0001
#define AM_J2 0x0002
#define AM_J3 0x0004

/*!
Abstract base class for all beam factories. A beam factory create a shape object for a beam as well as indicate which AASHTO approximate methods of computing J are supported.
*/
class CAbstractBeamFactory
{
public:
   /// Returns the number of beams that can be created by the factory
   virtual IndexType GetBeamCount() const = 0;

   /// Returns the name of a beam
   virtual LPCTSTR GetBeamName(IndexType beamIdx/**< [in] beam index*/) const = 0;

   /// Creates a shape object for a beam
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const = 0;
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const = 0;

   /// Returns AM_NONE if approximate methods aren't supported otherwise a computation of AM_J1 | AM_J2
   virtual int GetApproxMethods(IndexType beamIdx /**< [in] beam index*/) const = 0; 

   /// Computes Approx J by AASHTO LRFD C4.6.2.2.1-1
   virtual Float64 GetJApprox1(IndexType beamIdx /**< [in] beam index*/, IUnitConvert* pConvert) const = 0;
   virtual Float64 GetJApprox1(IndexType beamIdx /**< [in] beam index*/) const = 0;

   virtual Float64 GetJApprox3(IndexType beamIdx /**< [in] beam index*/) const = 0;
};

/// Template method that returns the number of beam types based on an enum T
template<typename T>
IndexType _GetBeamCount() { return (IndexType)T::nSections;  }

/// Template method that returns a beam name
template<typename T,class FACTORY>
LPCTSTR _GetBeamName(IndexType beamIdx)
{
   return FACTORY::GetName((T)beamIdx);
}

/// Template method create a beam shape
template<typename T, class FACTORY>
bool _CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape)
{
   FACTORY::CreateBeam((T)beamIdx, pConvert, ppShape);
   return *ppShape != nullptr;
}

template<typename T, class FACTORY>
std::unique_ptr<WBFL::Geometry::Shape> _CreateBeam(IndexType beamIdx)
{
   return FACTORY::CreateBeam((T)beamIdx);
}

/// Template method that returns the types of approximate methods for computing J
template<typename T, class FACTORY>
int _ApproxMethods(IndexType beamIdx)
{
   return FACTORY::GetApproxMethods((T)beamIdx);
}

/// Template method that computes J
template<typename T, class FACTORY>
Float64 _GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert)
{
   return FACTORY::GetJApprox1((T)beamIdx,pConvert);
}
template<typename T, class FACTORY>
Float64 _GetJApprox1(IndexType beamIdx)
{
   return FACTORY::GetJApprox1((T)beamIdx);
}
template<typename T, class FACTORY>
Float64 _GetJApprox3(IndexType beamIdx)
{
   return FACTORY::GetJApprox3((T)beamIdx);
}
