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

#ifndef GESHADER_H
#define GESHADER_H

#include "GEDisplayDevice.h"

class CShader
{
  public:
    enum
    {
      NEON,
      D3DEFFECT,
    };

  public:
                        CShader          () { m_bOk = false; }
                       ~CShader          () { End(); }

    bool                Init             (const string &sFile);
    void                End              ();
    bool                IsOk             () const { return m_bOk; }

    void                Release          ();
    bool                Restore          ();

    unsigned            BeginDraw        (unsigned uTech);
    void                EndDraw          ();
    void                BeginPass        (unsigned uPass);
    void                EndPass          ();

    bool                FixedPipeline    () const;

    const string       &GetFilename      () const { return m_Filename; }
    
    void                AddRef           () { m_iCount++; }
    bool                SubRef           () { m_iCount--; return (m_iCount > 0); }

    void                SetID            (int iID) { m_iID = iID; }
    int                 GetID            () const { return m_iID; }

    int                 GetNumTechniques ()               { return m_iTechniques; }
    const char         *GetTechniqueName (unsigned uTech) { return m_pTechniquesName[uTech]; }
    D3DXHANDLE          GetTechnique     (unsigned uTech) { return m_pTechniques[uTech];     }

  private:

    void                BorraVars        ();
    void                LiberaVars       ();

    bool                LoadShaderNeon   (const string &pszFile);
    bool                LoadShaderD3D    (const string &pszFile);

    unsigned            GetTextureOp     (const char *pValue);
    unsigned            GetTextureArg    (const char *pValue);

  private:

    bool                m_bOk;
    int                 m_iID;
    int                 m_iType;
    int                 m_iCount;
    int                 m_iTechniques;
    D3DXHANDLE          m_pTechniques[16];
    char                m_pTechniquesName[16][32];
    char                m_Dir[MAX_PATH];
    string              m_Filename;
    D3DSTATEBLOCK      *m_pStateBlock;
    D3DVS              *m_pVS;
    D3DPS              *m_pPS;
    LPD3DXEFFECT        m_pD3DEffect;
    // Parametros
    struct TParam
    {
      int        iRegister;
      D3DXHANDLE hParam;
    };
    int                 m_iNumParameters;
    TParam             *m_pParameters;
};

#endif
