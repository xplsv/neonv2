//---------------------------------------------------------------------------//
// File: FilterDist.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "FilterDist.h"

#define PLASMA_X  (18)
#define PLASMA_Y  (15)
#define PLASMA_W  (1.f / PLASMA_X)
#define PLASMA_H  (1.f / PLASMA_Y)

enum 
{
  DIST_CRYSTAL = 0,
  DIST_CIRCCRYSTAL,
  DIST_SIN,
  DIST_POLAR,
};

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "Type",   true,  4, {"Crystal", "CircCrystal", "Sin", "Polar"}},
  {TCtrlVar::SLIDER,    1, "Speed",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,    2, "Intens", true,  0, {NULL}},
  {TCtrlVar::SLIDER,    3, "Var 1",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,    4, "Var 2",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,    5, "Var 3",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,    6, "Var 4",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,    7, "Var 5",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,    8, "Var 6",  true,  0, {NULL}},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterDist::GetVarCtrls()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterDist::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_iType    = *(int   *)pData; break;
    case 1: m_fSpeed   = *(float *)pData; break;
    case 2: m_fIntens  = *(float *)pData; break;
    case 3: m_fVars[0] = *(float *)pData; break;
    case 4: m_fVars[1] = *(float *)pData; break;
    case 5: m_fVars[2] = *(float *)pData; break;
    case 6: m_fVars[3] = *(float *)pData; break;
    case 7: m_fVars[4] = *(float *)pData; break;
    case 8: m_fVars[5] = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterDist::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_iType);
    case 1: return (&m_fSpeed);
    case 2: return (&m_fIntens);
    case 3: return (&m_fVars[0]);
    case 4: return (&m_fVars[1]);
    case 5: return (&m_fVars[2]);
    case 6: return (&m_fVars[3]);
    case 7: return (&m_fVars[4]);
    case 8: return (&m_fVars[5]);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterDist::Init(COptions &aOptions)
{
  m_Ok      = true;
  m_iType   = DIST_CRYSTAL;
  m_fSpeed  = 0.5f;
  m_fIntens = 0.f;
  for (int i = 0; i < 6; i++)
    m_fVars[i] = 0.f;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterDist::End()
{
  CFilter::End();
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterDist::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
bool CFilterDist::Apply(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  switch (m_iType)
  {
    case DIST_CRYSTAL:     DrawCrystal    (pDD, pRenderTgt, iTexTgt); break;
    case DIST_CIRCCRYSTAL: DrawCircCrystal(pDD, pRenderTgt, iTexTgt); break;
    case DIST_SIN:         DrawSin        (pDD, pRenderTgt, iTexTgt); break;
    case DIST_POLAR:       DrawPolar      (pDD, pRenderTgt, iTexTgt); break;
  }
  return true;
}


//---------------------------------------------------------------------------//
// DrawCrystal
//
//---------------------------------------------------------------------------//
void CFilterDist::DrawCrystal(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  float fTime = m_fTime * m_fSpeed;

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget (iTexTgt);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
  pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  pDD->SetBilinearFiltering (0, false);

  TViewport const   &Viewport  = pDD->GetViewport();
  ushort            *pIndices  = pDD->LockIndexBuffer(PLASMA_X*PLASMA_Y*6);
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, (PLASMA_X+1)*(PLASMA_Y+1));
  TVertex_HCV_XYZ_T *pTemp     = pVertices;
  for (int i = 0; i <= PLASMA_Y; i++)
  {
    for (int j = 0; j <= PLASMA_X; j++)
    {
      float fX = j*PLASMA_W;
      float fY = i*PLASMA_H;
      HCV_SET_XYZW(pTemp, fX * (Viewport.x1-Viewport.x0+1) + Viewport.x0, fY * (Viewport.y1-Viewport.y0+1) + Viewport.y0, 0.f, 1.f);
      float fU = fX + ((cosf(fTime * 5.74f + (fX-0.5f) * m_fVars[0] * 50.f) * m_fVars[1] * 70.f) +
                       (sinf(fTime * 5.74f + (fY-0.5f) * m_fVars[0] * 50.f) * m_fVars[1] * 70.f)) * m_fIntens * 2.f / pDD->Width ();
      float fV = fY + ((cosf(fTime * 5.74f + (fX-0.5f) * m_fVars[2] * 50.f) * m_fVars[3] * 70.f) +
                       (sinf(fTime * 5.74f + (fY-0.5f) * m_fVars[2] * 50.f) * m_fVars[3] * 70.f)) * m_fIntens * 2.f / pDD->Height();
      HCV_SET_UV0(pTemp, fU, fV);
      HCV_SET_COLOR(pTemp, HARD_COLOR_ARGB(255,255,255,255));
      if (i < PLASMA_Y && j < PLASMA_X)
      {
        *pIndices++=j+(i*(PLASMA_X+1));
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+2;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
      }
      pTemp++;
    }
  }
  pDD->UnlockIndexBuffer   ();
  pDD->UnlockVertexBuffer  (HCV_XYZ_T);
  pDD->DrawIndexedPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLELIST, PLASMA_X*PLASMA_Y*2);
}


