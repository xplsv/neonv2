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

#include <MGPch.h>
#include "MGAppWindow.h"
#include "MGApplication.h"

#include <winuser.h>

#define APP_NAME  "Neonv2.5"
#define APP_TITLE "Neon v2.5 Beta"

class CMGAppWindow : public IMGAppWindow
{
  public:

    virtual              ~CMGAppWindow   ();
    virtual bool          Init           (unsigned hInst, int iWidth, int iHeight);
    virtual unsigned      GetHDC         () const { return (unsigned)m_hDC;  }
    virtual unsigned      GetHWnd        () const { return (unsigned)m_hWnd; }
    virtual void          ProcessMessages();
    virtual void          Flip           ();

  private:

    HDC			              m_hDC;
    HGLRC                 m_hRC;
    HWND                  m_hWnd;
};


static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
  {
    case WM_CLOSE:
      g_pMGApp->Close();
      return FALSE;
  	case WM_DESTROY:
			PostQuitMessage(0);
			return FALSE;
    case 0x215:
    case WM_SETFOCUS:
      break;
    case WM_MOVE:
    case WM_SIZE:
    case WM_ACTIVATEAPP:
      g_pMGApp->MustRepaint();
      return FALSE;
    case WM_SETCURSOR:
      SetCursor(LoadCursor(0,IDC_ARROW));
      return FALSE;
    case WM_MOUSEMOVE:
      g_pMGApp->OnMouseMove(lParam & 0xFFFF, lParam>>16);
      return FALSE;
    case WM_LBUTTONDOWN:
      SetCapture(hWnd);
      g_pMGApp->OnMouseDown(lParam & 0xFFFF, lParam>>16, 1);
      return FALSE;
    case WM_LBUTTONUP:
      ReleaseCapture();
      g_pMGApp->OnMouseUp(lParam & 0xFFFF, lParam>>16, 1);
      return FALSE;
    case WM_RBUTTONDOWN:
      SetCapture(hWnd);
      g_pMGApp->OnMouseDown(lParam & 0xFFFF, lParam>>16, 2);
      return FALSE;
    case WM_RBUTTONUP:
      ReleaseCapture();
      g_pMGApp->OnMouseUp(lParam & 0xFFFF, lParam>>16, 2);
      return FALSE;
    case 0x020A: //WM_MOUSEWHEEL:
    {
      POINT p;
      p.x = lParam & 0xFFFF;
      p.y = lParam>>16;
      ScreenToClient(hWnd, &p);
      if ((short)HIWORD(wParam) > 0)
        g_pMGApp->OnMouseDown(p.x, p.y, 4);
      else
        g_pMGApp->OnMouseDown(p.x, p.y, 5);
      return FALSE;
    }
    case WM_KEYDOWN:
      g_pMGApp->OnVKeyDown(wParam);
      return FALSE;
    case WM_KEYUP:
      g_pMGApp->OnVKeyUp(wParam);
      return FALSE;
    case WM_CHAR:
      g_pMGApp->OnChar((char)wParam);
      return FALSE;
	}
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
IMGAppWindow *IMGAppWindow::Create(unsigned hInst, int iWidth, int iHeight)
{
  CMGAppWindow *pAppWindow = NEW CMGAppWindow();
  if (!pAppWindow->Init(hInst, iWidth, iHeight))
    DISPOSE(pAppWindow);
  IMGAppWindow *pWin = pAppWindow;
  return pWin;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMGAppWindow::Init(unsigned hInst, int iWidth, int iHeight)
{
  int iPosX = (GetSystemMetrics(SM_CXSCREEN)-iWidth )>>1;
  int iPosY = (GetSystemMetrics(SM_CYSCREEN)-iHeight)>>1;

  WNDCLASS wc;
  wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WindowProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = (HINSTANCE)hInst;
  wc.hIcon         = NULL;
  wc.hCursor       = NULL; //LoadCursor(hInst, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszMenuName  = APP_NAME;
  wc.lpszClassName = APP_NAME;
  RegisterClass(&wc);

	RECT WindowRect;
	WindowRect.left  = 0;
	WindowRect.right = iWidth;
	WindowRect.top   = 0;
	WindowRect.bottom= iHeight;
  AdjustWindowRectEx(&WindowRect, WS_POPUP, FALSE, WS_EX_APPWINDOW);

  m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
                          APP_NAME,
                          APP_TITLE,
                          WS_POPUP,
								          0, 0,
								          WindowRect.right -WindowRect.left,
								          WindowRect.bottom-WindowRect.top,
                          NULL,
                          NULL,
                          (HINSTANCE)hInst,
                          NULL);

  if (!m_hWnd)
  {
    GLOG(("ERR: Can't create main window\n"));
		return false;
  }

	static PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |		// Format Must Support Window
		PFD_SUPPORT_OPENGL |		// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,				// Must Support Double Buffering
		PFD_TYPE_RGBA,					// Request An RGBA Format
		32,										  // Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

  m_hDC = GetDC(m_hWnd);
  GLuint PixelFormat = ChoosePixelFormat(m_hDC, &pfd);
  if (!SetPixelFormat(m_hDC, PixelFormat, &pfd))
  {
    return false;
  }
  m_hRC = wglCreateContext(m_hDC);
  wglMakeCurrent(m_hDC, m_hRC);

  return true;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
CMGAppWindow::~CMGAppWindow()
{
  if (m_hWnd)
    DestroyWindow(m_hWnd);
}


//---------------------------------------------------------------------------//
// ProcessMessages
//
//---------------------------------------------------------------------------//
void CMGAppWindow::ProcessMessages()
{
  MSG Msg;
  while (PeekMessage(&Msg, NULL, 0,0, PM_REMOVE))
  {
    TranslateMessage(&Msg);
	  DispatchMessage (&Msg);
  }
}


//---------------------------------------------------------------------------//
// Flip
//
//---------------------------------------------------------------------------//
void CMGAppWindow::Flip()
{
  SwapBuffers(m_hDC);
}
