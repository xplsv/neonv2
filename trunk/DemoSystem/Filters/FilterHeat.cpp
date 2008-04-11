//---------------------------------------------------------------------------//
// File: FilterHeat.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Xml.h"
#include "FilterHeat.h"
#include "GEEngineGrafico.h"
#include "CtrlVar.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterHeat::GetVars()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterHeat::SetVar(int iVar, void *pData)
{
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterHeat::GetVar(int iVar)
{
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterHeat::Init(TiXmlElement *pElem)
{
  CFilter::Init(pElem);
  
  m_iTexAnterior = g_pGestorMateriales->AddTextura(g_DisplayDevice.Width(), g_DisplayDevice.Height(), HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_iTexHeat[0]  = g_pGestorMateriales->AddTextura(g_DisplayDevice.Width(), g_DisplayDevice.Height(), HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_iTexHeat[1]  = g_pGestorMateriales->AddTextura(g_DisplayDevice.Width(), g_DisplayDevice.Height(), HARD_TEX_FORMAT_A8R8G8B8, false, true);

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterHeat::End()
{
  if (IsOk())
  {
    g_pGestorMateriales->RemoveTextura(m_iTexAnterior);
    g_pGestorMateriales->RemoveTextura(m_iTexHeat[0]);
    g_pGestorMateriales->RemoveTextura(m_iTexHeat[1]);
    CFilter::End();
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterHeat::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFilterHeat::Draw(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  CShader   *pShader;
  unsigned   uPasses;

  pDD->EndScene  ();
  pDD->BeginScene();

  // Pass1: update heat texture
  int iTexA = m_bFrame ? m_iTexHeat[0] : m_iTexHeat[1];
  int iTexB = m_bFrame ? m_iTexHeat[1] : m_iTexHeat[0];
  pDD->ApplyBasicShader();
  pDD->SetRenderTarget (iTexA);
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture,  0);
  g_pGestorMateriales->SetTextura(m_iTexAnterior, 1);
  g_pGestorMateriales->SetTextura(iTexB, 2);
  // Shader
  pShader = g_pGestorMateriales->GetShader(CEffects::GetShader(CEffects::SH_HEAT1));
  uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD);
    pShader->EndPass();
  }
  pShader->EndDraw();

  // Pass2: draw heat texture
  pDD->ApplyBasicShader();
  pDD->SetRenderTarget (-1);
  pDD->SetBilinearFiltering (0, true);
  g_pGestorMateriales->SetTextura(iTexA, 0);
  g_pGestorMateriales->SetTextura(-1,    1);
  ApplyMode (pDD, pRenderTgt, fAlphaGlobal);
  // Shader2
  pShader = g_pGestorMateriales->GetShader(CEffects::GetShader(CEffects::SH_HEAT2));
  uPasses = pShader->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pShader->BeginPass(uPass);
    DrawQuad(pDD);
    pShader->EndPass();
  }
  pShader->EndDraw();

  // Next Frame
  CTextura *pSrc = g_pGestorMateriales->GetTextura(pRenderTgt->iTexture);
  CTextura *pDst = g_pGestorMateriales->GetTextura(m_iTexAnterior);
  pDD->EndScene();
  pD3D->StretchRect(pSrc->GetSurfaceD3D(), NULL, pDst->GetSurfaceD3D(), NULL, D3DTEXF_POINT);
  pDD->BeginScene();
  m_bFrame = !m_bFrame;
}
