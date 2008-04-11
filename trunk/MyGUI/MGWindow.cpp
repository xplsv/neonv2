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
#include "MGWindow.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGWindow::CMGWindow(int iW, int iH, const string &sCaption) : CMGControl(NULL, 0,0, iW, iH)
{
  m_Moving  = false;
  m_Caption = sCaption;
  SetRect   (m_Border, 1,20,1,1);
  SetVisible(false);
  OnResize  ();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGWindow::~CMGWindow()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Show
//
//---------------------------------------------------------------------------//
void CMGWindow::Show(int iX, int iY, function<void (EModalResult eModalResult)> fOnModalResult)
{
  m_OnModalResult = fOnModalResult;
  g_pMGApp->PopupWindow(this, iX, iY);
}


//---------------------------------------------------------------------------//
// Close
//
//---------------------------------------------------------------------------//
void CMGWindow::Close(EModalResult eModalResult)
{
  g_pMGApp->PopupWindow(NULL);
  if (m_OnModalResult)
    m_OnModalResult(eModalResult);
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGWindow::OnMouseMove(int iX, int iY)
{
  if (m_Moving)
    UpdatePos(iX+m_MoveOffX,iY+m_MoveOffY);
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGWindow::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    m_MoveOffX = m_Rect.x - iX;
    m_MoveOffY = m_Rect.y - iY;
    m_AntX     = -1;
    m_AntY     = -1;
    m_Moving   = true;
    Focus();
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGWindow::OnMouseUp(int iX, int iY, int iButton)
{
  if (iButton == 1 && m_Moving)
  {
    m_Moving = false;
    g_pMGApp->MustRepaint();
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGWindow::DerivDraw()
{
  g_pMGDD->FillRect(TRect(Left(), Top(), Width(), 20), g_pMGDD->Skin()->Color(COL_BASE_BORDER));
  g_pMGDD->FillRect(TRect(Left(), Top()+20, Width(), Height()-20), g_pMGDD->Skin()->Color(COL_BASE_BG));
  g_pMGDD->Rect    (TRect(Left(), Top(), Width(), Height()), g_pMGDD->Skin()->Color(COL_BASE_BORDER));
  g_pMGDD->TextDraw(FONT_WINDOW_TITLE, Left()+2, Top(), 0,20, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_WINDOW_TITLE), m_Caption);
  //g_pMGDD->Draw    (m_SCaption, Left()+1, Top() + ((20-m_SCaption->h)>>1)); 
}


//---------------------------------------------------------------------------//
// UpdatePos
//
//---------------------------------------------------------------------------//
void CMGWindow::UpdatePos(int iX, int iY)
{
  iX = MAX(iX,0);
  iY = MAX(iY,0);
  iX = MIN(iX,(int)g_pMGApp->GetDisplay()->GetWidth ()-Width());
  iY = MIN(iY,(int)g_pMGApp->GetDisplay()->GetHeight()-Height());
  SetPos(iX,iY);
  //if (m_AntX != -1)
  //  g_pMGDD->XorRect(TRect(m_AntX,m_AntY,Width(),Height()), 0x8F8F8F);
  //m_AntX = iX;
  //m_AntY = iY;
  //g_pMGDD->XorRect(TRect(iX,iY,Width(),Height()), 0x8F8F8F);
  //Repaint();
  g_pMGApp->MustRepaint();
}
