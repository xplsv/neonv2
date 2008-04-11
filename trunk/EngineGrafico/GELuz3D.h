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

#ifndef GELUZ3D_H
#define GELUZ3D_H

class  CPath;
class  CSceneLoader;
struct TCtrlVar;

class CLuz3D
{
  friend class CSceneLoader;

  enum EType
  {
    OMNI,
    DIRECTIONAL,
    SPOT,
  };
  
  public:
                     CLuz3D      () { m_bOk = false; }
                    ~CLuz3D      () { End(); }

    bool             Init        ();
    void             End         ();
    bool             IsOk        () const { return  m_bOk; }

    void             SetTime     (float fTime);

    const string    &GetName     () const { return m_Nombre; }
    D3DLIGHT9       *GetD3DLight ()       { return &m_Light; }

    static TCtrlVar *GetVars     ();
    void             SetVar      (int iVar, void *pData);
    void            *GetVar      (int iVar);

    // Loader
    void             SetPosSrc   (TVector3 *pVertices, unsigned *pTimes, unsigned uFrames);
    void             SetPosTgt   (TVector3 *pVertices, unsigned *pTimes, unsigned uFrames);
    void             SetFalloff  (float    *pFalloff,  unsigned *pTimes, unsigned uFrames);

  private:          

    void             Update      ();
    void             BorraVars   ();
    void             LiberaVars  ();

  private:
    
    bool             m_bOk;

    string           m_Nombre;
    ushort           m_uType;
    float            m_fR, m_fG, m_fB;
    float            m_fIntens;
    float            m_fRange;
    CPath           *m_pPathSrc;
    CPath           *m_pPathTgt;
    CPath           *m_pPathFalloff;
    D3DLIGHT9        m_Light;
    // Vars
    float            m_fChangeIntens;
    float            m_fChangeRange;
};

#endif
