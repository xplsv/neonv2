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

#include "NeonPch.h"
#include "GEEngineGrafico.h"
#include "Launcher.h"
#include "AppOptions.h"
#include "Resource.h"
#include <windows.h>
#include <d3d9.h>

static CLauncher *s_Launcher = NULL;

//---------------------------------------------------------------------------//
// LauncherProc
//
//---------------------------------------------------------------------------//  
static INT_PTR CALLBACK LauncherProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  if (s_Launcher)
    return s_Launcher->OnMessage(hWnd, Msg, wParam, lParam);
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}


//---------------------------------------------------------------------------//
// Launcher
//
//---------------------------------------------------------------------------//  
bool CLauncher::Show(unsigned hInstance)
{
   s_Launcher = this;
   m_Close    = false;
   m_hWnd     = CreateDialog((HINSTANCE)hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, LauncherProc);
   m_D3D      = Direct3DCreate9(D3D_SDK_VERSION);
   m_EnumeratingMonitors = false;
   InitData();
   ShowWindow(m_hWnd, SW_SHOW);
   do
   {
      MSG Msg;
      while (PeekMessage(&Msg, NULL, 0,0, PM_REMOVE))
      {
        TranslateMessage(&Msg);
	      DispatchMessage (&Msg);
      }
   } while(!m_Close);
   DestroyWindow(m_hWnd);
   SaveData();
   HARD_RELEASE(m_D3D);
   return m_Launch;
}


//---------------------------------------------------------------------------//
// InitData
//
//---------------------------------------------------------------------------//  
void CLauncher::InitData()
{
  char pBuffer[1000];

  // Load vars
  m_Device      = IAppOptions::GetInstance()->Get("Device",      0);
  m_Width       = IAppOptions::GetInstance()->Get("Width",       800);
  m_Height      = IAppOptions::GetInstance()->Get("Height",      600);
  m_Mode        = IAppOptions::GetInstance()->Get("Mode",        0);
  m_Fullscreen  = IAppOptions::GetInstance()->Get("Fullscreen",  0) != 0;
  m_Expand      = IAppOptions::GetInstance()->Get("Expand",      0) != 0;
  m_AutoCustTex = IAppOptions::GetInstance()->Get("AutoCustTex", 0) != 0;
  m_CustTex1.x  = IAppOptions::GetInstance()->Get("CustTex1W", 320);
  m_CustTex1.y  = IAppOptions::GetInstance()->Get("CustTex1H", 240);
  m_CustTex2.x  = IAppOptions::GetInstance()->Get("CustTex2W", 256);
  m_CustTex2.y  = IAppOptions::GetInstance()->Get("CustTex2H", 256);

  // Update controls
  sprintf_s(pBuffer, 100, "%d", m_Width);
  SendDlgItemMessage(m_hWnd, IDC_RES_W, WM_SETTEXT, 0L, (LPARAM)pBuffer);
  sprintf_s(pBuffer, 100, "%d", m_Height);
  SendDlgItemMessage(m_hWnd, IDC_RES_H, WM_SETTEXT, 0L, (LPARAM)pBuffer);
  CheckRadioButton  (m_hWnd, IDC_RADIOFULLSCREEN, IDC_RADIOWINDOW, m_Fullscreen ? IDC_RADIOFULLSCREEN : IDC_RADIOWINDOW);
  CheckDlgButton    (m_hWnd, IDC_EXPANDWINDOW, m_Expand ? 1 : 0);

  CheckDlgButton    (m_hWnd, IDC_AUTOCUSTTEX, m_AutoCustTex ? 1 : 0);
  // CustTex1
  sprintf_s(pBuffer, 100, "%d", m_CustTex1.x);
  SendDlgItemMessage(m_hWnd, IDC_CUST1_W, WM_SETTEXT, 0L, (LPARAM)pBuffer);
  sprintf_s(pBuffer, 100, "%d", m_CustTex1.y);
  SendDlgItemMessage(m_hWnd, IDC_CUST1_H, WM_SETTEXT, 0L, (LPARAM)pBuffer);
  // CustTex2
  sprintf_s(pBuffer, 100, "%d", m_CustTex2.x);
  SendDlgItemMessage(m_hWnd, IDC_CUST2_W, WM_SETTEXT, 0L, (LPARAM)pBuffer);
  sprintf_s(pBuffer, 100, "%d", m_CustTex2.y);
  SendDlgItemMessage(m_hWnd, IDC_CUST2_H, WM_SETTEXT, 0L, (LPARAM)pBuffer);

  // Devices
  m_nAdapters = m_D3D->GetAdapterCount();
  if (m_Device >= m_nAdapters) m_Device = 0;
  int items = 0;
  for (int i = 0; i < m_nAdapters; i++)
  {
    m_D3D->GetAdapterIdentifier(i, 0, &m_Adapters[i]);
    sprintf_s(pBuffer, 1000, "%s on %s", m_Adapters[i].Description, m_Adapters[i].DeviceName);
    SendDlgItemMessage(m_hWnd, IDC_CB_MONITOR, CB_ADDSTRING, 0L, (LPARAM)pBuffer);
    if (m_Device == i)
      SendDlgItemMessage(m_hWnd, IDC_CB_MONITOR, CB_SELECTSTRING, -1, (LPARAM)pBuffer);
  }
  // UpdateControls
  UpdateControls();
}


