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

#ifndef GEESCENA3D_H
#define GEESCENA3D_H

#include "Lista.h"
#include <d3dx9.h>

class  CDisplayDevice;
class  CMaterial;
class  CObjeto3D;
class  CCamara3D;
class  CLuz3D;
class  CSceneLoader;
struct TCtrlVar;

enum
{
  FXS_3DOBJECT = 0,
  FXS_3DLIGHT  = 1,
  FXS_3DCAMERA = 2,
  FXS_3DFOG    = 3,
};

class CEscena3D
{
  friend class CSceneLoader;

  public:
    enum
    {
      WAVE_TIME  = 1<<0,
      WAVE_XFORM = 1<<1,
    };

  public:
                           CEscena3D      () { m_bOk = false; }
                          ~CEscena3D      () { End(); }

    bool                   Init           (const string &sFile);
    void                   End            ();
    bool                   IsOk           () const { return m_bOk; }

    void                   Reset          (float fTime);

    void                   SetTime        (float fTime);
    void                   SetCamara      (unsigned iCam) { m_iCamaraActiva = iCam; }
    CCamara3D             *GetCamara      (unsigned iCam);
    float                  GetLength      () const { return (m_fLastFrame - m_fFirstFrame) / m_fFPS; }

    void                   GetVarObjects  (vector<string> &Objects, int iScope) const;
    void                   SetVar         (int iScope, int iObj, int iVar, void *pData);
    void                  *GetVar         (int iScope, int iObj, int iVar);
    TCtrlVar              *GetVars        (int iScope) const;

    int                    GetCamaraActiva() const { return m_iCamaraActiva; }
    int                    GetNumCamaras  () const { return m_uNumCamaras; }
    vector<string> const  &GetCamaraNames () const { return m_CameraNames; }

    void                   Draw           (CDisplayDevice *pDD);

    // For Loader
    void                   SetMateriales  (int       *pMateriales, unsigned uCant);
    void                   SetObjetos     (CObjeto3D *pObjetos,    unsigned uCant);
    void                   SetCamaras     (CCamara3D *pCamaras,    unsigned uCant);
    void                   SetLuces       (CLuz3D    *pLuces,      unsigned uCant);

    private:

    void                   PreparaCamara  (CDisplayDevice *pDD);
    void                   PreparaLuces   (CDisplayDevice *pDD);
    void                   PreparaNiebla  (CDisplayDevice *pDD);
    void                   PreparaObjetos (CDisplayDevice *pDD);
    void                   DrawObjetos    (CDisplayDevice *pDD);

    void                   BorraVars      ();
    void                   LiberaVars     ();

    private:

    bool                   m_bOk;
    float                  m_fTime;
    unsigned               m_iCamaraActiva;
    D3DXMATRIX             m_MatrixView;
    D3DXMATRIX             m_MatrixProj;
    D3DXMATRIX             m_MatrixVP;
    // Datos de la escena
    float                  m_fFirstFrame;
    float                  m_fLastFrame;
    float                  m_fFrameSpeed;
    float                  m_fTicksPerFrame;
    float                  m_fFPS;
    float                  m_fAmbientR;
    float                  m_fAmbientG;
    float                  m_fAmbientB;
    // Lista de componentes
    ushort                 m_uNumMateriales;
    ushort                 m_uNumObjetos;
    ushort                 m_uNumCamaras;
    ushort                 m_uNumLuces;
    int                   *m_pMateriales;
    CObjeto3D             *m_pObjetos;
    CCamara3D             *m_pCamaras;
    CLuz3D                *m_pLuces;
    // Para la ordenacion de objetos en Z
    CObjeto3D            **m_ppSortObjetos;
    vector<string>         m_ObjectNames;
    vector<string>         m_LightNames;
    vector<string>         m_CameraNames;
    // Para la niebla
    bool                   m_bFog;
    float                  m_FogColorR;
    float                  m_FogColorG;
    float                  m_FogColorB;
    float                  m_FogNear;
    float                  m_FogFar;
    float                  m_FogDensity;
};

#endif
