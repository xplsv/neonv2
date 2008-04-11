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
#include "GEEffects.h"

using namespace CEffects;

static int s_pTexturas[TEX_NUM];
static int s_pShaders[SH_NUM];
static int s_Size   = 128;
static int s_SizeFX = 128;

//---------------------------------------------------------------------------//
// GetTexture/GetShader
//
//---------------------------------------------------------------------------//
int CEffects::GetTexture(int iTexEnum)
{
  return s_pTexturas[iTexEnum];
}

int CEffects::GetShader(int iShader)
{
  return s_pShaders[iShader];
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
void CEffects::Init()
{
  for (int i = 0; i < TEX_NUM; i++)
    s_pTexturas[i] = -1;
  for (int i = 0; i < SH_NUM; i++)
    s_pShaders[i] = -1;
  s_Size = 512;
  if      (g_DisplayDevice.Width() >= 1024 || g_DisplayDevice.Height() >= 1024) s_Size = 2048;
  else if (g_DisplayDevice.Width() >=  512 || g_DisplayDevice.Height() >=  512) s_Size = 1024;
  s_SizeFX = s_Size >> 2;

  // Texturas
  s_pTexturas[TEX_TARGET]        = g_pGestorMateriales->AddTextura(s_Size, s_Size, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_GLOW_MIPMAP_0] = g_pGestorMateriales->AddTextura(g_DisplayDevice.Width()>>1, g_DisplayDevice.Height()>>1, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_GLOW_PS_0]     = g_pGestorMateriales->AddTextura(s_SizeFX,   s_SizeFX, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_GLOW_PS_1]     = g_pGestorMateriales->AddTextura(s_SizeFX,   s_SizeFX, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_MOTION_BLUR]   = g_pGestorMateriales->AddTextura(g_DisplayDevice.Width()>>1, g_DisplayDevice.Height()>>1, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_BLUR_0]        = g_pGestorMateriales->AddTextura(s_SizeFX,   s_SizeFX, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_BLUR_1]        = g_pGestorMateriales->AddTextura(s_SizeFX,   s_SizeFX, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_TEMP_0]        = g_pGestorMateriales->AddTextura(g_DisplayDevice.Width()>>1, g_DisplayDevice.Height()>>1, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  s_pTexturas[TEX_SPRITER]       = g_pGestorMateriales->AddTextura(g_DisplayDevice.Width(),    g_DisplayDevice.Height(),    HARD_TEX_FORMAT_A8R8G8B8, false, false, false, CTextura::SYSTEMMEM);
  // Shaders
  PushDir("resources");
  s_pShaders[SH_BLURXGLOW] = g_pGestorMateriales->AddShader("blurxglow.fx");
  s_pShaders[SH_BLURX]     = g_pGestorMateriales->AddShader("blurx.fx");
  s_pShaders[SH_BLURY]     = g_pGestorMateriales->AddShader("blury.fx");
  s_pShaders[SH_HEAT1]     = g_pGestorMateriales->AddShader("heat1.fx");
  s_pShaders[SH_HEAT2]     = g_pGestorMateriales->AddShader("heat2.fx");
  PopDir();
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CEffects::End()
{
  for (int i = 0; i < TEX_NUM; i++)
    g_pGestorMateriales->RemoveTextura(s_pTexturas[i]);
  for (int i = 0; i < SH_NUM; i++)
    g_pGestorMateriales->RemoveShader(s_pShaders[i]);
}


//---------------------------------------------------------------------------//
// Restore
//
//---------------------------------------------------------------------------//
void CEffects::Restore()
{
}


//---------------------------------------------------------------------------//
// DoRadialBlur
//
//---------------------------------------------------------------------------//
void CEffects::DoRadialBlur(CDisplayDevice *pDD, int iIDTexSrc, int iRadio2, float fIntens, TVector2 const &vPos)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  // Calcular el radio^2
  //int nRadius = (int)pow(2, iRadio2);
  //if(nRadius < 1) nRadius = 1;
  int nRadius = (int)iRadio2;
  if(nRadius < 1) nRadius = 1;

  // Tamaños
  CTextura *pSrc = g_pGestorMateriales->GetTextura(iIDTexSrc);
  CTextura *pDst = g_pGestorMateriales->GetTextura(s_pTexturas[TEX_TARGET]);
  int iW = pSrc->GetWidth();
  int iH = pSrc->GetHeight();
  RECT Rect = {0,0, iW,iH};
  float fU = (float)iW / pDst->GetWidth();
  float fV = (float)iH / pDst->GetHeight();

  // Compute blur direction
  TVector2 vDir;
  vDir.x = (0.5f - vPos.x) * ((float)nRadius / iW) * 2.f;
  vDir.y = (0.5f - vPos.y) * ((float)nRadius / iH) * 2.f;

  // States
  pDD->SetRenderTarget(iIDTexSrc);
  pDD->ApplyBasicShader();
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pDD->SetBilinearFiltering(0, true);
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_TARGET], 0);

  float fHalfWidth  = (float)iW * 0.5f;
  float fHalfHeight = (float)iH * 0.5f;
  int   nCurRadius  = 1;
  float fAlpha      = fIntens;
  while(nCurRadius < nRadius)
  {
    pDD->EndScene();
    pD3D->StretchRect(pSrc->GetSurfaceD3D(), &Rect, pDst->GetSurfaceD3D(), &Rect, D3DTEXF_POINT);
    pDD->BeginScene();

    // Calcular el quad
    float fPos     = (float)nCurRadius / (float)nRadius;
    float fCenterX = (0.5f + (vDir.x * fPos)) * iW;
    float fCenterY = (0.5f + (vDir.y * fPos)) * iH;

    // Compute some parameters
    float fLeft   = fCenterX - fHalfWidth  - nCurRadius;
    float fTop    = fCenterY - fHalfHeight - nCurRadius;
    float fRight  = fCenterX + fHalfWidth  + nCurRadius;
    float fBottom = fCenterY + fHalfHeight + nCurRadius;

    unsigned uAlpha = int(fAlpha * 255.f);
    pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, (uAlpha<<24));

    // Draw Quad
    DrawQuadUV(pDD, TVector2(fLeft,fTop), TVector2(fRight,fBottom), TVector2(0.f,0.f), TVector2(fU, fV));

    nCurRadius = nCurRadius * 2;
  }
}


