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
#include "GESceneLoader.h"
#include "GEEscena3D.h"
#include "GEObjeto3D.h"
#include "GECamara3D.h"
#include "GELuz3D.h"
#include "N3DFormat.h"

// Utils
static void ReadData(const char *&pSrc, void *pDst, unsigned uSize)
{
  memcpy(pDst, pSrc, uSize);
  pSrc+=uSize;
}

//---------------------------------------------------------------------------//
// LoadN3Dv2
//
//---------------------------------------------------------------------------//
bool CSceneLoader::LoadN3Dv2(CEscena3D *pEscena3D, const char *pData, unsigned uDataSize)
{
  unsigned i, j;

  // Leer cabecera y comprobar que todo esta bien
  TN3DHeaderv1 Header;
  ReadData(pData, &Header, sizeof(TN3DHeaderv1));

  if (Header.uNumObjetos < 1)
  {
    GLOG(("ERR: At least one object must exist in 3D scenes\n"));
    return false;
  }
  if (Header.uNumCamaras < 1)
  {
    GLOG(("ERR: At least one camera must exist in 3D scenes\n"));
    return false;
  }

  
  //---------------------------------------------------------------------------//
  // Datos de la escena
  //---------------------------------------------------------------------------//
  pEscena3D->m_fFirstFrame    = Header.fFirstFrame;
  pEscena3D->m_fLastFrame     = Header.fLastFrame;
  pEscena3D->m_fFrameSpeed    = Header.fFrameSpeed;
  pEscena3D->m_fTicksPerFrame = Header.fTicksPerFrame;
  pEscena3D->m_fFPS           = Header.fFPS;
  pEscena3D->m_fAmbientR      = Header.fAmbientR;
  pEscena3D->m_fAmbientG      = Header.fAmbientG;
  pEscena3D->m_fAmbientB      = Header.fAmbientB;


  //---------------------------------------------------------------------------//
  // Materiales
  //---------------------------------------------------------------------------//
  int *pMateriales = NULL;
  if (Header.uNumMateriales > 0)
    pMateriales = NEW_ARRAY(int, Header.uNumMateriales);
  for (i = 0; i < Header.uNumMateriales; i++)
  {
    pMateriales[i] = -1;
    TN3DMaterialv1 Mat;
    ReadData(pData, &Mat, sizeof(Mat));
    pMateriales[i] = g_pGestorMateriales->AddMaterial(Mat.pNombre);
    if (pMateriales[i] < 0)
    {
      // Si no se puede cargar el material, intentaremos ponerle uno flat
      GLOG(("ERR: Can't load material %s. Setting default flat material\n", Mat.pNombre));
      pMateriales[i] = g_pGestorMateriales->AddMaterial("flat.nmt");
      // Si aun asi no se puede cargar, 
      if (pMateriales[i] < 0)
      {
        GLOG(("ERR: Can't load standard material flat.nmt\n"));
        return false;
      }
    }
  }
  pEscena3D->SetMateriales(pMateriales, Header.uNumMateriales);

  
  //---------------------------------------------------------------------------//
  // Objetos
  //---------------------------------------------------------------------------//
  CObjeto3D *pObjetos = NEW_ARRAY(CObjeto3D, Header.uNumObjetos);
  for (i = 0; i < Header.uNumObjetos; i++)
  {
    CObjeto3D *pObj = &pObjetos[i];
    TObj3DHeaderv1 ObjHeader;
    pObj->Init(Header.fLastFrame - Header.fFirstFrame);
    ReadData(pData, &ObjHeader, sizeof(TObj3DHeaderv1));
    if (ObjHeader.uNumFaces < 0)
    {
      GLOG(("ERR: 3D object does have 0 faces\n"));
      return false;
    }

    // Header
    pObj->m_Nombre = ObjHeader.pNombre;
    pObj->m_uFlags = ObjHeader.uFlags;
    pObj->m_uColor = ObjHeader.uColor;

    // Faces
    TFace *pFaces = NEW_ARRAY(TFace, ObjHeader.uNumFaces);
    ReadData(pData, pFaces, ObjHeader.uNumFaces * sizeof(TFace));
    for (j = 0; j < ObjHeader.uNumFaces; j++)
      swap(pFaces[j].i1, pFaces[j].i2);
    pObj->SetFaces(pFaces, ObjHeader.uNumFaces);

    // UV
    if (ObjHeader.uNumUV > 0)
    {
      unsigned *pFrameTimesUV = NEW_ARRAY(unsigned, 1);
      TVector2 *pUV           = NEW_ARRAY(TVector2, ObjHeader.uNumUV);
      ReadData(pData, pUV, ObjHeader.uNumUV * sizeof(TVector2));
      pFrameTimesUV[0] = 0;
      for (j = 0; j < ObjHeader.uNumUV; j++)
        swap(pUV[j*3+1], pUV[j*3+2]);
      pObj->SetUV(pUV, ObjHeader.uNumUV, pFrameTimesUV, 1);
      pObj->m_uColor = 0xFFFFFFFF;
    }

    // Vertices
    unsigned *pFrameTimesVertices = NEW_ARRAY(unsigned, ObjHeader.uNumFramesVertices);
    TVector3 *pVertices           = NEW_ARRAY(TVector3, ObjHeader.uNumVertices * ObjHeader.uNumFramesVertices);
    ReadData(pData, pFrameTimesVertices, ObjHeader.uNumFramesVertices * sizeof(unsigned));
    ReadData(pData, pVertices, ObjHeader.uNumVertices * ObjHeader.uNumFramesVertices * sizeof(TVector3));
    pObj->SetVertices(pVertices, ObjHeader.uNumVertices, pFrameTimesVertices, ObjHeader.uNumFramesVertices);

    // Posiciones
    unsigned *pFrameTimesPosiciones = NEW_ARRAY(unsigned, ObjHeader.uNumFramesPosiciones);
    TVector3 *pPosiciones           = NEW_ARRAY(TVector3, ObjHeader.uNumFramesPosiciones);
    ReadData(pData, pFrameTimesPosiciones, ObjHeader.uNumFramesPosiciones * sizeof(unsigned));
    ReadData(pData, pPosiciones, ObjHeader.uNumFramesPosiciones * sizeof(TVector3));
    pObj->SetPosiciones(pPosiciones, pFrameTimesPosiciones, ObjHeader.uNumFramesPosiciones);

    // Rotaciones
    unsigned *pFrameTimesRotaciones = NEW_ARRAY(unsigned, ObjHeader.uNumFramesRotaciones);
    TVector4 *pRotaciones           = NEW_ARRAY(TVector4, ObjHeader.uNumFramesRotaciones);
    ReadData(pData, pFrameTimesRotaciones, ObjHeader.uNumFramesRotaciones * sizeof(unsigned));
    ReadData(pData, pRotaciones, ObjHeader.uNumFramesRotaciones * sizeof(TVector4));
    pObj->SetRotaciones(pRotaciones, pFrameTimesRotaciones, ObjHeader.uNumFramesRotaciones);

    // Escalados
    unsigned *pFrameTimesEscalados = NEW_ARRAY(unsigned, ObjHeader.uNumFramesEscalados);
    TVector3 *pEscalados           = NEW_ARRAY(TVector3, ObjHeader.uNumFramesEscalados);
    ReadData(pData, pFrameTimesEscalados, ObjHeader.uNumFramesEscalados * sizeof(unsigned));
    ReadData(pData, pEscalados, ObjHeader.uNumFramesEscalados * sizeof(TVector3));
    pObj->SetEscalados(pEscalados, pFrameTimesEscalados, ObjHeader.uNumFramesEscalados);

    // Prepara el objeto
    pObj->Prepara(pMateriales, Header.uNumMateriales);
  }
  pEscena3D->SetObjetos(pObjetos, Header.uNumObjetos);


  //---------------------------------------------------------------------------//
  // Camaras
  //---------------------------------------------------------------------------//
  CCamara3D *pCamaras = NEW_ARRAY(CCamara3D, Header.uNumCamaras);
  for (i = 0; i < Header.uNumCamaras; i++)
  {
    TCam3DHeaderv1 CamHeader;
    CCamara3D *pCam = &pCamaras[i];
    pCam->Init();

    // Header
    ReadData(pData, &CamHeader, sizeof(TCam3DHeaderv1));
    char pBuffer[32];
    sprintf_s(pBuffer, 32, "%d", i);
    pCam->m_Nombre = pBuffer;
  
    // PosSrc
    unsigned *pFrameTimesPosSrc = NEW_ARRAY(unsigned, CamHeader.uNumFramesPosSrc);
    TVector3 *pPosSrc           = NEW_ARRAY(TVector3, CamHeader.uNumFramesPosSrc);
    ReadData(pData, pFrameTimesPosSrc, CamHeader.uNumFramesPosSrc * sizeof(unsigned));
    ReadData(pData, pPosSrc,           CamHeader.uNumFramesPosSrc * sizeof(TVector3));
    pCam->SetPosSrc(pPosSrc, pFrameTimesPosSrc, CamHeader.uNumFramesPosSrc);

    // PosTgt
    unsigned *pFrameTimesPosTgt = NEW_ARRAY(unsigned, CamHeader.uNumFramesPosTgt);
    TVector3 *pPosTgt           = NEW_ARRAY(TVector3, CamHeader.uNumFramesPosTgt);
    ReadData(pData, pFrameTimesPosTgt, CamHeader.uNumFramesPosTgt * sizeof(unsigned));
    ReadData(pData, pPosTgt,           CamHeader.uNumFramesPosTgt * sizeof(TVector3));
    pCam->SetPosTgt(pPosTgt, pFrameTimesPosTgt, CamHeader.uNumFramesPosTgt);

    // Fov
    unsigned *pFrameTimesFov = NEW_ARRAY(unsigned, CamHeader.uNumFramesFov);
    float    *pFov           = NEW_ARRAY(float,    CamHeader.uNumFramesFov);
    ReadData(pData, pFrameTimesFov, CamHeader.uNumFramesFov * sizeof(unsigned));
    ReadData(pData, pFov,           CamHeader.uNumFramesFov * sizeof(float));
    pCam->SetFov(pFov, pFrameTimesFov, CamHeader.uNumFramesFov);
  }
  pEscena3D->SetCamaras(pCamaras, Header.uNumCamaras);


  //---------------------------------------------------------------------------//
  // Luces
  //---------------------------------------------------------------------------//
  CLuz3D *pLuces = NEW_ARRAY(CLuz3D, Header.uNumLuces);
  for (i = 0; i < Header.uNumLuces; i++)
  {
    TLuz3DHeaderv1 LuzHeader;
    CLuz3D *pLuz = &pLuces[i];
    pLuz->Init();

    // Header
    ReadData(pData, &LuzHeader, sizeof(TLuz3DHeaderv1));
    char pBuffer[32];
    sprintf_s(pBuffer, 32, "%d", i);
    pLuz->m_Nombre = pBuffer;

    // D3DLight
    switch (LuzHeader.uType)
    {
       case CLuz3D::OMNI:        pLuz->m_Light.Type = D3DLIGHT_POINT;       break;
       case CLuz3D::DIRECTIONAL: pLuz->m_Light.Type = D3DLIGHT_DIRECTIONAL; break;
       case CLuz3D::SPOT:        pLuz->m_Light.Type = D3DLIGHT_SPOT;        break;
    }
    pLuz->m_Light.Diffuse.r    = LuzHeader.fR;
    pLuz->m_Light.Diffuse.g    = LuzHeader.fG;
    pLuz->m_Light.Diffuse.b    = LuzHeader.fB;
    pLuz->m_Light.Attenuation0 = 1.f;
    pLuz->m_Light.Attenuation1 = 0.f;
    pLuz->m_Light.Attenuation2 = 0.f;
    pLuz->m_Light.Falloff      = 0.f;
    pLuz->m_Light.Range        = LuzHeader.fRange;

    // PosSrc
    unsigned *pFrameTimesPosSrc = NEW_ARRAY(unsigned, LuzHeader.uNumFramesPosSrc);
    TVector3 *pPosSrc           = NEW_ARRAY(TVector3, LuzHeader.uNumFramesPosSrc);
    ReadData(pData, pFrameTimesPosSrc, LuzHeader.uNumFramesPosSrc * sizeof(unsigned));
    ReadData(pData, pPosSrc,           LuzHeader.uNumFramesPosSrc * sizeof(TVector3));
    pLuz->SetPosSrc(pPosSrc, pFrameTimesPosSrc, LuzHeader.uNumFramesPosSrc);

    // PosTgt
    /*
    unsigned *pFrameTimesPosTgt = NEW_ARRAY(unsigned, LuzHeader.uNumFramesPosTgt);
    TVector3 *pPosTgt           = NEW_ARRAY(TVector3, LuzHeader.uNumFramesPosTgt);
    ReadData(pData, pFrameTimesPosTgt, LuzHeader.uNumFramesPosTgt * sizeof(unsigned));
    ReadData(pData, pPosTgt,           LuzHeader.uNumFramesPosTgt * sizeof(TVector3));
    pLuz->SetPosTgt(pPosTgt, pFrameTimesPosTgt, LuzHeader.uNumFramesPosTgt);

    // Falloff
    unsigned *pFrameTimesFalloff = NEW_ARRAY(unsigned, LuzHeader.uNumFramesFalloff);
    float    *pFalloff           = NEW_ARRAY(float,    LuzHeader.uNumFramesFalloff);
    ReadData(pData, pFrameTimesFalloff, LuzHeader.uNumFramesFalloff * sizeof(unsigned));
    ReadData(pData, pFalloff,           LuzHeader.uNumFramesFalloff * sizeof(float));
    pLuz->SetFalloff(pFalloff, pFrameTimesFalloff, LuzHeader.uNumFramesFalloff);
    */
  }
  pEscena3D->SetLuces(pLuces, Header.uNumLuces);

  return true;
}
