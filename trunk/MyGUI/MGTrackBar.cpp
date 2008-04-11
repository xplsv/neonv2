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
#include "MGTrackBar.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGTrackBar::CMGTrackBar(CMGControl *pParent, int iX,int iY, int iSize, bool Vertical, boost::function0<void> fOnChange) : CMGControl(pParent, iX,iY,0,0)
{
  m_OnChange = fOnChange;
  m_Vertical = Vertical;
  m_Pressed  = false;
  m_Pos      = 0.f;
  if (Vertical)
    SetSize(16,iSize);
  else
    SetSize(iSize,16);
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGTrackBar::~CMGTrackBar()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//
void CMGTrackBar::Update()
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
int CMGTrackBar::GetPosRealTB() const
{
  if (m_Vertical)
    return (int)(Top()  + (m_Pos * (Height()-12)));
  else
    return (int)(Left() + (m_Pos * (Width() -12)));
}


//---------------------------------------------------------------------------//
// UpdateFromTB
//
//---------------------------------------------------------------------------//
void CMGTrackBar::UpdateFromTB(int Pos)
{
  if (m_Vertical)
    m_Pos = ((float)(Pos - Top ()) / (float)(Height()-12));
  else
    m_Pos = ((float)(Pos - Left()) / (float)(Width ()-12));
  m_Pos = MIN(MAX(m_Pos, 0.f), 1.f);
  if (m_OnChange)
    m_OnChange();
  Repaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGTrackBar::OnMouseMove(int iX, int iY)
{
  if (m_Pressed)
  {
    if (iX < 0) iX = 0;
    if (iY < 0) iY = 0;
    if (m_Vertical)
      UpdateFromTB(iY-6);
    else
      UpdateFromTB(iX-6);
  }
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGTrackBar::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    int pos = GetPosRealTB();
    if (m_Vertical)
      m_Pressed = (iY >= pos && iY < (pos+12));
    else
      m_Pressed = (iX >= pos && iX < (pos+12));
    // Si no le hemos dado al TB ponemos la Pos donde hemos clicado y dejamos apretado
    if (!m_Pressed)
    {
      if (m_Vertical)
        UpdateFromTB(iY-6);
      else
        UpdateFromTB(iX-6);
      m_Pressed = true;
    }
    Focus();
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGTrackBar::OnMouseUp(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    if (m_Pressed)
    {
      if (m_Vertical)
        UpdateFromTB(iY-6);
      else
        UpdateFromTB(iX-6);
      Update();
    }
    else
      Repaint();
    m_Pressed = false;
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGTrackBar::DerivDraw()
{
  int pos  = GetPosRealTB();
  int posb = m_Pressed ? 170 : 150;
  g_pMGDD->FillRect(TRect(Left(), Top(), Width(), Height()), g_pMGDD->Skin()->Color(COL_BASE_BG));
  if (m_Vertical)
  {
    g_pMGDD->Rect(TRect(Left()+6, Top(),   4, Height()  ), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
    g_pMGDD->Rect(TRect(Left()+7, Top()+1, 2, Height()-2), g_pMGDD->Skin()->Color(COL_EDIT_BG));
    g_pMGDD->DrawItem(TRect(posb,76, 16,12), TRect(Left(),(int)pos,16,12));
  }
  else
  {
    g_pMGDD->Rect(TRect(Left(),   Top()+6, Width(),   4), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
    g_pMGDD->Rect(TRect(Left()+1, Top()+7, Width()-2, 2), g_pMGDD->Skin()->Color(COL_EDIT_BG));
    g_pMGDD->DrawItem(TRect(posb,60, 12,16), TRect((int)pos,Top(), 12,16));
  }
}
