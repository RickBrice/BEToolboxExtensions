///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2021  Washington State Department of Transportation
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


// CGirderDimensionsDlg dialog
#include "RCCapacityDoc.h"
#include <MfcTools\MetaFileStatic.h>

class CGirderDimensionsDlg : public CDialog
{
	DECLARE_DYNAMIC(CGirderDimensionsDlg)

public:
	CGirderDimensionsDlg(BeamType beamType,const std::array<Float64,14>& values,CWnd* pParent = nullptr);   // standard constructor
	virtual ~CGirderDimensionsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GIRDER_DIMENSIONS };
#endif

	std::array<Float64, 14> m_Values;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BeamType m_BeamType;
	CMetaFileStatic m_GirderPicture;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
