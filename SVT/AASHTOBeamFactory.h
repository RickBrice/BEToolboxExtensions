#pragma once


#include "AbstractBeamFactory.h"

enum class AASHTOBeamType
{
   TypeI, TypeII, TypeIII, TypeIV, TypeV, TypeVI, BT54, BT63, BT72, nSections
};

class AASHTOBeamFactory
{
public:
   static void CreateBeam(AASHTOBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(AASHTOBeamType type);
   static LPCTSTR GetName(AASHTOBeamType type);
   static int GetApproxMethods(AASHTOBeamType type);
   static Float64 GetJApprox1(AASHTOBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(AASHTOBeamType type);
};

class CAASHTOBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<AASHTOBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<AASHTOBeamType, AASHTOBeamFactory>(beamIdx); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<AASHTOBeamType, AASHTOBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<AASHTOBeamType, AASHTOBeamFactory>(beamIdx, pConvert,ppShape); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<AASHTOBeamType, AASHTOBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<AASHTOBeamType, AASHTOBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<AASHTOBeamType, AASHTOBeamFactory>(beamIdx); }
};
