#pragma once


#include "AbstractBeamFactory.h"

enum class OhioBeamType
{
   WF36_49, WF42_49, WF48_49, WF54_49, WF60_49, WF66_49, WF72_49, TypeIV_60, TypeIV_66, TypeIV_72, nSections
};

class OhioBeamFactory
{
public:
   static void CreateBeam(OhioBeamType type, IShape** ppShape);
   static LPCTSTR GetName(OhioBeamType type);
   static int GetApproxMethods(OhioBeamType type);
   static Float64 GetJApprox1(OhioBeamType type);
};

class COhioBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<OhioBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<OhioBeamType, OhioBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<OhioBeamType, OhioBeamFactory>(beamIdx, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<OhioBeamType, OhioBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<OhioBeamType, OhioBeamFactory>(beamIdx); }
};
