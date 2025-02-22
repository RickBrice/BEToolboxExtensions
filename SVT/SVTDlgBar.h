///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright � 1999-2025  Washington State Department of Transportation
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

// CSVTDlgBar
class CSVTDlgBar : public CDialogBar
{
	DECLARE_DYNAMIC(CSVTDlgBar)

public:
   CSVTDlgBar();
	virtual ~CSVTDlgBar();

   virtual BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID) override;
   virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) override;

   void UpdateGirderList();

   void GetGirder(IndexType& typeIdx, IndexType& beamIdx);

   Float64 GetMaxElementSize();

protected:
	DECLARE_MESSAGE_MAP()

   virtual void DoDataExchange(CDataExchange* pDX) override;

   void FillTypeList();
   void FillMeshSizeList();
};
