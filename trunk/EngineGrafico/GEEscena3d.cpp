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

#include "Base.h"
#include "GEEngineGrafico.h"
#include "GEEscena3D.h"
#include "GEObjeto3D.h"
#include "GECamara3D.h"
#include "GEMotor3D.h"
#include "GELuz3D.h"
#include "GESceneLoader.h"
#include "CtrlVar.h"

static TCtrlVar s_VarsFog[] =
{
  {TCtrlVar::CHECK_BOX, 0, "Enable Fog", true, 0, {NULL}},
  {TCtrlVar::SLIDER,    1, "Red",        true, 0, {NULL}},
  {TCtrlVar::SLIDER,    2, "Green",      true, 0, {NULL}},
  {TCtrlVar::SLIDER,    3, "Blue",       true, 0, {NULL}},
  {TCtrlVar::SLIDER,    4, "Near",       true, 0, {NULL}},
  {TCtrlVar::SLIDER,    5, "Far",        true, 0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarObjects
//
//---------------------------------------------------------------------------//
void CEscena3D::GetVarObjects(vector<string> &Objects, int iScope) const
{
  Objects.clear();
  switch (iScope)
  {
    case FXS_3DOBJECT: Objects = m_ObjectNames; break;
    case FXS_3DLIGHT:  Objects = m_LightNames;  break;
    case FXS_3DCAMERA: Objects = m_CameraNames; break;
    case FXS_3DFOG:    Objects.push_back("Fog"); break;
  }
}


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CEscena3D::GetVars(int iScope) const
{
  switch (iScope)
  {
    case FXS_3DOBJECT: return CObjeto3D::GetVars(); break;
    case FXS_3DLIGHT:  return CLuz3D   ::GetVars(); break;
    case FXS_3DCAMERA: return CCamara3D::GetVars(); break;
    case FXS_3DFOG:    return s_VarsFog;            break;
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CEscena3D::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iScope)
  {
    case FXS_3DOBJECT: m_pObjetos[iObj].SetVar(iVar, pData); break;
    case FXS_3DLIGHT:  m_pLuces[iObj].SetVar(iVar, pData);   break;
    case FXS_3DCAMERA: m_pCamaras[iObj].SetVar(iVar, pData); break;
    // Niebla
    case FXS_3DFOG:
      switch (iVar)
      {
        case 0: m_bFog      = *(bool  *)pData; break;
        case 1: m_FogColorR = *(float *)pData; break;
        case 2: m_FogColorG = *(float *)pData; break;
        case 3: m_FogColorB = *(float *)pData; break;
        case 4: m_FogNear   = *(float *)pData; break;
        case 5: m_FogFar    = *(float *)pData; break;
      }
      break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CEscena3D::GetVar(int iScope, int iObj, int iVar)
{
  switch (iScope)
  {
    case FXS_3DOBJECT: return m_pObjetos[iObj].GetVar(iVar);
    case FXS_3DLIGHT:  return m_pLuces[iObj].GetVar(iVar);
    case FXS_3DCAMERA: return m_pCamaras[iObj].GetVar(iVar);
    // Niebla
    case FXS_3DFOG:
      switch (iVar)
      {
        case 0: return(&m_bFog);
        case 1: return(&m_FogColorR);
        case 2: return(&m_FogColorG);
        case 3: return(&m_FogColorB);
        case 4: return(&m_FogNear);
        case 5: return(&m_FogFar);
      }
      break;
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CEscena3D::Init(const string &sFile)
{
  End();
  BorraVars();

  CFichero File;
  if (!File.Open(sFile.c_str(), "rb"))
    return false;
 
  // Leer todo el fichero en memoria
  unsigned uLength = File.GetLength();
  char *pData = NEW_ARRAY(char, uLength);
  File.Read (pData, 1, uLength);
  File.Close();

  // Load Scene
  if (CSceneLoader::Load(this, pData, uLength))
  {
    Reset(0.f);
    m_bOk = true;
  }
  else
  {
    GLOG(("ERR: Can't load 3D file %s\n", sFile.c_str()));
    LiberaVars();
  }

  DISPOSE_ARRAY(pData);

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CEscena3D::End()
{
  if (IsOk())
  {
    LiberaVars();
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// BorraVars
//
//---------------------------------------------------------------------------//
void CEscena3D::BorraVars()
{
  m_fFirstFrame    = 0.f;
  m_fLastFrame     = 0.f;
  m_fFrameSpeed    = 0.f;
  m_fTicksPerFrame = 0.f;
  m_fFPS           = 0.f;
  m_fAmbientR      = 0.f;
  m_fAmbientG      = 0.f;
  m_fAmbientB      = 0.f;
  m_iCamaraActiva  = 0;
  m_bFog           = false;
  m_FogColorR      = 0.f;
  m_FogColorG      = 0.f;
  m_FogColorB      = 0.f;
  m_FogNear        = 0.25f;
  m_FogFar         = 0.5f;
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CEscena3D::LiberaVars()
{
  for (int i = 0; i < m_uNumMateriales; i++)
    g_pGestorMateriales->RemoveMaterial(m_pMateriales[i]);
  DISPOSE_ARRAY(m_pMateriales);
  DISPOSE_ARRAY(m_ppSortObjetos);
  DISPOSE_ARRAY(m_pObjetos);
  DISPOSE_ARRAY(m_pCamaras);
  DISPOSE_ARRAY(m_pLuces);
}


//---------------------------------------------------------------------------//
// Reset
//
//---------------------------------------------------------------------------//
void CEscena3D::Reset(float fTime)
{
  SetTime(fTime);
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CEscena3D::SetTime(float fTime)
{
  m_fTime = fTime;
  // Aplicar a los objetos
  for (int i = 0; i < m_uNumObjetos; i++)
    m_pObjetos[i].SetTime(m_fTime * m_fFPS);
  // Aplicar a las camaras
  //for (i = 0; i < m_uNumCamaras; i++)
  m_pCamaras[m_iCamaraActiva].SetTime(m_fTime * m_fFPS);
}


//---------------------------------------------------------------------------//
// GetCamara
//
//---------------------------------------------------------------------------//
CCamara3D *CEscena3D::GetCamara(unsigned iCam)
{
  ASSERT (iCam < m_uNumCamaras);
  if (iCam < m_uNumCamaras)
    return &m_pCamaras[iCam];
  return NULL;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CEscena3D::Draw(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
 
  // RenderStates especificos
  pDD->ApplyBasicShader();

  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE  );
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE    );
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE    );
  pD3D->SetRenderState(D3DRS_CULLMODE,         D3DCULL_CCW);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR,    (255<<24));

  PreparaCamara  (pDD);
  PreparaLuces   (pDD);
  PreparaNiebla  (pDD);
  PreparaObjetos (pDD);
  DrawObjetos    (pDD);

  pD3D->SetRenderState(D3DRS_LIGHTING, FALSE);
}


//---------------------------------------------------------------------------//
// PreparaCamara
//
//---------------------------------------------------------------------------//
void CEscena3D::PreparaCamara(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  CCamara3D       *pCam = GetCamara(m_iCamaraActiva);
  D3DXVECTOR3 vFr(pCam->GetSrc()->x, pCam->GetSrc()->y, pCam->GetSrc()->z);
  D3DXVECTOR3 vAt(pCam->GetTgt()->x, pCam->GetTgt()->y, pCam->GetTgt()->z);
  D3DXVECTOR3 vUp(pCam->GetUp ()->x, pCam->GetUp ()->y, pCam->GetUp ()->z);
  D3DXMatrixLookAtLH(&m_MatrixView, &vFr, &vAt, &vUp);
  pD3D->SetTransform(D3DTS_VIEW, &m_MatrixView);
  D3DXMatrixPerspectiveFovLH(&m_MatrixProj, pCam->GetFov(), 4.f/3.f, 1.f, 2000.f);
  pD3D->SetTransform(D3DTS_PROJECTION, &m_MatrixProj);

  m_MatrixVP = m_MatrixView * m_MatrixProj;
  CMotor3D::SetRegisterData(CMotor3D::V_VIEW,           (float *)&m_MatrixView);
  CMotor3D::SetRegisterData(CMotor3D::V_PROJECTION,     (float *)&m_MatrixProj);
  CMotor3D::SetRegisterData(CMotor3D::V_VIEWPROJECTION, (float *)&m_MatrixVP);
  CMotor3D::SetRegisterData(CMotor3D::V_CAMERAPOSITION, (float *)pCam->GetSrc());
}


//---------------------------------------------------------------------------//
// PreparaLuces
//
//---------------------------------------------------------------------------//
void CEscena3D::PreparaLuces(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  // Si tiene lucas las activamos, sino desactivamos la iluminacion
  if (m_uNumLuces > 0)
  {
    pD3D->SetRenderState(D3DRS_LIGHTING, TRUE);
    for (int i = 0; i < m_uNumLuces; i++)
    {
      pD3D->SetLight   (i, m_pLuces[i].GetD3DLight());
      pD3D->LightEnable(i, true);
    }
  }
  else
  {
    pD3D->SetRenderState(D3DRS_LIGHTING, FALSE);
  }

  // Variables
  TVector3 vLight;
  if (m_uNumLuces > 0)
  {
    vLight.x = m_pLuces[0].GetD3DLight()->Position.x;
    vLight.y = m_pLuces[0].GetD3DLight()->Position.y;
    vLight.z = m_pLuces[0].GetD3DLight()->Position.z;
  }
  else
  {
    vLight = *GetCamara(m_iCamaraActiva)->GetSrc();
    vLight.x = -vLight.x;
    vLight.y = -vLight.y;
    vLight.z = -vLight.z;
  } 
  Vector_Unit(&vLight, &vLight);
  CMotor3D::SetRegisterData(CMotor3D::V_DIRECTION, (float *)&vLight);
}


//---------------------------------------------------------------------------//
// PreparaNiebla
//
//---------------------------------------------------------------------------//
void CEscena3D::PreparaNiebla(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  if (m_bFog)
  {
    pD3D->SetRenderState(D3DRS_FOGENABLE,      TRUE);
    pD3D->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
    pD3D->SetRenderState(D3DRS_FOGCOLOR, HARD_COLOR_ARGB(255, (char)(m_FogColorR * 255.0), (char)(m_FogColorG * 255.0), (char)(m_FogColorB * 255.0)));
    float fStart = m_FogNear * 2000.f;
    float fEnd   = m_FogFar  * 2000.f;
    pD3D->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    pD3D->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fStart));
    pD3D->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&fEnd));
    //fStart = m_FogNear * (1.f + m_FogFar * (5.f-1.f));
    //pD3D->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_EXP2);
    //pD3D->SetRenderState(D3DRS_FOGDENSITY,   *(DWORD *)(&fStart));
  }
  else
    pD3D->SetRenderState(D3DRS_FOGENABLE, FALSE);
}


//---------------------------------------------------------------------------//
// PreparaObjetos
// Los ordena por Z con un qsort
//
//---------------------------------------------------------------------------//
int ComparaObj(const void *p1, const void *p2)
{
  CObjeto3D const *pObj1 = *((CObjeto3D const **)p1);
  CObjeto3D const *pObj2 = *((CObjeto3D const **)p2);
  return (pObj1->GetZ() < pObj2->GetZ()) ? 1 : -1;
}

void CEscena3D::PreparaObjetos(CDisplayDevice *pDD)
{
  for (int i = 0; i < m_uNumObjetos; i++)
  {
    m_pObjetos[i].CalculaUV(m_MatrixView);
    m_pObjetos[i].CalculaZ(m_MatrixVP);
    m_ppSortObjetos[i] = &m_pObjetos[i];
  }
  qsort(m_ppSortObjetos, m_uNumObjetos, sizeof(CObjeto3D *), ComparaObj);
}


//---------------------------------------------------------------------------//
// DrawObjetos
//
//---------------------------------------------------------------------------//
void CEscena3D::DrawObjetos(CDisplayDevice *pDD)
{
  for (unsigned iObj = 0; iObj < m_uNumObjetos; iObj++)
    m_ppSortObjetos[iObj]->Draw(pDD);
}


//---------------------------------------------------------------------------//
// SetMateriales
//
//---------------------------------------------------------------------------//
void CEscena3D::SetMateriales(int *pMateriales, unsigned uCant)
{
  m_pMateriales    = pMateriales;
  m_uNumMateriales = uCant;
}


//---------------------------------------------------------------------------//
// SetObjetos
//
//---------------------------------------------------------------------------//
void CEscena3D::SetObjetos(CObjeto3D *pObjetos, unsigned uCant)
{
  m_pObjetos    = pObjetos;
  m_uNumObjetos = uCant;
  for (unsigned i = 0; i < m_uNumObjetos; i++)
    m_ObjectNames.push_back(m_pObjetos[i].GetName());
  // Para la ordenacion
  m_ppSortObjetos = NEW_ARRAY(CObjeto3D *, m_uNumObjetos);
}


//---------------------------------------------------------------------------//
// SetCamaras
//
//---------------------------------------------------------------------------//
void CEscena3D::SetCamaras(CCamara3D *pCamaras, unsigned uCant)
{
  m_pCamaras    = pCamaras;
  m_uNumCamaras = uCant;
  for (unsigned i = 0; i < m_uNumCamaras; i++)
    m_CameraNames.push_back(m_pCamaras[i].GetName());
}


//---------------------------------------------------------------------------//
// SetLuces
//
//---------------------------------------------------------------------------//
void CEscena3D::SetLuces(CLuz3D *pLuces, unsigned uCant)
{
  m_pLuces    = pLuces;
  m_uNumLuces = uCant;
  for (unsigned i = 0; i < m_uNumLuces; i++)
    m_LightNames.push_back(m_pLuces[i].GetName());
}
