#pragma once


#include "AbstractBeamFactory.h"

enum class NUBeamType
{
   NU900, NU1100, NU1350, NU1600, NU1800, NU2000, nSections
};

class NUBeamFactory
{
public:
   static void CreateBeam(NUBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(NUBeamType type);
   static LPCTSTR GetName(NUBeamType type);
   static int GetApproxMethods(NUBeamType type);
   static Float64 GetJApprox1(NUBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(NUBeamType type);
   static Float64 GetJApprox3(NUBeamType type);
};

class CNUBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<NUBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<NUBeamType, NUBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<NUBeamType, NUBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<NUBeamType, NUBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<NUBeamType, NUBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<NUBeamType, NUBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<NUBeamType, NUBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<NUBeamType, NUBeamFactory>(beamIdx); }
};
