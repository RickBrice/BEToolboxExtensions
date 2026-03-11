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

enum class TxDOTBeamType
{
   Tx28, Tx34, Tx40, Tx46, Tx54, Tx62, Tx70, 
   U40, U54,
   nSections
};

class TxDOTBeamFactory
{
public:
   static void CreateBeam(TxDOTBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(TxDOTBeamType type);
   static LPCTSTR GetName(TxDOTBeamType type);
   static int GetApproxMethods(TxDOTBeamType type);
   static Float64 GetJApprox1(TxDOTBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(TxDOTBeamType type);
   static Float64 GetJApprox3(TxDOTBeamType type);
};

class CTxDOTBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<TxDOTBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<TxDOTBeamType, TxDOTBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<TxDOTBeamType, TxDOTBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<TxDOTBeamType, TxDOTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<TxDOTBeamType, TxDOTBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<TxDOTBeamType, TxDOTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<TxDOTBeamType, TxDOTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<TxDOTBeamType, TxDOTBeamFactory>(beamIdx); }
};
