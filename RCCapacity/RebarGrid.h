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

// RebarGrid.h : header file
//
// 

#include "RCCapacityDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CRebarGrid window

class CRebarGrid : public CGXGridWnd
{
	//GRID_DECLARE_REGISTER()
// Construction
public:
   CRebarGrid();

// Attributes
public:

// Operations
public:
   void DoDataExchange(CDataExchange* pDX);

   void OnUnitsModeChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRebarGrid();

	void AddRow();
	void RemoveSelectedRows();
	BOOL AreRowsSelected();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRebarGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//afx_msg void OnEditInsertrow();
	//afx_msg void OnEditRemoverows();
	//afx_msg void OnUpdateEditRemoverows(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

   void InsertRow(const RebarData& rebarData);
	void GetRebarData(ROWCOL row, RebarData& rebar);

   // virtual overrides for grid
   //virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol) override;
   //virtual void OnModifyCell(ROWCOL nRow,ROWCOL nCol) override;

   void UpdateColumnHeaders();

public:
   // custom stuff for grid
   void CustomInit();

   // get a cell value whether is is selected or not
   CString GetCellValue(ROWCOL nRow, ROWCOL nCol);


#if defined _DEBUG
   void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