//---------------------------------------------------------------------------//
// UpdateControls
//
//---------------------------------------------------------------------------//  
void CLauncher::UpdateControls()
{
  EnableWindow(GetDlgItem(m_hWnd, IDC_CB_RESOLUTION),  m_Fullscreen);
  EnableWindow(GetDlgItem(m_hWnd, IDC_EXPANDWINDOW),  !m_Fullscreen);
  EnableWindow(GetDlgItem(m_hWnd, IDC_RES_W),         !m_Fullscreen);
  EnableWindow(GetDlgItem(m_hWnd, IDC_RES_H),         !m_Fullscreen);
  EnableWindow(GetDlgItem(m_hWnd, IDOK),               m_Width > 0 && m_Height > 0);
  EnableWindow(GetDlgItem(m_hWnd, IDC_CUST1_W),       !m_AutoCustTex);
  EnableWindow(GetDlgItem(m_hWnd, IDC_CUST1_H),       !m_AutoCustTex);
  EnableWindow(GetDlgItem(m_hWnd, IDC_CUST2_W),       !m_AutoCustTex);
  EnableWindow(GetDlgItem(m_hWnd, IDC_CUST2_H),       !m_AutoCustTex);
  UpdateModes ();
}


//---------------------------------------------------------------------------//
// UpdateRes
//
//---------------------------------------------------------------------------//  
void CLauncher::UpdateModes()
{
  char pBuffer[1000];
  if (m_Fullscreen)
  {
    SendDlgItemMessage(m_hWnd, IDC_CB_RESOLUTION, CB_RESETCONTENT, 0L, 0L);
    m_nModes = m_D3D->GetAdapterModeCount(m_Device, D3DFMT_X8R8G8B8);
    if (m_Mode >= m_nModes) m_Mode = 0;
    for (int i = 0; i < m_nModes; i++)
    {
      m_D3D->EnumAdapterModes(m_Device, D3DFMT_X8R8G8B8, i, &m_Modes[i]);
      sprintf_s(pBuffer, "%d x %d @ %dHz", m_Modes[i].Width, m_Modes[i].Height, m_Modes[i].RefreshRate);
      SendDlgItemMessage(m_hWnd, IDC_CB_RESOLUTION, CB_ADDSTRING, 0L, (LPARAM)pBuffer);
      if (i == m_Mode)
        SendDlgItemMessage(m_hWnd, IDC_CB_RESOLUTION, CB_SELECTSTRING, -1, (LPARAM)pBuffer);
    }
  }
}


//---------------------------------------------------------------------------//
// EnumMonitors
//
//---------------------------------------------------------------------------//  
BOOL CALLBACK MyEnumMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
  return s_Launcher->EnumMonitors(hMonitor, hdcMonitor, lprcMonitor, dwData);
}


//---------------------------------------------------------------------------//
// EnumMonitors
//
//---------------------------------------------------------------------------//  
BOOL CLauncher::EnumMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
  if (m_EnumeratingMonitors)
  {
    if (m_Monitor == hMonitor)
      m_MonitorRect = *lprcMonitor;
    else
      return true;
  }
  return false;
}


//---------------------------------------------------------------------------//
// SaveData
//
//---------------------------------------------------------------------------//  
void CLauncher::SaveData()
{
  m_EnumeratingMonitors = true;
  m_Monitor             = m_D3D->GetAdapterMonitor(m_Device);
  EnumDisplayMonitors(NULL, NULL, MyEnumMonitors, 0);
  m_EnumeratingMonitors = false;
  IAppOptions::GetInstance()->Set("Device",      m_Device);
  IAppOptions::GetInstance()->Set("Width",       m_Fullscreen ? m_Modes[m_Mode].Width  : m_Width);
  IAppOptions::GetInstance()->Set("Height",      m_Fullscreen ? m_Modes[m_Mode].Height : m_Height);
  IAppOptions::GetInstance()->Set("RLeft",       m_MonitorRect.left);
  IAppOptions::GetInstance()->Set("RTop",        m_MonitorRect.top);
  IAppOptions::GetInstance()->Set("RWidth",      m_Expand ? m_MonitorRect.right - m_MonitorRect.left : m_Width);
  IAppOptions::GetInstance()->Set("RHeight",     m_Expand ? m_MonitorRect.bottom - m_MonitorRect.top : m_Height);
  IAppOptions::GetInstance()->Set("Mode",        m_Mode);
  IAppOptions::GetInstance()->Set("Fullscreen",  m_Fullscreen  ? 1 : 0);
  IAppOptions::GetInstance()->Set("Expand",      m_Expand      ? 1 : 0);
  IAppOptions::GetInstance()->Set("AutoCustTex", m_AutoCustTex ? 1 : 0);
  if (m_AutoCustTex)
  {
    int w = IAppOptions::GetInstance()->Get("Width",  0);
    int h = IAppOptions::GetInstance()->Get("Height", 0);
    IAppOptions::GetInstance()->Set("CustTex1W", w>>1);
    IAppOptions::GetInstance()->Set("CustTex1H", h>>1);
    IAppOptions::GetInstance()->Set("CustTex2W", w>>2);
    IAppOptions::GetInstance()->Set("CustTex2H", h>>2);
  }
  else
  {
    IAppOptions::GetInstance()->Set("CustTex1W", m_CustTex1.x);
    IAppOptions::GetInstance()->Set("CustTex1H", m_CustTex1.y);
    IAppOptions::GetInstance()->Set("CustTex2W", m_CustTex2.x);
    IAppOptions::GetInstance()->Set("CustTex2H", m_CustTex2.y);
  }
}


