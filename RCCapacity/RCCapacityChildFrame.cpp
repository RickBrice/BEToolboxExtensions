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
#include "RCCapacityChildFrame.h"
#include "RCCapacityDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRCCapacityChildFrame, CEAFChildFrame)

CRCCapacityChildFrame::CRCCapacityChildFrame(void)
{
}

CRCCapacityChildFrame::~CRCCapacityChildFrame(void)
{
}

BEGIN_MESSAGE_MAP(CRCCapacityChildFrame, CEAFChildFrame)
   ON_WM_CREATE()
	//ON_MESSAGE(WM_HELP, OnCommandHelp)
END_MESSAGE_MAP()

//LRESULT CRCCapacityChildFrame::OnCommandHelp(WPARAM, LPARAM lParam)
//{
//   EAFHelp( EAFGetDocument()->GetDocumentationSetName(), IDH_RCCapacity );
//   return TRUE;
//}

BOOL CRCCapacityChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
   // force this window to be maximized (not sure why WS_VISIBLE is required)
   cs.style |= WS_MAXIMIZE | WS_VISIBLE;
   
   // I have no idea why, but creating this window fail the second time a BEToolbox document
   // is created in a single BridgeLink run. For some reason, MFC determines the wrong
   // class name. lpszClass is nullptr the first time this window is created successfully, so
   // we'll force it to nullptr here so it works on subsequent creation attempts
   cs.lpszClass = nullptr;

   return CEAFChildFrame::PreCreateWindow(cs);
}

int CRCCapacityChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CEAFChildFrame::OnCreate(lpCreateStruct) == -1)
      return -1;

   {
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   EnableToolTips();
   SetIcon(AfxGetApp()->LoadIcon(IDR_RCCAPACITY),TRUE);
   }

   return 0;
}

#if defined _DEBUG
void CRCCapacityChildFrame::AssertValid() const
{
   CEAFChildFrame::AssertValid();
}
#endif

BOOL CRCCapacityChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
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