//---------------------------------------------------------------------------//
// DoRadialBlurSteps
//
//---------------------------------------------------------------------------//
void CEffects::DoRadialBlurSteps(CDisplayDevice *pDD, int iIDTexSrc, float fRadio, float fIntens, int nSteps, TVector2 const &vPos)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  float fRadius = fRadio;
  if(fRadius < 1) fRadius = 1;

  // Tamaños
  CTextura *pSrc = g_pGestorMateriales->GetTextura(iIDTexSrc);
  CTextura *pDst = g_pGestorMateriales->GetTextura(s_pTexturas[TEX_TARGET]);
  int iW = pSrc->GetWidth();
  int iH = pSrc->GetHeight();
  RECT Rect = {0,0, iW,iH};
  float fU = (float)iW / pDst->GetWidth();
  float fV = (float)iH / pDst->GetHeight();

  // Compute blur direction
  TVector2 vDir;
  vDir.x = (0.5f - vPos.x) * (fRadius / iW) * 2.f;
  vDir.y = (0.5f - vPos.y) * (fRadius / iH) * 2.f;

  // States
  pDD->SetRenderTarget(iIDTexSrc);
  pDD->ApplyBasicShader();
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pDD->SetBilinearFiltering(0, true);
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_TARGET], 0);

  float fHalfWidth  = (float)iW * 0.5f;
  float fHalfHeight = (float)iH * 0.5f;
  float fCurRadius  = 1;
  float fStep       = (fRadius - fCurRadius) / nSteps;
  float fAlpha      = fIntens;
  for (int i = 0; i < nSteps; i++)
  {
    pDD->EndScene();
    pD3D->StretchRect(pSrc->GetSurfaceD3D(), &Rect, pDst->GetSurfaceD3D(), &Rect, D3DTEXF_POINT);
    pDD->BeginScene();

    // Calcular el quad
    float fPos     = fCurRadius / fRadius;
    float fCenterX = (0.5f + (vDir.x * fPos)) * iW;
    float fCenterY = (0.5f + (vDir.y * fPos)) * iH;

    // Compute some parameters
    float fLeft   = fCenterX - fHalfWidth  - fCurRadius;
    float fTop    = fCenterY - fHalfHeight - fCurRadius;
    float fRight  = fCenterX + fHalfWidth  + fCurRadius;
    float fBottom = fCenterY + fHalfHeight + fCurRadius;

    unsigned uAlpha = int(fAlpha * 255.f);
    pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, (uAlpha<<24));

    // Draw Quad
    DrawQuadUV(pDD, TVector2(fLeft,fTop), TVector2(fRight,fBottom), TVector2(0.f,0.f), TVector2(fU, fV));

    fCurRadius+= fStep;
  }
}


