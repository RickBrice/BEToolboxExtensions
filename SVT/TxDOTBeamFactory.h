#pragma once


#include "AbstractBeamFactory.h"

enum class TxDOTBeamType
{
   Tx28, Tx34, Tx40, Tx46, Tx54, Tx62, Tx70, nSections
};

class TxDOTBeamFactory
{
public:
   static void CreateBeam(TxDOTBeamType type, IShape** ppShape);
   static LPCTSTR GetName(TxDOTBeamType type);
};

class CTxDOTBeamFactory : public CAbstractBeamFactory
{
public:
   virtual IndexType GetBeamCount() const override { return _GetBeamCount<TxDOTBeamType>(); }
   virtual LPCTSTR GetBeamName(IndexType beamIdx) const override { return _GetBeamName<TxDOTBeamType, TxDOTBeamFactory>(beamIdx); }
   virtual bool CreateBeam(IndexType beamIdx, IShape** ppShape) const override { return _CreateBeam<TxDOTBeamType, TxDOTBeamFactory>(beamIdx, ppShape); }
};
