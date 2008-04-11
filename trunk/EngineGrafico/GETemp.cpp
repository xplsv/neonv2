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
#include "GEEscena3D.h"


//---------------------------------------------------------------------------//
//
//---------------------------------------------------------------------------//
void DrawLine(CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1)
{
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 2);

  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_COLOR(pVertices, HARD_COLOR_ARGB(255,255,255,255));
  pVertices++;

  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_COLOR(pVertices, HARD_COLOR_ARGB(255,255,255,255));
  pVertices++;

  pDD->UnlockVertexBuffer(HCV_XYZ_T);
  pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_LINESTRIP, 1);
}


//---------------------------------------------------------------------------//
// File: GETemp.cpp
//
//---------------------------------------------------------------------------//
void DrawRect(CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1, TVector2 const &v2, TVector2 const &v3)
{
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 5);

  HCV_SET_XYZW (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_COLOR(pVertices, HARD_COLOR_ARGB(255,255,255,255));
  pVertices++;

  HCV_SET_XYZW (pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_COLOR(pVertices, HARD_COLOR_ARGB(255,255,255,255));
  pVertices++;

  HCV_SET_XYZW  (pVertices, v2.x-0.5f, v2.y-0.5f, 0.0f, 1.f);
  HCV_SET_COLOR(pVertices, HARD_COLOR_ARGB(255,255,255,255));
  pVertices++;

  HCV_SET_XYZW  (pVertices, v3.x-0.5f, v3.y-0.5f, 0.0f, 1.f);
  HCV_SET_COLOR(pVertices, HARD_COLOR_ARGB(255,255,255,255));
  pVertices++;

  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_COLOR(pVertices, HARD_COLOR_ARGB(255,255,255,255));
  pVertices++;
  
  pDD->UnlockVertexBuffer(HCV_XYZ_T);
  pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_LINESTRIP, 4);
}


//---------------------------------------------------------------------------//
// DrawQuad
//
//---------------------------------------------------------------------------//
void DrawQuad(CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1)
{
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 4);

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 0.f, 0.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 1.f, 0.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 1.f, 1.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 0.f, 1.f);
  pVertices++;
  
  pDD->UnlockVertexBuffer(HCV_XYZ_T);
  pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLEFAN, 2);
}


//---------------------------------------------------------------------------//
// DrawQuad
//
//---------------------------------------------------------------------------//
void DrawQuad(CDisplayDevice *pDD, bool bFlipY)
{
  TViewport const &Rect = pDD->GetViewport();
  if (bFlipY)
    DrawQuad(pDD, TVector2((float)Rect.x0, (float)Rect.y1), TVector2((float)Rect.x1, (float)Rect.y0));
  else
    DrawQuad(pDD, TVector2((float)Rect.x0, (float)Rect.y0), TVector2((float)Rect.x1, (float)Rect.y1));
}


//---------------------------------------------------------------------------//
// DrawQuad
//
//---------------------------------------------------------------------------//
void DrawQuadUV(CDisplayDevice *pDD, TVector2 const &uv0, TVector2 const &uv1, bool bFlipY)
{
  TViewport const &Rect = pDD->GetViewport();
  if (bFlipY)
    DrawQuadUV(pDD, TVector2((float)Rect.x0, (float)Rect.y1), TVector2((float)Rect.x1, (float)Rect.y0), uv0, uv1);
  else
    DrawQuadUV(pDD, TVector2((float)Rect.x0, (float)Rect.y0), TVector2((float)Rect.x1, (float)Rect.y1), uv0, uv1);
}

//---------------------------------------------------------------------------//
// DrawQuadUV
//
//---------------------------------------------------------------------------//
void DrawQuadUV(CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1, TVector2 const &uv0, TVector2 const &uv1)
{
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 4);

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, uv0.x, uv0.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, uv1.x, uv0.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, uv1.x, uv1.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, uv0.x, uv1.y);
  pVertices++;
  
  pDD->UnlockVertexBuffer(HCV_XYZ_T);
  pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLEFAN, 2);
}


//---------------------------------------------------------------------------//
// DrawQuadFlat
//
//---------------------------------------------------------------------------//
void DrawQuadFlat(CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1, unsigned uColor, float fZ)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  pDD->ApplyBasicShader();
  pD3D->SetRenderState(D3DRS_CULLMODE,      D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,       D3DZB_TRUE);
  pD3D->SetRenderState(D3DRS_ZWRITEENABLE,  D3DZB_TRUE);
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 4);

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, fZ, 1.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v0.y-0.5f, fZ, 1.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v1.y-0.5f, fZ, 1.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v1.y-0.5f, fZ, 1.f);
  pVertices++;
  
  pDD->UnlockVertexBuffer(HCV_XYZ_T);
  pDD->DrawPrimitive     (HCV_XYZ_T, HARD_PRIM_TRIANGLEFAN, 2);
}


//---------------------------------------------------------------------------//
// PreparaVertices
//
//---------------------------------------------------------------------------//
void PreparaVertices(TVertex_HCV_XYZ_T *pVertices, TVector2 const &v0, TVector2 const &v1, unsigned uColor)
{
  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 0.f, 0.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 1.f, 0.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 1.f, 1.f);
  pVertices++;
  
  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 0.f, 0.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 1.f, 1.f);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v1.y-0.5f, 0.0f, 1.f);
  HCV_SET_UV0   (pVertices, 0.f, 1.f);
  pVertices++;
}
