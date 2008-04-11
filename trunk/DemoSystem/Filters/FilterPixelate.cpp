//---------------------------------------------------------------------------//
// File: FilterPixelate.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Xml.h"
#include "FilterPixelate.h"
#include "GEEngineGrafico.h"
#include "CtrlVar.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::CHECK_BOX, 0, "Wireframe", true,  0, {NULL}},
  {TCtrlVar::SLIDER,    1, "Intens",    true,  0, {NULL}},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterPixelate::GetVars()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterPixelate::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_bWireFrame = *(bool  *)pData; break;
    case 1: m_fIntens    = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterPixelate::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_bWireFrame);
    case 1: return (&m_fIntens);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterPixelate::Init(TiXmlElement *pElem)
{
  CFilter::Init(pElem);

  m_bWireFrame = false;
  m_fIntens    = 0.5f;

  if (!IsOk())
    End();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterPixelate::End()
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
void CFilterPixelate::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFilterPixelate::Draw(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  float fTime = m_fTime;

  float fIntens = (1-m_fIntens)*(1-m_fIntens)*0.9f + 0.1f;
  float fU, fV;
  int   iIDTex = CEffects::DoPixelate(pDD, pRenderTgt->iTexture, fIntens, fU, fV);

  // Dibujar en wireframe
  if (m_bWireFrame)
  {
    //float fDist = pFilterSource->iW * fIntens;
    //for (int i = )
  }

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget(-1);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(iIDTex, 0);
  ApplyMode(pDD, pRenderTgt, fAlphaGlobal);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
  pDD->SetBilinearFiltering(0, false);

  DrawQuadUV(pDD, TVector2(pRenderTgt->vUV0.x*fU, pRenderTgt->vUV0.y*fV), TVector2(pRenderTgt->vUV1.x*fU, pRenderTgt->vUV1.y*fV));
}
