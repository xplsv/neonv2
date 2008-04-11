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
#include "FXEscena3D.h"
#include "GEEscena3D.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "Camera",        true, 0, {NULL}},
  {TCtrlVar::CHECK_BOX, 1, "Clear",         true, 0, {NULL}},
  {TCtrlVar::SLIDER,    2, "AlphaBack",     true, 0, {NULL}},
  {TCtrlVar::SLIDER,    3, "Red",           true, 0, {NULL}},
  {TCtrlVar::SLIDER,    4, "Green",         true, 0, {NULL}},
  {TCtrlVar::SLIDER,    5, "Blue",          true, 0, {NULL}},
  {TCtrlVar::CHECK_BOX, 6, "Motion Blur",   true, 0, {NULL}},
  {TCtrlVar::SLIDER,    7, "Motion Intens", true, 0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarScopes
//
//---------------------------------------------------------------------------//
void CFXEscena3D::GetVarScopes(vector<string> &Scopes) const
{
  Scopes.clear();
  Scopes.push_back("Objects");
  Scopes.push_back("Lights");
  Scopes.push_back("Cameras");
  Scopes.push_back("Fog");
}


//---------------------------------------------------------------------------//
// GetVarScopes
//
//---------------------------------------------------------------------------//
void CFXEscena3D::GetVarObjects(vector<string> &Objects, int iScope) const
{
  m_pEscena3D->GetVarObjects(Objects, iScope);
}


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXEscena3D::GetVarCtrls(int iScope)
{
  if (iScope == FXSCOPE_MAIN)
    return s_Vars;
  else
    return m_pEscena3D->GetVars(iScope);
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXEscena3D::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  if (iScope == FXSCOPE_MAIN)
  {
    switch (iVar)
    {
      case 0: m_pEscena3D->SetCamara(*(int *)pData); break;
      case 1: m_bClear           = *(bool  *)pData; break;
      case 2: m_fAlphaBackground = *(float *)pData; break;
      case 3: m_fR               = *(float *)pData; break;
      case 4: m_fG               = *(float *)pData; break;
      case 5: m_fB               = *(float *)pData; break;
      case 6: m_bMotionBlur      = *(bool  *)pData; break;
      case 7: m_fMotionIntens    = *(float *)pData; break;
    }
  }
  else
    m_pEscena3D->SetVar(iScope, iObj, iVar, pData);
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXEscena3D::GetVar(int iScope, int iObj, int iVar)
{
  static int iCam;
  if (iScope == FXSCOPE_MAIN)
  {
    switch (iVar)
    {
      case 0: iCam = m_pEscena3D->GetCamaraActiva(); return (&iCam);
      case 1: return (&m_bClear);
      case 2: return (&m_fAlphaBackground);
      case 3: return (&m_fR);
      case 4: return (&m_fG);
      case 5: return (&m_fB);
      case 6: return (&m_bMotionBlur);
      case 7: return (&m_fMotionIntens);
    }
  }
  else
    return m_pEscena3D->GetVar(iScope, iObj, iVar);

  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXEscena3D::Init(const string &sFile)
{
  m_bOk       = true;
  m_pEscena3D = NEW CEscena3D;
  if (!m_pEscena3D->Init(sFile))
  {
    DISPOSE(m_pEscena3D);
    GLOG(("ERR: Can't load 3D Scene from file %s\n", sFile.c_str()));
    m_bOk = false;
  }
  else
  {
    m_fAlphaBackground = m_fR = m_fG = m_fB = 0.f;
    m_bClear           = true;
    m_bMotionBlur      = false;
    m_fMotionIntens    = 0.f;
    m_fLength = m_pEscena3D->GetLength();
    // Rellenar el ComboBox de las camaras
    s_Vars[0].iNumOptions=m_pEscena3D->GetNumCamaras();
    for (int i = 0; i < m_pEscena3D->GetNumCamaras(); i++)
      strncpy(s_Vars[0].pszOptions[i], m_pEscena3D->GetCamaraNames()[i].c_str(), 16);
    if (m_pEscena3D)
      m_pEscena3D->SetTime(0.f);
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXEscena3D::End()
{
  if (IsOk())
  {
    DISPOSE(m_pEscena3D);
  }
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CFXEscena3D::SetTime(float fTime)
{
  CEffect::SetTime(fTime);
  if (m_pEscena3D)
    m_pEscena3D->SetTime(fTime);
}



//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXEscena3D::Draw(CDisplayDevice *pDD, int iTextureTgt)
{
  float fSinTime = sinf(m_fTime);
  CMotor3D::SetRegisterData(CMotor3D::V_TIME,    &m_fTime);
  CMotor3D::SetRegisterData(CMotor3D::V_SINTIME, &fSinTime);

  pDD->SetRenderTarget(iTextureTgt);
  if (m_bFirstClear || (m_bClear && !m_bMotionBlur))
  {
    pDD->Clear(true, true, HARD_COLOR_ARGB((uchar)(m_fAlphaBackground * 255.f), (uchar)(m_fR * 255.f), (uchar)(m_fG * 255.f), (uchar)(m_fB * 255.f)), 1.f);
    m_bFirstClear = false;
  }
  else
  {
    // Guardar el frame del motion blur
    if (m_bMotionBlur)
      CEffects::DoMotionBlurPass1(pDD, iTextureTgt);
    pDD->Clear(false, true, 0, 1.f);
  }

  if (m_pEscena3D)
    m_pEscena3D->Draw(pDD);

  // Aplicar el frame anterior con la intensidad que toca
  if (m_bMotionBlur)
    CEffects::DoMotionBlurPass2(pDD, iTextureTgt, m_fMotionIntens * 0.4f + 0.55f);
}
