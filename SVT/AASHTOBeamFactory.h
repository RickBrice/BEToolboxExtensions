#pragma once


#include "AbstractBeamFactory.h"

enum class AASHTOBeamType
{
   TypeI, TypeII, TypeIII, TypeIV, TypeV, TypeVI, nSections
};

class AASHTOBeamFactory
{
public:
   static void CreateBeam(AASHTOBeamType type, IShape** ppShape);
   static LPCTSTR GetName(AASHTOBeamType type);
   static Float64 GetJApprox(AASHTOBeamType type);
};

class CAASHTOBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<AASHTOBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<AASHTOBeamType, AASHTOBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<AASHTOBeamType, AASHTOBeamFactory>(beamIdx, ppShape); }
   virtual Float64 GetJApprox(IndexType beamIdx) const override { return _GetJApprox<AASHTOBeamType, AASHTOBeamFactory>(beamIdx); }
};
