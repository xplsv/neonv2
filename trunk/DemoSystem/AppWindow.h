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

#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "Base.h"
#include "GEDisplayDevice.h"

class  CAppWindow;
extern CAppWindow g_AppWindow;

class CAppWindow
{
  public:
                 CAppWindow      () { m_bOk = false; }
                ~CAppWindow      () { End(); }

    bool         Init            (TModoGrafico const &Modo, TRect const &rWindow, bool bCanMove, HINSTANCE hInst, HWND hWndMain, bool bShowMouse);
    void         End             ();
    bool         IsOk            () const { return m_bOk; }
    
    int          GetHandle       () const { return (int)m_hWnd; }
    bool         GetActive       () const { return m_bIsActive; }
    void         SetActive       (bool bActive) { m_bIsActive = bActive; }
    void         AdjustWindow    ();

    void         InputKey        (int iKey);
    void         ProcessMessages ();

    void         LButtonDown     (int iX, int iY);
    void         LButtonUp       (int iX, int iY);
    void         MouseMove       (int iX, int iY);
    void         SwitchFullScreen(bool bFullScreen);
    void         UpdateFullScreen();
    bool         IsFullScreen    () { return m_Modo.bFullScreen; }
    bool         ShowMouse       () { return m_bShowMouse; }
    
  private:

    bool         InitWindow      (HINSTANCE hInst, bool bFullscreen);
    void         BorraVars       ();
    void         LiberaVars      ();

  private:

    bool         m_bOk;
    bool         m_bIsActive;
    HINSTANCE    m_hInst;
    HWND         m_hWnd;
    HWND         m_hWndMain;
    TModoGrafico m_Modo;
    bool         m_bShowMouse;
    bool         m_bUpdateDevice;
    bool         m_bCanMove;
    bool         m_bMovingWindow;
    TRect        m_Window;
    int          m_iOldWindowPosX;
    int          m_iOldWindowPosY;
    int          m_iMousePosX;
    int          m_iMousePosY;
};

#endif
