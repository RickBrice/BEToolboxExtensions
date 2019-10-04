#pragma once


#include "AbstractBeamFactory.h"

enum class WSDOTBeamType
{
   WF36G, WF42G, WF50G, WF58G, WF66G, WF74G, WF83G, WF95G, WF100G, nSections
};

class WSDOTBeamFactory
{
public:
   static void CreateBeam(WSDOTBeamType type, IShape** ppShape);
   static LPCTSTR GetName(WSDOTBeamType type);
};

class CWSDOTBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<WSDOTBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<WSDOTBeamType, WSDOTBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<WSDOTBeamType, WSDOTBeamFactory>(beamIdx, ppShape); }
};
