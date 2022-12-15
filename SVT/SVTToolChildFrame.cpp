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

#include "StdAfx.h"
#include "..\resource.h"
#include "SVTToolChildFrame.h"
#include "SVTToolDoc.h"
#include "SVTToolView3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSVTToolChildFrame, CEAFChildFrame)

CSVTToolChildFrame::CSVTToolChildFrame(void)
{
}

CSVTToolChildFrame::~CSVTToolChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CSVTToolChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
   ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
   //ON_MESSAGE(WM_HELP, OnCommandHelp)
   ON_CBN_SELCHANGE(IDC_TYPE, OnTypeChanged)
   ON_CBN_SELCHANGE(IDC_GIRDERS, OnGirderChanged)
   ON_CBN_SELCHANGE(IDC_MESH_SIZE,OnSizeChanged)
   ON_BN_CLICKED(IDC_COMPUTE, OnCompute)
END_MESSAGE_MAP()

//LRESULT CSVTToolChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
//{
//   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_SVTTOOL );
//   return TRUE;
//}

BOOL CSVTToolChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;
   cs.style &= ~WS_MAXIMIZEBOX; // we don't want a maximize box
   cs.style &= ~WS_MINIMIZEBOX; // we don't want a minimize box
   cs.style &= ~WS_SYSMENU; // we don't want a system menu

   // I have no idea why, but creating this window fail the second time a BEToolbox document
   // is created in a single BridgeLink run. For some reason, MFC determines the wrong
   // class name. lpszClass is nullptr the first time this window is created successfully, so
   // we'll force it to nullptr here so it works on subsequent creation attempts
   cs.lpszClass = nullptr;

   return CEAFChildFrame::PreCreateWindow(cs);
}


BOOL CSVTToolChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
   // Create a splitter window with 1 row and 2 columns
   if (!m_SplitterWnd.CreateStatic(this, 1, 2))
   {
      TRACE0("Failed to create static splitter");
      return FALSE;
   }

   // Add the first pane
   int left_size = (int)(((Float64)lpcs->cx)*0.5);
   int right_size = lpcs->cx - left_size;
   if (!m_SplitterWnd.CreateView(0, 0, pContext->m_pNewViewClass, CSize(left_size, lpcs->cy), pContext))
   {
      TRACE0("Failed to create first pane");
      return FALSE;
   }

   // Add the second pane
   if (!m_SplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CSVTToolView3D), CSize(right_size, lpcs->cy), pContext))
   {
      TRACE0("Failed to create second pane");
      return FALSE;
   }

   // Activate the first pane
   SetActiveView((CView*)m_SplitterWnd.GetPane(0, 0));

   // I don't know why you don't call the parent but it makes the difference between
   // the splitter working and not.  See the ViewEx example. They don't call the
   // parent method either.
   //return CChildFrame::OnCreateClient(lpcs, pContext);
   return TRUE;
}

int CSVTToolChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
      AFX_MANAGE_STATE(AfxGetStaticModuleState());

      if (!m_DlgBar.Create(this, IDD_SVT, CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, AFX_IDW_CONTROLBAR_FIRST))
      {
         TRACE0("Failed to create dialog bar\n");
         return -1;
      }
      EnableToolTips();
      m_DlgBar.EnableToolTips();
      SetIcon(AfxGetApp()->LoadIcon(IDR_SVT), TRUE);
   }


   return 0;
}


BOOL CSVTToolChildFrame::OnToolTipNotify(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
   TOOLTIPTEXT* pTTT = (TOOLTIPTEXT*)pNMHDR;
   HWND hwndTool = (HWND)pNMHDR->idFrom;
   if (pTTT->uFlags & TTF_IDISHWND)
   {
      // idFrom is actually HWND of tool
      UINT nID = ::GetDlgCtrlID(hwndTool);
      switch (nID)
      {
      case IDC_GIRDERS:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Select a girder"));
         break;

      case IDC_COMPUTE:
         _tcscpy_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), _T("Compute the St. Venant Torsional Constant"));
         break;

      default:
         return FALSE;
      }

      return TRUE;
   }
   return FALSE;
}

#if defined _DEBUG
void CSVTToolChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CSVTToolChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
   // CEAFChildFrame::LoadFrame is going to load resources based on nIDResource
   // We have to set the module context to this module so the load happens
   // correctly
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if (!CEAFChildFrame::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
   {
      return FALSE;
   }

   return TRUE;
}

void CSVTToolChildFrame::OnTypeChanged()
{
   m_DlgBar.UpdateGirderList();
   OnGirderChanged();
}

void CSVTToolChildFrame::OnGirderChanged()
{
   IndexType typeIdx, beamIdx;
   m_DlgBar.GetGirder(typeIdx, beamIdx);
   CSVTToolDoc* pDoc = (CSVTToolDoc*)EAFGetDocument();
   pDoc->SetGirder(typeIdx, beamIdx);
   pDoc->UpdateAllViews(nullptr);
}