//---------------------------------------------------------------------------//
// DoBoxBlur
//
//---------------------------------------------------------------------------//
void DrawBoxBlurQuad(CDisplayDevice *pDD, int iXOffset, int iYOffset)
{
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 4);

  TViewport const &Rect = pDD->GetViewport();
  TVector2 v0((float)Rect.x0, (float)Rect.y0);
  TVector2 v1((float)Rect.x1, (float)Rect.y1);
  float fXD = (float)iXOffset / pDD->ViewportWidth ();
  float fYD = (float)iYOffset / pDD->ViewportHeight();

  HCV_SET_COLOR (pVertices, HARD_COLOR_ARGB(128,255,255,255));
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, 0.f+fXD, 0.f+fYD);
  HCV_SET_UV1   (pVertices, 0.f-fXD, 0.f-fYD);
  pVertices++;

  HCV_SET_COLOR (pVertices, HARD_COLOR_ARGB(128,255,255,255));
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v0.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, 1.f+fXD, 0.f+fYD);
  HCV_SET_UV1   (pVertices, 1.f-fXD, 0.f-fYD);
  pVertices++;

  HCV_SET_COLOR (pVertices, HARD_COLOR_ARGB(128,255,255,255));
  HCV_SET_XYZW  (pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, 1.f+fXD, 1.f+fYD);
  HCV_SET_UV1   (pVertices, 1.f-fXD, 1.f-fYD);
  pVertices++;

  HCV_SET_COLOR (pVertices, HARD_COLOR_ARGB(128,255,255,255));
  HCV_SET_XYZW  (pVertices, v0.x-0.5f, v1.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, 0.f+fXD, 1.f+fYD);
  HCV_SET_UV1   (pVertices, 0.f-fXD, 1.f-fYD);
  pVertices++;

  pDD->UnlockVertexBuffer(HCV_XYZ_T);
  pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLEFAN, 2);
}

void CEffects::DoBoxBlur(CDisplayDevice *pDD, int iIDTexSrc, float fRadio)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  pDD->SetRenderTarget(iIDTexSrc);
  pDD->ApplyBasicShader();

  pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,  D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
  pD3D->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

  pD3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_BLENDDIFFUSEALPHA);
  pD3D->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
  pD3D->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

  g_pGestorMateriales->SetTextura(TEX_TEMP_0, 0);
  g_pGestorMateriales->SetTextura(TEX_TEMP_0, 1);

  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB(128,255,255,255));

  int iRadio = (int)fRadio;
  int iOffset = 1;
  int iOldOffset = 1;
  while (iOldOffset < iRadio)
  {
    if (iOffset >= iRadio)
    {
      int iIntens = 128 - int((float(iOffset-iRadio)*128)/float(iOldOffset));
      pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB(iIntens,255,255,255));
    }

    D3DSURFACE *pSrc = g_pGestorMateriales->GetTextura(iIDTexSrc)->GetSurfaceD3D();
    D3DSURFACE *pDst = g_pGestorMateriales->GetTextura(TEX_TEMP_0)->GetSurfaceD3D();
    pD3D->StretchRect(pSrc,  NULL, pDst, NULL, D3DTEXF_POINT);

    DrawBoxBlurQuad(pDD, iOffset, 0);
    DrawBoxBlurQuad(pDD, 0, iOffset);
    iOldOffset=iOffset;
    iOffset   =iOffset<<1;
  }
}


