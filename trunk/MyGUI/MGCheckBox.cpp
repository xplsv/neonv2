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
#include "MGCheckBox.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGCheckBox::CMGCheckBox(CMGControl *pParent, int iX, int iY, const string &sCaption, bool IconLeft, boost::function<void()> fOnChange) : CMGControl(pParent, iX, iY, 0, 0)
{
  m_OnChange = fOnChange;
  m_Checked  = false;
  m_IconLeft = IconLeft;
  m_iTextW   = 0;
  m_iTextH   = 0;
  m_Caption  = sCaption;
  if (m_Caption != "")
  {
    m_iTextW  = g_pMGDD->Skin()->Font(FONT_NORMAL)->TextLength(m_Caption);
    m_iTextH  = g_pMGDD->Skin()->Font(FONT_NORMAL)->TextHeight();
    SetSize(m_iTextW + 16, 20);
  }
  else
    SetSize(15, 20);
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGCheckBox::~CMGCheckBox()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// OnChange
//
//---------------------------------------------------------------------------//
void CMGCheckBox::OnChange()
{
  if (m_OnChange)
    m_OnChange();
  Repaint ();
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGCheckBox::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    iX-=Left();
    iY-=Top ();
    if (iX >= 0 && iX < Width() && iY >= 0 && iY < Height())
    {
      m_Checked = !m_Checked;
      OnChange();
    }
    SetFocus(true);
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGCheckBox::DerivDraw()
{
  int iX = Left();
  if (m_IconLeft)
  {
    if (m_Caption != "") g_pMGDD->TextDraw(FONT_NORMAL, Left() + 16, Top() + ((Height()-m_iTextH)>>1), 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Caption);
  }
  else
  {
    if (m_Caption != "") g_pMGDD->TextDraw(FONT_NORMAL, Left() + 1,  Top() + ((Height()-m_iTextH)>>1), 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Caption);
    iX+=m_iTextW+2;

  }
  if (m_Checked)
  {
    if (m_Enabled)
      g_pMGDD->DrawItem(TRect(26,30,13,13), TRect(iX, Top()+4, 13,13));
    else
      g_pMGDD->DrawItem(TRect(39,30,13,13), TRect(iX, Top()+4, 13,13));
  }
  else
  {
    if (m_Enabled)
      g_pMGDD->DrawItem(TRect( 0,30,13,13), TRect(iX, Top()+4, 13,13));
    else
      g_pMGDD->DrawItem(TRect(13,30,13,13), TRect(iX, Top()+4, 13,13));
  }
}
