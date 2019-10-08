#pragma once


#include "AbstractBeamFactory.h"

enum class FloridaBeamType
{
   FIB36, FIB45, FIB54, FIB63, FIB72, FIB78, FIB84, FIB96, nSections
};

class FloridaBeamFactory
{
public:
   static void CreateBeam(FloridaBeamType type, IShape** ppShape);
   static LPCTSTR GetName(FloridaBeamType type);
   static Float64 GetJApprox(FloridaBeamType type);

private:
   static void GenerateFillet(IPolyShape* pShape, Float64 cx, Float64 cy, Float64 r, Float64 startAngle, Float64 delta, long nSpaces);
};

class CFloridaBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<FloridaBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<FloridaBeamType, FloridaBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<FloridaBeamType, FloridaBeamFactory>(beamIdx, ppShape); }
   virtual Float64 GetJApprox(IndexType beamIdx) const override { return _GetJApprox<FloridaBeamType, FloridaBeamFactory>(beamIdx); }
};
