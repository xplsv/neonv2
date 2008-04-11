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
#include "FXImagen.h"

enum
{
  FXS_IMAGE    = 0,
  STRETCH_FILL = 0,
  STRETCH_TOW  = 1,
  STRETCH_TOH  = 2,
};

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "StretchMode", false,  3, {"Fill", "ToWidth", "ToHeight"}},
  //{TCtrlVar::EDIT_BOX,  1, "PosIniX",     false,  0, {NULL}},
  //{TCtrlVar::EDIT_BOX,  2, "PosIniY",     false,  0, {NULL}},
  //{TCtrlVar::EDIT_BOX,  3, "SpeedX",      false,  0, {NULL}},
  //{TCtrlVar::EDIT_BOX,  4, "SpeedY",      false,  0, {NULL}},
  //{TCtrlVar::CHECK_BOX, 5, "WaveXform",   false,  0, {NULL}},
  //{TCtrlVar::SLIDER,    6, "XformX",      false,  0, {NULL}},  
  //{TCtrlVar::SLIDER,    7, "XformY",      false,  0, {NULL}},  
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXImagen::GetVarCtrls(int iScope)
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXImagen::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_Imagen.iStretchMode = *(int *)pData; break;
    case 1: if (pData) m_Imagen.vPosIni.x= (float)strtofloat((const char *)pData); break;
    case 2: if (pData) m_Imagen.vPosIni.y= (float)strtofloat((const char *)pData); break;
    case 3: if (pData) m_Imagen.vSpeed.x = (float)strtofloat((const char *)pData); break;
    case 4: if (pData) m_Imagen.vSpeed.y = (float)strtofloat((const char *)pData); break;
    case 5: m_Imagen.bWaveXform   = *(bool *)pData; break;
    case 6: m_Imagen.vXform.x = *(float *)pData; break;
    case 7: m_Imagen.vXform.y = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXImagen::GetVar(int iScope, int iObj, int iVar)
{
  static char pBuffer[100];
  switch (iVar)
  {
    case 0: return (&m_Imagen.iStretchMode);
    case 1: sprintf_s(pBuffer, 100, "%f", m_Imagen.vPosIni.x); return pBuffer;
    case 2: sprintf_s(pBuffer, 100, "%f", m_Imagen.vPosIni.y); return pBuffer;
    case 3: sprintf_s(pBuffer, 100, "%f", m_Imagen.vSpeed.x);  return pBuffer;
    case 4: sprintf_s(pBuffer, 100, "%f", m_Imagen.vSpeed.y);  return pBuffer;
    case 5: return (&m_Imagen.bWaveXform);
    case 6: return (&m_Imagen.vXform.x);
    case 7: return (&m_Imagen.vXform.y);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXImagen::Init(const string &sFile)
{
  m_bOk     = true;
  m_iImagen = 0;
  m_Nombre  = sFile;

  // Imagenes
  m_Imagen.iIDTex = g_pGestorMateriales->AddTextura(sFile.c_str());
  if (m_Imagen.iIDTex < 0)
    GLOG(("ERR: Can't load image file %s\n", sFile.c_str()));
  m_Imagen.iStretchMode = STRETCH_FILL;
  m_Imagen.vPos   = TVector2(0.f, 0.f);
  m_Imagen.vPosIni= TVector2(0.f, 0.f);
  m_Imagen.vSpeed = TVector2(0.f, 0.f);
  m_Imagen.vXform = TVector2(0.f, 0.f);

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXImagen::End()
{
  if (IsOk())
  {
    if (m_Imagen.iIDTex >= 0)
      g_pGestorMateriales->RemoveTextura(m_Imagen.iIDTex);
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXImagen::SetTime(float fTime)
{
  CEffect::SetTime(fTime);

  m_Imagen.vPos.x = m_Imagen.vPosIni.x + m_Imagen.vSpeed.x * m_fTime;
  m_Imagen.vPos.y = m_Imagen.vPosIni.y + m_Imagen.vSpeed.y * m_fTime;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXImagen::Draw(CDisplayDevice *pDD, int iTextureTgt)
{
  pDD->SetRenderTarget(iTextureTgt);

  // Si la imagen no se cargó bien, salimos
  if (m_Imagen.iIDTex < 0) return;

  CTextura *pTex = g_pGestorMateriales->GetTextura(m_Imagen.iIDTex);
  TVector2  vPos = m_Imagen.vPos;
  TVector2  vSize;

  // Tamaño
  int iW = pDD->ViewportWidth ();
  int iH = pDD->ViewportHeight();
  switch (m_Imagen.iStretchMode)
  {
    case STRETCH_FILL:
      vSize.x = (float)iW;
      vSize.y = (float)iH;
      break;
    case STRETCH_TOW:
      vSize.x = (float)iW;
      vSize.y = (float)iW * pTex->GetHeight() / (float)pTex->GetWidth();
      break;
    case STRETCH_TOH:
      vSize.x = (float)iH * pTex->GetWidth () / (float)pTex->GetHeight();
      vSize.y = (float)iH;
      break;
  }
  
  // Quad con textura
  LPDIRECT3DDEVICE9 pD3D = pDD->GetD3DDevice();
  g_pGestorMateriales->SetTextura(m_Imagen.iIDTex, 0);

  // RenderStates especificos
  pDD->ApplyBasicShader();
  pDD->SetBilinearFiltering(false,false);

  DrawQuad(pDD, TVector2(vPos.x, vPos.y), TVector2(vPos.x+vSize.x, vPos.y+vSize.y));
}
