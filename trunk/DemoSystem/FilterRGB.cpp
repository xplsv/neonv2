//---------------------------------------------------------------------------//
// File: FilterRGB.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Xml.h"
#include "FilterRGB.h"
#include "GEEngineGrafico.h"
#include "CtrlVar.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::SLIDER,    0, "IntensX",   true,  0, NULL},
  {TCtrlVar::SLIDER,    1, "IntensY",   true,  0, NULL},
  {TCtrlVar::SLIDER,    2, "Random",    true,  0, NULL},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterRGB::GetVars()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterRGB::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_fIntensX = *(float *)pData; break;
    case 1: m_fIntensY = *(float *)pData; break;
    case 2: m_fRandom  = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterRGB::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_fIntensX);
    case 1: return (&m_fIntensY);
    case 2: return (&m_fRandom);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterRGB::Init(TiXmlElement *pElem)
{
  CFilter::Init(pElem);

  m_fIntensX = 0.f;
  m_fIntensY = 0.f;
  m_fRandom  = 0.f;

  if (!IsOk())
    End();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterRGB::End()
{
  if (IsOk())
  {
    CFilter::End();
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterRGB::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFilterRGB::Draw(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  int iTex = CEffects::GetTexture(CEffects::TEX_TARGET);
  pDD->SetRenderTarget(iTex);
  pDD->Clear(true, true, 0, 1.f);

  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ONE);
	pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

  TVertex_HCV_XYZ_T *pVertices = (TVertex_HCV_XYZ_T *)pDD->LockVertexBuffer(HCV_XYZ_T, 6*3);

  // 3 channels
  TViewport const &vp = pDD->GetViewport();
  TVector2 vo0 = TVector2((float)vp.x0, (float)vp.y0);
  TVector2 vo1 = TVector2((float)vp.x1, (float)vp.y1);
  for (int i = 0; i < 3; i++)
  {
    static float    pOff[3] = { 25.f/pDD->Width(), 19.f/pDD->Width(), -35.f/pDD->Width()};
    static unsigned pCol[3] = { HARD_COLOR_ARGB(255,255,0,0), HARD_COLOR_ARGB(255,0,255,0), HARD_COLOR_ARGB(255,0,0,255)};
    TVector2 v0, v1;
    float fOffX = (pOff[i] * m_fIntensX + ((((rand() & 127)-64) * 1.f/3.f * m_fRandom) / pDD->Width ()));
    float fOffY = (pOff[i] * m_fIntensY + ((((rand() & 127)-64) * 1.f/3.f * m_fRandom) / pDD->Height()));
    v0.x = vo0.x + fOffX;
    v0.y = vo0.y + fOffY;
    v1.x = vo1.x + fOffX;
    v1.y = vo1.y + fOffY;
    PreparaVertices(pVertices+i*6, v0, v1, pCol[i]);
  }
  pDD->UnlockVertexBuffer(HCV_XYZ_T);
  pDD->DrawPrimitive(HCV_XYZ_T, HARD_PRIM_TRIANGLELIST, 6);

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget(-1);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(iTex, 0);
  ApplyMode(pDD, pRenderTgt, fAlphaGlobal);
  pDD->SetBilinearFiltering(0, true);

  DrawQuad(pDD);
}
