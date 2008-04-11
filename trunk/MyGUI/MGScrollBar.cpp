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
#include "MGScrollBar.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGScrollBar::CMGScrollBar(CMGControl *pParent, bool bVertical, boost::function0<void> fOnChange) : CMGControl(pParent, 0,0, 15,15)
{
  m_OnChange = fOnChange;
  m_Align    = bVertical ? MGALIGN_RIGHT : MGALIGN_BOTTOM;
  m_Vertical = bVertical;
  m_Pressed  = NONE;
  SetPos  (0.f);
  OnResize();
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGScrollBar::CMGScrollBar(CMGControl *pParent, bool bVertical, int iX, int iY, int iSize, boost::function0<void> fOnChange) : CMGControl(pParent, iX,iY,0,0)
{
  m_OnChange = fOnChange;
  m_Vertical = bVertical;
  m_Pressed  = NONE;
  if (bVertical)
    SetSize(15,iSize);
  else
    SetSize(iSize,15);
  SetPos  (0.f);
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGScrollBar::~CMGScrollBar()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//
void CMGScrollBar::Update()
{
  m_Pos = MIN(MAX(m_Pos, 0.f), 1.f);
  if (m_OnChange)
    m_OnChange();
  Repaint();
}


//---------------------------------------------------------------------------//
// GetPosRealTB
//
//---------------------------------------------------------------------------//
int CMGScrollBar::GetPosRealTB() const
{
  if (m_Vertical)
    return (int)(Top()  + 15 + (m_Pos * (Height()-45)));
  else
    return (int)(Left() + 15 + (m_Pos * (Width() -45)));
}


//---------------------------------------------------------------------------//
// UpdateFromTB
//
//---------------------------------------------------------------------------//
void CMGScrollBar::UpdateFromTB(int Pos)
{
  if (m_Vertical)
    m_Pos = ((float)(Pos - Top()  - 15) / (float)(Height()-45));
  else
    m_Pos = ((float)(Pos - Left() - 15) / (float)(Width ()-45));
  m_Pos = MIN(MAX(m_Pos, 0.f), 1.f);
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGScrollBar::OnMouseMove(int iX, int iY)
{
  if (m_Pressed == TRACKBAR)
  {
    if (m_Vertical)
      UpdateFromTB(iY-7);
    else
      UpdateFromTB(iX-7);
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGScrollBar::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    int pos = GetPosRealTB();
    if (m_Vertical)
    {
      // Vertical
      if (iY < (Top()+15))
      {
        m_Pressed = LEFTTOP;
        m_Pos    -= 0.05f;
      }
      else if (iY >= (Top()+Height()-15))
      {
        m_Pressed = RIGHTBOTTOM;
        m_Pos    += 0.05f;
      }
      else //if (iY >= pos && iY < (pos+15))
      {
        m_Pressed = TRACKBAR;
        UpdateFromTB(iY-7);
      }
      Update();
    }
    else
    {
      // Horizontal
      if (iX < (Left()+15))
      {
        m_Pressed = LEFTTOP;
        m_Pos    -= 0.05f;
      }
      else if (iX >= (Left()+Width()-15))
      {
        m_Pressed = RIGHTBOTTOM;
        m_Pos    += 0.05f;
      }
      else //if (iX >= pos && iX < (pos+15))
      {
        m_Pressed = TRACKBAR;
        UpdateFromTB(iY-7);
      }
      Update();
    }
    SetFocus(true);
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGScrollBar::OnMouseUp(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    if (m_Pressed == TRACKBAR)
    {
      if (m_Vertical)
        UpdateFromTB(iY-7);
      else
        UpdateFromTB(iX-7);
      Update();
    }
    else
      Repaint();
    m_Pressed = NONE;
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGScrollBar::DerivDraw()
{
  int x, y, w, h;
  if (m_Vertical)
  {
    if (m_Pressed == LEFTTOP)
      g_pMGDD->DrawItem(TRect(95, 30, 15,15), TRect(Left(),Top(), 15,15));
    else 
      g_pMGDD->DrawItem(TRect(80, 30, 15,15), TRect(Left(),Top(), 15,15));
    if (m_Pressed == RIGHTBOTTOM)
      g_pMGDD->DrawItem(TRect(125,30, 15,15), TRect(Left(),Top()+Height()-15, 15,15));
    else
      g_pMGDD->DrawItem(TRect(110,30, 15,15), TRect(Left(),Top()+Height()-15, 15,15));
    x = Left();
    w = Width();
    y = Top()+15;
    h = Height()-30;
  }
  else
  {
    if (m_Pressed == LEFTTOP)
      g_pMGDD->DrawItem(TRect(95, 45, 15,15), TRect(Left(),Top(), 15,15));
    else 
      g_pMGDD->DrawItem(TRect(80, 45, 15,15), TRect(Left(),Top(), 15,15));
    if (m_Pressed == RIGHTBOTTOM)
      g_pMGDD->DrawItem(TRect(125,45, 15,15), TRect(Left()+Width()-15,Top(), 15,15));
    else
      g_pMGDD->DrawItem(TRect(110,45, 15,15), TRect(Left()+Width()-15,Top(), 15,15));
    x = Left()+15;
    w = Width()-30;
    y = Top();
    h = Height();
  }
  g_pMGDD->FillRect(TRect(x,y,w,h), g_pMGDD->Skin()->Color(COL_BASE_BG));
  int pos  = GetPosRealTB();
  int posb = m_Pressed == TRACKBAR ? 155 : 140;
  if (m_Vertical)
    g_pMGDD->DrawItem(TRect(posb,30, 15,15), TRect(Left(),(int)pos,15,15));
  else
    g_pMGDD->DrawItem(TRect(posb,30, 15,15), TRect((int)pos,Top(), 15,15));
}
