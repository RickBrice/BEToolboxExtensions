#pragma once


#include "AbstractBeamFactory.h"

enum class FloridaBeamType
{
   FIB36, FIB45, FIB54, FIB63, FIB72, FIB78, FIB84, FIB96, FIB102, 
   FUB48, FUB54, FUB63, FUB72,
   nSections
};

class FloridaBeamFactory
{
public:
   static void CreateBeam(FloridaBeamType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(FloridaBeamType type);
   static LPCTSTR GetName(FloridaBeamType type);
   static int GetApproxMethods(FloridaBeamType type);
   static Float64 GetJApprox1(FloridaBeamType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(FloridaBeamType type);
   static Float64 GetJApprox3(FloridaBeamType type);

private:
   static void GenerateFillet(IPolyShape* pShape, Float64 cx, Float64 cy, Float64 r, Float64 startAngle, Float64 delta, long nSpaces);
   static void GenerateFillet(std::unique_ptr<WBFL::Geometry::Polygon>& shape, Float64 cx, Float64 cy, Float64 r, Float64 startAngle, Float64 delta, long nSpaces);
};

class CFloridaBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<FloridaBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<FloridaBeamType, FloridaBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<FloridaBeamType, FloridaBeamFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<FloridaBeamType, FloridaBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx, IUnitConvert* pConvert) const override { return _GetJApprox1<FloridaBeamType, FloridaBeamFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<FloridaBeamType, FloridaBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<FloridaBeamType, FloridaBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox3(IndexType beamIdx) const override { return _GetJApprox3<FloridaBeamType, FloridaBeamFactory>(beamIdx); }
};
