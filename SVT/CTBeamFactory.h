///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2026  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#pragma once


#include "AbstractBeamFactory.h"

enum class CTBeamType
{
   BT49, BT55, BT61, BT67, BT73, BT79, BT85,
   WF48, WF54, WF60, WF66, WF72, WF78, WF84, WF90, WF96, WF102, WF108, WF114, WF120,
   I36, I42, I48, I54, I60, I66,
   Tub55, Tub61, Tub67, Tub73, Tub79, Tub85,
   nSections
};

class CTBeamFactory
{
public:
   static void CreateBeam(CTBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(CTBeamType type);
   static LPCTSTR GetName(CTBeamType type);
   static int GetApproxMethods(CTBeamType type);
   static Float64 GetJApprox1(CTBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(CTBeamType type);
   static Float64 GetJApprox3(CTBeamType type);
};


class CCTBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<CTBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<CTBeamType, CTBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<CTBeamType, CTBeamFactory>(beamIdx,pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<CTBeamType, CTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<CTBeamType, CTBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<CTBeamType, CTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<CTBeamType, CTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<CTBeamType, CTBeamFactory>(beamIdx); }
};
