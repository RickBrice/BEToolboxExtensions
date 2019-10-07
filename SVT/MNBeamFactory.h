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
   static void CreateBeam(MNBeamType type, IShape** ppShape);
   static LPCTSTR GetName(MNBeamType type);
   static Float64 GetJApprox(MNBeamType type);
};


class CMNBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<MNBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<MNBeamType, MNBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<MNBeamType, MNBeamFactory>(beamIdx, ppShape); }
   virtual Float64 GetJApprox(IndexType beamIdx) const override { return _GetJApprox<MNBeamType, MNBeamFactory>(beamIdx); }
};
