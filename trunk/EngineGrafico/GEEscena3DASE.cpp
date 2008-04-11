//---------------------------------------------------------------------------//
// File: GEEscena3D.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "GEEscena3D.h"
#include "GEObjeto3D.h"
#include "GECamara3D.h"
#include "GEMaterial.h"
#include "Ase.h"

#define  N3D_VERSION   (('1' << 0) | ('0' << 8) | ('N' << 16) | ('2' << 24))

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CEscena3D::Init(const char *pszFile)
{
  End();
  BorraVars();

  CFichero File;

  File.Open(pszFile);

  // Header
  unsigned uV;
  File.Read(&uV, 1, 4);
  if (uV != N3D_VERSION)
    goto LoadError;

  // Materiales
  m_ListaMateriales.Init();
  if (File.IntoChild("MATERIAL_LIST"))
  {
    LoadMaterials(&File);
    File.OutOfChild();
  }

  // Objetos
  File.ToBegin();
  m_ListaObjetos.Init();
  while (File.IntoChild("GEOMOBJECT"))
  {
    CObjeto3D *pObjeto3D = NEW CObjeto3D;
    if (pObjeto3D->LoadASE(&File))
      m_ListaObjetos.Inserta(pObjeto3D);
    else
      DISPOSE(pObjeto3D);
    File.OutOfChild();
  }

  // Camaras
  File.ToBegin();
  m_ListaCamaras.Init();
  while (File.IntoChild("CAMERAOBJECT"))
  {
    CCamara3D *pCamara3D = NEW CCamara3D;
    if (pCamara3D->LoadASE(&File, m_fLastFrame))
      m_ListaCamaras.Inserta(pCamara3D);
    else
      DISPOSE(pCamara3D);
    File.OutOfChild();
  }

  Reset(0.f);
  // Todo Ok!
  m_bOk = true;

LoadError:
  File.End();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CEscena3D::InitASE(const char *pszFile)
{
  /*
  End();
  BorraVars();

  int i;
  CAseFile File;
  if (!File.Init(pszFile))
    return false;

  // Firma ASCIIEXPORT 2.00
  File.IntoKey("3DSMAX_ASCIIEXPORT");
  sscanf(File.GetData(), "%d", &i);
  if (i != 200) goto LoadError;

  // Settings
  File.ToBegin();
  if (File.IntoChild("SCENE"))
  {
    if (File.IntoKey("SCENE_FIRSTFRAME"))
      sscanf(File.GetData(), "%f", &m_fFirstFrame);
    if (File.IntoKey("SCENE_LASTFRAME"))
      sscanf(File.GetData(), "%f", &m_fLastFrame);
    if (File.IntoKey("SCENE_FRAMESPEED"))
      sscanf(File.GetData(), "%f", &m_fFrameSpeed);
    if (File.IntoKey("SCENE_TICKSPERFRAME"))
      sscanf(File.GetData(), "%f", &m_fTicksPerFrame);
    m_fFPS        = m_fFrameSpeed * m_fTicksPerFrame;
    m_fFirstFrame*= m_fTicksPerFrame;
    m_fLastFrame *= m_fTicksPerFrame;
    File.OutOfChild();
  }
    
  // Materiales
  File.ToBegin();
  m_ListaMateriales.Init();
  if (File.IntoChild("MATERIAL_LIST"))
  {
    LoadMaterials(&File);
    File.OutOfChild();
  }

  // Objetos
  File.ToBegin();
  m_ListaObjetos.Init();
  while (File.IntoChild("GEOMOBJECT"))
  {
    CObjeto3D *pObjeto3D = NEW CObjeto3D;
    if (pObjeto3D->LoadASE(&File))
      m_ListaObjetos.Inserta(pObjeto3D);
    else
      DISPOSE(pObjeto3D);
    File.OutOfChild();
  }

  // Camaras
  File.ToBegin();
  m_ListaCamaras.Init();
  while (File.IntoChild("CAMERAOBJECT"))
  {
    CCamara3D *pCamara3D = NEW CCamara3D;
    if (pCamara3D->LoadASE(&File, m_fLastFrame))
      m_ListaCamaras.Inserta(pCamara3D);
    else
      DISPOSE(pCamara3D);
    File.OutOfChild();
  }

  Reset(0.f);
  // Todo Ok!
  m_bOk = true;

LoadError:
  File.End();
  */
  m_bOk = false;
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
  m_iCamaraActiva = 0;
  m_fFirstFrame   = 0.f;
  m_fLastFrame    = 0.f;
  m_fTicksPerFrame= 0.f;
  m_fFrameSpeed   = 0.f;
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CEscena3D::LiberaVars()
{
  if (IsOk())
  {
    VaciaListaConDispose(m_ListaObjetos);
    VaciaListaConDispose(m_ListaCamaras);
    //VaciaListaConDispose(m_ListaLuces);
  }
}


//---------------------------------------------------------------------------//
// Reset
//
//---------------------------------------------------------------------------//
void CEscena3D::Reset(float fTime)
{
  m_iCamaraActiva = 0;
  SetTime(fTime);
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CEscena3D::SetTime(float fTime)
{
  m_fTime = fTime;
  // Aplicar a las camaras
  CListaIter<CCamara3D *> Iter(m_ListaCamaras);
  while (!Iter.EsFinal())
  {
    CCamara3D *pCam = Iter;
    pCam->SetTime(m_fTime * m_fFPS);
    Iter++;
  }
}


//---------------------------------------------------------------------------//
// SetCamara
//
//---------------------------------------------------------------------------//
void CEscena3D::SetCamara(const char *pszNombre)
{
  CListaIter<CCamara3D *> Iter(m_ListaCamaras);
  int iCam = 0;
  while (!Iter.EsFinal())
  {
    CCamara3D *pCam = Iter;
    if (!Stricmp(pCam->GetNombre(), pszNombre))
    {
      m_iCamaraActiva = iCam;
      break;
    }
    iCam++;
    Iter++;
  }
  GLOG(("ERR: Can't find Camera %s\n", pszNombre));
}


//---------------------------------------------------------------------------//
// NextCamara
//
//---------------------------------------------------------------------------//
void CEscena3D::NextCamara()
{
  m_iCamaraActiva++;
  if (m_iCamaraActiva >= m_ListaCamaras.Cuenta())
    m_iCamaraActiva = 0;
}


//---------------------------------------------------------------------------//
// PrevCamara
//
//---------------------------------------------------------------------------//
void CEscena3D::PrevCamara()
{
  m_iCamaraActiva--;
  if (m_iCamaraActiva < 0)
    m_iCamaraActiva = m_ListaCamaras.Cuenta()-1;
}


//---------------------------------------------------------------------------//
// GetCamara
//
//---------------------------------------------------------------------------//
CCamara3D *CEscena3D::GetCamara(unsigned iCam)
{
  ASSERT (iCam < m_ListaCamaras.Cuenta());
  if (iCam < m_ListaCamaras.Cuenta())
  {
    CListaIter<CCamara3D *> Iter(m_ListaCamaras);
    for (unsigned i = 0; i < iCam; i++)
      Iter++;
    CCamara3D *pCam = Iter;
    return pCam;
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CEscena3D::Run(float fRunTime)
{
  SetTime(m_fTime+fRunTime);
  //GetCamara(m_iCamaraActiva)->Run(fRunTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CEscena3D::Draw(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
 
  // Posicion de la camara
  CCamara3D       *pCam = GetCamara(m_iCamaraActiva);
  TVector3  const *pTgt = pCam->GetTgt();
  TVector3  const *pSrc = pCam->GetSrc();
  D3DXMATRIX  matWorld;
  D3DXVECTOR3 vAt(pTgt->x, pTgt->y, pTgt->z);
  D3DXVECTOR3 vUp(0.f, 1.f, 0.f);
  D3DXVECTOR3 vFr(pSrc->x, pSrc->y, pSrc->z);
  D3DXMatrixLookAtLH(&matWorld, &vFr, &vAt, &vUp);
  pD3D->SetTransform(D3DTS_VIEW, &matWorld);
  D3DXMATRIX matProjection;
  D3DXMatrixPerspectiveFovLH(&matProjection, pCam->GetFov(), pDD->GetAspectRatio(), 1.0f, 20000.f);
  pD3D->SetTransform(D3DTS_PROJECTION,&matProjection);

  //_-----------------------------------------
  // LUCES
  D3DLIGHT9 Light;
  ZeroMemory(&Light,sizeof(Light));
  Light.Type = D3DLIGHT_DIRECTIONAL;
  Light.Diffuse.r = 1.0f;
  Light.Diffuse.g = 1.0f;
  Light.Diffuse.b = 1.0f;
  Light.Direction.x = 250.0f;
  Light.Direction.y = 0.0f;
  Light.Direction.z = -250.0f;
  Light.Range = 2000.0f;
  pD3D->SetLight(0,&Light);
  pD3D->LightEnable(0,false);

  // RenderStates especificos
  pD3D->SetRenderState(D3DRS_ZENABLE,          D3DZB_TRUE);
  pD3D->SetRenderState(D3DRS_ZWRITEENABLE,     D3DZB_TRUE);
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE     );
  pD3D->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE     );
  pD3D->SetRenderState(D3DRS_LIGHTING,         FALSE     );
  pD3D->SetRenderState(D3DRS_CLIPPING,         TRUE      );
  pD3D->SetRenderState(D3DRS_CULLMODE,         D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR,    (255<<24));

  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE    );

  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE    );

  pD3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

  // Renderizar todos los objetos en pantalla
  bool bFirst = true;
  CListaIter<CObjeto3D *> IterObj(m_ListaObjetos);
  while (!IterObj.EsFinal())
  {
    CObjeto3D *pObj = IterObj;

    CListaIter<TMaterial *> IterMat(m_ListaMateriales);
    while (!IterMat.EsFinal())
    {
      TMaterial *pMat = IterMat;
      if (pMat->iRef == pObj->GetMaterialRef())
      {
        ushort *pID = pObj->GetIDMateriales();

        // Calcular numero de indices con esta textura
        int iNumIndices = 0;
        for (int i = 0; i < pObj->GetNumIndices(); i++)
        {
          if (pID[i] == pMat->iID)
            iNumIndices++;
        }
        if (iNumIndices > 0)
        {
          // Sources
          TVector3 *pSrc = pObj->GetVertices();
          TVector2 *pUV  = pObj->GetUV();
          ushort   *pIndSrc = pObj->GetIndices();

          if (!pMat->pMat->TieneShader())
          {
            TVertex_HCV_XYZ_UV_N *pVertices = (TVertex_HCV_XYZ_UV_N *)pDD->LockVertexBuffer(HCV_XYZ_UV_N, iNumIndices);
            for (i = 0; i < pObj->GetNumIndices(); i++)
            {
              if (pID[i] == pMat->iID)
              {
                int iIndice = *pIndSrc;
                HCV_SET_XYZ   (pVertices, pSrc[iIndice].x, pSrc[iIndice].y, pSrc[iIndice].z);
                if (pUV)
                  HCV_SET_UV0(pVertices, pUV[i].x, pUV[i].y);
                TVector3 pN = pSrc[iIndice];
                Normalize(pN);
                HCV_SET_NORMAL(pVertices, pN.x, pN.y, pN.z);
                pVertices++;
              }
              pIndSrc++;
            }
            pDD->UnlockVertexBuffer(HCV_XYZ_UV_N);
            if (pMat->pMat)
              pMat->pMat->Set();

            pDD->DrawPrimitive(HCV_XYZ_UV_N, HARD_PRIM_TRIANGLELIST, iNumIndices/3);
          }
          else
          {
            TVertex_HCV_XYZ_C_UV_N *pVertices = (TVertex_HCV_XYZ_C_UV_N *)pDD->LockVertexBuffer(HCV_XYZ_C_UV_N, iNumIndices);
            TVector3 *pNormales = pObj->GetNormales();
            for (i = 0; i < pObj->GetNumIndices(); i++)
            {
              if (pID[i] == pMat->iID)
              {
                int iIndice = *pIndSrc;
                HCV_SET_XYZ   (pVertices, pSrc[iIndice].x, pSrc[iIndice].y, pSrc[iIndice].z);
                HCV_SET_COLOR (pVertices, HARD_COLOR_ARGB(255,255,255,255));
                HCV_SET_NORMAL(pVertices, pNormales[iIndice].x, pNormales[iIndice].y, pNormales[iIndice].z);
                if (pUV)
                  HCV_SET_UV0(pVertices, pUV[i].x, pUV[i].y);
                pVertices++;
              }
              pIndSrc++;
            }
            pDD->UnlockVertexBuffer(HCV_XYZ_C_UV_N);
        
             // No need world matrix since axis at origin
            D3DXMATRIX matWVP = matWorld * matProjection;
            D3DXMatrixTranspose( &matWVP, &matWVP );
            float p0[4] = {pCam->GetSrc()->x, pCam->GetSrc()->y, pCam->GetSrc()->z, 1.0f};
            pMat->pMat->Set();
            pD3D->SetVertexShaderConstantF(0, (float *)&matWVP, 4);
            pD3D->SetPixelShaderConstantF (4, (float *)p0,      1);
          
            pDD->DrawPrimitive(HCV_XYZ_C_UV_N, HARD_PRIM_TRIANGLELIST, iNumIndices/3);
          }
        }
      }
      IterMat++;
    }
    IterObj++;
  }
}


//---------------------------------------------------------------------------//
// LoadMateriales
//
//---------------------------------------------------------------------------//
void CEscena3D::LoadMaterials(CAseFile *pFile)
{
  int iRef = 0;
  while (pFile->IntoChild("MATERIAL"))
  {
    LoadMaterial(pFile, iRef, 0);
    pFile->OutOfChild();
    iRef++;
  }
}


//---------------------------------------------------------------------------//
// LoadMaterial
//
//---------------------------------------------------------------------------//
void CEscena3D::LoadMaterial(CAseFile *pFile, int iRef, int iID)
{
  // Material directo?
  if (pFile->IntoChild("MAP_DIFFUSE"))
  {
    if (pFile->IntoKey("BITMAP"))
    {
      char pNombre[256];
      sscanf(pFile->GetData(), "\"%s\"", pNombre);
      pNombre[strlen(pNombre)-1] = 0;
      TMaterial *pMat = NEW TMaterial;
      pMat->iID       = iID;
      pMat->iRef      = iRef;
      pMat->pMat      = NEW CMaterial;
      if (pMat->pMat->Init(pNombre))
        m_ListaMateriales.Inserta(pMat);
      else
      {
        DISPOSE(pMat->pMat);
        DISPOSE(pMat);
      }
    }
    pFile->OutOfChild();
  }
  else if (pFile->IntoKey("NUMSUBMTLS"))
  {
    int iNumSubMtls = 0;
    sscanf(pFile->GetData(), "%d", &iNumSubMtls);
    for (int i = 0; i < iNumSubMtls; i++)
    {
      if (pFile->IntoChild("SUBMATERIAL"))
      {
        LoadMaterial(pFile, iRef, i);
        pFile->OutOfChild();
      }
    }
  }
}


/*
s_fRotation+=0.01f;
D3DXMATRIX matRotationX,matRotationY,matTranslation;
if (bFirst)
{
  D3DXMatrixRotationX(&matRotationX, s_fRotation);
  D3DXMatrixRotationY(&matRotationY, s_fRotation * 0.75f);
  TVector3 const &vPos = pObj->GetPos();
  D3DXMatrixTranslation(&matTranslation, vPos.x, vPos.y, vPos.z);
  pD3D->SetTransform(D3DTS_WORLD, &(matTranslation * matRotationX * matRotationY));
  bFirst = false;
}
else
{
  TVector3 const &vPos = pObj->GetPos();
  D3DXMatrixTranslation(&matTranslation, vPos.x, vPos.y, vPos.z);
  pD3D->SetTransform(D3DTS_WORLD, &matTranslation);
  bFirst = false;
}
*/


// 2 pasadas
/*
pD3D->SetRenderState(D3DRS_CULLMODE,         D3DCULL_NONE);
pD3D->SetRenderState(D3DRS_ZENABLE,          D3DZB_FALSE );
pD3D->SetRenderState(D3DRS_LIGHTING,         FALSE       );
pD3D->SetRenderState(D3DRS_CLIPPING,         TRUE        );
pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR    );
pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB(255,255,255,255));
pD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
pDD->DrawPrimitive(HCV_XYZ_UV_N, HARD_PRIM_TRIANGLELIST, pObj->GetNumIndices()/3);
*/


/*
RenderStates de la escena
  // Lighting
  if (!Stricmp(SafeString(pParent->ToElement(), "LIGHTING", "TRUE"), "FALSE")
    pD3D->SetRenderState(D3DRS_LIGHTING, FALSE);
  else
    pD3D->SetRenderState(D3DRS_LIGHTING, TRUE);
  
  // ZEnable 
  if (!Stricmp(SafeString(pParent->ToElement(), "ZENABLE", "TRUE"), "FALSE")
    pD3D->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  else
    pD3D->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

  // ZWriteEnable
  if (!Stricmp(SafeString(pParent->ToElement(), "ZWRITEENABLE", "TRUE"), "FALSE")
    pD3D->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
  else
    pD3D->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

  FOG, etc
  */
