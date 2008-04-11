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
#include "MGEditBox.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGEditBox::CMGEditBox(CMGControl *pParent, int iX, int iY, int iSize, boost::function<void()> fOnChange) : CMGControl(pParent, iX,iY,iSize,20)
{
  m_Text     = "";
  m_OnChange = fOnChange;
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGEditBox::~CMGEditBox()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGEditBox::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    m_Pos = 0;
    iX-=Left()-3;
    for (unsigned i = 1; i < m_Text.length() && !m_Pos; i++)
    {
      if (GetPosFromChar(i) > iX)
        m_Pos = i-1;
    }
    SetFocus(true);
  }
}


//---------------------------------------------------------------------------//
// OnKeyDown
//
//---------------------------------------------------------------------------//
void CMGEditBox::OnChar(char c)
{
  if (IsFocused())
  {
    // 8   = BACKSPACE
    // 127 = DEL
    switch (c)
    {
      // Backspace
      case VK_BACK:
        if (m_Pos > 0)
        {
          m_Text.erase(m_Pos-1, 1);
          m_Pos--;
        }
        break;
      // Delete
      case VK_DELETE:
        if (m_Pos < (int)m_Text.length())
          m_Text.erase(m_Pos, 1);
        break;
      // Right cursor
      case VK_RIGHT:
        if (m_Pos < (int)m_Text.length())
          m_Pos++;
        break;
      // Left cursor
      case VK_LEFT:
        if (m_Pos > 0)
          m_Pos--;
        break;
      // Text
      default:
        m_Text.insert(m_Pos, string(1,c));
        m_Pos++;
        break;
    }
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// SetText
//
//---------------------------------------------------------------------------//
void CMGEditBox::SetText(const string &sText)
{
  m_Text = sText;
  if (m_OnChange)
    m_OnChange();
  Repaint();
}
         

//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGEditBox::DerivDraw()
{
  g_pMGDD->Rect    (TRect(Left(),   Top(),   Width(),   20), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
  g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, 18), g_pMGDD->Skin()->Color(COL_EDIT_BG));
  g_pMGDD->SetClipRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2));
  g_pMGDD->TextDraw(FONT_NORMAL, Left()+3, Top(), 0,20, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Text);
  if (IsFocused())
  {
    int x = Left()+GetPosFromChar(m_Pos);
    g_pMGDD->HLine(x-2, Top()+3,  4, g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
    g_pMGDD->HLine(x-2, Top()+18, 4, g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
    g_pMGDD->VLine(Left()+GetPosFromChar(m_Pos), Top()+2, 16, g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
  }
  g_pMGDD->SetClipRect();
}


//---------------------------------------------------------------------------//
// GetPosFromChar
//
//---------------------------------------------------------------------------//
int CMGEditBox::GetPosFromChar(int iChar)
{
  return (g_pMGDD->Skin()->Font(FONT_NORMAL)->TextLength(m_Text, iChar) + 3);
}
