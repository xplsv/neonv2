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
#include "MGSpinBox.h"
#include <stdio.h>


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGSpinBox::CMGSpinBox(CMGControl *pParent, int iX, int iY, int iSize, int iMin, int iMax, int iStep, boost::function0<void> fOnChange) : CMGControl(pParent, iX,iY,iSize,20)
{
  m_Value    = 0;
  m_Min      = iMin;
  m_Max      = iMax;
  m_Step     = iStep;
  m_OnChange = fOnChange;
  m_ButtonPressed = NONE;
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGSpinBox::~CMGSpinBox()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGSpinBox::OnMouseMove(int iX, int iY)
{
  if (m_ButtonPressed != NONE)
  {
    int iV = ((m_MouseY - iY) >> 3) * m_Step;
    Set(iV + m_CurrentValue);
  }
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGSpinBox::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    iX-=Left();
    iY-=Top ();
    if (iX > (Width()-12))
    {
      if (iY < 10)
      {
        Set(m_Value+m_Step);
        m_ButtonPressed = UP;
        m_MouseY = iY + Top();
        m_CurrentValue = GetValue();
      }
      else
      {
        Set(m_Value-m_Step);
        m_ButtonPressed = DOWN;
        m_MouseY = iY + Top();
        m_CurrentValue = GetValue();
      }
      Focus();
    }
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGSpinBox::OnMouseUp(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    m_ButtonPressed = NONE;
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//
void CMGSpinBox::Set(int iValue)
{
  int iOld = m_Value;
  m_Value  = iValue;
  if (m_Value < m_Min) m_Value = m_Min;
  if (m_Value > m_Max) m_Value = m_Max;
  if (iOld != m_Value)
  {
    if (m_OnChange)
      m_OnChange();
    Repaint();
  }
}        


//---------------------------------------------------------------------------//
// SetMinMax
//
//---------------------------------------------------------------------------//
void CMGSpinBox::SetMinMax(int iMin, int iMax)
{
  m_Min = iMin;
  m_Max = iMax;
  Set(m_Value);
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGSpinBox::DerivDraw()
{
  char pBuffer[32];
  sprintf_s(pBuffer, 32, "%d", m_Value);
  g_pMGDD->Rect       (TRect(Left(),   Top(),   Width(),   20), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
  g_pMGDD->FillRect   (TRect(Left()+1, Top()+1, Width()-2, 18), g_pMGDD->Skin()->Color(COL_EDIT_BG));
  g_pMGDD->SetClipRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2));
  g_pMGDD->TextDraw(FONT_NORMAL, Left()+3, Top(), 0,20, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT), pBuffer);

  // Up button
  if (m_ButtonPressed == UP)
    g_pMGDD->DrawItem(TRect(91,70,11,9), TRect(Left()+Width()-12,Top()+ 1,11,9));
  else
    g_pMGDD->DrawItem(TRect(80,70,11,9), TRect(Left()+Width()-12,Top()+ 1,11,9));
  // Down button
  if (m_ButtonPressed == DOWN)
    g_pMGDD->DrawItem(TRect(91,79,11,9), TRect(Left()+Width()-12,Top()+10,11,9));
  else
    g_pMGDD->DrawItem(TRect(80,79,11,9), TRect(Left()+Width()-12,Top()+10,11,9));

  g_pMGDD->SetClipRect();
}
