#pragma once


#include "AbstractBeamFactory.h"

enum class NEBeamType
{
   NEBT39, NEBT47, NEBT55, NEBT63, NEBT71, NEBT79, NEBT83, NEBT87, nSections
};

class NEBeamFactory
{
public:
   static void CreateBeam(NEBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(NEBeamType type);
   static LPCTSTR GetName(NEBeamType type);
   static int GetApproxMethods(NEBeamType type);
   static Float64 GetJApprox1(NEBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(NEBeamType type);
};

class CNEBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<NEBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<NEBeamType, NEBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<NEBeamType, NEBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<NEBeamType, NEBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<NEBeamType, NEBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<NEBeamType, NEBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<NEBeamType, NEBeamFactory>(beamIdx); }
};
