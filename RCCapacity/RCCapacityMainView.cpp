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
// RCCapacityMainView.cpp : implementation file
//

#include "stdafx.h"
#include "..\resource.h"
#include "RCCapacityMainView.h"

#include "RCCapacityInputView.h"
#include "RCCapacitySectionView.h"

// CRCCapacityMainView

IMPLEMENT_DYNCREATE(CRCCapacityMainView, CView)

CRCCapacityMainView::CRCCapacityMainView()
{

}

CRCCapacityMainView::~CRCCapacityMainView()
{
}

BEGIN_MESSAGE_MAP(CRCCapacityMainView, CView)
   ON_WM_CREATE()
   ON_WM_SIZE()
END_MESSAGE_MAP()

int CRCCapacityMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CView::OnCreate(lpCreateStruct) == -1)
      return -1;


   return 0;
}

// CRCCapacityMainView drawing

void CRCCapacityMainView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CRCCapacityMainView diagnostics

#ifdef _DEBUG
void CRCCapacityMainView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRCCapacityMainView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CRCCapacityMainView message handlers


void CRCCapacityMainView::OnSize(UINT nType, int cx, int cy)
{
   CView::OnSize(nType, cx, cy);

   // TODO: Add your message handler code here
   if (m_SplitterWnd.GetSafeHwnd())
   {
      // Maintain the relative size/shape of the windows

      // get the row and column information
      int col1, col1Min;
      int col2, col2Min;
      int row1, row1Min;
      m_SplitterWnd.GetColumnInfo(0, col1, col1Min);
      m_SplitterWnd.GetColumnInfo(1, col2, col2Min);
      m_SplitterWnd.GetRowInfo(0, row1, row1Min);

      int col_total = col1 + col2;
      int row_total = row1;

      // resize the window
      m_SplitterWnd.SetWindowPos(this, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);

      // compute the new sizes
      col1 = cx*col1 / col_total;
      col2 = cx*col2 / col_total;
      row1 = cy*row1 / row_total;
      
      // set the new sizes
      m_SplitterWnd.SetColumnInfo(0, col1, col1Min);
      m_SplitterWnd.SetColumnInfo(1, col2, col2Min);
      m_SplitterWnd.SetRowInfo(0, row1, row1Min);

      // update the splitters
      m_SplitterWnd.RecalcLayout();
   }
}


void CRCCapacityMainView::OnInitialUpdate()
{
   CView::OnInitialUpdate();

   // TODO: Add your specialized code here and/or call the base class

   if (!m_SplitterWnd.CreateStatic(this, 1, 2))
   {
      return;
   }

   CRect rect;
   GetClientRect(&rect);

   CCreateContext context;
   context.m_pNewViewClass = RUNTIME_CLASS(CRCCapacityMainView);
   context.m_pCurrentDoc = GetDocument();
   context.m_pCurrentFrame = nullptr;
   context.m_pLastView = nullptr;
   context.m_pNewDocTemplate = nullptr;

   if (!m_SplitterWnd.CreateView(0, 0, RUNTIME_CLASS(CRCCapacityInputView), CSize(rect.Width() / 2, rect.Height()/ 2), &context))
   {
      return;
   }

   if (!m_SplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CRCCapacitySectionView), CSize(rect.Width() / 2, rect.Height() / 2), &context))
   {
      return;
   }

   CRCCapacityInputView* pInputView = (CRCCapacityInputView*)GetLeftView();
   CRCCapacitySectionView* pSectionView = (CRCCapacitySectionView*)GetRightView();
   pInputView->SetSectionView(pSectionView);
   pSectionView->SetInputView(pInputView);
}

CView* CRCCapacityMainView::GetLeftView()
{
   return (CView*)m_SplitterWnd.GetPane(0, 0);
}

CView* CRCCapacityMainView::GetRightView()
{
   return (CView*)m_SplitterWnd.GetPane(0, 1);
}