//---------------------------------------------------------------------------//
// GetTBValue
//
//---------------------------------------------------------------------------//  
int CLauncher::GetTBValue(int iID)
{
  char pBuffer[10];
  int iLen = SendDlgItemMessage(m_hWnd, iID, WM_GETTEXTLENGTH, 0L, 0L);
  SendDlgItemMessage(m_hWnd, iID, WM_GETTEXT, MAX(iLen + 1,10), (LPARAM)pBuffer);
  try
  {
    return atoi(pBuffer);
  }
  catch(...)
  {
    return 0;
  }
}


//---------------------------------------------------------------------------//
// OnMessage
//
//---------------------------------------------------------------------------//  
INT_PTR CLauncher::OnMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  switch (Msg)
  {
    case WM_COMMAND:
      switch(LOWORD(wParam))
      {
        // Ok/Cancel
        case IDOK:     m_Launch = true;  m_Close = true; break;
        case IDCANCEL: m_Launch = false; m_Close = true; break;
        // Device
        case IDC_CB_MONITOR:
          if (HIWORD(wParam) == CBN_SELCHANGE)
          {
            m_Device = SendDlgItemMessage(m_hWnd, IDC_CB_MONITOR, CB_GETCURSEL, 0L, 0L);
            m_Mode   = 0;
            UpdateControls();
          }
          break;
        // Window
        case IDC_RADIOWINDOW:
          m_Fullscreen = false;
          UpdateControls();
          break;
        // Fullscreen
        case IDC_RADIOFULLSCREEN:
          m_Fullscreen = true;
          UpdateControls();
          break;
        // Resolution
        case IDC_CB_RESOLUTION:
          if (HIWORD(wParam) == CBN_SELCHANGE)
          {
            m_Mode  = SendDlgItemMessage(m_hWnd, IDC_CB_RESOLUTION, CB_GETCURSEL, 0L, 0L);
            UpdateControls();
          }
        // Width
        case IDC_RES_W:
          if (HIWORD(wParam) == EN_CHANGE)
          {
            m_Width = GetTBValue(IDC_RES_W);
            UpdateControls();
          }
          break;
        // Height
        case IDC_RES_H:
          if (HIWORD(wParam) == EN_CHANGE)
          {
            m_Height = GetTBValue(IDC_RES_H);
            UpdateControls();
          }
          break;
        // Expand
        case IDC_EXPANDWINDOW:
          m_Expand = IsDlgButtonChecked(m_hWnd, IDC_EXPANDWINDOW) != 0;
          UpdateControls();
          break;
        // AutoCustTex
        case IDC_AUTOCUSTTEX:
          m_AutoCustTex = IsDlgButtonChecked(m_hWnd, IDC_AUTOCUSTTEX) != 0;
          UpdateControls();
          break;
        // CustTex1 Width
        case IDC_CUST1_W:
          if (HIWORD(wParam) == EN_CHANGE)
          {
            m_CustTex1.x = GetTBValue(IDC_CUST1_W);
            if (!m_AutoCustTex) UpdateControls();
          }
          break;
        // CustTex1 Height
        case IDC_CUST1_H:
          if (HIWORD(wParam) == EN_CHANGE)
          {
            m_CustTex1.y = GetTBValue(IDC_CUST1_H);
            if (!m_AutoCustTex) UpdateControls();
          }
          break;
        // CustTex2 Width
        case IDC_CUST2_W:
          if (HIWORD(wParam) == EN_CHANGE)
          {
            m_CustTex2.x = GetTBValue(IDC_CUST2_W);
            if (!m_AutoCustTex) UpdateControls();
          }
          break;
        // CustTex2 Height
        case IDC_CUST2_H:
          if (HIWORD(wParam) == EN_CHANGE)
          {
            m_CustTex2.y = GetTBValue(IDC_CUST2_H);
            if (!m_AutoCustTex) UpdateControls();
          }
          break;
      }
      break;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
