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
#include "FilterGlow.h"

enum
{
  MIPMAP,
  PIXELSHADER,
  PSYCHO,
};

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "GlowType",      true, 3, {"MipMap", "Pixel (PS2.0)", "Psycho (PS2.0)"}},
  {TCtrlVar::SLIDER,    1, "GlowIntensity", true, 0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterGlow::GetVarCtrls()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterGlow::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_iType   = *(int   *)pData; break;
    case 1: m_fIntens = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterGlow::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_iType);
    case 1: return (&m_fIntens);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterGlow::Init(COptions &aOptions)
{
  m_Ok      = true;
  m_iType   = 1;
  m_fIntens = 1.0f;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterGlow::End()
{
  CFilter::End();
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CFilterGlow::SetTime(float fTime)
{
  CFilter::SetTime(fTime);
}


//---------------------------------------------------------------------------//
// Apply
//
//---------------------------------------------------------------------------//
bool CFilterGlow::Apply(CDisplayDevice *pDD, int iTexSrc, int iTexTgt)
{
  switch (m_iType)
  {
    case MIPMAP:      CEffects::DoGlowMipmap  (pDD, iTexSrc, m_fIntens); break;
    case PIXELSHADER: CEffects::DoGlowPS      (pDD, iTexSrc, m_fIntens); break;
    case PSYCHO:      CEffects::DoPsychoEffect(pDD, iTexSrc, m_fIntens); break;
  }
  return false;
}
