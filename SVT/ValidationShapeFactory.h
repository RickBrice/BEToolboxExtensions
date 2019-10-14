#pragma once


#include "AbstractBeamFactory.h"

enum class ValidationShapeType
{
   Square, Circle, Triangle, nSections
};

class ValidationShapeFactory
{
public:
   static void CreateBeam(ValidationShapeType type, IShape** ppShape);
   static LPCTSTR GetName(ValidationShapeType type);
   static int GetApproxMethods(ValidationShapeType type);
   static Float64 GetJApprox1(ValidationShapeType type);
};

class CValidationShapeFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<ValidationShapeType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<ValidationShapeType, ValidationShapeFactory>(beamIdx, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
};