//---------------------------------------------------------------------------//
// DoGlowMipmap
//
//---------------------------------------------------------------------------//
void CEffects::DoGlowMipmap(CDisplayDevice *pDD, int iIDTexSrc, float fIntens)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  // Pintaremos encima de esta
  pDD->SetRenderTarget(s_pTexturas[TEX_GLOW_MIPMAP_0]);
  pDD->ApplyBasicShader();
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  pD3D->SetRenderState(D3DRS_CULLMODE,  D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,   D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB(255,128,128,128));
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(2, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(2, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(3, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(3, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(3, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SUBTRACT);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

  pD3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
  pD3D->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

  pD3D->SetTextureStageState(2, D3DTSS_COLOROP,   D3DTOP_MODULATE4X);
  pD3D->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT);
  pD3D->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

  pD3D->SetTextureStageState(3, D3DTSS_COLOROP,   D3DTOP_MODULATE4X);
  pD3D->SetTextureStageState(3, D3DTSS_COLORARG1, D3DTA_CURRENT);
  pD3D->SetTextureStageState(3, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

  CTextura *pTex = g_pGestorMateriales->GetTextura(s_pTexturas[TEX_GLOW_MIPMAP_0]);
  g_pGestorMateriales->SetTextura(iIDTexSrc, 0);
  g_pGestorMateriales->SetTextura(iIDTexSrc, 1);
  g_pGestorMateriales->SetTextura(iIDTexSrc, 2);
  g_pGestorMateriales->SetTextura(iIDTexSrc, 3);
  DrawQuad(pDD);
  pDD->DesactivaStage(1);
  pDD->DesactivaStage(2);
  pDD->DesactivaStage(3);

  DoBoxBlur(pDD, TEX_GLOW_MIPMAP_0, 24);

  pDD->SetRenderTarget(iIDTexSrc);
  pDD->ApplyBasicShader();
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB((int)(fIntens*255.f),0,0,0));
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  g_pGestorMateriales->SetTextura(TEX_GLOW_MIPMAP_0, 0);
  DrawQuad(pDD);
}


//---------------------------------------------------------------------------//
// DoGlowPS
//
//---------------------------------------------------------------------------//
void CEffects::DoGlowPS(CDisplayDevice *pDD, int iIDTexSrc, float fIntens)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  CShader *pShader;

  // Aplicar un filtro de blur en X
  pDD->ApplyBasicShader();
  pDD->SetRenderTarget(s_pTexturas[TEX_GLOW_PS_0]);
  g_pGestorMateriales->SetTextura(iIDTexSrc, 0);
  // Shader
  pShader = g_pGestorMateriales->GetShader(s_pShaders[SH_BLURXGLOW]);
  unsigned uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD, TVector2(0.f, 0.f), TVector2((float)s_SizeFX, (float)s_SizeFX));
    pShader->EndPass();
  }
  pShader->EndDraw();

  // Aplicar un filtro de blur en Y
  pDD->SetRenderTarget(s_pTexturas[TEX_GLOW_PS_1]);
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_GLOW_PS_0], 0);
  // Shader
  pShader = g_pGestorMateriales->GetShader(s_pShaders[SH_BLURY]);
  uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD, TVector2(0.f, 0.f), TVector2((float)s_SizeFX, (float)s_SizeFX));
    pShader->EndPass();
  }
  pShader->EndDraw();

  unsigned uAlpha = (unsigned)(fIntens * 255.f);
  // Rendear el resultado encima de la textura
  pDD->ApplyBasicShader();
  pDD->SetRenderTarget(iIDTexSrc);
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_GLOW_PS_1], 0);

  pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,  D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, (uAlpha<<24));
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
  DrawQuad(pDD);
}