void CSVTToolChildFrame::OnSizeChanged()
{
   CSVTToolDoc* pDoc = (CSVTToolDoc*)EAFGetDocument();
   pDoc->SetMaxElementSize(m_DlgBar.GetMaxElementSize());
   pDoc->UpdateAllViews(nullptr);
}

#if defined USE_COM_GEOMETRY
void CSVTToolChildFrame::OnCompute()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CWaitCursor cursor;

   CSVTToolDoc* pDoc = (CSVTToolDoc*)EAFGetDocument();
   const Results& r = pDoc->GetTorsionalConstant();
   Float64 A, Yt, Yb, Ix, Iy, J;
   r.Props->get_Area(&A);
   r.Props->get_Ytop(&Yt);
   r.Props->get_Ybottom(&Yb);
   r.Props->get_Ixx(&Ix);
   r.Props->get_Iyy(&Iy);
   J = r.J;

   auto& convert = pDoc->GetUnitConvert();

   VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(A, CComBSTR("in^2"), &A)));
   VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(Yt, CComBSTR("in"), &Yt)));
   VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(Yb, CComBSTR("in"), &Yb)));
   VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(Ix, CComBSTR("in^4"), &Ix)));
   VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(Iy, CComBSTR("in^4"), &Iy)));
   VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(J, CComBSTR("in^4"), &J)));

   CString str;
   str.Format(_T("A = %f\nYt = %f\nYb = %f\nIx = %f\nIy = %f\nJ = %f\n"), A, Yt, Yb, Ix, Iy, J);

   if (r.ApproxMethods & AM_J1)
   {
      VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(r.Japprox1, CComBSTR("in^4"), &J)));

      CString s;
      s.Format(_T("Japprox1 = %f\n"), J);
      str += s;
   }
   else
   {
      str += _T("Japprox1 = -\n");
   }

   if (r.ApproxMethods & AM_J2)
   {
      VERIFY(SUCCEEDED(convert->ConvertFromBaseUnits(r.Japprox2, CComBSTR("in^4"), &J)));

      CString s;
      s.Format(_T("Japprox2 = %f\n"), J);
      str += s;
   }
   else
   {
      str += _T("Japprox2 = -\n");
   }

   CString s;
   s.Format(_T("#Elements = %d\n#Points = %d"), r.nElements, r.nInteriorNodes);

   str += s;

   pDoc->UpdateAllViews(nullptr);

   AfxMessageBox(str);
}
#else
void CSVTToolChildFrame::OnCompute()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   CWaitCursor cursor;

   CSVTToolDoc* pDoc = (CSVTToolDoc*)EAFGetDocument();
   const Results2& r = pDoc->GetTorsionalConstant();
   Float64 A, Yt, Yb, Ix, Iy, J;
   A = r.Props.GetArea();
   Yb = r.Props.GetYbottom();
   Yt = r.Props.GetYtop();
   Ix = r.Props.GetIxx();
   Iy = r.Props.GetIyy();
   J = r.J;

   A = WBFL::Units::ConvertFromSysUnits(A, WBFL::Units::Measure::Inch2);
   Yb = WBFL::Units::ConvertFromSysUnits(Yb, WBFL::Units::Measure::Inch);
   Yt = WBFL::Units::ConvertFromSysUnits(Yt, WBFL::Units::Measure::Inch);
   Ix = WBFL::Units::ConvertFromSysUnits(Ix, WBFL::Units::Measure::Inch4);
   Iy = WBFL::Units::ConvertFromSysUnits(Iy, WBFL::Units::Measure::Inch4);
   J = WBFL::Units::ConvertFromSysUnits(J, WBFL::Units::Measure::Inch4);

   CString str;
   str.Format(_T("A = %f\nYt = %f\nYb = %f\nIx = %f\nIy = %f\nJ = %f\nMaxSlope = %f\n"), A, Yt, Yb, Ix, Iy, J, r.MaxSlope);

   if (r.ApproxMethods & AM_J1)
   {
      CString s;
      s.Format(_T("Japprox1 = %f\n"), WBFL::Units::ConvertFromSysUnits(r.Japprox1,WBFL::Units::Measure::Inch4));
      str += s;
   }
   else
   {
      str += _T("Japprox1 = -\n");
   }

   if (r.ApproxMethods & AM_J2)
   {
      CString s;
      s.Format(_T("Japprox2 = %f\n"), WBFL::Units::ConvertFromSysUnits(r.Japprox2,WBFL::Units::Measure::Inch4));
      str += s;
   }
   else
   {
      str += _T("Japprox2 = -\n");
   }

   CString s;
   s.Format(_T("#Elements = %d\n#Points = %d"), r.nElements, r.nInteriorNodes);

   str += s;

   pDoc->UpdateAllViews(nullptr);

   AfxMessageBox(str);
}
#endif