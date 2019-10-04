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
};

class CValidationShapeFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<ValidationShapeType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<ValidationShapeType, ValidationShapeFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<ValidationShapeType, ValidationShapeFactory>(beamIdx, ppShape); }
};
