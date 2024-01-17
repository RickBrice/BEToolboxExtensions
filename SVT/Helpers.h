///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2024  Washington State Department of Transportation
//                        Bridge and Structures Office
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as 
// published by the Washington State Department of Transportation, 
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful, but 
// distribution is AS IS, WITHOUT ANY WARRANTY; without even the implied 
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See 
// the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Route Open Source 
// License along with this program; if not, write to the Washington 
// State Department of Transportation, Bridge and Structures Office, 
// P.O. Box  47340, Olympia, WA 98503, USA or e-mail 
// Bridge_Support@wsdot.wa.gov
///////////////////////////////////////////////////////////////////////

#pragma once

#include <WBFLGeometry.h>
#include <WBFLUnitServer.h>
#include <EngTools/PrandtlMembraneSolution.h>
#include <GeomModel/GeomModel.h>


namespace _NUBeam
{
   enum NUBeamDimensions { D1, D2, D3, D4, D5, R1, R2, R3, R4, T, W1, W2, C1 };
}

namespace _ILBeam
{
   enum NUBeamDimensions { D1, D2, D3, D4, D5, R1, R2, T, W1, W2, C1 };
}

namespace IBeam
{
   enum IBeamDimensions { C1, D1, D2, D3, D4, D5, D6, D7, T1, T2, W1, W2, W3, W4 };
}

namespace IBeam2
{
   enum IBeamDimensions { C1, D1, D2, D3, D4, D5, D6, H, T1, T2, W1, W2, W3, W4, W5 };
}

namespace _UBeam
{
   enum Dimensions { D1, D2, D3, D4, D5, D6, D7, T, W1, W2, W3, W4, W5 };
}

namespace _UBeam2
{
   enum Dimensions { C1, D1, D2, D3, D4, D5, D6, W1, W2, W3, W4, W5, W6, W7 };
}

namespace _FIBBeam
{
   enum Dimensions{ D1, D2, D3, D4, D5, H, T, R, W1, W2, W3, C1 };
}

using Results = struct
{
   WBFL::EngTools::PrandtlMembraneSolution solution;
   Float64 J;
   int ApproxMethods;
   Float64 Japprox1;
   Float64 Japprox2;
   //Float64 Japprox3;
   IndexType nElements;
   IndexType nInteriorNodes;
   Float64 ApproxArea;
   CComPtr<IShape> Shape;
   CComPtr<IShapeProperties> Props;
};


using Results2 = struct
{
   WBFL::EngTools::PrandtlMembraneSolution solution;
   Float64 J;
   Float64 MaxSlope;
   IndexType MaxSlopeElementIdx;
   Float64 Tmax_per_T;
   int ApproxMethods;
   Float64 Japprox1;
   Float64 Japprox2;
   Float64 Japprox3;
   IndexType nElements;
   IndexType nInteriorNodes;
   Float64 ApproxArea;
   std::unique_ptr<WBFL::Geometry::Shape> Shape;
   WBFL::Geometry::ShapeProperties Props;
};

std::tuple<Float64, Float64, Float64> GetColor(Float64 min, Float64 max, Float64 value);

Float64 GetJApprox2(IShapeProperties* pProps);
Float64 GetJApprox2(const WBFL::Geometry::ShapeProperties& props);


Float64 ComputeJApprox_IBeam(int i, IUnitConvert* pConvert, const Float64 dimensions[][14]);
Float64 ComputeJApprox_IBeam2(int i, IUnitConvert* pConvert, const Float64 dimensions[][15]);
Float64 ComputeJApprox_NU(int i, IUnitConvert* pConvert, const Float64 dimensions[][13]);
Float64 ComputeJApprox_UBeam(int i, IUnitConvert* pConvert, const Float64 dimensions[][13]);
Float64 ComputeJApprox_UBeam2(int i, IUnitConvert* pConvert, const Float64 dimensions[][14]);


Float64 ComputeJApprox_IBeam(int i, const Float64 dimensions[][14]);
Float64 ComputeJApprox_IBeam2(int i, const Float64 dimensions[][15]);
Float64 ComputeJApprox_NU(int i, const Float64 dimensions[][13]);
Float64 ComputeJApprox_UBeam(int i, const Float64 dimensions[][13]);
Float64 ComputeJApprox_UBeam2(int i, const Float64 dimensions[][14]);


Float64 ComputeJApprox3_IBeam(int i, const Float64 dimensions[][14]);
Float64 ComputeJApprox3_IBeam2(int i, const Float64 dimensions[][15]);
Float64 ComputeJApprox3_NU(int i, const Float64 dimensions[][13]);
Float64 ComputeJApprox3_UBeam(int i, const Float64 dimensions[][13]);
Float64 ComputeJApprox3_UBeam2(int i, const Float64 dimensions[][14]);

// maps beam dimensions from the old WBFLGeometry::PrecastBeam dimensions to the new WBFL::Geometry::PrecastBeam dimensions (which are the same as WBFLGeometry::IPrecastBeam2)
void MapPrecastBeamDimensions(std::unique_ptr<WBFL::Geometry::PrecastBeam>& beam, Float64 c1, Float64 d1, Float64 d2, Float64 d3, Float64 d4, Float64 d5, Float64 d6, Float64 d7, Float64 t1, Float64 t2, Float64 w1, Float64 w2, Float64 w3, Float64 w4);
