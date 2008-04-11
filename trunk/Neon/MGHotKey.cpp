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

#include <NeonPch.h>
#include "MGAppMain.h"
#include "MGHotKey.h"
#include "KeyboardDevice.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGHotKey::CMGHotKey(CMGControl *pParent, int iX, int iY, int iSize, boost::function<void()> fOnChange) : CMGControl(pParent, iX,iY,iSize,20)
{
  m_OnChange = fOnChange;
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGHotKey::~CMGHotKey()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGHotKey::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    SetFocus(true);
  }
}


//---------------------------------------------------------------------------//
// OnChar
//
//---------------------------------------------------------------------------//
void CMGHotKey::OnChar(char c)
{
  c = toupper(c);
  if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z'))
  {
    m_Char = c;
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGHotKey::DerivDraw()
{
  string text = "None";
  if (m_Char >= 0)
  {
    text = "";
    //if (m_Ctrl ) text = "Ctrl + ";
    //if (m_Shift) text+= "Shift + ";
    text+= m_Char;
  }
  
  if (IsFocused())
  {
    g_pMGDD->Rect    (TRect(Left(),   Top(),   Width(),   20), g_pMGDD->Skin()->Color(COL_EDIT_BORDER_ACTIVE));
    g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, 18), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE));
    g_pMGDD->SetClipRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2));
    g_pMGDD->TextDraw   (FONT_NORMAL, Left()+3, Top(), 0,20, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT_ACTIVE), text);
    g_pMGDD->SetClipRect();
  }
  else
  {
    g_pMGDD->Rect    (TRect(Left(),   Top(),   Width(),   20), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
    g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, 18), g_pMGDD->Skin()->Color(COL_EDIT_BG));
    g_pMGDD->SetClipRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2));
    g_pMGDD->TextDraw   (FONT_NORMAL, Left()+3, Top(), 0,20, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT), text);
    g_pMGDD->SetClipRect();
  }
}