//---------------------------------------------------------------------------//
// DrawCircCrystal
//
//---------------------------------------------------------------------------//
void CFilterDist::DrawCircCrystal(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  float fTime = m_fTime * m_fSpeed;

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget (iTexTgt);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
    pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  pDD->SetBilinearFiltering (0, false);

  TViewport const     &Viewport  = pDD->GetViewport();
  ushort              *pIndices  = pDD->LockIndexBuffer(PLASMA_X*PLASMA_Y*6);
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, (PLASMA_X+1)*(PLASMA_Y+1));
  TVertex_HCV_XYZ_T *pTemp     = pVertices;
  for (int i = 0; i <= PLASMA_Y; i++)
  {
    for (int j = 0; j <= PLASMA_X; j++)
    {
      float fX = j*PLASMA_W;
      float fY = i*PLASMA_H;
      HCV_SET_XYZW(pTemp, fX * (Viewport.x1-Viewport.x0+1) + Viewport.x0, fY * (Viewport.y1-Viewport.y0+1) + Viewport.y0, 0.f, 1.f);
      float fU = fX + (cosf(fTime * 5.74f + (fX-0.5f) * (fY-0.5f) * m_fVars[0] * 50.f) * m_fVars[1] * 70.f) * m_fIntens * 2.f / pDD->Width ();
      float fV = fY + (cosf(fTime * 5.45f + (fX-0.5f) * (fY-0.5f) * m_fVars[2] * 50.f) * m_fVars[3] * 70.f) * m_fIntens * 2.f / pDD->Height();
      HCV_SET_UV0  (pTemp, fU, fV);
      HCV_SET_COLOR(pTemp, HARD_COLOR_ARGB(255,255,255,255));
      if (i < PLASMA_Y && j < PLASMA_X)
      {
        *pIndices++=j+(i*(PLASMA_X+1));
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+2;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
      }
      pTemp++;
    }
  }
  pDD->UnlockIndexBuffer   ();
  pDD->UnlockVertexBuffer  (HCV_XYZ_T);
  pDD->DrawIndexedPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLELIST, PLASMA_X*PLASMA_Y*2);
}


//---------------------------------------------------------------------------//
// DrawSin
//
//---------------------------------------------------------------------------//
void CFilterDist::DrawSin(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  float fTime = m_fTime * m_fSpeed;

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget (iTexTgt);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
  pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  pDD->SetBilinearFiltering (0, false);

  TViewport const     &Viewport  = pDD->GetViewport();
  ushort              *pIndices  = pDD->LockIndexBuffer(PLASMA_X*PLASMA_Y*6);
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, (PLASMA_X+1)*(PLASMA_Y+1));
  TVertex_HCV_XYZ_T *pTemp     = pVertices;
  for (int i = 0; i <= PLASMA_Y; i++)
  {
    for (int j = 0; j <= PLASMA_X; j++)
    {
      float fX = j*PLASMA_W;
      float fY = i*PLASMA_H;
      HCV_SET_XYZW(pTemp, fX * (Viewport.x1-Viewport.x0+1) + Viewport.x0, fY * (Viewport.y1-Viewport.y0+1) + Viewport.y0, 0.f, 1.f);
      float fU = fX + (cosf(fTime*10.f + (fX-0.5f) * (fY-0.5f) * 10.f * m_fVars[1]) * m_fVars[0] * 50.f / pDD->Width ()) * m_fIntens;
      float fV = fY + (cosf(fTime*10.f + (fY-0.5f) * (fX-0.5f) * 10.f * m_fVars[3]) * m_fVars[2] * 50.f / pDD->Height()) * m_fIntens;
      HCV_SET_UV0(pTemp, fU, fV);
      HCV_SET_COLOR(pTemp, HARD_COLOR_ARGB(255,255,255,255));
      if (i < PLASMA_Y && j < PLASMA_X)
      {
        *pIndices++=j+(i*(PLASMA_X+1));
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+2;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
      }
      pTemp++;
    }
  }
  pDD->UnlockIndexBuffer   ();
  pDD->UnlockVertexBuffer  (HCV_XYZ_T);
  pDD->DrawIndexedPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLELIST, PLASMA_X*PLASMA_Y*2);
}


