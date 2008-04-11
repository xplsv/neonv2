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
#include "3DSFormat.h"

//---------------------------------------------------------------------------//
struct T3DSMaterial
{
  char pMaterial[80];
};

struct T3DSObject
{
  ushort     uNumFaces;
  ushort     uNumUV;
  ushort     uNumVertices;
  ushort     uNumPosiciones;
  ushort     uNumRotaciones;
  ushort     uNumEscalados;
  TFace     *pFaces;
  TVector2  *pUV;
  unsigned  *pFrameTimesVertices;
  TVector3  *pVertices;
  unsigned  *pFrameTimesPosiciones;
  TVector3  *pPosiciones;
  unsigned  *pFrameTimesRotaciones;
  TVector4  *pRotaciones;
  unsigned  *pFrameTimesEscalados;
  TVector3  *pEscalados;
};

struct T3DSCamera
{
  unsigned *pFrameTimesPosSrc;
  TVector3 *pPosSrc;
  unsigned *pFrameTimesPosTgt;
  TVector3 *pPosTgt;
  unsigned *pFrameTimesPosUp;
  TVector3 *pPosUp;
  unsigned *pFrameTimesFov;
  float    *pFov;
};

struct T3DSLight
{
  unsigned *pFrameTimesPosSrc;
  TVector3 *pPosSrc;
  unsigned *pFrameTimesPosTgt;
  TVector3 *pPosTgt;
  unsigned *pFrameTimesFalloff;
  float    *pFalloff;
};

T3DSMaterial *s_pMaterials;
CObjeto3D    *s_pObjects;
T3DSObject   *s_p3DSObjects;
CCamara3D    *s_pCameras;
T3DSCamera   *s_p3DSCameras;
CLuz3D       *s_pLights;
T3DSLight    *s_p3DSLights;


//---------------------------------------------------------------------------//
// Count chunks
//
//---------------------------------------------------------------------------//
static int Count3DSMaterials(CChunker *pChunker)
{
  unsigned uCount = 0;
  pChunker->First();
  do
  {
    if (pChunker->Enter() == EDIT_MATERIAL) uCount++;
    pChunker->Leave();
  } while (!pChunker->EndOf());
  return uCount;
}


static int Count3DSObjects(CChunker *pChunker, int iObjType)
{
  unsigned uCount = 0;
  pChunker->First();
  do
  {
    if (pChunker->Enter() == EDIT_OBJECT)
    {
      char pBuffer[80];
      pChunker->ReadStr(pBuffer);
      if (pChunker->Enter() == iObjType) uCount++;
      pChunker->Leave();
    }
    pChunker->Leave();
  } while (!pChunker->EndOf());
  return uCount;
}

static int GetMaterialID(const char *pszMaterial, unsigned uNumMateriales)
{
  for (unsigned i = 0; i < uNumMateriales; i++)
  {
    if (!Stricmp(pszMaterial, s_pMaterials[i].pMaterial))
      return i;
  }
  return -1;
};

