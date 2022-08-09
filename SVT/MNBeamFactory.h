#pragma once


#include "AbstractBeamFactory.h"

enum class MNBeamType
{
   M27, M36, // regular I-Beams
   MH30, MH35, MH40, MN45, MN54, MN63, MW82, MW96, // NU Beams
   nSections
};

class MNBeamFactory
{
public:
   static void CreateBeam(MNBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(MNBeamType type);
   static LPCTSTR GetName(MNBeamType type);
   static int GetApproxMethods(MNBeamType type);
   static Float64 GetJApprox1(MNBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(MNBeamType type);
   static Float64 GetJApprox3(MNBeamType type);
};


class CMNBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<MNBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<MNBeamType, MNBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<MNBeamType, MNBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<MNBeamType, MNBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<MNBeamType, MNBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<MNBeamType, MNBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<MNBeamType, MNBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<MNBeamType, MNBeamFactory>(beamIdx); }
};
