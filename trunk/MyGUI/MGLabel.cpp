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
#include "MGLabel.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGLabel::CMGLabel(CMGControl *pParent, int iX, int iY, unsigned Align, int iFont, const string &sText) : CMGControl(pParent, iX,iY, 0,0)
{
  m_Font  = iFont;
  m_Align = Align;
  Set(sText);
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGLabel::~CMGLabel()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//
void CMGLabel::Set(const string &sText)
{
  m_Text  = sText;
  SetSize(g_pMGDD->Skin()->Font(m_Font)->TextLength(m_Text)+2, g_pMGDD->Skin()->Font(m_Font)->TextHeight());
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGLabel::DerivDraw()
{
  g_pMGDD->TextDraw(m_Font, Left(), Top(), 0,Height(), CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Text.c_str());
}
