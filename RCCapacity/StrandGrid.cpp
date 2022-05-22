///////////////////////////////////////////////////////////////////////
// BEToolbox
// Copyright © 1999-2022  Washington State Department of Transportation
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

// RebarGrid.cpp : implementation file
//

#include "stdafx.h"
#include "StrandGrid.h"
#include "RCCapacityInputView.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>

#include <array>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CStrandGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CStrandGrid

CStrandGrid::CStrandGrid()
{
//   RegisterClass();
}

CStrandGrid::~CStrandGrid()
{
}

BEGIN_MESSAGE_MAP(CStrandGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CStrandGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CStrandGrid::DoDataExchange(CDataExchange* pDX)
{
	CWnd* pWnd = GetParent();
	ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CRCCapacityInputView)));
	CRCCapacityInputView* pParent = (CRCCapacityInputView*)pWnd;
	if (pDX->m_bSaveAndValidate)
	{
		pParent->m_ModelData.Strands.clear();
		ROWCOL nRows = GetRowCount();
		for (ROWCOL row = 1; row <= nRows; row++)
		{
			StrandData strand;
			GetStrandData(row, strand);
			pParent->m_ModelData.Strands.push_back(strand);
		}
	}
	else
	{
		if (0 < GetRowCount())  RemoveRows(1, GetRowCount());

		for (auto strand : pParent->m_ModelData.Strands)
		{
			InsertRow(strand);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CStrandGrid message handlers
//
void CStrandGrid::RemoveSelectedRows()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}
}

