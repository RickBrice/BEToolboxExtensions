#pragma once


#include "AbstractBeamFactory.h"

enum class OregonBeamType
{
   BI51, BI63, BI75, BT48, BT60, BT72, BT84, BT90, BT96, nSections
};

class OregonBeamFactory
{
public:
   static void CreateBeam(OregonBeamType type, IShape** ppShape);
   static LPCTSTR GetName(OregonBeamType type);
   static Float64 GetJApprox(OregonBeamType type);
};

class COregonBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<OregonBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<OregonBeamType, OregonBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<OregonBeamType, OregonBeamFactory>(beamIdx, ppShape); }
   virtual Float64 GetJApprox(IndexType beamIdx) const override { return _GetJApprox<OregonBeamType, OregonBeamFactory>(beamIdx); }
};
