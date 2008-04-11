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
#include "GEObjeto3D.h"
#include "GEMotor3D.h"
#include "GETextura.h"
#include "GEMaterial.h"
#include "GEGestorMateriales.h"
#include "CtrlVar.h"
#include "PathLinear.h"

enum
{
  RENDER_NORMAL = 0,
  RENDER_ENVMAP = 1,
  RENDER_ENVMAPFLAT  = 2,
  RENDER_POINTSPRITE = 3,
};

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "RenderMode",     false,  4, {"Normal", "EnvMap", "EnvMapFlat", "PointSprite"}},
  {TCtrlVar::CHECK_BOX, 1, "WaveXform",      false,  0, {NULL}},
  {TCtrlVar::COMBO_BOX, 2, "XformChan",      false,  6, {"Global", "1", "2", "3", "4", "5"}},
  {TCtrlVar::COMBO_BOX, 3, "MorphMode",      false,  5, {"None", "Rotate", "Sin Morph", "Sin Y Morph", "Tentacle"}},
  {TCtrlVar::SLIDER,    4, "MorphScale",     false,  0, {NULL}},
  {TCtrlVar::SLIDER,    5, "MorphSpeed",     true,   0, {NULL}},
  {TCtrlVar::SLIDER,    6, "MorphX",         true,   0, {NULL}},
  {TCtrlVar::SLIDER,    7, "MorphY",         true,   0, {NULL}},
  {TCtrlVar::SLIDER,    8, "MorphZ",         true,   0, {NULL}},
  {TCtrlVar::CHECK_BOX, 9, "Update Normals", false,  0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CObjeto3D::GetVars()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case  0: m_iRenderMode    = *(int   *)pData; break;
    case  1: m_bWaveXform     = *(bool  *)pData; break;
    case  2: m_iWaveXformChan = *(int   *)pData; break;
    case  3: m_iMorphMode     = *(int   *)pData; break;
    case  4: m_fMorphScale    = *(float *)pData; break;
    case  5: m_fMorphSpeed    = *(float *)pData; break;
    case  6: m_fMorphX        = *(float *)pData; break;
    case  7: m_fMorphY        = *(float *)pData; break;
    case  8: m_fMorphZ        = *(float *)pData; break;
    case  9: m_bRecalcNormals = *(bool  *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void *CObjeto3D::GetVar(int iVar)
{
  static char pBuffer[256];
  switch (iVar)
  {
    case  0: return (&m_iRenderMode);
    case  1: return (&m_bWaveXform);
    case  2: return (&m_iWaveXformChan);
    case  3: return (&m_iMorphMode);
    case  4: return (&m_fMorphScale);
    case  5: return (&m_fMorphSpeed);
    case  6: return (&m_fMorphX);
    case  7: return (&m_fMorphY);
    case  8: return (&m_fMorphZ);
    case  9: return (&m_bRecalcNormals);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CObjeto3D::Init(float fNumFrames)
{
  End();
  BorraVars();

  m_fNumFrames = fNumFrames;
  m_bOk        = true;
  
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CObjeto3D::End()
{
  if (IsOk())
  {
    DISPOSE_ARRAY(m_pFaces);
    DISPOSE      (m_pPathVertices);
    DISPOSE_ARRAY(m_pVertices);
    DISPOSE      (m_pUV);
    DISPOSE      (m_pPathUV);
    DISPOSE      (m_pPathPosiciones);
    DISPOSE      (m_pPathRotaciones);
    DISPOSE      (m_pPathEscalados);
    DISPOSE_ARRAY(m_pCacheUV);
    DISPOSE_ARRAY(m_pCacheUVEnvMap);
    DISPOSE_ARRAY(m_pCacheUVEnvMapFlat);
    DISPOSE_ARRAY(m_pCacheVertices);
    DISPOSE_ARRAY(m_pCacheFNormales);
    DISPOSE_ARRAY(m_pCacheVNormales);
    DISPOSE_ARRAY(m_pFNormales);
    DISPOSE_ARRAY(m_pVNormales);
    DISPOSE_ARRAY(m_pTangentes);
    DISPOSE_ARRAY(m_pBinormales);
    for (int i = 0; i < m_iNumBatches; i++)
      DISPOSE_ARRAY(m_pBatches[i].pFaces);
    DISPOSE_ARRAY(m_pBatches);
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// SetFrame
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetTime(float fTime)
{
  float fOrigTime = fTime;
  // Xform?
  if (m_bWaveXform)
  {
    float fWave;
    CMotor3D::GetRegisterData(CMotor3D::V_WAVE + m_iWaveXformChan, &fWave);
    fWave = fWave * 1.2f;
    if (fWave > 1.f) fWave = 1.f;
    fTime = fWave * m_fNumFrames;
  }

  // Cacheado
  bool bVrtxChanged = false;
  //if (m_fCacheFrame != fTime)
  {
    bool bChanged = false;
    m_fCacheFrame = fTime;
    // Mesh animation
    if (m_pPathVertices && m_pPathVertices->GetNumKeys() > 1)
    {
      m_pPathVertices->SetTime     (fTime);
      m_pPathVertices->GetKeyValues(m_pVertices, m_pCacheVertices, m_uNumVertices);
      bVrtxChanged = true;
    }
    // UV animation
    if (m_pPathUV && m_pPathUV->GetNumKeys() > 1 && !m_bUseUVEnvMap && !m_bUseUVEnvMapFlat)
    {
      m_pPathUV->SetTime     (fTime);
      m_pPathUV->GetKeyValues(m_pUV, m_pCacheUV, m_uNumUV);
    }
    // Pos animation
    if (m_pPathPosiciones && m_pPathPosiciones->GetNumKeys() > 1)
    {
      m_pPathPosiciones->SetTime    (fTime);
      m_pPathPosiciones->GetKeyValue(m_vCachePosicion);
      D3DXMatrixTranslation(&m_CacheMatrixPos, m_vCachePosicion.x, m_vCachePosicion.y, m_vCachePosicion.z);
      bChanged = true;
    }
    // Rot animation
    if (m_pPathRotaciones && m_pPathRotaciones->GetNumKeys() > 1)
    {
      m_pPathRotaciones->SetTime    (fTime);
      m_pPathRotaciones->GetKeyValue(m_vCacheRotacion);
      D3DXMatrixRotationQuaternion(&m_CacheMatrixRot, (D3DXQUATERNION *)&m_vCacheRotacion);
      bChanged = true;
    }
    // Scale animation
    if (m_pPathEscalados && m_pPathEscalados->GetNumKeys() > 1)
    {
      m_pPathEscalados->SetTime    (fTime);
      m_pPathEscalados->GetKeyValue(m_vCacheEscalado);
      D3DXMatrixScaling(&m_CacheMatrixScale, m_vCacheEscalado.x, m_vCacheEscalado.y, m_vCacheEscalado.z);
      bChanged = true;
    }
    // Matriz compuesta
    if (bChanged)
      m_CacheMatrixPRS = m_CacheMatrixScale * m_CacheMatrixRot * m_CacheMatrixPos;
  }

  // Morph
  switch (m_iMorphMode)
  {
    case 1: AplicaMorphRot (bVrtxChanged, fOrigTime); break;
    case 2: AplicaMorphSin (bVrtxChanged, fOrigTime); break;
    case 3: AplicaMorphSinY(bVrtxChanged, fOrigTime); break;
    case 4: AplicaMorphTent(bVrtxChanged, fOrigTime); break;
  }

  // Update normals?
  if (m_bRecalcNormals)
  {
    CalculaFNormales();
    CalculaVNormales();
  }
}


//---------------------------------------------------------------------------//
// CalculaZ
//
//---------------------------------------------------------------------------//
void CObjeto3D::CalculaZ(D3DXMATRIX const &matProj)
{
  D3DXMATRIX matWVP = m_CacheMatrixPRS * matProj;
  TVector3 vSrc;
  vSrc.x = vSrc.y = vSrc.z = 0.f;
  m_fCacheZ = matWVP.m[0][2] * vSrc.x +
              matWVP.m[1][2] * vSrc.y +
              matWVP.m[2][2] * vSrc.z +
              matWVP.m[3][2];
  //m_fCacheZ = m_CacheMatrixPRS.m[0][2] * vSrc.x +
  //            m_CacheMatrixPRS.m[1][2] * vSrc.y +
  //            m_CacheMatrixPRS.m[2][2] * vSrc.z +
  //            m_CacheMatrixPRS.m[3][2];
  /*
  pD3D->SetTransform(D3DTS_WORLD, &m_CacheMatrixPRS);

  TVertex_HCV_XYZ_C *pVertices = (TVertex_HCV_XYZ_C *)pDD->LockVertexBuffer(HCV_XYZ_C, 1);
  HCV_SET_XYZ   (pVertices, 0.f, 0.f, 0.f);
  pDD->UnlockVertexBuffer(HCV_XYZ_C);
  pDD->ProcessVertices(HCV_XYZ_C, 1);

  TVertex_HCV_XYZ *pOut = (TVertex_HCV_XYZ *)pDD->LockVertexBuffer(HCV_XYZ, 1);
  m_fCacheZ = pOut->fZ;
  pDD->UnlockVertexBuffer(HCV_XYZ);
  */
}


//---------------------------------------------------------------------------//
// CalculaFNormales
//
//---------------------------------------------------------------------------//
void CObjeto3D::CalculaFNormales()
{
  int       i;
  TFace    *pTri;
  TVector3 *pVrtx, *pNTri;
  TVector3 *v1, *v2, *v3;

  //-- Calcular normales de las caras --//
  pTri  = m_pFaces;
  pNTri = m_pFNormales;
  pVrtx = m_pCacheVertices;
  for (i = 0; i < m_uNumFaces; i++, pNTri++)
  {
    v1 = &pVrtx[m_pFaces[i].i0];
    v2 = &pVrtx[m_pFaces[i].i1];
    v3 = &pVrtx[m_pFaces[i].i2];
    Vector_Normal (pNTri, v1, v3, v2);
  }
  memcpy(m_pCacheFNormales, m_pFNormales, m_uNumFaces    * sizeof(TVector3));
}


//---------------------------------------------------------------------------//
// CalculaVNormales
//
//---------------------------------------------------------------------------//
void CObjeto3D::CalculaVNormales()
{
  int       i, k, t1, t2, t3;
  int       incident;
  TFace    *pTri;
  TVector3 *pVrtx, *pNVrtx, *pNTri;
  static TVector3 pUsed[65000];
  
  pVrtx = m_pCacheVertices;

  //pUsed = NEW_ARRAY(TVector3, m_uNumVertices);
  //-- Calcular normales de los vertices --//
  pNVrtx = m_pVNormales;
  for (k = 0; k < m_uNumVertices; k++, pNVrtx++, pVrtx++)
  {
    pNVrtx->x = 0.f;
    pNVrtx->y = 0.f;
    pNVrtx->z = 0.f;
    incident = 0;
    pTri     = m_pFaces;
    pNTri    = m_pFNormales;
    for (i = 0; i < m_uNumFaces; i++, pNTri++)
    {
      t1 = m_pFaces[i].i0;
      t2 = m_pFaces[i].i1;
      t3 = m_pFaces[i].i2;
      if ((t1 == k) || (t2 == k) || (t3 == k))
      {
      //---------------------------------------------------------
        /*
        bool usable = 1;
        for (unsigned u = 0; u < incident; u++)
        {
          if ((fabs (pNTri->x - pUsed[u].x) < 0.1f) &&
              (fabs (pNTri->y - pUsed[u].y) < 0.1f) &&
              (fabs (pNTri->z - pUsed[u].z) < 0.1f) )
          {
            usable = 0;
            break;
          }
        }
        if (usable)
        */
        {
          //pUsed[incident].x = pNTri->x;
          //pUsed[incident].y = pNTri->y;
          //pUsed[incident].z = pNTri->z;
          pNVrtx->x+= pNTri->x;
          pNVrtx->y+= pNTri->y;
          pNVrtx->z+= pNTri->z;
          incident++;
        }
      }
    }
    float d = 1.f / incident;
    pNVrtx->x*=d;
    pNVrtx->y*=d;
    pNVrtx->z*=d;
    Vector_Unit(pNVrtx, pNVrtx);
  }
  //DISPOSE_ARRAY(pUsed);
  memcpy(m_pCacheVNormales, m_pVNormales, m_uNumVertices * sizeof(TVector3));
}


//---------------------------------------------------------------------------//
// CalculaTangentes
//
//---------------------------------------------------------------------------//
void CObjeto3D::CalculaTangentes()
{
	TVector3 *pTan1 = NEW_ARRAY(TVector3, m_uNumVertices);
	//TVector3 *pTan2 = pTan1 + m_uNumVertices;
  memset(pTan1, 0, m_uNumVertices * sizeof(TVector3));
	
	for (int i = 0; i < m_uNumFaces; i++)
	{
		int i1 = m_pFaces[i].i0;
		int i2 = m_pFaces[i].i1;
		int i3 = m_pFaces[i].i2;
		
		const TVector3 &v1 = m_pVertices[i1];
		const TVector3 &v2 = m_pVertices[i2];
		const TVector3 &v3 = m_pVertices[i3];
		
		const TVector2 &w1 = m_pCacheUV[i*3+0];
		const TVector2 &w2 = m_pCacheUV[i*3+1];
		const TVector2 &w3 = m_pCacheUV[i*3+2];
		
    float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;
		
		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;
		
		float r = 1.0f / (s1 * t2 - s2 * t1);
		TVector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		//TVector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
		
		pTan1[i1].x += sdir.x; pTan1[i1].y += sdir.y; pTan1[i1].z += sdir.z;
    pTan1[i2].x += sdir.x; pTan1[i2].y += sdir.y; pTan1[i2].z += sdir.z;
    pTan1[i3].x += sdir.x; pTan1[i3].y += sdir.y; pTan1[i3].z += sdir.z;
		/*
		pTan2[i1].x += tdir.x; pTan2[i1].y += tdir.y; pTan2[i1].z += tdir.z;
    pTan2[i2].x += tdir.x; pTan2[i2].y += tdir.y; pTan2[i2].z += tdir.z;
    pTan2[i3].x += tdir.x; pTan2[i3].y += tdir.y; pTan2[i3].z += tdir.z;
    */
	}
	
	for (int i = 0; i < m_uNumVertices; i++)
	{
		TVector3 n = m_pVNormales[i];
		TVector3 t = pTan1[i];
		
    TVector3 r;
    float fDot = Vector_Dot(&n, &t);
    r.x = t.x - n.x * fDot;
    r.y = t.y - n.y * fDot;
    r.z = t.z - n.z * fDot;
		Vector_Unit (&m_pTangentes[i], &r);
    Vector_Cross(&m_pBinormales[i], &n, &m_pTangentes[i]);
	}
	
  DISPOSE_ARRAY(pTan1);
}

//---------------------------------------------------------------------------//
// CalculaUV
//
//---------------------------------------------------------------------------//
void CObjeto3D::CalculaUV(D3DXMATRIX const &matProj)
{
  if ((m_iRenderMode == RENDER_ENVMAP) || m_bUseUVEnvMap)
  {
    TVector2 *pUV = NEW_ARRAY(TVector2, m_uNumVertices);
    D3DXMATRIX matWVP = m_CacheMatrixRot * matProj;
    for (int i = 0; i < m_uNumVertices; i++)
    {
      TVector3 vSrc = m_pCacheVNormales[i];
      pUV[i].x = matWVP.m[0][0] * vSrc.x +
                 matWVP.m[1][0] * vSrc.y +
                 matWVP.m[2][0] * vSrc.z;
      pUV[i].y = matWVP.m[0][1] * vSrc.x +
                 matWVP.m[1][1] * vSrc.y +
                 matWVP.m[2][1] * vSrc.z;
      pUV[i].x = pUV[i].x * 0.5f + 0.5f;
      pUV[i].y = pUV[i].y * 0.5f + 0.5f; 
    }
    for (int i = 0; i < m_uNumFaces; i++)
    {
      m_pCacheUVEnvMap[i*3+0] = pUV[m_pFaces[i].i0];
      m_pCacheUVEnvMap[i*3+1] = pUV[m_pFaces[i].i1];
      m_pCacheUVEnvMap[i*3+2] = pUV[m_pFaces[i].i2];
    }
    DISPOSE_ARRAY(pUV);
  }
  
  if ((m_iRenderMode == RENDER_ENVMAPFLAT) || m_bUseUVEnvMapFlat)
  {
    D3DXMATRIX matWVP = m_CacheMatrixRot * matProj;
    for (int i = 0; i < m_uNumFaces; i++)
    {
      TVector3 vSrc = m_pCacheFNormales[i];
      TVector2 uv;
      uv.x = matWVP.m[0][0] * vSrc.x +
             matWVP.m[1][0] * vSrc.y +
             matWVP.m[2][0] * vSrc.z;
      uv.y = matWVP.m[0][1] * vSrc.x +
             matWVP.m[1][1] * vSrc.y +
             matWVP.m[2][1] * vSrc.z;
      uv.x = uv.x * 0.5f + 0.5f;
      uv.y = uv.y * 0.5f + 0.5f; 
      m_pCacheUVEnvMapFlat[i*3+0] = uv;
      m_pCacheUVEnvMapFlat[i*3+1] = uv;
      m_pCacheUVEnvMapFlat[i*3+2] = uv;
    }
  }
}


//---------------------------------------------------------------------------//
// PreparaBatches
//
//---------------------------------------------------------------------------//
void CObjeto3D::PreparaBatches(int *pTraductorMateriales, int iNumMateriales)
{
  m_iNumBatches = iNumMateriales;
  m_pBatches = NEW_ARRAY(TFaceBatch, iNumMateriales);
  for (int i = 0; i < iNumMateriales; i++)
  {
    m_pBatches[i].iMat      = -1;
    m_pBatches[i].uNumFaces = 0;
    m_pBatches[i].pFaces    = NULL;
    int uNumFaces = 0;
    for (int j = 0; j < m_uNumFaces; j++)
    {
      if (m_pFaces[j].iMat == i)
        uNumFaces++;
    }
    // Hay faces, guardar el batch
    if (uNumFaces > 0)
    {
      m_pBatches[i].iMat      = pTraductorMateriales[i];
      m_pBatches[i].uNumFaces = uNumFaces;
      m_pBatches[i].pFaces    = NEW_ARRAY(int, uNumFaces);
      int iFace = 0;
      for (int j = 0; j < m_uNumFaces; j++)
      {
        if (m_pFaces[j].iMat == i)
          m_pBatches[i].pFaces[iFace++] = j;
      }
    }
  }
}


//---------------------------------------------------------------------------//
// Rotacion
//
//---------------------------------------------------------------------------//
void CObjeto3D::AplicaMorphRot(bool bChanged, float fTime)
{
  float fMorphTime = fTime * m_fMorphSpeed * 1.f / 200.f;

  TVector3 *pVerticesSrc = bChanged ? m_pCacheVertices : m_pVertices;
  TVector3 *pVerticesDst = m_pCacheVertices;

  for (int i = 0; i < m_uNumVertices; i++)
  {
    float fVTime = fMorphTime;
    TVector4 vRot;
    vRot.x = sinf(fVTime * 0.92f) * m_fMorphX * 3.00f + 
             cosf(fVTime * 0.64f) * m_fMorphX * 3.00f;
    vRot.y = cosf(fVTime * 1.44f) * m_fMorphY * 3.00f +
             sinf(fVTime * 0.82f) * m_fMorphY * 3.00f;
    vRot.z = sinf(fVTime * 0.94f) * m_fMorphZ * 3.00f + 
             cosf(fVTime * 1.22f) * m_fMorphZ * 3.00f;
    vRot.x*=m_fMorphScale;
    vRot.y*=m_fMorphScale;
    vRot.z*=m_fMorphScale;
    vRot.w = 1.f;

    Vector_Unit(&vRot, &vRot);
    D3DXMATRIX Matrix;
    D3DXMatrixRotationQuaternion(&Matrix, (D3DXQUATERNION *)&vRot);

    D3DXVec3TransformCoord((D3DXVECTOR3 *)pVerticesDst, (D3DXVECTOR3 *)pVerticesSrc, &Matrix);
    D3DXVec3TransformCoord((D3DXVECTOR3 *)&m_pCacheFNormales[i], (D3DXVECTOR3 *)&m_pFNormales[i], &Matrix);
    D3DXVec3TransformCoord((D3DXVECTOR3 *)&m_pCacheVNormales[i], (D3DXVECTOR3 *)&m_pVNormales[i], &Matrix);
    pVerticesSrc++;
    pVerticesDst++;
  }
}


//---------------------------------------------------------------------------//
// Morph sinusoidal en los 3 ejes
//
//---------------------------------------------------------------------------//
void CObjeto3D::AplicaMorphSin(bool bChanged, float fTime)
{
  float fMorphTime = fTime * m_fMorphSpeed * 1.f / 200.f;
  
  TVector3 *pVerticesSrc = bChanged ? m_pCacheVertices : m_pVertices;
  TVector3 *pVerticesDst = m_pCacheVertices;
  for (int i = 0; i < m_uNumVertices; i++)
  {
    float x = pVerticesSrc->x;
    float y = pVerticesSrc->y;
    float z = pVerticesSrc->z;
    pVerticesDst->x = x * ((sinf(z * 0.087f * m_fMorphScale * 2.f + fMorphTime * 0.35f)) * m_fMorphX * 0.5f + 1.0f);
    pVerticesDst->y = y * ((cosf(z * 0.065f * m_fMorphScale * 2.f + fMorphTime * 0.46f)) * m_fMorphY * 0.5f + 1.0f);
    pVerticesDst->z = z * ((cosf(x * 0.077f * m_fMorphScale * 2.f + fMorphTime * 0.25f)) * m_fMorphZ * 0.2f + 
                           (sinf(y * 0.096f * m_fMorphScale * 2.f + fMorphTime * 0.57f)) * m_fMorphZ * 0.3f + 1.0f);
    pVerticesSrc++;
    pVerticesDst++;
  }
}


//---------------------------------------------------------------------------//
// MorphY sinusoidal (solo en eje Y)
//
//---------------------------------------------------------------------------//
static void RotateYScale(float fAng, float fScale, TVector3 const *pPos, TVector3 *pDst)
{
  float x = pPos->x;
  float z = pPos->z;
  float fS= sinf(fAng);
  float fC= cosf(fAng);
  pDst->x = (x * fC - z * fS) * fScale;
  pDst->z = (z * fC + x * fS) * fScale;
}

static void RotateY(float fAng, TVector3 const *pPos, TVector3 *pDst)
{
  float x = pPos->x;
  float z = pPos->z;
  float fS= sinf(fAng);
  float fC= cosf(fAng);
  pDst->x = (x * fC - z * fS);
  pDst->z = (z * fC + x * fS);
}

void CObjeto3D::AplicaMorphSinY(bool bChanged, float fTime)
{
  float fMorphTime = fTime * m_fMorphSpeed * 1.f / 200.f;

  TVector3 *pVerticesSrc = bChanged ? m_pCacheVertices : m_pVertices;
  TVector3 *pVerticesDst = m_pCacheVertices;
  for (int i = 0; i < m_uNumVertices; i++)
  {
    float fAng = sinf(fMorphTime * 0.21f + pVerticesSrc->y * 0.005f * m_fMorphScale * 3.f) * m_fMorphY * 4.f +
                 cosf(fMorphTime * 0.13f + pVerticesSrc->y * 0.007f * m_fMorphScale * 3.f) * m_fMorphZ * 4.f;
    RotateYScale(fAng,
                 cosf(fMorphTime * 0.43f + pVerticesSrc->y * 0.0378f * m_fMorphScale * 3.f) * m_fMorphX * 0.25f + 
                 sinf(fMorphTime * 0.37f + pVerticesSrc->y * 0.0413f * m_fMorphScale * 3.f) * m_fMorphX * 0.25f + 1.f,
                 pVerticesSrc, pVerticesDst);
    RotateY(fAng, &m_pFNormales[i], &m_pCacheFNormales[i]);
    RotateY(fAng, &m_pVNormales[i], &m_pCacheVNormales[i]);
    pVerticesSrc++;
    pVerticesDst++;
  }
}


//---------------------------------------------------------------------------//
// Morph Tent (Morph para el tentaculo de la Sound Pressure)
//
//---------------------------------------------------------------------------//
void CObjeto3D::AplicaMorphTent(bool bChanged, float fTime)
{
  float fMorphTime = fTime * m_fMorphSpeed * 1.f / 200.f;

  TVector3 *pVerticesSrc = bChanged ? m_pCacheVertices : m_pVertices;
  TVector3 *pVerticesDst = m_pCacheVertices;

  for (int i = 0; i < m_uNumVertices; i++)
  {
    float fX = pVerticesSrc->x - m_vCachePosicion.x;
    float fY = pVerticesSrc->y - m_vCachePosicion.y;
    float fZ = pVerticesSrc->z - m_vCachePosicion.z;
    float fVTime = fMorphTime - sqrtf(fX*fX + fY*fY + fZ*fZ) * m_fMorphScale * 0.05f;
    TVector4 vRot;
    vRot.x = sinf(fVTime * 0.52f) * m_fMorphX * 0.57f;
    vRot.y = cosf(fVTime * 1.44f) * m_fMorphY * 0.34f;
    vRot.z = sinf(fVTime * 0.94f) * m_fMorphZ * 0.78f;
    vRot.w = 1.f;
    
    Vector_Unit(&vRot, &vRot);
    D3DXMATRIX Matrix;
    D3DXMatrixRotationQuaternion(&Matrix, (D3DXQUATERNION *)&vRot);

    D3DXVec3TransformCoord((D3DXVECTOR3 *)pVerticesDst, (D3DXVECTOR3 *)pVerticesSrc, &Matrix);
    D3DXVec3TransformCoord((D3DXVECTOR3 *)&m_pCacheFNormales[i], (D3DXVECTOR3 *)&m_pFNormales[i], &Matrix);
    D3DXVec3TransformCoord((D3DXVECTOR3 *)&m_pCacheVNormales[i], (D3DXVECTOR3 *)&m_pVNormales[i], &Matrix);
    pVerticesSrc++;
    pVerticesDst++;
  }
}


//---------------------------------------------------------------------------//
// BorraVars
//
//---------------------------------------------------------------------------//
void CObjeto3D::BorraVars()
{
  m_uColor         = HARD_COLOR_ARGB(255,255,255,255);
  m_Nombre         = "";
  m_uFlags         = 0;
  m_uNumFaces      = 0;
  m_uNumUV         = 0;
  m_uNumVertices   = 0;
  m_iRenderMode    = RENDER_NORMAL;
  m_bWaveXform     = false;
  m_iWaveXformChan = 0;
  m_fMorphScale    = 1.f;
  m_fMorphSpeed    = 0.5f;
  m_fMorphX        = 0.25f;
  m_fMorphY        = 0.25f;
  m_fMorphZ        = 0.25f;
  m_bRecalcNormals = false;
  m_pFNormales     = NULL;
  m_pVNormales     = NULL;
  m_pTangentes     = NULL;
  m_pBinormales    = NULL;
  m_pCacheUV       = NULL;
  m_pCacheUVEnvMap = NULL;
  m_pCacheVertices = NULL;
  m_pCacheFNormales= NULL;
  m_pCacheVNormales= NULL;
}


//---------------------------------------------------------------------------//
// FillVertex
//
//---------------------------------------------------------------------------//
void CObjeto3D::FillVertex(TVertex_HCV_XYZ_F *pVertices, CMaterial *pMat, int i, int uv, unsigned *pFlags)
{
  TVector3 const *pSrc       = GetVertices();
  TVector3 const *pNor       = GetVNormales();
  TVector3 const *pFN        = GetFNormales();
  TVector2 const *pUVNormal  = GetUV();
  TVector2 const *pUVEnv     = GetUVEnvMap();
  TVector2 const *pUVEnvFlat = GetUVEnvMapFlat();
  TVector2 const *pUV;

  bool bEnvMap     = (m_iRenderMode == RENDER_ENVMAP);
  bool bEnvMapFlat = (m_iRenderMode == RENDER_ENVMAPFLAT);
  
  HCV_SET_COLOR (pVertices, GetColor());
  HCV_SET_XYZ   (pVertices, pSrc[i].x, pSrc[i].y, pSrc[i].z);
  // Tex 0
  pUV = pUVNormal;
  if (bEnvMap)
    pUV = pUVEnv;
  else if (bEnvMapFlat)
    pUV = pUVEnvFlat;
  HCV_SET_UV0 (pVertices, pUV[uv].x, pUV[uv].y);
  // Tex 1
  if (pMat->GetNumTexturas() > 1)
  {
    pUV = pUVNormal;
    if (bEnvMap)
      pUV = pUVEnv;
    else if (bEnvMapFlat)
      pUV = pUVEnvFlat;
    HCV_SET_UV1(pVertices, pUV[uv].x, pUV[uv].y);
  }
  // Normal
  HCV_SET_NORMAL (pVertices, pNor[i].x, pNor[i].y, pNor[i].z);
}


//---------------------------------------------------------------------------//
// FillVertex
//
//---------------------------------------------------------------------------//
void CObjeto3D::FillVertex(TVertex_HCV_XYZ_F *pVertices, int i0)
{
  TVector3 const *pSrc  = GetVertices();
  TVector3 const *pNor  = GetVNormales();

  HCV_SET_COLOR (pVertices, GetColor());
  HCV_SET_XYZ   (pVertices, pSrc[i0].x, pSrc[i0].y, pSrc[i0].z);
  HCV_SET_NORMAL(pVertices, pNor[i0].x, pNor[i0].y, pNor[i0].z);
}


//---------------------------------------------------------------------------//
// FillVertex
//
//---------------------------------------------------------------------------//
void CObjeto3D::FillVertex(TVertex_HCV_XYZ_P *pVertices, CMaterial *pMat, int i, int uv, unsigned *pFlags)
{
  TVector3 const *pSrc       = GetVertices();
  TVector3 const *pNor       = GetVNormales();
  TVector3 const *pFN        = GetFNormales();
  TVector2 const *pUVNormal  = GetUV();
  TVector2 const *pUVEnv     = GetUVEnvMap();
  TVector2 const *pUVEnvFlat = GetUVEnvMapFlat();
  TVector2 const *pUV;

  bool bEnvMap     = (m_iRenderMode == RENDER_ENVMAP);
  bool bEnvMapFlat = (m_iRenderMode == RENDER_ENVMAPFLAT);
  
  HCV_SET_COLOR (pVertices, GetColor());
  HCV_SET_XYZ   (pVertices, pSrc[i].x, pSrc[i].y, pSrc[i].z);
  // Tex 0
  pUV = pUVNormal;
  if (bEnvMap)
    pUV = pUVEnv;
  else if (bEnvMapFlat)
    pUV = pUVEnvFlat;
  HCV_SET_UV0 (pVertices, pUV[uv].x, pUV[uv].y);
  // Tex 1
  if (pMat->GetNumTexturas() > 1)
  {
    pUV = pUVNormal;
    if (bEnvMap)
      pUV = pUVEnv;
    else if (bEnvMapFlat)
      pUV = pUVEnvFlat;
    HCV_SET_UV1(pVertices, pUV[uv].x, pUV[uv].y);
  }
  // Tex 2
  if (pMat->GetNumTexturas() > 2)
  {
    pUV = pUVNormal;
    if (bEnvMap)
      pUV = pUVEnv;
    else if (bEnvMapFlat)
      pUV = pUVEnvFlat;
    HCV_SET_UV2(pVertices, pUV[uv].x, pUV[uv].y);
  }
  // Tex 3
  if (pMat->GetNumTexturas() > 3)
  {
    pUV = pUVNormal;
    if (bEnvMap)
      pUV = pUVEnv;
    else if (bEnvMapFlat)
      pUV = pUVEnvFlat;
    HCV_SET_UV3(pVertices, pUV[uv].x, pUV[uv].y);
  }
  // Normal,Tangente,Binormal
  HCV_SET_NORMAL  (pVertices, pNor[i].x, pNor[i].y, pNor[i].z);
  HCV_SET_TANGENT (pVertices, GetTangentes ()[i].x, GetTangentes ()[i].y, GetTangentes ()[i].z, 1);
  HCV_SET_BINORMAL(pVertices, GetBinormales()[i].x, GetBinormales()[i].y, GetBinormales()[i].z, 1);
}


//---------------------------------------------------------------------------//
// FillVertex
//
//---------------------------------------------------------------------------//
void CObjeto3D::FillVertex(TVertex_HCV_XYZ_P *pVertices, int i0)
{
  TVector3 const *pSrc  = GetVertices();
  TVector3 const *pNor  = GetVNormales();

  HCV_SET_COLOR   (pVertices, GetColor());
  HCV_SET_XYZ     (pVertices, pSrc[i0].x, pSrc[i0].y, pSrc[i0].z);
  HCV_SET_NORMAL  (pVertices, pNor[i0].x, pNor[i0].y, pNor[i0].z);
  HCV_SET_TANGENT (pVertices, GetTangentes ()[i0].x, GetTangentes ()[i0].y, GetTangentes ()[i0].z, 1);
  HCV_SET_BINORMAL(pVertices, GetBinormales()[i0].x, GetBinormales()[i0].y, GetBinormales()[i0].z, 1);
}

//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CObjeto3D::Draw(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  pD3D->SetTransform(D3DTS_WORLD, &m_CacheMatrixPRS);
  D3DXMATRIX matVP, matWVP;
  CMotor3D::GetRegisterData(CMotor3D::V_VIEWPROJECTION, (float *)&matVP);
  matWVP = m_CacheMatrixPRS * matVP;
  CMotor3D::SetRegisterData(CMotor3D::V_WORLD, (float *)&m_CacheMatrixPRS);
  CMotor3D::SetRegisterData(CMotor3D::V_WORLDVIEWPROJECTION, (float *)&matWVP);

  for (int iBatch = 0; iBatch < m_iNumBatches; iBatch++)
  {
    CMaterial *pMat = g_pGestorMateriales->GetMaterial(m_pBatches[iBatch].iMat);
    // Numero de caras a pintar
    int iNumFaces = m_pBatches[iBatch].uNumFaces;
    if (pMat && iNumFaces > 0)
    {
      bool bFixed = pMat->FixedPipeline();
      // Calcular los flags para el envmap
      unsigned pFlags[MAX_TEXTURES];
      for (int f = 0; f < MAX_TEXTURES; f++)
      {
        if (pMat->GetIDTextura(f) >= 0)
          pFlags[f] = g_pGestorMateriales->GetTextura(pMat->GetIDTextura(f))->GetFlags();
        else
          pFlags[f] = 0;
      }

      if (m_iRenderMode != RENDER_POINTSPRITE)
      {
        if (GetUV())
        {
          // Vertices
          int iHCV;
          TVertex_HCV_XYZ_F *pVerticesF;
          TVertex_HCV_XYZ_P *pVerticesP;
          if (bFixed)
          {
            iHCV       = HCV_XYZ_F;
            pVerticesF = (TVertex_HCV_XYZ_F *)pDD->LockVertexBuffer(HCV_XYZ_F, iNumFaces*3);
          }
          else
          {
            iHCV       = HCV_XYZ_P;
            pVerticesP = (TVertex_HCV_XYZ_P *)pDD->LockVertexBuffer(HCV_XYZ_P, iNumFaces*3);
          }
          //TVertex_HCV_XYZ *pVertices = (TVertex_HCV_XYZ *)pDD->LockVertexBuffer(HCV_XYZ, iNumFaces*3);
          for (int i = 0; i < iNumFaces; i++)
          {
            int iFace = m_pBatches[iBatch].pFaces[i];
            int i0 = m_pFaces[iFace].i0;
            int i1 = m_pFaces[iFace].i1;
            int i2 = m_pFaces[iFace].i2;
            int uv = iFace*3;
            if (bFixed)
            {
              FillVertex(pVerticesF++, pMat, i0, uv,   pFlags);
              FillVertex(pVerticesF++, pMat, i1, uv+1, pFlags);
              FillVertex(pVerticesF++, pMat, i2, uv+2, pFlags);
            }
            else
            {
              FillVertex(pVerticesP++, pMat, i0, uv,   pFlags);
              FillVertex(pVerticesP++, pMat, i1, uv+1, pFlags);
              FillVertex(pVerticesP++, pMat, i2, uv+2, pFlags);
            }
          }
          pDD->UnlockVertexBuffer(iHCV);
          // Material
          unsigned uPasses = pMat->BeginDraw(0);
          for (unsigned uPass = 0; uPass < uPasses; uPass++)
          {
            pMat->BeginPass(uPass);
            pDD->DrawPrimitive(iHCV, HARD_PRIM_TRIANGLELIST, iNumFaces);
            pMat->EndPass();
          }
          pMat->EndDraw();
        }
        else
        {
          // Vertices
          int iHCV;
          TVertex_HCV_XYZ_F *pVerticesF;
          TVertex_HCV_XYZ_P *pVerticesP;
          if (bFixed)
          {
            iHCV       = HCV_XYZ_F;
            pVerticesF = (TVertex_HCV_XYZ_F *)pDD->LockVertexBuffer(HCV_XYZ_F, iNumFaces*3);
          }
          else
          {
            iHCV       = HCV_XYZ_P;
            pVerticesP = (TVertex_HCV_XYZ_P *)pDD->LockVertexBuffer(HCV_XYZ_P, iNumFaces*3);
          }
          for (int i = 0; i < iNumFaces; i++)
          {
            int iFace = m_pBatches[iBatch].pFaces[i];
            int i0 = m_pFaces[iFace].i0;
            int i1 = m_pFaces[iFace].i1;
            int i2 = m_pFaces[iFace].i2;
            if (bFixed)
            {
              FillVertex(pVerticesF++, i0);
              FillVertex(pVerticesF++, i1);
              FillVertex(pVerticesF++, i2);
            }
            else
            {
              FillVertex(pVerticesP++, i0);
              FillVertex(pVerticesP++, i1);
              FillVertex(pVerticesP++, i2);
            }
          }
          pDD->UnlockVertexBuffer(iHCV);
          // Material
          unsigned uPasses = pMat->BeginDraw(0);
          for (unsigned uPass = 0; uPass < uPasses; uPass++)
          {
            pMat->BeginPass(uPass);
            pDD->DrawPrimitive(iHCV, HARD_PRIM_TRIANGLELIST, iNumFaces);
            pMat->EndPass();
          }
          pMat->EndDraw();
        }
      }
      else
      {
        // POINTSPRITE
        pD3D->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
        pD3D->SetRenderState(D3DRS_POINTSCALEENABLE,  TRUE);
        float fValue = 64.f;
        pD3D->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fValue));      
        fValue = 1.f;
        pD3D->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&fValue));
        fValue = 0.f;
        pD3D->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*)&fValue));
        fValue = 0.f;
        pD3D->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&fValue));
        fValue = 1.f;
        pD3D->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&fValue));
        pD3D->SetRenderState(D3DRS_ZENABLE, FALSE);

        int iNumVertices = m_uNumVertices;
        // Vertices
        int iHCV;
        TVertex_HCV_XYZ_F *pVerticesF;
        TVertex_HCV_XYZ_P *pVerticesP;
        if (bFixed)
        {
          iHCV       = HCV_XYZ_F;
          pVerticesF = (TVertex_HCV_XYZ_F *)pDD->LockVertexBuffer(HCV_XYZ_F, iNumVertices);
        }
        else
        {
          iHCV       = HCV_XYZ_P;
          pVerticesP = (TVertex_HCV_XYZ_P *)pDD->LockVertexBuffer(HCV_XYZ_P, iNumVertices);
        }
        //TVertex_HCV_XYZ *pVertices = (TVertex_HCV_XYZ *)pDD->LockVertexBuffer(HCV_XYZ, iNumVertices);
        for (int i = 0; i < iNumVertices; i++)
        {
          if (bFixed) FillVertex(pVerticesF++, i);
          else        FillVertex(pVerticesP++, i);
        }
        pDD->UnlockVertexBuffer(iHCV);
        // Material
        unsigned uPasses = pMat->BeginDraw(0);
        for (unsigned uPass = 0; uPass < uPasses; uPass++)
        {
          pMat->BeginPass(uPass);
          pDD->DrawPrimitive(iHCV, HARD_PRIM_POINTLIST, iNumVertices);
          pMat->EndPass();
        }
        pMat->EndDraw();
      }
    }
  }
}


