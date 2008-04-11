//---------------------------------------------------------------------------//
// File: FilterScanner.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Xml.h"
#include "FilterScanner.h"
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
TCtrlVar *CFilterScanner::GetVars()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterScanner::SetVar(int iVar, void *pData)
{
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterScanner::GetVar(int iVar)
{
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterScanner::Init(TiXmlElement *pElem)
{
  CFilter::Init(pElem);

  if (!IsOk())
    End();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterScanner::End()
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
void CFilterScanner::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CalcScannerRect(TVector2 const &v0, TVector2 const &v1, float fZoom, TVector2 *pV0, TVector2 *pV1)
{
  float fX  = (v1.x - v0.x) * 0.5f;
  float fY  = (v1.y - v0.y) * 0.5f;
  float fCX = v0.x + fX;
  float fCY = v0.y + fY;

  pV0->x = fCX - fX * fZoom;
  pV0->y = fCY - fY * fZoom;
  pV1->x = fCX + fX * fZoom;
  pV1->y = fCY + fY * fZoom;
}

//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFilterScanner::Draw(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  int iTex = CEffects::GetTexture(CEffects::TEX_TARGET);
  pDD->SetRenderTarget(iTex);

  // Render el fondo y la capa de scanner
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
  DrawQuad(pDD);

  TVector2 v0,   v1;
  TVector2 uv0, uv1;
  CalcScannerRect(TVector2(100,100), TVector2(420,340), 0.7f, &v0, &v1);
  uv0 = TVector2(100.f/pDD->Width(), 100.f/pDD->Height());
  uv1 = TVector2(420.f/pDD->Width(), 340.f/pDD->Height());
  DrawQuadUV(pDD, v0, v1, uv0, uv1);

  // Dibujar con el filtro y el modo seleccionados
  pDD->SetRenderTarget(-1);
  pDD->ApplyBasicShader();
  g_pGestorMateriales->SetTextura(iTex, 0);
  ApplyMode(pDD, pRenderTgt, fAlphaGlobal);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
  pDD->SetBilinearFiltering(0, true);

  DrawQuad(pDD);
}
