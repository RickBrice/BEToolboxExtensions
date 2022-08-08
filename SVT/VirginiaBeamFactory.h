#pragma once


#include "AbstractBeamFactory.h"

enum class VirginiaBeamType
{
   PCBT_29, PCBT_37, PCBT_45, PCBT_53, PCBT_61, PCBT_69, PCBT_77, PCBT_85, PCBT_93, nSections
};

class VirginiaBeamFactory
{
public:
   static void CreateBeam(VirginiaBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(VirginiaBeamType type);
   static LPCTSTR GetName(VirginiaBeamType type);
   static int GetApproxMethods(VirginiaBeamType type);
   static Float64 GetJApprox1(VirginiaBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(VirginiaBeamType type);
};

class CVirginiaBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<VirginiaBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<VirginiaBeamType, VirginiaBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<VirginiaBeamType, VirginiaBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<VirginiaBeamType, VirginiaBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<VirginiaBeamType, VirginiaBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<VirginiaBeamType, VirginiaBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<VirginiaBeamType, VirginiaBeamFactory>(beamIdx); }
};
