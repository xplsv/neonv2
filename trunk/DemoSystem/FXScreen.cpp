//---------------------------------------------------------------------------//
// File: FXScreen.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "FXScreen.h"


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXScreen::Init(const string &sFile)
{
  m_bOk = true;
  m_RenderTarget.bUseAlpha = false;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXScreen::End()
{
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXScreen::Draw(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  CTextura  *pDst = g_pGestorMateriales->GetTextura(m_RenderTarget.iTexture);

  pDD->SetRenderTarget(-1);
  pD3D->StretchRect(pDD->GetBackBufferSurface(), NULL, pDst->GetSurfaceD3D(), NULL, D3DTEXF_POINT);
}
