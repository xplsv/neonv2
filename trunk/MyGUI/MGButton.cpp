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
#include "MGButton.h"

struct TButtonInfo
{
  int w;
  int h;
  int y;
  int x;
  int xfocus;
  int xpress;
  int xlink;
  int xlinkfocus;
  int xlinkpress;
};

static TButtonInfo s_InfoBitmaps[CMGButton::MAX_BUTTONS] =
{
  {75,25,  0,  0,75,150,   0,75,150},
  {75,25,  0,  0,75,150,   0,75,150},
  {20,20,120,  0, 0, 20,  40,40, 60},
  {25,25, 90, 75,75,100,  75,75,100},
  {40,20,140,  0, 0, 40,  40,40, 40},
};

//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGButton::CMGButton(CMGControl *pParent, int iX, int iY, int Type, const string &sCaption, boost::function<void()> fOnClick) : CMGControl(pParent, iX,iY, 0,0)
{
  m_Type    = Type;
  m_Linked  = false;
  m_Pressed = false;
  m_OnClick = fOnClick;
  m_Caption = sCaption;
  SetSize(s_InfoBitmaps[m_Type].w, s_InfoBitmaps[m_Type].h);
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGButton::~CMGButton()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGButton::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    m_Pressed = true;
    Focus();
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGButton::OnMouseUp(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    if (m_Pressed && m_OnClick && IsInside(iX, iY))
        m_OnClick();
    m_Pressed = false;
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGButton::DerivDraw()
{
  int x = 0;
  if (m_Linked)
  {
    if (m_Pressed)
      x = s_InfoBitmaps[m_Type].xlinkpress;
    else if (IsFocused())
      x = s_InfoBitmaps[m_Type].xlinkfocus;
    else
      x = s_InfoBitmaps[m_Type].xlink;
  }
  else
  {
    if (m_Pressed)
      x = s_InfoBitmaps[m_Type].xpress;
    else if (IsFocused())
      x = s_InfoBitmaps[m_Type].xfocus;
    else
      x = s_InfoBitmaps[m_Type].x;
  }
  g_pMGDD->DrawItem(TRect(x,s_InfoBitmaps[m_Type].y, Width(),Height()), TRect(Left(), Top(), Width(), Height()));
  if (m_Caption != "")
  {
    g_pMGDD->TextDraw(FONT_NORMAL, Left() + (m_Pressed ? 1 : 0), Top() + (m_Pressed ? 1 : 0), Width(), Height(), CMGFont::HCENTER, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_BUTTON), m_Caption);
  }
}
