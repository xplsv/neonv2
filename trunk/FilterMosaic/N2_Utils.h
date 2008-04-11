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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <d3d9.h>
#include "N2_FilterDLL.h"

#define OffsetOf(s,m)   (size_t)&(((s *)0)->m)
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//---------------------------------------------------------------------------//
// Tools for D3D
//---------------------------------------------------------------------------//
#define HCV_SET_XYZRHW(_pV,_fX,_fY,_fZ,_fW)   { (_pV)->Pos.x = _fX; (_pV)->Pos.y = _fY; (_pV)->Pos.z = _fZ; (_pV)->Pos.w = _fW; }
#define HCV_SET_COLOR(_pV,_Diffuse)           { (_pV)->Diffuse = _Diffuse;                         }
#define HCV_SET_ARGB(_pV,_uA,_uR,_uG,_uB)     { (_pV)->Diffuse = HARD_COLOR_ARGB(_uA,_uR,_uG,_uB); }
#define HCV_SET_UV0(_pV,_fU,_fV)              { (_pV)->UV0.x = _fU; (_pV)->UV0.y = _fV;            }
#define HCV_SET_UV1(_pV,_fU,_fV)              { (_pV)->UV1.x = _fU; (_pV)->UV1.y = _fV;            }

#define HCV_DECL(Name) struct TVertex_##Name## {
#define HCV_DECL_END() };

//---------------------------------------------------------------------------//
// Fixed Vertex
//---------------------------------------------------------------------------//
struct TVertex_XYZRHW
{
  TVector4  Pos;            // Vertice (transformado)
  unsigned  Diffuse;        // Diffuse
  D3DVECTOR UV0;            // Coordenadas de textura
  D3DVECTOR UV1;            // Coordenadas de textura
};

//---------------------------------------------------------------------------//
// CUtils class
//---------------------------------------------------------------------------//
class CUtils
{
  public:
                      CUtils      () { m_bOk = false; }
                     ~CUtils      () { End(); }

    bool              IsOk        () { return m_bOk; }
    IDirect3DDevice9 *GetD3DDevice() { return m_pD3DDevice; }

    //----------------------------------------------------------------------------
    // Init
    //----------------------------------------------------------------------------
    bool Init(IDirect3DDevice9 *pD3DDevice, int iVertices)
    {
      m_pVB        = NULL;
      m_pDecl      = NULL;
      m_pD3DDevice = pD3DDevice;
      m_iVertices  = iVertices;
      HRESULT hRet = m_pD3DDevice->CreateVertexBuffer(m_iVertices * sizeof(TVertex_XYZRHW),
                                                      D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
                                                      0,
                                                      D3DPOOL_DEFAULT,
                                                      &m_pVB,
                                                      NULL);
      if (SUCCEEDED(hRet))
      {
        D3DVERTEXELEMENT9 Elements[] =
        {
          {0, OffsetOf(TVertex_XYZRHW, Pos),     D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
          {0, OffsetOf(TVertex_XYZRHW, Diffuse), D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0},
          {0, OffsetOf(TVertex_XYZRHW, UV0),     D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0},
          {0, OffsetOf(TVertex_XYZRHW, UV1),     D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1},
          D3DDECL_END(),
        };
        hRet = m_pD3DDevice->CreateVertexDeclaration(Elements, &m_pDecl);
        if (SUCCEEDED(hRet))
        {
          m_bOk = true;
        }
      }
      return (IsOk());
    }

    //----------------------------------------------------------------------------
    // End
    //----------------------------------------------------------------------------
    void End()
    {
      if (IsOk())
      {
        SAFE_RELEASE(m_pVB);
        SAFE_RELEASE(m_pDecl);
        m_bOk = false;
      }
    }

    //----------------------------------------------------------------------------
    // LockVertexBuffer
    //----------------------------------------------------------------------------
    TVertex_XYZRHW *LockVertexBuffer(int iVertices)
    {
      TVertex_XYZRHW *pVertices;
      m_pVB->Lock(0, sizeof(TVertex_XYZRHW) * iVertices, (void **)&pVertices, D3DLOCK_DISCARD);
      return (pVertices);
    }

    //----------------------------------------------------------------------------
    // UnlockVertexBuffer
    //----------------------------------------------------------------------------
    void UnlockVertexBuffer()
    {
      m_pVB->Unlock();
    }     
    
    //----------------------------------------------------------------------------
    // DrawPrimitive
    //----------------------------------------------------------------------------
    void DrawPrimitive(int iPrimitive, int iNumPrimitives)
    {
      m_pD3DDevice->SetVertexDeclaration(m_pDecl);
      m_pD3DDevice->SetStreamSource     (0, m_pVB, 0, sizeof(TVertex_XYZRHW));
      m_pD3DDevice->DrawPrimitive       ((D3DPRIMITIVETYPE)iPrimitive, 0, iNumPrimitives);
    }

    //---------------------------------------------------------------------------//
    // DrawQuad
    //
    //---------------------------------------------------------------------------//
    void DrawQuad(TVectorI2 const &v0, TVectorI2 const &v1, TVector2 const &uv0, TVector2 const &uv1)
    {
      TVertex_XYZRHW *pVertices = LockVertexBuffer(4);

      HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
      HCV_SET_XYZRHW(pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.0f);
      HCV_SET_UV0   (pVertices, uv0.x, uv0.y);
      pVertices++;

      HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
      HCV_SET_XYZRHW(pVertices, v1.x-0.5f, v0.y-0.5f, 0.0f, 1.0f);
      HCV_SET_UV0   (pVertices, uv1.x, uv0.y);
      pVertices++;

      HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
      HCV_SET_XYZRHW(pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.0f);
      HCV_SET_UV0   (pVertices, uv1.x, uv1.y);
      pVertices++;

      HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
      HCV_SET_XYZRHW(pVertices, v0.x-0.5f, v1.y-0.5f, 0.0f, 1.0f);
      HCV_SET_UV0   (pVertices, uv0.x, uv1.y);
      pVertices++;
  
      UnlockVertexBuffer();
      DrawPrimitive     (D3DPT_TRIANGLEFAN, 2);
    }

  private:

    bool                         m_bOk;
    int                          m_iVertices;
    IDirect3DDevice9            *m_pD3DDevice;
    LPDIRECT3DVERTEXBUFFER9      m_pVB;
    LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
};

#endif
