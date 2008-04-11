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

#ifndef GETEXTURA_H
#define GETEXTURA_H

#include "GEVector.h"
#include "GEDisplayDevice.h"
#include <d3d9.h>

struct TSurfaceDesc
{
  int   iWidth;
  int   iHeight;
  int   iPitch;
  void *pBits;
};

class CTextura
{
  public:
    enum
    {
      CUBE       = 1<<1,
      SYSTEMMEM  = 1<<2,
    };

  public:
                        CTextura     () { m_bOk = false; }
                       ~CTextura     () { End(); }

    bool                Init         (const string &sFile, unsigned uFlags=0);
    bool                Init         (int iWidth, int iHeight, int iFormat, bool bDynamic = false, bool bRenderTarget = false, bool bDepthStencil = false, unsigned uFlags=0);
    void                End          ();
    bool                IsOk         () const { return m_bOk; }
    
    void                Release      ();
    void                Restore      ();

    int                 GetWidth     () const { return m_iWidth;  }
    int                 GetHeight    () const { return m_iHeight; }
    int                 GetFormat    () const { return m_iFormat; }
    const string       &GetFilename  () const { return m_Filename; }
    
    bool                Lock         (TRect const *pRect, TSurfaceDesc &Desc);
    bool                Unlock       ();

    void                AddRef       () { m_iCount++; }
    bool                SubRef       () { m_iCount--; return (m_iCount > 0); }

    void                Set          (int iStage);

    D3DTEXTURE         *GetTexturaD3D() { return m_pTextura; }
    D3DSURFACE         *GetSurfaceD3D() { return m_pSurface; }

    void                SetFlags     (unsigned uFlags) { m_uFlags = true; }
    unsigned            GetFlags     () const { return m_uFlags; }

    void                SetID        (int iID) { m_iID = iID; }
    int                 GetID        () const { return m_iID; }

  private:

    bool                m_bOk;
    int                 m_iID;
    bool                m_bLocked;
    char                m_Dir[MAX_PATH];
    string              m_Filename;
    int                 m_iCount;
    int                 m_iWidth;
    int                 m_iHeight;
    int                 m_iFormat;
    bool                m_bDynamic;
    unsigned            m_uUsage;
    unsigned            m_uPool;
    unsigned            m_uFlags;
    D3DTEXTURE         *m_pTextura;
    D3DSURFACE         *m_pSurface;
};

#endif
