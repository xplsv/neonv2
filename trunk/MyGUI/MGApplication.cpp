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
#include "MGApplication.h"
#include "MGAppWindow.h"
#include "MGDisplay.h"
#include "MGControl.h"
#include "MGWindow.h"
#include <gl/gl.h>

#include <commdlg.h>

CMGApplication *g_pMGApp;

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMGApplication::Init(unsigned hInstance, int w, int h)
{
  g_pMGApp       = this;
  m_hInst        = hInstance;
  m_MovingWindow = false;
  m_PosMouseX    = 0;
  m_PosMouseY    = 0;
  m_WinX         = 0;
  m_WinY         = 0;
  m_Repaint      = true;
  m_Flip         = true;
  m_bWantClose   = false;
  m_DragAccepted = false;
  m_pItemRoot    = NULL;
  m_pItemFocused = NULL;
  m_pItemTop     = NULL;
  m_pItemDragged = NULL;
  m_pItemPopup   = NULL;
  // AppWindow
  m_pAppWindow = IMGAppWindow::Create(m_hInst, 1024,768);
  if (!m_pAppWindow)
    return false;
  m_hWnd = m_pAppWindow->GetHWnd();
  m_hDC  = m_pAppWindow->GetHDC ();
  // Display
  m_pDisplay = NEW CMGDisplay;
  if (!m_pDisplay->Init(m_hInst, 0,0, w,h, 32))
  {
    DISPOSE(m_pAppWindow);
    DISPOSE(m_pDisplay);
    return false;
  }

  return true;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMGApplication::End()
{
  DISPOSE(m_pAppWindow);
  DISPOSE(m_pItemRoot);
  DISPOSE(m_pDisplay);
}


//---------------------------------------------------------------------------//
// MoveMainWindow
//---------------------------------------------------------------------------//
void CMGApplication::MoveMainWindow(int x, int y)
{
  m_MovingWindow = true;
  m_PosMouseX    = x;
  m_PosMouseY    = y;
}


//---------------------------------------------------------------------------//
// OnResize
//---------------------------------------------------------------------------//
void CMGApplication::OnResize(int w, int h)
{
  m_pDisplay->OnResize(w, h);
  if (m_pItemRoot)
    m_pItemRoot->OnResize();
  MustRepaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//---------------------------------------------------------------------------//
void CMGApplication::OnMouseMove(int x, int y)
{
  // If moving main window, handle special case
  if (m_MovingWindow)
  {
      m_WinX += x - m_PosMouseX;
      m_WinY += y - m_PosMouseY;
      MoveWindow((HWND)m_pAppWindow->GetHWnd(), m_WinX,m_WinY,1024,768, true);
      return;
  }

  // Other cases, treat normally
  if (m_pItemTop)
  {
    CMGControl *pItem = m_pItemTop->GetItemAt(x, y);
    if (!pItem)
      pItem = m_pItemTop;
    pItem->OnMouseMove(x, y);
    if (m_pItemFocused && (m_pItemFocused != pItem))
      m_pItemFocused->OnMouseMove(x, y);
  }
  else
  {
    CMGControl *pItem = GetItemAt(x, y);
    if (pItem)
    {
      pItem->OnMouseMove(x, y);
      if (m_pItemDragged)
      {
        pItem->OnDragOver(m_pItemDragged, x, y, m_DragAccepted);
        if (m_DragAccepted)
          ::SetCursor(m_pDisplay->Skin()->Cursor(CUR_DRAGDROP));
        else
          ::SetCursor(m_pDisplay->Skin()->Cursor(CUR_DRAGDROP_NO));
      }
    }
    if (m_pItemFocused && (pItem != m_pItemFocused))
      m_pItemFocused->OnMouseMove(x, y);
  }
}


//---------------------------------------------------------------------------//
// OnMouseDown
//---------------------------------------------------------------------------//
void CMGApplication::OnMouseDown(int x, int y, int button)
{
  if (m_pItemTop)
  {
    CMGControl *pItem = m_pItemTop->GetItemAt(x, y);
    if (!pItem)
      pItem = m_pItemTop;
    pItem->OnMouseDown(x, y, button);
    if (m_pItemFocused && (m_pItemFocused != pItem) && m_pItemFocused->IsInside(x, y))
      m_pItemFocused->OnMouseDown(x, y, button);
  }
  else
  {
    CMGControl *pItem = GetItemAt(x, y);
    if (pItem)
      pItem->OnMouseDown(x, y, button);
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//---------------------------------------------------------------------------//
void CMGApplication::OnMouseUp(int x, int y, int button)
{
  // If moving main window, handle special case
  if (m_MovingWindow)
  {
    m_MovingWindow = false;
    m_WinX += x - m_PosMouseX;
    m_WinY += y - m_PosMouseY;
    MoveWindow((HWND)m_pAppWindow->GetHWnd(), m_WinX,m_WinY,1024,768, true);
    return;
  }

  // Other cases, treat normally
  if (m_pItemTop)
  {
    CMGControl *pItem = m_pItemTop->GetItemAt(x, y);
    if (!pItem)
      pItem = m_pItemTop;
    pItem->OnMouseUp(x, y, button);
    //if (m_pItemFocused && (m_pItemFocused != pItem))
    //  m_pItemFocused->OnMouseUp(x, y, button);
  }
  else
  {
    if (m_pItemDragged)
    {
      if (m_DragAccepted)
      {
        CMGControl *pItem = GetItemAt(x, y);
        if (pItem)
          pItem->OnDragDrop(m_pItemDragged, x, y);
      }
      DragItem(NULL);
    }
    else
    {
      if (m_pItemFocused)
        m_pItemFocused->OnMouseUp(x, y, button);
    }
  }
}


//---------------------------------------------------------------------------//
// OnVKeyDown
//---------------------------------------------------------------------------//
void CMGApplication::OnVKeyDown(int vkey)
{
  if (m_pItemFocused)
    m_pItemFocused->OnVKeyDown(vkey);
}


//---------------------------------------------------------------------------//
// OnVKeyUp
//---------------------------------------------------------------------------//
void CMGApplication::OnVKeyUp(int vkey)
{
  if (m_pItemFocused)
    m_pItemFocused->OnVKeyUp(vkey);
}


//---------------------------------------------------------------------------//
// OnChar
//---------------------------------------------------------------------------//
void CMGApplication::OnChar(char c)
{
  if (m_pItemFocused)
    m_pItemFocused->OnChar(c);
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//
void CMGApplication::Update()
{
  // Repaint if needed
  m_pAppWindow->ProcessMessages();
  if (m_Repaint)
  {
    m_RepaintItems.Vacia();
    Repaint();
    Flip   ();
  }
  else if (m_RepaintItems.Cuenta() > 0)
  {
    //CListaIter<CMGControl *> Iter(m_RepaintItems);
    //while (!Iter.EsFinal())
    //{
    //  CMGControl *pControl = Iter;
    //  CMGControl *pTop = pControl->GetTop();
    //  if (pTop == m_pItemRoot || pTop == m_pItemPopup)
    //    pControl->Draw();
    //  Iter++;
    //}
    m_RepaintItems.Vacia();
    Repaint(); // TEMP
    Flip();
  }
}


//---------------------------------------------------------------------------//
// Repaint
//
//---------------------------------------------------------------------------//
void CMGApplication::Repaint()
{
  m_Repaint = false;
  g_pMGDD->FillRect(TRect(0, 0, m_pDisplay->GetWidth(), m_pDisplay->GetHeight()), g_pMGDD->Skin()->Color(COL_BASE_BG));
  if (m_pItemRoot)
    m_pItemRoot->Draw();
  if (m_pItemPopup)
    m_pItemPopup->Draw();
  if (m_pItemTop)
    m_pItemTop->Draw();
}


//---------------------------------------------------------------------------//
// Flip
//
//---------------------------------------------------------------------------//
void CMGApplication::Flip()
{
  //m_pDisplay->Flip();
  m_pAppWindow->Flip();
  m_Flip = false;
}


//---------------------------------------------------------------------------//
// IsControlVisible
//
//---------------------------------------------------------------------------//
bool CMGApplication::IsControlVisible(CMGControl *pControl)
{
  return (pControl != m_pItemPopup && pControl != m_pItemRoot);
}


//---------------------------------------------------------------------------//
// GetItemAt
//
//---------------------------------------------------------------------------//
CMGControl *CMGApplication::GetItemAt(int iX, int iY)
{
  CMGControl *pItem = NULL;
  if (m_pItemPopup) pItem = m_pItemPopup->GetItemAt(iX,iY);
  if (!pItem)       pItem = m_pItemRoot ->GetItemAt(iX,iY);
  return pItem;
}


//---------------------------------------------------------------------------//
// SetRootItem
//
//---------------------------------------------------------------------------//
void CMGApplication::SetRootItem(CMGControl *pItem)
{
  m_pItemRoot = pItem;
}


//---------------------------------------------------------------------------//
// FocusItem
//
//---------------------------------------------------------------------------//
void CMGApplication::FocusItem(CMGControl *pItem)
{
  if (m_pItemFocused)
  {
    RepaintItem(m_pItemFocused);
    if (pItem != m_pItemFocused)
      m_pItemFocused->SetFocus(false);
  }
  m_pItemFocused = pItem;
}


//---------------------------------------------------------------------------//
// TopItemF
//
//---------------------------------------------------------------------------//
void CMGApplication::TopItem(CMGControl *pItem)
{
  if (m_pItemTop)
    RepaintItem(m_pItemTop);
  m_pItemTop = pItem;
}


//---------------------------------------------------------------------------//
// RepaintItem
//
//---------------------------------------------------------------------------//
void CMGApplication::RepaintItem(CMGControl *pItem)
{
  if (!m_RepaintItems.EstaInsertado(pItem))
    m_RepaintItems.Inserta(pItem);
  //m_Repaint = true;
}


//---------------------------------------------------------------------------//
// DragItem
//
//---------------------------------------------------------------------------//
void CMGApplication::DragItem(CMGControl *pItem)
{
  m_pItemDragged = pItem;
  m_DragAccepted = false;
  ::SetCursor(pItem ? m_pDisplay->Skin()->Cursor(CUR_DRAGDROP_NO) : LoadCursor(0, IDC_ARROW));
}


//---------------------------------------------------------------------------//
// PopupWindow
//
//---------------------------------------------------------------------------//
void CMGApplication::PopupWindow(CMGWindow *pItem, int iX, int iY)
{
  if (!m_pItemPopup ^ !pItem)
  {
    if (iX != -1 || iY != -1)
    {
      if (iX + pItem->Width () > 1024) iX = 1024 - pItem->Width ();
      if (iY + pItem->Height() >  768) iY =  768 - pItem->Height();
      pItem->SetPos(iX,iY);
    }
    if (m_pItemPopup) m_pItemPopup->SetVisible(false);
    m_pItemPopup = pItem;
    if (m_pItemPopup) m_pItemPopup->SetVisible(true);
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// OpenFileOpen
// Open file for reading
//---------------------------------------------------------------------------//
bool CMGApplication::OpenFileOpen(const char *pTitle, const char *pInitialDir, const char *pWildcard, string &rFile)
{
  rFile = "";

  PushDir(".");

  char pBuffer[1000];
  pBuffer[0] = 0;
  OPENFILENAME ofn;
  memset (&ofn, 0, sizeof (ofn));
  ofn.lStructSize     = sizeof(OPENFILENAME);
  ofn.hInstance       = (HINSTANCE)m_hInst;
  ofn.hwndOwner       = (HWND)m_hWnd;
  ofn.lpstrInitialDir = pInitialDir;
  ofn.lpstrFilter     = pWildcard;
  ofn.lpstrFile       = pBuffer;
  ofn.nMaxFile        = MAX_PATH;
  ofn.lpstrTitle      = pTitle;
  ofn.nFilterIndex    = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON;
  if (GetOpenFileName(&ofn))
    rFile = ofn.lpstrFile;

  PopDir();

  return rFile != "";
}


//---------------------------------------------------------------------------//
// OpenFileSave
// Open file for saving
//---------------------------------------------------------------------------//
bool CMGApplication::OpenFileSave(const char *pTitle, const char *pInitialDir, const char *pWildcard, const char *pDefExt, string &rFile)
{
  rFile = "";

  PushDir(".");

  char pBuffer[MAX_PATH];
  pBuffer[0] = 0;
  OPENFILENAME ofn;
  memset (&ofn, 0, sizeof (ofn));
  ofn.lStructSize     = sizeof(OPENFILENAME);
  ofn.hInstance       = (HINSTANCE)m_hInst;
  ofn.hwndOwner       = (HWND)m_hWnd;
  ofn.lpstrInitialDir = pInitialDir;
  ofn.lpstrFilter     = pWildcard;
  ofn.lpstrFile       = pBuffer;
  ofn.nMaxFile        = MAX_PATH;
  ofn.lpstrTitle      = pTitle;
  ofn.lpstrDefExt     = pDefExt;
  ofn.nFilterIndex    = 1;
  ofn.Flags = OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
  if (GetSaveFileName(&ofn))
    rFile = ofn.lpstrFile;

  PopDir();

  return rFile != "";
}
