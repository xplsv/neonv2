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
#include "MGGroupBox.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGGroupBox::CMGGroupBox(CMGControl *pParent, int iX, int iY, int iW, int iH, int Align) : CMGControl(pParent, iX, iY, iW, iH)
{
  m_Align = Align;
  m_Color = g_pMGDD->Skin()->Color(COL_BASE_BG);
  m_BorderColor = g_pMGDD->Skin()->Color(COL_BASE_BORDER);
  SetRect (m_Border, 2,2,2,2);
  OnResize();
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGGroupBox::CMGGroupBox(CMGControl *pParent, int iX, int iY, int iW, int iH, int Align, MGColor const &cBorderColor) : CMGControl(pParent, iX, iY, iW, iH)
{
  m_Align = Align;
  m_Color = g_pMGDD->Skin()->Color(COL_BASE_BG);
  m_BorderColor = cBorderColor;
  SetRect (m_Border, 2,2,2,2);
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGGroupBox::~CMGGroupBox()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGGroupBox::DerivDraw()
{
  g_pMGDD->Rect(TRect(Left()+1, Top()+1, Width()-2, Height()-2), m_BorderColor);
}