//---------------------------------------------------------------------------//
// Load Objects
//
//---------------------------------------------------------------------------//
void CSceneLoader::Load3DSObjects(CEscena3D *pEscena3D, CChunker *pChunker)
{
  // Cargar materiales
  unsigned uNumMateriales = Count3DSMaterials(pChunker);

  // Crear array de materiales
  int *pMateriales = NULL;
  if (uNumMateriales > 0)
  {
    pMateriales = NEW_ARRAY(int,          uNumMateriales);
    s_pMaterials= NEW_ARRAY(T3DSMaterial, uNumMateriales);
  }

  // Luego se cargan
  int i = 0;
  pChunker->First();
  while (pChunker->Find(EDIT_MATERIAL))
  {
    if (pChunker->Find(MAT_NAME))
    {
      pMateriales[i] = -1;
      pChunker->ReadStr(s_pMaterials[i].pMaterial);
      pMateriales[i] = g_pGestorMateriales->AddMaterial(s_pMaterials[i].pMaterial);
      if (pMateriales[i] < 0)
      {
        // Si no se puede cargar el material, intentaremos ponerle uno flat
        GLOG(("ERR: Can't load material %s. Setting default flat material\n", s_pMaterials[i].pMaterial));
        /*
        pMateriales[i] = g_pGestorMateriales->AddMaterial("flat.nmt");
        // Si aun asi no se puede cargar, 
        if (pMateriales[i] < 0)
        {
          GLOG(("ERR: Can't load standard material flat.nmt\n"));
          return false;
        }
        */
      }
      i++;
      pChunker->Leave();
    }
    pChunker->Leave();
  };
  pEscena3D->SetMateriales(pMateriales, uNumMateriales);


  // Contar 
  s_pObjects    = NULL;
  s_p3DSObjects = NULL;
  s_pCameras    = NULL;
  s_p3DSCameras = NULL;
  s_pLights     = NULL;
  s_p3DSLights  = NULL;

  unsigned uNumObjects = Count3DSObjects(pChunker, OBJ_TRIMESH);
  unsigned uNumCameras = Count3DSObjects(pChunker, OBJ_CAMERA );
  unsigned uNumLights  = Count3DSObjects(pChunker, OBJ_LIGHT  );

  //-----------------------------------------------------------
  // Cargar objetos
  if (uNumObjects > 0)
  {
    s_p3DSObjects = NEW_ARRAY(T3DSObject, uNumObjects);
    s_pObjects    = NEW_ARRAY(CObjeto3D,  uNumObjects);

    // Cargar
    int i = 0;
    pChunker->First();
    while (pChunker->Find(EDIT_OBJECT))
    {
      char pBuffer[80];
      pChunker->ReadStr(pBuffer);
      if (pChunker->Find(OBJ_TRIMESH))
      {
        T3DSObject *p3DSObj = &s_p3DSObjects[i];
        
        // Vertex List
        pChunker->First();
        if (pChunker->Find(TRI_VERTEXL))
        {
          pChunker->ReadData(&p3DSObj->uNumVertices, sizeof(ushort));
          p3DSObj->pVertices = NEW_ARRAY(TVector3, p3DSObj->uNumVertices);
          p3DSObj->pFrameTimesVertices    = NEW_ARRAY(unsigned, 1);
          p3DSObj->pFrameTimesVertices[0] = 0;
          for (int j = 0; j < p3DSObj->uNumVertices; j++)
          {
            pChunker->ReadData(&p3DSObj->pVertices[j].x, sizeof(float));
            pChunker->ReadData(&p3DSObj->pVertices[j].y, sizeof(float));
            pChunker->ReadData(&p3DSObj->pVertices[j].z, sizeof(float));
          }
          pChunker->Leave();
        }

        // Face List
        pChunker->First();
        if (pChunker->Find(TRI_FACEL))
        {
          pChunker->ReadData(&p3DSObj->uNumFaces, sizeof(ushort));
          p3DSObj->pFaces = NEW_ARRAY(TFace, p3DSObj->uNumFaces);
          for (int j = 0; j < p3DSObj->uNumFaces; j++)
          {
            pChunker->ReadData(&p3DSObj->pFaces[j].i0, sizeof(ushort));
            pChunker->ReadData(&p3DSObj->pFaces[j].i1, sizeof(ushort));
            pChunker->ReadData(&p3DSObj->pFaces[j].i2, sizeof(ushort));
            p3DSObj->pFaces[j].iMat = 0;
          }
          pChunker->Leave();
        }

        // Face Materials
        pChunker->First();
        if (pChunker->Find(TRI_FACEM))
        {
          char pMat[80];
          pChunker->ReadStr(pMat);
          int iMat = GetMaterialID(pMat, uNumMateriales);
          if (iMat >= 0)
          {
            ushort uFace, uFaces;
            pChunker->ReadData(&uFaces, sizeof(ushort));
            for (unsigned k = 0; k < uFaces; k++)
            {
              pChunker->ReadData(&uFace, sizeof(ushort));
              p3DSObj->pFaces[uFace].iMat = iMat;
            }
          }
          pChunker->Leave();
        }
        
        i++;
        pChunker->Leave();
      }
      pChunker->Leave();
    }
  }
  pEscena3D->SetObjetos(s_pObjects, uNumObjects);

  //-----------------------------------------------------------
  // Cargar Camaras
  if (uNumCameras > 0)
  {
    s_p3DSCameras = NEW_ARRAY(T3DSCamera, uNumCameras);
    s_pCameras    = NEW_ARRAY(CCamara3D,  uNumCameras);

    // Cargar
    int i = 0;
    pChunker->First();
    while (pChunker->Find(EDIT_OBJECT))
    {
      char pBuffer[80];
      pChunker->ReadStr(pBuffer);
      if (pChunker->Find(OBJ_CAMERA))
      {
      }
      pChunker->Leave();
    }
  }
  pEscena3D->SetCamaras(s_pCameras, uNumCameras);

  //-----------------------------------------------------------
  // Cargar Luces
  if (uNumLights > 0)
  {
    s_p3DSLights = NEW_ARRAY(T3DSLight, uNumLights);
    s_pLights    = NEW_ARRAY(CLuz3D,    uNumLights);

    // Cargar
    int i = 0;
    pChunker->First();
    while (pChunker->Find(EDIT_OBJECT))
    {
      char pBuffer[80];
      pChunker->ReadStr(pBuffer);
      if (pChunker->Find(OBJ_LIGHT))
      {
      }
      pChunker->Leave();
    }
  }
  pEscena3D->SetLuces(s_pLights, uNumLights);
}


