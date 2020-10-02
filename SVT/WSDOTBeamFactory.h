#pragma once


#include "AbstractBeamFactory.h"

enum class WSDOTBeamType
{
   W42G, W50G, W58G, W74G,
   WF36G, WF42G, WF50G, WF58G, WF66G, WF74G, WF83G, WF95G, WF100G, 
   WF100G_Modified,
   U54G4, U54G5, U54G6, U66G4, U66G5, U66G6, U78G4, U78G5, U78G6,
   UF60G4, UF60G5, UF60G6, UF72G4, UF72G5, UF72G6, UF84G4, UF84G5, UF84G6,
   nSections
};

class WSDOTBeamFactory
{
public:
   static void CreateBeam(WSDOTBeamType type, IShape** ppShape);
   static LPCTSTR GetName(WSDOTBeamType type);
   static int GetApproxMethods(WSDOTBeamType type);
   static Float64 GetJApprox1(WSDOTBeamType type);
};

class CWSDOTBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<WSDOTBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<WSDOTBeamType, WSDOTBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<WSDOTBeamType, WSDOTBeamFactory>(beamIdx, ppShape); }
   virtual int GetApproxMethods(IndexType beamIdx) const override { return _ApproxMethods<WSDOTBeamType, WSDOTBeamFactory>(beamIdx); }
   virtual Float64 GetJApprox1(IndexType beamIdx) const override { return _GetJApprox1<WSDOTBeamType, WSDOTBeamFactory>(beamIdx); }
};