//---------------------------------------------------------------------------//
// DoBlur
//
//---------------------------------------------------------------------------//
void CEffects::DoBlur(CDisplayDevice *pDD, int iIDTexSrc, float fIntens)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  CShader *pShader;

  // Aplicar un filtro de blur en X
  pDD->ApplyBasicShader();
  pDD->SetRenderTarget(s_pTexturas[TEX_BLUR_0]);
  g_pGestorMateriales->SetTextura(iIDTexSrc, 0);
  // Shader
  pShader = g_pGestorMateriales->GetShader(s_pShaders[SH_BLURX]);
  unsigned uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD);
    pShader->EndPass();
  }
  pShader->EndDraw();

  // Aplicar un filtro de blur en Y
  pDD->SetRenderTarget(s_pTexturas[TEX_BLUR_1]);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_BLUR_0], 0);
  // Shader
  pShader = g_pGestorMateriales->GetShader(s_pShaders[SH_BLURY]);
  uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD);
    pShader->EndPass();
  }
  pShader->EndDraw();

  unsigned uAlpha = (unsigned)(fIntens * 1.2f * 255.f);
  if (uAlpha > 255) uAlpha = 255;
  // Rendear el resultado encima de la textura
  pDD->SetRenderTarget(iIDTexSrc);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_BLUR_1], 0);

  pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,  D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, (uAlpha<<24));
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
  DrawQuad(pDD);
}


//---------------------------------------------------------------------------//
// DoHorzBlur
//
//---------------------------------------------------------------------------//
void CEffects::DoHorzBlur(CDisplayDevice *pDD, int iIDTexSrc, float fIntens)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  CShader *pShader;

  // Aplicar un filtro de blur en X
  pDD->SetRenderTarget(s_pTexturas[TEX_BLUR_0]);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(iIDTexSrc, 0);
  // Shader
  pShader = g_pGestorMateriales->GetShader(s_pShaders[SH_BLURX]);
  unsigned uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD, TVector2(0.f, 0.f), TVector2((float)s_SizeFX, (float)s_SizeFX));
    pShader->EndPass();
  }
  pShader->EndDraw();

  unsigned uAlpha = (unsigned)(fIntens * 255.f);
  // Rendear el resultado encima de la textura
  pDD->SetRenderTarget(iIDTexSrc);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_BLUR_0], 0);

  pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,  D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, (uAlpha<<24));
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  DrawQuad(pDD);
}


//---------------------------------------------------------------------------//
// DoVertBlur
//
//---------------------------------------------------------------------------//
void CEffects::DoVertBlur(CDisplayDevice *pDD, int iIDTexSrc, float fIntens)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  CShader *pShader;

  // Aplicar un filtro de blur en X
  pDD->SetRenderTarget(s_pTexturas[TEX_BLUR_0]);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(iIDTexSrc, 0);
  // Shader
  pShader = g_pGestorMateriales->GetShader(s_pShaders[SH_BLURY]);
  unsigned uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD, TVector2(0.f, 0.f), TVector2((float)s_SizeFX, (float)s_SizeFX));
    pShader->EndPass();
  }
  pShader->EndDraw();

  unsigned uAlpha = (unsigned)(fIntens * 255.f);
  // Rendear el resultado encima de la textura
  pDD->SetRenderTarget(iIDTexSrc);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_BLUR_0], 0);

  pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,  D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, (uAlpha<<24));
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  DrawQuad(pDD);
}


