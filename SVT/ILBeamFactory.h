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
   static void CreateBeam(ILBeamType type, IShape** ppShape);
   static LPCTSTR GetName(ILBeamType type);
};

class CILBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<ILBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<ILBeamType, ILBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<ILBeamType, ILBeamFactory>(beamIdx, ppShape); }
};
