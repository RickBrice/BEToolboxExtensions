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

enum class PCIUHPCBeamType
{
   PCI_DBT, /*NU_DBT,*/ nSections
};

class PCIUHPCBeamFactory
{
public:
   static void CreateBeam(PCIUHPCBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(PCIUHPCBeamType type);
   static LPCTSTR GetName(PCIUHPCBeamType type);
   static int GetApproxMethods(PCIUHPCBeamType type);
   static Float64 GetJApprox1(PCIUHPCBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(PCIUHPCBeamType type);
   static Float64 GetJApprox3(PCIUHPCBeamType type);
};

class CPCIUHPCBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<PCIUHPCBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<PCIUHPCBeamType, PCIUHPCBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<PCIUHPCBeamType, PCIUHPCBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<PCIUHPCBeamType, PCIUHPCBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<PCIUHPCBeamType, PCIUHPCBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<PCIUHPCBeamType, PCIUHPCBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<PCIUHPCBeamType, PCIUHPCBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<PCIUHPCBeamType, PCIUHPCBeamFactory>(beamIdx); }
};