//---------------------------------------------------------------------------//
// DoPixelate
//
//---------------------------------------------------------------------------//
int CEffects::DoPixelate(CDisplayDevice *pDD, int iIDTexSrc, float fIntens, float &fU, float &fV)
{
  CTextura *pTex = g_pGestorMateriales->GetTextura(iIDTexSrc);
  pDD->ApplyBasicShader();
  fU = fIntens * pTex->GetWidth ();
  fV = fIntens * pTex->GetHeight();
  pDD->SetRenderTarget(s_pTexturas[TEX_TARGET]);
  pDD->SetBilinearFiltering(0, false);
  g_pGestorMateriales->SetTextura(iIDTexSrc, 0);
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  DrawQuad(pDD, TVector2(0.f, 0.f), TVector2(fU, fV));
  pTex = g_pGestorMateriales->GetTextura(s_pTexturas[TEX_TARGET]);
  fU = fU / pTex->GetWidth ();
  fV = fV / pTex->GetHeight();
  return s_pTexturas[TEX_TARGET];
  /*
  D3DSURFACE *pSrc = pTex->GetSurfaceD3D();
  D3DSURFACE *pDst = g_pGestorMateriales->GetTextura(m_iTextura)->GetSurfaceD3D();
  RECT RectDest;
  RectDest.left  = 0;
  RectDest.top   = 0;
  RectDest.right = (1.f - (m_fIntensAct * 0.8f)) * pTex->GetWidth ();
  RectDest.bottom= (1.f - (m_fIntensAct * 0.8f)) * pTex->GetHeight();
  pD3D->StretchRect(pSrc,  NULL, pDst, &RectDest, D3DTEXF_POINT);
  */
}


//---------------------------------------------------------------------------//
// DoPsychoEffect
//
//---------------------------------------------------------------------------//
void CEffects::DoPsychoEffect(CDisplayDevice *pDD, int iIDTexSrc, float fIntens)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  pDD->EndScene();

  // Copiar el source en varias texturas
  D3DSURFACE *pSrc  = g_pGestorMateriales->GetTextura(iIDTexSrc)->GetSurfaceD3D();
  D3DSURFACE *pDst0 = g_pGestorMateriales->GetTextura(s_pTexturas[TEX_GLOW_PS_0])->GetSurfaceD3D();

  pD3D->StretchRect(pSrc, NULL, pDst0, NULL, D3DTEXF_POINT);

  // Los siguientes los pintamos en aditivo
  pDD->ApplyBasicShader();
  unsigned uAlpha = (int)(fIntens*255.f);
  if (uAlpha > 255) uAlpha = 255;
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  pD3D->SetRenderState(D3DRS_ZENABLE,  D3DZB_FALSE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

  float fX = 0.f;
  float fY = 0.f;
  float fW = (float)g_pGestorMateriales->GetTextura(iIDTexSrc)->GetWidth();
  float fH = (float)g_pGestorMateriales->GetTextura(iIDTexSrc)->GetHeight();
  float fDist  = 1.f;
  float fHDist;

  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB(uAlpha,110,110,110));
  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SUBTRACT);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

  // Preparar los vertices
  TVertex_HCV_XYZ_T *pVertices;

  // Dibujar el mas pequeño (mas blurreado)
  int iNumPasadas = 3;
  g_pGestorMateriales->SetTextura(s_pTexturas[TEX_GLOW_PS_0], 0);
  pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 18*iNumPasadas);
  fDist  = 16.f;
  fHDist = fDist*0.5f;
  for (int i = 0; i < iNumPasadas; i++)
  {
    fY = -fDist;
    PreparaVertices(pVertices,    TVector2(fX,fY), TVector2(fX+fW,fY+fH));
    fY = fHDist; fX = -fHDist;
    PreparaVertices(pVertices+6,  TVector2(fX,fY), TVector2(fX+fW,fY+fH));
    fY = fHDist; fX =  fHDist;
    PreparaVertices(pVertices+12, TVector2(fX,fY), TVector2(fX+fW,fY+fH));
    pVertices+=18;
  }

  pDD->UnlockVertexBuffer(HCV_XYZ_T);

  // Pintaremos encima de esta
  pDD->SetRenderTarget(iIDTexSrc);  
  pDD->BeginScene();
  pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLELIST, 6*iNumPasadas);
}


//---------------------------------------------------------------------------//
// DoMotionBlurPass1
//
//---------------------------------------------------------------------------//
void CEffects::DoMotionBlurPass1(CDisplayDevice *pDD, int iIDTexSrc)
{
  D3DSURFACE *pSrc = g_pGestorMateriales->GetTextura(iIDTexSrc)->GetSurfaceD3D();
  D3DSURFACE *pDst = g_pGestorMateriales->GetTextura(s_pTexturas[TEX_MOTION_BLUR])->GetSurfaceD3D();
  pDD->GetD3DDevice()->StretchRect(pSrc, NULL, pDst, NULL, D3DTEXF_POINT);
}


