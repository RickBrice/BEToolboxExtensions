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

// RebarGrid.cpp : implementation file
//

#include "stdafx.h"
#include "RebarGrid.h"
#include "RCCapacityInputView.h"
#include <EAF\EAFUtilities.h>
#include <EAF\EAFApp.h>
#include <array>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//GRID_IMPLEMENT_REGISTER(CRebarGrid, CS_DBLCLKS, 0, 0, 0);

/////////////////////////////////////////////////////////////////////////////
// CRebarGrid

CRebarGrid::CRebarGrid()
{
//   RegisterClass();
}

CRebarGrid::~CRebarGrid()
{
}

BEGIN_MESSAGE_MAP(CRebarGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CRebarGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRebarGrid::DoDataExchange(CDataExchange* pDX)
{
   CWnd* pWnd = GetParent();
   ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CRCCapacityInputView)));
   CRCCapacityInputView* pParent = (CRCCapacityInputView*)pWnd;
   if (pDX->m_bSaveAndValidate)
   {
      pParent->m_ModelData.Rebar.clear();
      ROWCOL nRows = GetRowCount();
      for (ROWCOL row = 1; row <= nRows; row++)
      {
         RebarData rebar;
         GetRebarData(row, rebar);
         pParent->m_ModelData.Rebar.push_back(rebar);
      }
   }
   else
   {
      if ( 0 < GetRowCount() )  RemoveRows(1,GetRowCount());

      for (auto rebar : pParent->m_ModelData.Rebar)
      {
         InsertRow(rebar);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// CRebarGrid message handlers
//
void CRebarGrid::RemoveSelectedRows()
{
	CGXRangeList* pSelList = GetParam()->GetRangeList();
	if (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1)
	{
		CGXRange range = pSelList->GetHead();
		range.ExpandRange(1, 0, GetRowCount(), 0);
		RemoveRows(range.top, range.bottom);
	}

   //UpdateData(TRUE);
   //CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
   //pDoc->UpdateAllViews(nullptr,0,nullptr);
}

BOOL CRebarGrid::AreRowsSelected()
{
	if (GetParam() == nullptr)
	{
      return FALSE;
	}

	CGXRangeList* pSelList = GetParam()->GetRangeList();
   return (pSelList->IsAnyCellFromCol(0) && pSelList->GetCount() == 1) ? TRUE : FALSE;
}

void CRebarGrid::CustomInit()
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

   EnableIntelliMouse();
}

void CRebarGrid::AddRow()
{
   RebarData rebarData;
   // Get RebarData to make new row the same as the last row
   InsertRow(rebarData);
}

void CRebarGrid::GetRebarData(ROWCOL row,RebarData& rebar)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   ROWCOL col = 1;

   CString strValue;
   Float64 value;
   strValue = GetCellValue(row,col++);
   rebar.size = lrfdRebarPool::GetBarSize(strValue);

   strValue = GetCellValue(row, col++);
   long iValue;
   sysTokenizer::ParseLong(strValue, &iValue);
   rebar.nBars = iValue;

   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   rebar.spacing = ::ConvertToSysUnits(value,pDispUnits->ComponentDim.UnitOfMeasure);
   
   strValue = GetCellValue(row, col++);
   sysTokenizer::ParseDouble(strValue, &value);
   rebar.location = ::ConvertToSysUnits(value, pDispUnits->ComponentDim.UnitOfMeasure);

   strValue = GetCellValue(row, col++);
   if (strValue == _T("Top Girder"))
      rebar.measure = TopGirder;
   else if (strValue == _T("Bottom Girder"))
      rebar.measure = BottomGirder;
   else if (strValue == _T("Top Slab"))
      rebar.measure = TopDeck;
   else if (strValue == _T("Bottom Slab"))
      rebar.measure = BottomDeck;
   else
   {
      ATLASSERT(false); // should never get here
      rebar.measure = TopGirder;
   }
}

