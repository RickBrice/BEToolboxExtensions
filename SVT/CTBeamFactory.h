#pragma once


#include "AbstractBeamFactory.h"

enum class CTBeamType
{
   BT49, BT55, BT61, BT67, BT73, BT79, BT85,
   WF48, WF54, WF60, WF66, WF72, WF78, WF84, WF90, WF96, WF102, WF108, WF114, WF120,
   I36, I42, I48, I54, I60, I66,
   //Tub55, Tub61, Tub67, Tub73, Tub79, Tub85
   nSections
};

class CTBeamFactory
{
public:
   static void CreateBeam(CTBeamType type, IShape** ppShape);
   static LPCTSTR GetName(CTBeamType type);
   static Float64 GetJApprox(CTBeamType type);
};


class CCTBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<CTBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<CTBeamType, CTBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<CTBeamType, CTBeamFactory>(beamIdx, ppShape); }
   virtual Float64 GetJApprox(IndexType beamIdx) const override { return _GetJApprox<CTBeamType, CTBeamFactory>(beamIdx); }
};