//---------------------------------------------------------------------------//
// DoMotionBlurPass2
//
//---------------------------------------------------------------------------//
void CEffects::DoMotionBlurPass2(CDisplayDevice *pDD, int iIDTexSrc, float fIntens)
{
  pDD->SetRenderTarget(iIDTexSrc);

  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  pDD->ApplyBasicShader();
  pD3D->SetRenderState (D3DRS_CULLMODE,      D3DCULL_NONE);
  pD3D->SetRenderState (D3DRS_ZENABLE,       D3DZB_FALSE);
  pD3D->SetRenderState (D3DRS_ZWRITEENABLE,  D3DZB_FALSE);
  pD3D->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState (D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState (D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SUBTRACT);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetRenderState (D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB((int)(fIntens * 255.f),2,2,2));

  g_pGestorMateriales->SetTextura(TEX_MOTION_BLUR, 0);

  TViewport const &Rect = pDD->GetViewport();
  DrawQuad(pDD, TVector2((float)Rect.x0 - 0.5f, (float)Rect.y0 - 0.5f), TVector2((float)Rect.x1 - 0.5f, (float)Rect.y1 - 0.5f));
}


//---------------------------------------------------------------------------//
// DoSpriter
//
//---------------------------------------------------------------------------//
void CEffects::DoSpriter(CDisplayDevice *pDD, int iIDTexSrc, int iIDTexTgt, int iSprite, float fResolution, float fMinSize, float fMaxSize)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  CTextura *pTexSrc = g_pGestorMateriales->GetTextura(iIDTexSrc);
  CTextura *pTexDst = g_pGestorMateriales->GetTextura(s_pTexturas[TEX_SPRITER]);
  pD3D->GetRenderTargetData(pTexSrc->GetSurfaceD3D(), pTexDst->GetSurfaceD3D());

  TViewport const &Rect = pDD->GetViewport();
  float fRatio = (Rect.x1 - Rect.x0) / 512.f;
  TSurfaceDesc Desc;
  if (pTexDst->Lock(NULL, Desc))
  {
    unsigned *pBits = (unsigned *)Desc.pBits;
    int iResX = int(fResolution * 50.f) + 20;
    int iResY = (iResX*3)>>2;
    int iDX   = (pTexSrc->GetWidth () << 16) / iResX;
    int iDY   = (pTexSrc->GetHeight() << 16) / iResY;
    int iY    = iDY >> 1;
    TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, iResX*iResY);
    for (int j = 0; j < iResY; j++)
    {
      int iX    = iDX >> 1;
      int iCY   = iY >> 16;
      int iBase = iCY*pTexSrc->GetWidth();
      for (int i = 0; i < iResX; i++)
      {
        int iCX = iX >> 16;
        unsigned uColor = pBits[iBase + iCX];
        unsigned uSize  = HARD_COLOR_GET_R(uColor) + HARD_COLOR_GET_G(uColor) + HARD_COLOR_GET_B(uColor);
        float    fSize  = ((uSize / 768.f) * (fMaxSize - fMinSize) + fMinSize) * fRatio;
        HCV_SET_XYZW  (pVertices, (float)iCX,(float)iCY,0.f,1.f);
        HCV_SET_COLOR (pVertices, uColor);
        HCV_SET_PSIZE (pVertices, fSize);
        pVertices++;
        iX+=iDX;
      }
      iY+=iDY;
    }
    pDD->UnlockVertexBuffer  (HCV_XYZ_T);
    pTexDst->Unlock();

    pDD->ApplyBasicShader    ();
    pDD->SetRenderTarget     (iIDTexTgt);
    pDD->SetBilinearFiltering(0, true);
    pD3D->SetRenderState(D3DRS_CLIPPING, TRUE);
    pD3D->SetRenderState(D3DRS_ZENABLE,   FALSE);
    pD3D->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
    pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
    pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
    pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pGestorMateriales->SetTextura(iSprite, 0);
    pDD->Clear(true, false, 0, 0.f);

    pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_POINTLIST, iResX*iResY);
  }
}
