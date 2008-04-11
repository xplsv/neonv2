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

#ifndef GEMATERIAL_H
#define GEMATERIAL_H

#define MAX_TEXTURES 4

class CMaterial
{
  public:
                        CMaterial        () { m_bOk = false; }
                       ~CMaterial        () { End(); }

    bool                Init             (const string &sFile);
    void                End              ();
    bool                IsOk             () const { return m_bOk; }

    void                Release          ();
    void                Restore          ();

    unsigned            BeginDraw        (unsigned uTech);
    void                EndDraw          ();
    void                BeginPass        (unsigned uPass);
    void                EndPass          ();

    const string       &GetFilename      () const { return m_Filename; }
    bool                GetEsMaterial    () const { return m_bMaterial; }
    int                 GetNumTexturas   () const { return m_iNumTexturas; }
    int                 GetIDTextura     (int iTex) const { return m_pTexturas[iTex]; }
    bool                FixedPipeline    () const;

    void                AddRef           () { m_iCount++; }
    bool                SubRef           () { m_iCount--; return (m_iCount > 0); }

    void                SetID            (int iID) { m_iID = iID; }
    int                 GetID            () const { return m_iID; }

  private:

    bool                LoadMaterialNeon (const string &sFile);
    bool                LoadMaterialFX   (const string &sFile);
    bool                LoadMaterialTex  (const string &sFile);
    void                BorraVars        ();
    void                LiberaVars       ();

  private:

    bool                m_bOk;
    int                 m_iID;
    string              m_Filename;
    bool                m_bMaterial;
    int                 m_iCount;
    int                 m_iNumTexturas;
    int                 m_pTexturas[MAX_TEXTURES];
    int                 m_iShader;
};

#endif
