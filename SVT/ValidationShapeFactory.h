#pragma once


#include "AbstractBeamFactory.h"

enum class ValidationShapeType
{
   Square, Rectangle101, Rectangle201, Circle, Triangle, nSections
};

class ValidationShapeFactory
{
public:
   static void CreateBeam(ValidationShapeType type, IUnitConvert* pConvert, IShape** ppShape);
   static std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(ValidationShapeType type);
   static LPCTSTR GetName(ValidationShapeType type);
   static int GetApproxMethods(ValidationShapeType type);
   static Float64 GetJApprox1(ValidationShapeType type, IUnitConvert* pConvert);
   static Float64 GetJApprox1(ValidationShapeType type);
};

class CValidationShapeFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<ValidationShapeType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IUnitConvert* pConvert, IShape** ppShape) const override { return _CreateBeam<ValidationShapeType, ValidationShapeFactory>(beamIdx, pConvert, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx,IUnitConvert* pConvert) const override { return _GetJApprox1<ValidationShapeType, ValidationShapeFactory>(beamIdx,pConvert); }
   virtual std::unique_ptr<WBFL::Geometry::Shape> CreateBeam(IndexType beamIdx) const override { return _CreateBeam<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
};
