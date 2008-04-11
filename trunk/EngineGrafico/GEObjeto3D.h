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

#ifndef GEOBJETO3D_H
#define GEOBJETO3D_H

#include "GEEngineGrafico.h"

class  CMaterial;
class  CPath;
class  CSceneLoader;
struct TCtrlVar;

struct TFace
{
  ushort i0, i1, i2;
  ushort iMat;
};

struct TFaceBatch
{
  int  iMat;
  int  uNumFaces;
  int *pFaces;
};

class CObjeto3D
{
  friend class CSceneLoader;

  public:
                    CObjeto3D       () { m_bOk = false; }
                   ~CObjeto3D       () { End(); }

    bool            Init            (float fNumFrames);
    void            End             ();
    bool            IsOk            () const { return  m_bOk; }

    const string   &GetName         () const { return  m_Nombre;         }
    unsigned        GetColor        () const { return  m_uColor;         }
    TFace    const *GetFaces        () const { return  m_pFaces;         }
    TVector2 const *GetUV           () const { return  m_pCacheUV;       }
    TVector2 const *GetUVEnvMap     () const { return  m_pCacheUVEnvMap; }
    TVector2 const *GetUVEnvMapFlat () const { return  m_pCacheUVEnvMapFlat; }
    TVector3 const *GetVertices     () const { return  m_pCacheVertices; }
    TVector3 const *GetFNormales    () const { return  m_pCacheFNormales;}
    TVector3 const *GetVNormales    () const { return  m_pCacheVNormales;}
    TVector3 const *GetTangentes    () const { return  m_pTangentes;     }
    TVector3 const *GetBinormales   () const { return  m_pBinormales;    }
    TVector3 const *GetPos          () const { return &m_vCachePosicion; }
    TVector4 const *GetRot          () const { return &m_vCacheRotacion; }
    TVector3 const *GetScale        () const { return &m_vCacheEscalado; }
    float           GetZ            () const { return  m_fCacheZ; }

    ushort          GetNumFaces     () const { return  m_uNumFaces;    }
    ushort          GetNumUV        () const { return  m_uNumUV;       }
    ushort          GetNumVertices  () const { return  m_uNumVertices; }

    TFaceBatch     *GetFaceBatch    (int iIDMat) { return &m_pBatches[iIDMat]; }
    D3DMATRIX      *GetMatrixPRS    () { return &m_CacheMatrixPRS; }

    void            SetTime         (float fTime);
    
    void            Draw            (CDisplayDevice *pDD);

    void            CalculaFNormales();
    void            CalculaVNormales();
    void            CalculaTangentes();
    void            CalculaUV       (D3DXMATRIX const &matProj);
    void            CalculaZ        (D3DXMATRIX const &matProj);

    static TCtrlVar *GetVars        ();

    void            SetVar          (int iVar, void *pData);
    void           *GetVar          (int iVar);

    // For Loader
    void            SetFaces        (TFace    *pFaces,      unsigned uCant);
    void            SetUV           (TVector2 *pUV,         unsigned uCant, unsigned *pTimes, unsigned uFrames);
    void            SetVertices     (TVector3 *pVertices,   unsigned uCant, unsigned *pTimes, unsigned uFrames);
    void            SetPosiciones   (TVector3 *pPosiciones, unsigned *pTimes, unsigned uFrames);
    void            SetRotaciones   (TVector4 *pRotaciones, unsigned *pTimes, unsigned uFrames);
    void            SetEscalados    (TVector3 *pEscalados,  unsigned *pTimes, unsigned uFrames);
    void            Prepara         (int *pTraductorMateriales, int iNumMateriales);

  private:          

    void            FillVertex      (TVertex_HCV_XYZ_F *pVertices, CMaterial *pMat, int i0, int uv, unsigned *pFlags);
    void            FillVertex      (TVertex_HCV_XYZ_F *pVertices, int i0);
    void            FillVertex      (TVertex_HCV_XYZ_P *pVertices, CMaterial *pMat, int i0, int uv, unsigned *pFlags);
    void            FillVertex      (TVertex_HCV_XYZ_P *pVertices, int i0);
    void            PreparaBatches  (int *pTraductorMateriales, int iNumMateriales);
    void            AplicaMorphRot  (bool bChanged, float fTime);
    void            AplicaMorphSin  (bool bChanged, float fTime);
    void            AplicaMorphSinY (bool bChanged, float fTime);
    void            AplicaMorphTent (bool bChanged, float fTime);

    void            BorraVars       ();
    void            LiberaVars      ();

  private:
    
    bool            m_bOk;

    unsigned        m_uColor;
    string          m_Nombre;
    ushort          m_uFlags;
    ushort          m_uNumFaces;
    ushort          m_uNumUV;
    ushort          m_uNumVertices;
    bool            m_bUseUVEnvMap;
    bool            m_bUseUVEnvMapFlat;
    TFace          *m_pFaces;
    // UV
    TVector2       *m_pUV;
    CPath          *m_pPathUV;
    // Vertices
    CPath          *m_pPathVertices;
    TVector3       *m_pVertices;
    TVector3       *m_pFNormales;
    TVector3       *m_pVNormales;
    TVector3       *m_pTangentes;
    TVector3       *m_pBinormales;
    // PRS
    CPath          *m_pPathPosiciones;
    CPath          *m_pPathRotaciones;
    CPath          *m_pPathEscalados;
    // Batches
    int             m_iNumBatches;
    TFaceBatch     *m_pBatches;
    // Datos del frame cacheado
    float           m_fCacheZ;
    float           m_fCacheFrame;
    TVector2       *m_pCacheUV;
    TVector2       *m_pCacheUVEnvMap;
    TVector2       *m_pCacheUVEnvMapFlat;
    TVector3       *m_pCacheVertices;
    TVector3        m_vCachePosicion;
    TVector4        m_vCacheRotacion;
    TVector3        m_vCacheEscalado;
    TVector3       *m_pCacheFNormales;
    TVector3       *m_pCacheVNormales;
    D3DXMATRIX      m_CacheMatrixPos;
    D3DXMATRIX      m_CacheMatrixRot;
    D3DXMATRIX      m_CacheMatrixScale;
    D3DXMATRIX      m_CacheMatrixPRS;

    float           m_fNumFrames;
    float           m_fTime;
    // Vars
    int             m_iRenderMode;
    bool            m_bWaveXform;
    int             m_iWaveXformChan;
    int             m_iMorphMode;
    float           m_fMorphScale;
    float           m_fMorphX;
    float           m_fMorphY;
    float           m_fMorphZ;
    float           m_fMorphSpeed;
    bool            m_bRecalcNormals;
};

#endif