//---------------------------------------------------------------------------//
// DrawPolar
//
//---------------------------------------------------------------------------//
void CFilterDist::DrawPolar(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  float fTime = m_fTime * m_fSpeed;

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget (iTexTgt);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
  pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
  pD3D->SetSamplerState     (0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
  pDD->SetBilinearFiltering (0, false);

  TViewport const     &Viewport  = pDD->GetViewport();
  ushort              *pIndices  = pDD->LockIndexBuffer(PLASMA_X*PLASMA_Y*6);
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, (PLASMA_X+1)*(PLASMA_Y+1));
  TVertex_HCV_XYZ_T *pTemp     = pVertices;
  for (int i = 0; i <= PLASMA_Y; i++)
  {
    for (int j = 0; j <= PLASMA_X; j++)
    {
      float fX = j*PLASMA_W;
      float fY = i*PLASMA_H;
      HCV_SET_XYZW(pTemp, fX * (Viewport.x1-Viewport.x0+1) + Viewport.x0, fY * (Viewport.y1-Viewport.y0+1) + Viewport.y0, 0.f, 1.f);
      fX-=0.5f;
      fY-=0.5f;
      float fMag = sqrtf(fX*fX + fY*fY) * 2.f * m_fVars[0];
      // angle = arc tangent of y/x
      float fAngle = (((float)atan2(fY, fX)) * m_fVars[1]) * 1.f/6.28319f;
      while (fAngle >= 1.f)
        fAngle-=1.f;
      while (fAngle < 0.f)
        fAngle+=1.f;
      while (fMag > 1.f)
        fMag-=1.f;
      while (fMag < 0.f)
        fMag+=1.f;
      HCV_SET_UV0(pTemp, fAngle * m_fIntens + ((fX+0.5f)*(1-m_fIntens)), fMag * m_fIntens + ((fY+0.5f)*(1-m_fIntens)));
      HCV_SET_COLOR(pTemp, HARD_COLOR_ARGB(255,255,255,255));
      if (i < PLASMA_Y && j < PLASMA_X)
      {
        *pIndices++=j+(i*(PLASMA_X+1));
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+2;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
      }
      pTemp++;
    }
  }
  pDD->UnlockIndexBuffer   ();
  pDD->UnlockVertexBuffer  (HCV_XYZ_T);
  pDD->DrawIndexedPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLELIST, PLASMA_X*PLASMA_Y*2);
}


//---------------------------------------------------------------------------//
// DrawFish
//
//---------------------------------------------------------------------------//
/*
void CFilterDist::DrawFish(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  float fTime = m_fTime * m_fSpeed;

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget(-1);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
  ApplyMode(pDD, pRenderTgt, fAlphaGlobal);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pDD->SetBilinearFiltering (0, false);

  TViewport const     &Viewport  = pDD->GetViewport();
  ushort              *pIndices  = pDD->LockIndexBuffer(PLASMA_X*PLASMA_Y*6);
  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, (PLASMA_X+1)*(PLASMA_Y+1));
  TVertex_HCV_XYZ_T *pTemp     = pVertices;
  for (int i = 0; i <= PLASMA_Y; i++)
  {
    for (int j = 0; j <= PLASMA_X; j++)
    {
      float fX = j*PLASMA_W;
      float fY = i*PLASMA_H;
      HCV_SET_XYZ(pTemp, fX * (Viewport.x1-Viewport.x0) + Viewport.x0, fY * (Viewport.y1-Viewport.y0) + Viewport.y0, 1.f);
      float fLength = sqrtf((fX-0.5f) * (fX-0.5f) + (fY-0.5f) * (fY-0.5f));
      float fU = fX * (1.f - m_fIntens) + fY * m_fIntens;
      float fV = fY * (1.f - m_fIntens) + (sinf((fX-0.5f) * m_fVars[0] * 10.f * fLength) * m_fIntens);
      HCV_SET_UV0(pTemp, fU, fV);
      HCV_SET_COLOR(pTemp, HARD_COLOR_ARGB(255,255,255,255));
      if (i < PLASMA_Y && j < PLASMA_X)
      {
        *pIndices++=j+(i*(PLASMA_X+1));
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
        *pIndices++=j+(i*(PLASMA_X+1))+1;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+2;
        *pIndices++=j+(i*(PLASMA_X+1))+PLASMA_X+1;
      }
      pTemp++;
    }
  }
  pDD->UnlockIndexBuffer   ();
  pDD->UnlockVertexBuffer  (HCV_XYZ_T);
  pDD->DrawIndexedPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLELIST, PLASMA_X*PLASMA_Y*2);
}
*/