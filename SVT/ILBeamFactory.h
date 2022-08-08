#pragma once


#include "AbstractBeamFactory.h"

enum class ILBeamType
{
   IL27_1830,
   IL36_2438,
   IL36_3838,
   IL45_2438,
   IL45_3838,
   IL54_2438,
   IL54_3838,
   IL63_2438,
   IL63_3838,
   IL72_2438,
   IL72_3838,
   nSections
};

class ILBeamFactory
{
public:
   static void CreateBeam(ILBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(ILBeamType type);
   static LPCTSTR GetName(ILBeamType type);
   static int GetApproxMethods(ILBeamType type);
   static Float64 GetJApprox1(ILBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(ILBeamType type);

private:
   static void GetNUDimensions(int i, IUnitConvert* pConvert, Float64& d1, Float64& d2, Float64& d3, Float64& d4, Float64& d5, Float64& r1, Float64& r2, Float64& r3, Float64& r4, Float64& t, Float64& w1, Float64& w2, Float64& c1);
   static void GetNUDimensions(int i, Float64& d1, Float64& d2, Float64& d3, Float64& d4, Float64& d5, Float64& r1, Float64& r2, Float64& r3, Float64& r4, Float64& t, Float64& w1, Float64& w2, Float64& c1);
};

class CILBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<ILBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<ILBeamType, ILBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<ILBeamType, ILBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<ILBeamType, ILBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<ILBeamType, ILBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<ILBeamType, ILBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<ILBeamType, ILBeamFactory>(beamIdx); }
};
