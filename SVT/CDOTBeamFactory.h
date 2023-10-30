#pragma once


#include "AbstractBeamFactory.h"

enum class CDOTBeamType
{
   CBT_30, CBT_36, CBT_45, CBT_54, CBT_63, CBT_72, CBT_81, CBT_90, nSections
};

class CDOTBeamFactory
{
public:
   static void CreateBeam(CDOTBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(CDOTBeamType type);
   static LPCTSTR GetName(CDOTBeamType type);
   static int GetApproxMethods(CDOTBeamType type);
   static Float64 GetJApprox1(CDOTBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(CDOTBeamType type);
   static Float64 GetJApprox3(CDOTBeamType type);
};

class CCDOTBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<CDOTBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<CDOTBeamType, CDOTBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<CDOTBeamType, CDOTBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<CDOTBeamType, CDOTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<CDOTBeamType, CDOTBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<CDOTBeamType, CDOTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<CDOTBeamType, CDOTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<CDOTBeamType, CDOTBeamFactory>(beamIdx); }
};