void CRebarGrid::InsertRow(const RebarData& rebar)
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   CRCCapacityInputView* pParent = (CRCCapacityInputView*)GetParent();

   GetParam()->EnableUndo(FALSE);
	ROWCOL nRow = GetRowCount()+1;

	InsertRows(nRow, 1);

   ROWCOL col = 0;
	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
         );

   CString strBarSizeChoiceList;
   matRebar::Type type;
   matRebar::Grade grade;
   pParent->GetRebarType(type, grade);
   lrfdRebarIter rebarIter(type, grade);
   for (rebarIter.Begin(); rebarIter; rebarIter.Next())
   {
      const matRebar* pRebar = rebarIter.GetCurrentRebar();
      strBarSizeChoiceList += pRebar->GetName().c_str();
      strBarSizeChoiceList += _T("\n");
   }

   SetStyleRange(CGXRange(nRow, col++), CGXStyle()
      .SetEnabled(TRUE)
      .SetReadOnly(FALSE)
      .SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
      .SetChoiceList(strBarSizeChoiceList)
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(lrfdRebarPool::GetBarSize(rebar.size).c_str())
   );

   SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue((UINT)rebar.nBars)
         );

	SetStyleRange(CGXRange(nRow,col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(rebar.spacing,pDispUnits->ComponentDim.UnitOfMeasure))
         );

   SetStyleRange(CGXRange(nRow, col++), CGXStyle()
      .SetHorizontalAlignment(DT_RIGHT)
      .SetValue(::ConvertFromSysUnits(rebar.location, pDispUnits->ComponentDim.UnitOfMeasure))
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
      .SetValue(strMeasuredFrom[rebar.measure])
   );

   SetCurrentCell(nRow, GetLeftCol(), GX_SCROLLINVIEW|GX_DISPLAYEDITWND);
//	Invalidate();
//
   GetParam()->EnableUndo(TRUE);
}

CString CRebarGrid::GetCellValue(ROWCOL nRow, ROWCOL nCol)
{
    if (IsCurrentCell(nRow, nCol) && IsActiveCurrentCell())
    {
        CString s;
        CGXControl* pControl = GetControl(nRow, nCol);
        pControl->GetValue(s);
        return s;
    }
    else
    {
       return GetValueRowCol(nRow, nCol);
    }
}

// validate input
//BOOL CRebarGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
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
//      if ( !sysTokenizer::ParseLong(s,&l) )
//      {
//         SetWarningText(_T("Value must be a number"));
//         return FALSE;
//      }
//   }
//   else
//   {
//      Float64 d;
//      if ( !sysTokenizer::ParseDouble(s,&d) )
//      {
//         SetWarningText(_T("Value must be a number"));
//         return FALSE;
//      }
//   }
//
//	return CGXGridWnd::OnValidateCell(nRow, nCol);
//}
//   
//void CRebarGrid::OnModifyCell(ROWCOL nRow,ROWCOL nCol)
//{
//   CGXGridWnd::OnModifyCell(nRow,nCol);
//
//   CBoxGdrDoc* pDoc = (CBoxGdrDoc*)EAFGetDocument();
//   pDoc->UpdateAllViews(nullptr,0,nullptr);
//}
//
void CRebarGrid::OnUnitsModeChanged()
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

void CRebarGrid::UpdateColumnHeaders()
{
   CEAFApp* pApp = EAFGetApp();
   const unitmgtIndirectMeasure* pDispUnits = pApp->GetDisplayUnits();

   // set text along top row
   ROWCOL col = 0;
	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetEnabled(FALSE)          // disables usage as current cell
			.SetValue(_T("Row"))
		);

	SetStyleRange(CGXRange(0,col++), CGXStyle()
         .SetWrapText(TRUE)
			.SetEnabled(FALSE)          // disables usage as current cell
         .SetHorizontalAlignment(DT_CENTER)
         .SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("Size"))
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
}

#if defined _DEBUG
void CRebarGrid::AssertValid() const
{
   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CGXGridWnd::AssertValid();
}
#endif
