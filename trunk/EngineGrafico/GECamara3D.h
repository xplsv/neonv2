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

#ifndef GECAMARA3D_H
#define GECAMARA3D_H

#include "N3DFormat.h"
#include "GEVector.h"

class  CPath;
class  CFichero;
struct TCtrlVar;

class CCamara3D
{
  friend class CSceneLoader;
  
  public:
                     CCamara3D    () { m_bOk = false; }
                    ~CCamara3D    () { End(); }

    bool             Init         ();
    void             End          ();
    bool             IsOk         () const { return m_bOk; }

    void             SetTime      (float fTime);
    void             Run          (float fRunTime);

    const string    &GetName      () const { return  m_Nombre;       }
    TVector3 const  *GetSrc       () const { return &m_vCachePosSrc; }
    TVector3 const  *GetTgt       () const { return &m_vCachePosTgt; }
    TVector3 const  *GetUp        () const { return &m_vCachePosUp;  }
    float            GetFov       () const { return  m_fCacheFov;    }

    static TCtrlVar *GetVars      ();
    void             SetVar       (int iVar, void *pData);
    void            *GetVar       (int iVar);

    // Loader
    void             SetPosSrc    (TVector3 *pVertices, unsigned *pTimes, unsigned uFrames);
    void             SetPosTgt    (TVector3 *pVertices, unsigned *pTimes, unsigned uFrames);
    void             SetPosUp     (TVector3 *pVertices, unsigned *pTimes, unsigned uFrames);
    void             SetFov       (float    *pFov,      unsigned *pTimes, unsigned uFrames);

  private:

    void             BorraVars    ();
    void             LiberaVars   ();
    void             ApplyFFT     ();

  private:          

    bool             m_bOk;
    string           m_Nombre;
    float            m_fNearClip;
    float            m_fFarClip;
    // Paths
    CPath           *m_pPathSrc;
    CPath           *m_pPathTgt;
    CPath           *m_pPathUp;
    CPath           *m_pPathFov;
    // Frame cacheado
    TVector3         m_vCachePosSrc;
    TVector3         m_vCachePosTgt;
    TVector3         m_vCachePosUp;
    float            m_fCacheFov;
    // Vars
    int              m_iFFTChan;
    float            m_fFFTXOff;
    float            m_fFFTYOff;
    float            m_fFFTZOff;
    float            m_fFFTDOff;
    float            m_fNoiseScale;
    float            m_fNoise;
    bool             m_bManualFov;
    float            m_fFov;
};

#endif