//---------------------------------------------------------------------------//
// SetFaces
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetFaces(TFace *pFaces, unsigned uCant)
{
  m_pFaces    = pFaces;
  m_uNumFaces = uCant;
}


//---------------------------------------------------------------------------//
// SetUV
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetUV(TVector2 *pUV, unsigned uCant, unsigned *pTimes, unsigned uFrames)
{
  m_pUV     = pUV;
  m_uNumUV  = uCant;
  m_pPathUV = NEW CPathLinear;
  m_pPathUV->Init(CPath::PATH_TIME, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathUV->AddKey((float)pTimes[i]);
  // Cache
  m_pCacheUV = NEW_ARRAY(TVector2, uCant);
  memcpy(m_pCacheUV, m_pUV, uCant*sizeof(TVector2));
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetVertices
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetVertices(TVector3 *pVertices, unsigned uCant, unsigned *pTimes, unsigned uFrames)
{
  m_pVertices     = pVertices;
  m_uNumVertices  = uCant;
  m_pPathVertices = NEW CPathLinear;
  m_pPathVertices->Init(CPath::PATH_TIME, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathVertices->AddKey((float)pTimes[i]);
  // Cache
  m_pCacheVertices = NEW_ARRAY(TVector3, uCant);
  memcpy(m_pCacheVertices, m_pVertices, uCant*sizeof(TVector3));
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetPosiciones
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetPosiciones(TVector3 *pPosiciones, unsigned *pTimes, unsigned uFrames)
{
  m_pPathPosiciones = NEW CPathLinear;
  m_pPathPosiciones->Init(CPath::PATH_3D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathPosiciones->AddKey((float)pTimes[i], pPosiciones[i]);
  // Cache
  m_vCachePosicion = pPosiciones[0];
  DISPOSE_ARRAY(pPosiciones);
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetRotaciones
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetRotaciones(TVector4 *pRotaciones, unsigned *pTimes, unsigned uFrames)
{
  m_pPathRotaciones = NEW CPathLinear;
  m_pPathRotaciones->Init(CPath::PATH_4D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathRotaciones->AddKey((float)pTimes[i], pRotaciones[i]);
  // Cache
  m_vCacheRotacion = pRotaciones[0];
  DISPOSE_ARRAY(pRotaciones);
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetEscalados
//
//---------------------------------------------------------------------------//
void CObjeto3D::SetEscalados(TVector3 *pEscalados, unsigned *pTimes, unsigned uFrames)
{
  m_pPathEscalados = NEW CPathLinear;
  m_pPathEscalados->Init(CPath::PATH_3D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathEscalados->AddKey((float)pTimes[i], pEscalados[i]);
  // Cache
  m_vCacheEscalado = pEscalados[0];
  DISPOSE_ARRAY(pEscalados);
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// Prepara
//
//---------------------------------------------------------------------------//
void CObjeto3D::Prepara(int *pTraductorMateriales, int iNumMateriales)
{
  m_pCacheFNormales    = NEW_ARRAY(TVector3, m_uNumFaces);
  m_pCacheVNormales    = NEW_ARRAY(TVector3, m_uNumVertices);
  m_pCacheUVEnvMapFlat = NEW_ARRAY(TVector2, m_uNumVertices);
  m_pCacheUVEnvMap     = NEW_ARRAY(TVector2, m_uNumVertices);
  // Calcular normales si no estaban precalculadas
  if (!m_pFNormales)
  {
    m_pFNormales = NEW_ARRAY(TVector3, m_uNumFaces);
    CalculaFNormales();
  }
  if (!m_pVNormales)
  {
    m_pVNormales = NEW_ARRAY(TVector3, m_uNumVertices);
    CalculaVNormales();
  }
  // Calcular Tangentes/Binormales (se necesitan coordenadas de textura)
  if (m_pUV)
  {
    m_pTangentes  = NEW_ARRAY(TVector3, m_uNumVertices);
    m_pBinormales = NEW_ARRAY(TVector3, m_uNumVertices);
    CalculaTangentes();
  }
  // Preparar los batches
  PreparaBatches  (pTraductorMateriales, iNumMateriales);

  // Calcular si se usa EnvMap o EnvMapFlat
  /*
  m_bUseUVEnvMap     = false;
  m_bUseUVEnvMapFlat = false;
  for (int iMat = 0; iMat < iNumMateriales; iMat++)
  {
    CMaterial *pMat = g_pGestorMateriales->GetMaterial(pTraductorMateriales[iMat]);
    if (pMat)
    {
      for (int i = 0; i < pMat->GetNumTexturas(); i++)
      {
        if (pMat->GetIDTextura(i) >= 0)
        {
          m_bUseUVEnvMap     = m_bUseUVEnvMap     || (g_pGestorMateriales->GetTextura(pMat->GetIDTextura(i))->GetFlags() & CTextura::ENVMAP);
          m_bUseUVEnvMapFlat = m_bUseUVEnvMapFlat || (g_pGestorMateriales->GetTextura(pMat->GetIDTextura(i))->GetFlags() & CTextura::ENVMAPFLAT);
        }
      }
    }
  }
  */

  D3DXMatrixTranslation(&m_CacheMatrixPos, m_vCachePosicion.x, m_vCachePosicion.y, m_vCachePosicion.z);
  D3DXMatrixRotationQuaternion(&m_CacheMatrixRot, (D3DXQUATERNION *)&m_vCacheRotacion);
  D3DXMatrixScaling(&m_CacheMatrixScale, m_vCacheEscalado.x, m_vCacheEscalado.y, m_vCacheEscalado.z);
  m_CacheMatrixPRS = m_CacheMatrixScale * m_CacheMatrixRot * m_CacheMatrixPos;
}
