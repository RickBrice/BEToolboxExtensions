#pragma once


#include "AbstractBeamFactory.h"

enum class NCBeamType
{
   BT63, BT72, nSections
};

class NCBeamFactory
{
public:
   static void CreateBeam(NCBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static LPCTSTR GetName(NCBeamType type);
   static int GetApproxMethods(NCBeamType type);
   static Float64 GetJApprox1(NCBeamType type, IUnitConvert* pConvert);
};

class CNCBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<NCBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<NCBeamType, NCBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<NCBeamType, NCBeamFactory>(beamIdx, pConvert,ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<NCBeamType, NCBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<NCBeamType, NCBeamFactory>(beamIdx,pConvert); }
};
