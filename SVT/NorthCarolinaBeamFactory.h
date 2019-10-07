#pragma once


#include "AbstractBeamFactory.h"

enum class NCBeamType
{
   BT63, BT72, nSections
};

class NCBeamFactory
{
public:
   static void CreateBeam(NCBeamType type, IShape** ppShape);
   static LPCTSTR GetName(NCBeamType type);
   static Float64 GetJApprox(NCBeamType type);
};

class CNCBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<NCBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<NCBeamType, NCBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<NCBeamType, NCBeamFactory>(beamIdx, ppShape); }
   virtual Float64 GetJApprox(IndexType beamIdx) const override { return _GetJApprox<NCBeamType, NCBeamFactory>(beamIdx); }
};
