//---------------------------------------------------------------------------//
// File: FXImagen.cpp
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

static const char *s_Scopes[] =
{
  "Image",
};

//---------------------------------------------------------------------------//
// GetVarScopes
//
//---------------------------------------------------------------------------//
void CFXImagen::GetVarScopes(vector<string> &Scopes) const
{
  Scopes.clear();
  Scopes.push_back("Image");
}


//---------------------------------------------------------------------------//
// GetVarObjects
//
//---------------------------------------------------------------------------//
void CFXImagen::GetVarObjects(vector<string> &Objects, int iScope) const
{
  Objects.clear();
  Objects = m_NombreImagenes;
}


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXImagen::GetVarCtrls(int iScope)
{
  if (iScope == FXS_IMAGE)
    return s_Vars;
  else
    return NULL;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXImagen::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  TImage *pImage = &m_pImagenes[iObj];
  if (iScope == FXS_IMAGE)
  {
    switch (iVar)
    {
      case 0: pImage->iStretchMode = *(int *)pData; break;
      case 1: if (pData) pImage->vPosIni.x= (float)strtofloat((const char *)pData); break;
      case 2: if (pData) pImage->vPosIni.y= (float)strtofloat((const char *)pData); break;
      case 3: if (pData) pImage->vSpeed.x = (float)strtofloat((const char *)pData); break;
      case 4: if (pData) pImage->vSpeed.y = (float)strtofloat((const char *)pData); break;
      case 5: pImage->bWaveXform   = *(bool *)pData; break;
      case 6: pImage->vXform.x = *(float *)pData; break;
      case 7: pImage->vXform.y = *(float *)pData; break;
    }
  }
  m_bUpdate = true;
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXImagen::GetVar(int iScope, int iObj, int iVar)
{
  TImage *pImage = &m_pImagenes[iObj];
  static char pBuffer[256];
  if (iScope == FXS_IMAGE)
  {
    switch (iVar)
    {
      case 0: return (&pImage->iStretchMode);
      case 1: sprintf(pBuffer, "%f", pImage->vPosIni.x); return pBuffer;
      case 2: sprintf(pBuffer, "%f", pImage->vPosIni.y); return pBuffer;
      case 3: sprintf(pBuffer, "%f", pImage->vSpeed.x);  return pBuffer;
      case 4: sprintf(pBuffer, "%f", pImage->vSpeed.y);  return pBuffer;
      case 5: return (&pImage->bWaveXform);
      case 6: return (&pImage->vXform.x);
      case 7: return (&pImage->vXform.y);
    }
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXImagen::Init(const string &sFile)
{
  m_bOk       = true;
  m_pImagenes = NULL;
  m_iImagen   = 0;
  m_bUpdate   = true;

  CFichero File;

  if ((sFile.length() > 4) && (sFile.find(".nil", 0) < sFile.length()))
  {
    if (File.Open(sFile.c_str(), "rb"))
    {
      int iLineas = 0;
      do
      {
        char pBuffer[MAX_PATH];
        File.ReadLn(pBuffer, MAX_PATH);
        iLineas++;
      } while (!File.Eof());
      // Volver al principio
      File.SeekToIni();
      // Guardar los nombres
      m_iNumImagenes = 0;
      m_pImagenes    = NEW_ARRAY(TImage, iLineas);
      m_NombreImagenes.clear();
      for (int i = 0; i < iLineas; i++)
      {
        char pBuffer[MAX_PATH];
        File.ReadLn(pBuffer, MAX_PATH);
        if ((pBuffer[strlen(pBuffer)-1] == 10) || (pBuffer[strlen(pBuffer)-1] == 13))
          pBuffer[strlen(pBuffer)-1] = 0;
        if ((pBuffer[strlen(pBuffer)-1] == 10) || (pBuffer[strlen(pBuffer)-1] == 13))
          pBuffer[strlen(pBuffer)-1] = 0;
        // Si es un fichero valido
        if (strlen(pBuffer) > 0)
        {
          m_NombreImagenes.push_back(pBuffer);
          m_iNumImagenes++;
        }
      }
      File.Close();
    }
    else
    {
      GLOG(("ERR: Can't open image list %s\n", sFile.c_str()));
    }
  }
  else
  {
    // Solo una imagen
    m_iNumImagenes   = 1;
    m_pImagenes      = NEW_ARRAY(TImage, m_iNumImagenes);
    m_NombreImagenes.push_back(sFile.c_str());
  }

  // Si hay alguna imagen, rula bien todo
  if (m_iNumImagenes > 0)
  {
    // Imagenes
    for (int i = 0; i < m_iNumImagenes; i++)
    {
      TImage *pImage = &m_pImagenes[i];
      pImage->iIDTex = g_pGestorMateriales->AddTextura(m_NombreImagenes[i].c_str());
      if (pImage->iIDTex < 0)
        GLOG(("ERR: Can't load image file %s\n", sFile.c_str()));
      pImage->iStretchMode = STRETCH_FILL;
      pImage->vPos   = TVector2(0.f, 0.f);
      pImage->vPosIni= TVector2(0.f, 0.f);
      pImage->vSpeed = TVector2(0.f, 0.f);
      pImage->vXform = TVector2(0.f, 0.f);
    }
  }
  else
  {
    GLOG(("ERR: No images found for an FXImage\n"));
    m_bOk = false;
  }

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
    for (int i = 0; i < m_iNumImagenes; i++)
    {
      if (m_pImagenes[i].iIDTex >= 0)
        g_pGestorMateriales->RemoveTextura(m_pImagenes[i].iIDTex);
    }
    DISPOSE_ARRAY(m_pImagenes);
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//
void CFXImagen::Attach()
{
  m_bUpdate = true;
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXImagen::SetTime(float fTime)
{
  CEffect::SetTime(fTime);

  m_pImagenes[m_iImagen].vPos.x = m_pImagenes[m_iImagen].vPosIni.x + m_pImagenes[m_iImagen].vSpeed.x * m_fTime;
  m_pImagenes[m_iImagen].vPos.y = m_pImagenes[m_iImagen].vPosIni.y + m_pImagenes[m_iImagen].vSpeed.y * m_fTime;

  if ((m_pImagenes[m_iImagen].vSpeed.x != 0.f) || (m_pImagenes[m_iImagen].vSpeed.y != 0.f))
    m_bUpdate = true;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXImagen::Draw(CDisplayDevice *pDD, int iTextureTgt)
{
  if (!m_bUpdate) return;

  TImage *pImage = &m_pImagenes[m_iImagen];

  pDD->SetRenderTarget(iTextureTgt);
  /*
  if (m_bClear || m_bFirstClear)
  {
    pDD->Clear(true, true, 0, 1.f);
    m_bFirstClear = false;
  }
  */
  
  // Si la imagen no se cargó bien, salimos
  if (pImage->iIDTex < 0) return;

  CTextura *pTex = g_pGestorMateriales->GetTextura(pImage->iIDTex);
  TVector2  vPos = pImage->vPos;
  TVector2  vSize;

  // Tamaño
  int iW = pDD->ViewportWidth ();
  int iH = pDD->ViewportHeight();
  switch (pImage->iStretchMode)
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
  g_pGestorMateriales->SetTextura(pImage->iIDTex, 0);

  // RenderStates especificos
  pDD->ApplyBasicShader();
  pDD->SetBilinearFiltering(false,false);

  DrawQuad(pDD, TVector2(vPos.x, vPos.y), TVector2(vPos.x+vSize.x, vPos.y+vSize.y));

  m_bUpdate = false;
}
