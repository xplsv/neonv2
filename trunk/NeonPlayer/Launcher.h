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

#ifndef _LAUNCHER_H
#define _LAUNCHER_H

#include "Base.h"
#include "GEEngineGrafico.h"
#include <windows.h>
#include <d3d9.h>

class CLauncher
{
  public:
    
    bool        Show          (unsigned hInstance);
    INT_PTR     OnMessage     (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    BOOL        EnumMonitors  (HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

  private:

    void        InitData      ();
    void        UpdateControls();
    void        UpdateModes   ();
    void        SaveData      ();
    int         GetTBValue    (int iID);
  private:

    enum
    {
      MAX_ADAPTERS = 10,
      MAX_MODES    = 1000,
    };
    HWND        m_hWnd;
    bool        m_Launch;
    bool        m_Close;
    IDirect3D9 *m_D3D;
    int         m_nAdapters;
    int         m_nModes;
    bool        m_EnumeratingMonitors;
    RECT        m_MonitorRect;
    HMONITOR    m_Monitor;
    D3DADAPTER_IDENTIFIER9 m_Adapters[MAX_ADAPTERS];
    D3DDISPLAYMODE         m_Modes[MAX_MODES];
    // Vars
    int         m_Device;
    int         m_Width;
    int         m_Height;
    int         m_Mode;
    bool        m_Fullscreen;
    bool        m_Expand;
    bool        m_AutoCustTex;
    TVectorI2   m_CustTex1;
    TVectorI2   m_CustTex2;
};

#endif
