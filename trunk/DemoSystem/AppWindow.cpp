//---------------------------------------------------------------------------//
// Neon v2.5
// Copyright (C) 2006,2008 Jordi Ros <shine.3p@gmail.com>
// www.neonv2.com / www.xplsv.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program called license.txt
// If not, see <http://www.gnu.org/licenses/>
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "AviDecoder.h"
#include "FFMpegVideo.h"

#include <winuser.h>

#define APP_NAME  "Neon2 Server"
#define APP_TITLE ""

void PeticionEndApp();

//----------------------------------------------------------
// Globales
//
CAppWindow      g_AppWindow;
CDisplayDevice  g_DisplayDevice;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
  {
    case WM_CLOSE:
      return FALSE;
  	case WM_DESTROY:
			PostQuitMessage(0);
			return FALSE;
    case WM_ACTIVATEAPP:
      ShowCursor(g_AppWindow.ShowMouse());
      g_AppWindow.SetActive(wParam ? true : false);
      return FALSE;
    case WM_SETCURSOR:
      SetCursor(g_AppWindow.ShowMouse() ? LoadCursor(0,IDC_ARROW) : NULL);
      return FALSE;
    case WM_MOUSEMOVE:
      g_AppWindow.MouseMove(lParam & 0xFFFF, lParam>>16);
      return FALSE;
    case WM_LBUTTONDOWN:
      SetCapture(hWnd);
      g_AppWindow.LButtonDown(lParam & 0xFFFF, lParam>>16);
      return FALSE;
    case WM_LBUTTONUP:
      ReleaseCapture();
      g_AppWindow.LButtonUp(lParam & 0xFFFF, lParam>>16);
      return FALSE;
    case WM_KEYDOWN:
      g_AppWindow.InputKey(wParam);
      return FALSE;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CAppWindow::Init(TModoGrafico const &Modo, TRect const &rWindow, bool bCanMove, HINSTANCE hInst, HWND hWndMain, bool bShowMouse)
{
  m_Modo          = Modo;
  m_bCanMove      = bCanMove;
  m_bMovingWindow = false;
  m_bShowMouse    = bShowMouse;

  m_Window = rWindow;
  if (InitWindow(hInst, Modo.bFullScreen))
  {
    // Inits
    if (g_DisplayDevice.Init(Modo, g_AppWindow.GetHandle()))
    {
      if (Modo.bFullScreen)
        SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)WindowProc);
      CAviDecoder ::InitSubsystem();
      #ifdef USE_MPEG_VIDEO
      CFFMpegVideo::InitSubsystem();
      #endif
      m_bOk = true;
    }
    else
      GLOG(("ERR: Can't initialize DisplayDevice\n"));
  }
  else
    GLOG(("ERR: Can't initialize output window\n"));

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CAppWindow::End()
{
  if (IsOk())
  {
    CAviDecoder ::EndSubsystem();
    #ifdef USE_MPEG_VIDEO
     CFFMpegVideo::EndSubsystem();
    #endif
    g_DisplayDevice.End();

    DestroyWindow(m_hWnd);
	  ShowCursor   (TRUE);

    CoUninitialize();
    
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// InitWindow
//
//---------------------------------------------------------------------------//
bool CAppWindow::InitWindow(HINSTANCE hInst, bool bFullscreen)
{
  WNDCLASS wc;
	
  ShowCursor(FALSE);
  wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WindowProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInst;
  wc.hIcon         = NULL;
  wc.hCursor       = NULL; //LoadCursor(hInst, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszMenuName  = APP_NAME;
  wc.lpszClassName = APP_NAME;
  RegisterClass(&wc);

  int w = bFullscreen ? 0 : m_Window.w;
  int h = bFullscreen ? 0 : m_Window.h;

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
                          APP_NAME,
                          APP_TITLE,
                          WS_POPUP,
                          m_Window.x,
                          m_Window.y,
                          w, h,
                          NULL,
                          NULL,
                          hInst,
                          NULL);
  /*
  RECT Rect;
  GetWindowRect(m_hWnd, &Rect);
  m_Window.x = Rect.left;
  m_Window.y = Rect.top;
  m_Window.w = Rect.right  - Rect.left;
  m_Window.h = Rect.bottom - Rect.top;
  */

  if (!m_hWnd)
		return FALSE;

  m_hInst = hInst;
	ShowWindow  (m_hWnd, SW_SHOW);
  UpdateWindow(m_hWnd);
  SetFocus    (m_hWnd);
  ShowCursor  (g_AppWindow.ShowMouse());
	
  return TRUE;
}


//---------------------------------------------------------------------------//
// UpdateWindow
//
//---------------------------------------------------------------------------//
void CAppWindow::AdjustWindow()
{
  /*
  if (!bFullScreen)
  {
    RECT rectWindow;
	  RECT rectClient;
	  long lCoorxWin, lCooryWin;

	  GetWindowRect(m_hWnd, &rectWindow);
	  GetClientRect(m_hWnd, &rectClient);

	  rectClient.right -= m_Window.w;
	  rectClient.bottom-= m_Window.h;
	  rectWindow.right -= (rectClient.right - rectClient.left);
	  rectWindow.bottom-= (rectClient.bottom - rectClient.top);

	  // intenta centrar la ventana
	  lCoorxWin = (GetSystemMetrics(SM_CXSCREEN) - 640) /2;
	  lCooryWin = (GetSystemMetrics(SM_CYSCREEN) - 480) /2;
	  
	  GetWindowRect(m_hWnd, &rectClient);
	  lCoorxWin = rectClient.left;
	  lCooryWin = rectClient.top;

    LONG lWinLong = GetWindowLong(m_hWnd, GWL_STYLE);
    lWinLong |= (WS_BORDER |WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
    //lWinLong =g_dwWindowStyle;

    SetWindowLong(m_hWnd, GWL_STYLE, lWinLong);
    BOOL b = SetWindowPos(m_hWnd, HWND_NOTOPMOST, 
                          lCoorxWin, lCooryWin, 
                          rectWindow.right-rectWindow.left, rectWindow.bottom-rectWindow.top, 
                          SWP_SHOWWINDOW|SWP_FRAMECHANGED);			                      		      
  }
  else
  {
    long lWinLong=GetWindowLong(m_hWnd, GWL_STYLE);;
    lWinLong &= ~(WS_BORDER | WS_CAPTION);
	  SetWindowLong(m_hWnd, GWL_STYLE, lWinLong);
    SetWindowPos (m_hWnd, HWND_TOPMOST,
                  0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),
                  SWP_NOZORDER|SWP_NOACTIVATE);
  }  
  RedrawWindow(m_hWnd, 0,0,RDW_FRAME|RDW_INVALIDATE);
  */
}



//---------------------------------------------------------------------------//
// ProcessMessages
//
//---------------------------------------------------------------------------//
void CAppWindow::ProcessMessages()
{
  MSG Msg;
	while (PeekMessage(&Msg, NULL, 0,0, PM_REMOVE))
  {
		if (Msg.message == WM_QUIT)
	    PeticionEndApp();
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
    // Send messages to child
    if (m_hWndMain)
      SendMessage(m_hWndMain, Msg.message, Msg.wParam, Msg.lParam);
	}
}


//---------------------------------------------------------------------------//
// InputKey
//
//---------------------------------------------------------------------------//
void CAppWindow::InputKey(int iKey)
{
  if (iKey == VK_ESCAPE)
    PeticionEndApp();
}


//---------------------------------------------------------------------------//
// LButtonDown
//
//---------------------------------------------------------------------------//
void CAppWindow::LButtonDown(int iX, int iY)
{
  if (m_bCanMove)
  {
    m_bMovingWindow = true;
    m_iMousePosX    = iX;
    m_iMousePosY    = iY;
  }
}


//---------------------------------------------------------------------------//
// LButtonUp
//
//---------------------------------------------------------------------------//
void CAppWindow::LButtonUp(int iX, int iY)
{
  if (m_bCanMove && m_bMovingWindow)
  {
    m_bMovingWindow = false;
    m_Window.x+= iX - m_iMousePosX; 
    m_Window.y+= iY - m_iMousePosY;
    MoveWindow(m_hWnd, m_Window.x, m_Window.y, m_Window.w, m_Window.h, true);
  }
}



//---------------------------------------------------------------------------//
// LButtonUp
//
//---------------------------------------------------------------------------//
void CAppWindow::MouseMove(int iX, int iY)
{
  if (m_bMovingWindow)
  {
    m_Window.x+= iX - m_iMousePosX; 
    m_Window.y+= iY - m_iMousePosY;
    MoveWindow(m_hWnd, m_Window.x, m_Window.y, m_Window.w, m_Window.h, true);
    //SetWindowPos(m_hWnd, NULL, m_Window.x, m_Window.y, m_Window.w, m_Window.h, 0);
  }
}


//---------------------------------------------------------------------------//
// SwitchFullScreen
//
//---------------------------------------------------------------------------//
void CAppWindow::SwitchFullScreen(bool bFullScreen)
{
  m_bUpdateDevice    = true;
  m_Modo.bFullScreen = bFullScreen;
}


//---------------------------------------------------------------------------//
// UpdateFullScreen
//
//---------------------------------------------------------------------------//
void CAppWindow::UpdateFullScreen()
{
  if (m_bUpdateDevice)
  {
    m_bUpdateDevice = false;
    m_bCanMove      = !m_Modo.bFullScreen;
    if (m_Modo.bFullScreen)
    {
      m_iOldWindowPosX = m_Window.x;
      m_iOldWindowPosY = m_Window.y;
      //GetSystemMetrics()
      HMONITOR hMonitor = MonitorFromWindow(m_hWnd, 0); //MONITOR_DEFAULTTONEAREST
      MONITORINFOEX mi;
      mi.cbSize = sizeof(mi);
      GetMonitorInfo(hMonitor, (MONITORINFO*)&mi);
      SetDebugErrorLevel(0);
      SetRect(m_Window, mi.rcMonitor.left,mi.rcMonitor.top, mi.rcMonitor.right-mi.rcMonitor.left,mi.rcMonitor.bottom-mi.rcMonitor.top);
      //GetSystemMetrics(SM_CMONITORS) // Num monitores
      SetWindowPos(m_hWnd, HWND_TOPMOST, m_Window.x, m_Window.y, m_Window.w, m_Window.h, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE);
    }
    else
    {
      SetRect     (m_Window, m_iOldWindowPosX,m_iOldWindowPosY, m_Modo.iWidth,m_Modo.iHeight);
      SetWindowPos(m_hWnd, NULL, m_Window.x, m_Window.y, m_Window.w, m_Window.h, SWP_FRAMECHANGED);
      ShowWindow  (m_hWnd, SW_SHOW);
    }
  }
}
