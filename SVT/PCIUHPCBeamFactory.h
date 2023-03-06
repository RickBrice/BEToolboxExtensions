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