//---------------------------------------------------------------------------//
// Load Keys
//
//---------------------------------------------------------------------------//
void CSceneLoader::Load3DSKeys(CEscena3D *pEscena3D, CChunker *pChunker)
{
}

//---------------------------------------------------------------------------//
// Load3DS
//
//---------------------------------------------------------------------------//
bool CSceneLoader::Load3DS(CEscena3D *pEscena3D, const char *pData, unsigned uDataSize)
{
  CChunker *pChunker = NEW CChunker(pData);

  // Datos que seguramente no se cargaran
  pEscena3D->m_fAmbientR = 0.f;
  pEscena3D->m_fAmbientG = 0.f;
  pEscena3D->m_fAmbientB = 0.f;

  // Main chunk
  pChunker->Enter();  

  // Cargar 
  if (pChunker->Find(KEYF3DS))
  {
    // Leer datos de los keyframes
    if (pChunker->Find(KEYF_SEG))
    {
      int iFirstFrame, iLastFrame;
      pChunker->ReadData(&iFirstFrame, sizeof(int));
      pChunker->ReadData(&iLastFrame,  sizeof(int));
      pEscena3D->m_fFirstFrame    = (float)iFirstFrame;
      pEscena3D->m_fLastFrame     = (float)iLastFrame;
      pEscena3D->m_fFrameSpeed    = 1;
      pEscena3D->m_fTicksPerFrame = 30.f;
      pEscena3D->m_fFPS           = pEscena3D->m_fFrameSpeed * pEscena3D->m_fTicksPerFrame;
    }
    pChunker->Leave();
  }
  pChunker->Leave();

  // 3DSData
  pChunker->First();
  do
  {
    // Current Chunk
    switch (pChunker->Enter())
    {
      case EDIT3DS: Load3DSObjects(pEscena3D, pChunker); break;
      case KEYF3DS: Load3DSKeys   (pEscena3D, pChunker); break;
    }
    pChunker->Leave();
  } while (!pChunker->EndOf());

  DISPOSE(pChunker);
  /*


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
    sprintf(pObj->m_pNombre, ObjHeader.pNombre);
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
    sprintf (pCam->m_pNombre, "%d", CamHeader.uID);
  
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
    sprintf (pLuz->m_pNombre, "%d", i);

    // PosSrc
    unsigned *pFrameTimesPosSrc = NEW_ARRAY(unsigned, LuzHeader.uNumFramesPosSrc);
    TVector3 *pPosSrc           = NEW_ARRAY(TVector3, LuzHeader.uNumFramesPosSrc);
    ReadData(pData, pFrameTimesPosSrc, LuzHeader.uNumFramesPosSrc * sizeof(unsigned));
    ReadData(pData, pPosSrc,           LuzHeader.uNumFramesPosSrc * sizeof(TVector3));
    pLuz->SetPosSrc(pPosSrc, pFrameTimesPosSrc, LuzHeader.uNumFramesPosSrc);

    // D3DLight
    pLuz->m_Light.Type         = D3DLIGHT_POINT;
    pLuz->m_Light.Diffuse.r    = LuzHeader.fR;
    pLuz->m_Light.Diffuse.g    = LuzHeader.fG;
    pLuz->m_Light.Diffuse.b    = LuzHeader.fB;
    pLuz->m_Light.Attenuation0 = 1.f;
    pLuz->m_Light.Attenuation1 = 0.f;
    pLuz->m_Light.Attenuation2 = 0.f;
    pLuz->m_Light.Falloff      = 0.f;
    pLuz->m_Light.Range        = LuzHeader.fRange;
  }
  pEscena3D->SetLuces(pLuces, Header.uNumLuces);
  */

  return false;
}
