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

#ifndef FXIMAGEN_H
#define FXIMAGEN_H

#include "Effect.h"

class CDisplayDevice;

struct TImage
{
  int      iIDTex;
  int      iStretchMode;
  bool     bWaveXform;
  TVector2 vPosIni;
  TVector2 vPos;
  TVector2 vSpeed;
  TVector2 vXform;
};

class CFXImagen : public CEffect
{
  public:
    
    virtual             ~CFXImagen       () { End(); }

    virtual bool         Init            (const string &sFile);
    virtual void         End             ();

    virtual void         SetTime         (float fTime);
    virtual void         Draw            (CDisplayDevice *pDD, int iTextureTgt);

    virtual TCtrlVar    *GetVarCtrls     (int iScope);
    virtual void         SetVar          (int iScope, int iObj, int iVar, void *pData);
    virtual void        *GetVar          (int iScope, int iObj, int iVar);

  protected:

    int                  m_iImagen;
    TImage               m_Imagen;
    string               m_Nombre;
};

#endif
