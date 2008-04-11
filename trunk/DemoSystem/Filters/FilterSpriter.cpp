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
#include "FilterSpriter.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::SLIDER, 0, "Min Size",   true,  0, {NULL}},
  {TCtrlVar::SLIDER, 1, "Max Size",   true,  0, {NULL}},
  {TCtrlVar::SLIDER, 2, "Resolution", true,  0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterSpriter::GetVarCtrls()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterSpriter::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_fMinSize    = *(float *)pData; break;
    case 1: m_fMaxSize    = *(float *)pData; break;
    case 2: m_fResolution = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterSpriter::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_fMinSize);
    case 1: return (&m_fMaxSize);
    case 2: return (&m_fResolution);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterSpriter::Init(COptions &aOptions)
{
  m_fMinSize    = 0.1f;
  m_fMaxSize    = 0.5f;
  m_fResolution = 0.5f;
  m_iSprite     = -1;
  const string &sFile = aOptions.Option("sprite");
  if (sFile != "")
  {
    m_iSprite = g_pGestorMateriales->AddTextura(sFile);
    if (m_iSprite >= 0)
    {
      m_Ok = true;
    }
    else
      GLOG(("Err: Can't load sprite %s for CFilterSpriter\n", sFile.c_str()));
  }
  else
    GLOG(("ERR: Can't find sprite tag for CFilterSpriter\n"));

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterSpriter::End()
{
  if (IsOk())
  {
    g_pGestorMateriales->RemoveTextura(m_iSprite);
    CFilter::End();
  }
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CFilterSpriter::SetTime(float fTime)
{
  CFilter::SetTime(fTime);
}


//---------------------------------------------------------------------------//
// Apply
//
//---------------------------------------------------------------------------//
bool CFilterSpriter::Apply(CDisplayDevice *pDD, int iTexSrc, int iTexTgt)
{
  CEffects::DoSpriter(pDD, iTexSrc, iTexTgt, m_iSprite, m_fResolution, m_fMinSize*20.f+1.f, m_fMaxSize*75.f+m_fMinSize);

  return true;
}
