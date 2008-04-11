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

#include "NeonPch.h"
#include "MGAppMain.h"
#include "MGGrid.h"
#include "MGEffect.h"


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//  
void CMGGridItemFX::Init(bool bMaster)
{
  m_Master = bMaster;
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//  
void CMGGridItemFX::Update()
{
  if (m_Effect)
    m_Effect->Update();
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//  
void CMGGridItemFX::Draw(int x, int y, int w, int h, bool Selected)
{
  if (m_Master)
  {
    if (!m_Effect)
    {
      // No effect loaded
      g_pMGDD->DrawItem(TRect(151,120,69,80), TRect(x,y,69,80));
      return;
    }
    if (Selected)
      g_pMGDD->DrawItem(TRect( 82,165,69,15), TRect(x,y,69,15));
    else
      g_pMGDD->DrawItem(TRect( 82,150,69,15), TRect(x,y,69,15));

    g_pMGDD->SetClipRect(TRect(x+1,y+1,56,15));
    g_pMGDD->TextDraw(FONT_MINI, x+2, y+2, 0,11, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT_ACTIVE), m_Effect->GetFXName());
    g_pMGDD->SetClipRect();

    if (Selected)
      g_pMGDD->Rect(TRect(x,y+15,69,65), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE));
    // Dibujar el FX
    g_pMGDD->Stretch(m_Effect->GetBitmap(), TRect(x+1,y+16,67,63));
  }
  else
  {
    // NORMAL GRID
    if (!m_Effect) return;
    
    if (m_Effect->IsAttached())
      g_pMGDD->DrawItem(TRect(82,135,69,15), TRect(x,y,69,15));
    else
      g_pMGDD->DrawItem(TRect(82,120,69,15), TRect(x,y,69,15));

    g_pMGDD->SetClipRect(TRect(x+1,y+1,56,15));
    if (m_Effect->IsAttached())
      g_pMGDD->TextDraw(FONT_MINI, x+2, y+2, 0,11, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Effect->GetFXName());
    else
      g_pMGDD->TextDraw(FONT_MINI, x+2, y+2, 0,11, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT_ACTIVE), m_Effect->GetFXName());
    g_pMGDD->SetClipRect();

    if (Selected)
      g_pMGDD->Rect(TRect(x,y+15,69,61), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE));
    // Dibujar el FX
    g_pMGDD->Stretch(m_Effect->GetBitmap(), TRect(x+1,y+16,67,59));
  }
}
