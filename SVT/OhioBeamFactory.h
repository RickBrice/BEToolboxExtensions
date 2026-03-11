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

enum class OhioBeamType
{
   WF36_49, WF42_49, WF48_49, WF54_49, WF60_49, WF66_49, WF72_49, TypeIV_60, TypeIV_66, TypeIV_72, nSections
};

class OhioBeamFactory
{
public:
   static void CreateBeam(OhioBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(OhioBeamType type);
   static LPCTSTR GetName(OhioBeamType type);
   static int GetApproxMethods(OhioBeamType type);
   static Float64 GetJApprox1(OhioBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(OhioBeamType type);
   static Float64 GetJApprox3(OhioBeamType type);
};

class COhioBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<OhioBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<OhioBeamType, OhioBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<OhioBeamType, OhioBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<OhioBeamType, OhioBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<OhioBeamType, OhioBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<OhioBeamType, OhioBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<OhioBeamType, OhioBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<OhioBeamType, OhioBeamFactory>(beamIdx); }
};
