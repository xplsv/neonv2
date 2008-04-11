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

#include "DemoSystemPch.h"
#include "FXColor.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::SLIDER, 0, "Red",    false,  0, {NULL}},
  {TCtrlVar::SLIDER, 1, "Green",  false,  0, {NULL}},
  {TCtrlVar::SLIDER, 2, "Blue",   false,  0, {NULL}},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXColor::GetVarCtrls(int iScope)
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXColor::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_fRed   = *(float *)pData; break;
    case 1: m_fGreen = *(float *)pData; break;
    case 2: m_fBlue  = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXColor::GetVar(int iScope, int iObj, int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_fRed);
    case 1: return (&m_fGreen);
    case 2: return (&m_fBlue);
  }
  return NULL;
}

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXColor::Init(const string &sFile)
{
  m_bOk    = true;
  m_fRed   = 1.f;
  m_fGreen = 1.f;
  m_fBlue  = 1.f;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXColor::End()
{
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXColor::Draw(CDisplayDevice *pDD, int iTextureTgt)
{
  unsigned uColor = HARD_COLOR_ARGB(255,int(m_fRed * 255.f),int(m_fGreen * 255.f),int(m_fBlue * 255.f));
  pDD->SetRenderTarget(iTextureTgt);
  pDD->Clear(true, false, uColor, 0.f);
}
