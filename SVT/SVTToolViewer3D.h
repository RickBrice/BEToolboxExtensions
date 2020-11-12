#pragma once

#include <Inventor/Win/viewers/SoWinExaminerViewer.h>

class CSVTToolViewer3D : public SoWinExaminerViewer
{
public:
   CSVTToolViewer3D(HWND hwnd);

private:
   CWnd m_Wnd;
};