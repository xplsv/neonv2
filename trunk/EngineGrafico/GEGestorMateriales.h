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

#ifndef GEGESTORMATERIALES_H
#define GEGESTORMATERIALES_H

class CMaterial;
class CTextura;
class CShader;

class  CGestorMateriales;
extern CGestorMateriales *g_pGestorMateriales;

class CGestorMateriales
{
  public:
                CGestorMateriales () { m_bOk = false; }
               ~CGestorMateriales () { End(); }

    bool        Init              ();
    void        End               ();
    bool        IsOk              () const { return m_bOk; }

    void        Release           ();
    void        Restore           ();

    // Materiales
    int         AddMaterial       (const string &sFile);
    void        RemoveMaterial    (int iMat);
    CMaterial  *GetMaterial       (int iMat);

    // Texturas
    int         AddTextura        (const string &sFile, unsigned uFlags=0);
    int         AddTextura        (int iWidth, int iHeight, int iFormat, bool bDynamic = false, bool bRenderTarget = false, bool bDepthStencil = false, unsigned uFlagss=0);
    void        RemoveTextura     (int iTex);
    CTextura   *GetTextura        (int iTex);

    // Shaders
    int         AddShader         (const string &sFile);
    void        RemoveShader      (int iTex);
    CShader    *GetShader         (int iTex);

    void        SetTextura        (int iTex, int iStage);

  private:

    void        BorraVars         ();
    void        LiberaVars        ();

  private:

    bool                m_bOk;
    int                 m_iIDNextMaterial;
    int                 m_iIDNextTextura;
    int                 m_iIDNextShader;
    CLista<CMaterial *> m_ListaMateriales;
    CLista<CTextura  *> m_ListaTexturas;
    CLista<CShader   *> m_ListaShaders;
    // Default materials
    int                 m_iIDFlatMaterial;
    int                 m_iIDWireMaterial;
};

#endif