BOOL CStrandGrid::AreRowsSelected()
{
	if (GetParam() == nullptr)
	{
		return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
	return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CStrandGrid::CustomInit()
{
   // Initialize the grid. For CWnd based grids this call is // 
   // essential. For view based grids this initialization is done 
   // in OnInitialUpdate.
	Initialize( );

	GetParam( )->EnableUndo(FALSE);

   const ROWCOL num_rows = 0;
   const ROWCOL num_cols = 5;

	SetRowCount(num_rows);
	SetColCount(num_cols);

		// Turn off selecting whole columns when clicking on a column header
	GetParam()->EnableSelection((WORD) (GX_SELFULL & ~GX_SELCOL & ~GX_SELTABLE));

   // disable left side
	SetStyleRange(CGXRange(0,0,num_rows,0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetEnabled(FALSE)          // disables usage as current cell
		);

   UpdateColumnHeaders();

   // make it so that text fits correctly in header row
	ResizeRowHeightsToFit(CGXRange(0,0,0,num_cols));

   // don't allow users to resize grids
   GetParam( )->EnableTrackColWidth(0); 
   GetParam( )->EnableTrackRowHeight(0); 

	SetFocus();

	GetParam( )->EnableUndo(TRUE);

//   EnableIntelliMouse();
}

void CStrandGrid::AddRow()
{
	StrandData strandData;
	// Get StrandData to make new row the same as the last row
	InsertRow(strandData);
}

void CStrandGrid::GetStrandData(ROWCOL row, StrandData& strand)
{
	CEAFApp* pApp = EAFGetApp();
	const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

	ROWCOL col = 1;

	CString strValue;
	Float64 value;
	strValue = GetCellValue(row, col++);
	long iValue;
	WBFL::System::Tokenizer::ParseLong(strValue, &iValue);
	strand.nStrands = iValue;

	strValue = GetCellValue(row, col++);
	WBFL::System::Tokenizer::ParseDouble(strValue, &value);
	strand.spacing = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

	strValue = GetCellValue(row, col++);
	WBFL::System::Tokenizer::ParseDouble(strValue, &value);
	strand.location = WBFL::Units::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

	strValue = GetCellValue(row, col++);
	if (strValue == _T("Top Girder"))
		strand.measure = TopGirder;
	else if (strValue == _T("Bottom Girder"))
		strand.measure = BottomGirder;
	else if (strValue == _T("Top Slab"))
		strand.measure = TopDeck;
	else if (strValue == _T("Bottom Slab"))
		strand.measure = BottomDeck;
	else
	{
		ATLASSERT(false); // should never get here
		strand.measure = TopGirder;
	}

	strValue = GetCellValue(row, col++);
	WBFL::System::Tokenizer::ParseDouble(strValue, &value);
	strand.fpe = WBFL::Units::ConvertToSysUnits(value, pDispUnits->Stress.UnitOfMeasure);
}

void CStrandGrid::InsertRow(const StrandData& strand)
{
	CEAFApp* pApp = EAFGetApp();
	const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

	CRCCapacityInputView* pParent = (CRCCapacityInputView*)GetParent();

	GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount() + 1;

	InsertRows(nRow, 1);

	ROWCOL col = 0;
	SetStyleRange(CGXRange(nRow, col++), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
	);

	SetStyleRange(CGXRange(nRow, col++), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValue((UINT)strand.nStrands)
	);

	SetStyleRange(CGXRange(nRow, col++), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValue(WBFL::Units::ConvertFromSysUnits(strand.spacing, pDispUnits->ComponentDim.UnitOfMeasure))
	);

	SetStyleRange(CGXRange(nRow, col++), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValue(WBFL::Units::ConvertFromSysUnits(strand.location, pDispUnits->ComponentDim.UnitOfMeasure))
	);

	std::array<CString, 4> strMeasuredFrom{ _T("Top Girder"),_T("Bottom Girder"),_T("Top Slab"),_T("Bottom Slab") };
	int nChoices = pParent->HasSlab() ? 4 : 2;
	CString strMeasuredFromChoiceList;
	for (int i = 0; i < nChoices; i++)
	{
		strMeasuredFromChoiceList += strMeasuredFrom[i];
		strMeasuredFromChoiceList += _T("\n");
	}
	SetStyleRange(CGXRange(nRow, col++), CGXStyle()
		.SetEnabled(TRUE)
		.SetReadOnly(FALSE)
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetChoiceList(strMeasuredFromChoiceList)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValue(strMeasuredFrom[strand.measure])
	);

	SetStyleRange(CGXRange(nRow, col++), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValue(WBFL::Units::ConvertFromSysUnits(strand.fpe, pDispUnits->Stress.UnitOfMeasure))
	);

	SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW | GX_DISPLAYEDITWND);
	//	Invalidate();
	//
	GetParam()->EnableUndo(TRUE);
}

CString CStrandGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
{
    if (IsCurrentCell(nRow, nCol) && IsActiveCurrentCell())
    {
        CString s;
        CGXControl* pControl = GetControl(nRow, nCol);
        pControl->GetValue(s);
        return s;
  }
    else
        return GetValueRowCol(nRow, nCol);
}

// validate input
//BOOL CStrandGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
//{
//	CString s;
//	CGXControl* pControl = GetControl(nRow, nCol);
//	pControl->GetCurrentText(s);
//
//   if ( s.IsEmpty() )
//   {
//      SetWarningText(_T("Value must be a number"));
//      return FALSE;
//   }
//
//   if ( nCol == 2 )
//   {
//      long l;
//      if ( !WBFL::System::Tokenizer::ParseLong(s,&l) )
//      {
//         SetWarningText(_T("Value must be a number"));
//         return FALSE;
//      }
//   }
//   else
//   {
//      Float64 d;
//      if ( !WBFL::System::Tokenizer::ParseDouble(s,&d) )
//      {
//         SetWarningText(_T("Value must be a number"));
//         return FALSE;
//      }
//   }
//
//	return CGXGridWnd::OnValidateCell(nRow, nCol);
//}
//   
//void CStrandGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
//{
//   CGXGridWnd::OnModifyCell(nRow,nCol);
//
//   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
//   pDoc->UpdateAllViews(nullptr,0,nullptr);
//}
//
void CStrandGrid::OnUnitsModeChanged()
{
   ROWCOL nRows = GetRowCount();
   if ( 0 < nRows )
   {
      CGXRange range(1, 0, GetRowCount(), 0);
      RemoveRows(range.top, range.bottom);
   }

   UpdateColumnHeaders();
   UpdateData(FALSE);
}

void CStrandGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const WBFL::Units::IndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 0;
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetEnabled(FALSE)          // disables usage as current cell
			.SetValue(_T("Row"))
		);

	SetStyleRange(CGXRange(0, col++), CGXStyle()
		.SetWrapText(TRUE)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("Num."))
	);

	CString strDimension;
	strDimension.Format(_T("%s (%s)"), _T("Spacing"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0, col++), CGXStyle()
		.SetWrapText(TRUE)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(strDimension)
	);

	strDimension.Format(_T("%s (%s)"), _T("Location"), pDispUnits->ComponentDim.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0, col++), CGXStyle()
		.SetWrapText(TRUE)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(strDimension)
	);

	SetStyleRange(CGXRange(0, col++), CGXStyle()
		.SetWrapText(TRUE)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("Measured From"))
	);


	strDimension.Format(_T("%s (%s)"), _T("Effective Prestress"), pDispUnits->Stress.UnitOfMeasure.UnitTag().c_str());
	SetStyleRange(CGXRange(0, col++), CGXStyle()
		.SetWrapText(TRUE)
		.SetEnabled(FALSE)          // disables usage as current cell
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(strDimension)
	);
}

#if defined _DEBUG
void CStrandGrid::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
