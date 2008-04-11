//---------------------------------------------------------------------------//
// File: FXCapture.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "AviDecoder.h"
#include "FXCapture.h"
#include "GEGestorMateriales.h"
#include "GETemp.h"
#include "CtrlVar.h"
#include "VMRCapture.h"
#include "Xml.h"
#include <math.h>

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXCapture::GetVars(int iScope) const
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXCapture::SetVar(int iScope, int iObj, int iVar, void *pData)
{
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXCapture::GetVar(int iScope, int iObj, int iVar)
{
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXCapture::Init(TiXmlElement *pElem)
{
  CEffect::Init(pElem);

  m_fLength = 0;
  m_fStart  = 0.f;
  m_fEnd    = 1.f;
  m_pVMR    = NEW CVMRCapture;
  m_pVMR->Init();
  OnReset();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXCapture::End()
{
  if (IsOk())
  {
    DISPOSE(m_pVMR);
    CEffect::End();
  }
}


//---------------------------------------------------------------------------//
// OnReset
//
//---------------------------------------------------------------------------//
void CFXCapture::OnReset()
{
  CEffect::OnReset();
}


//---------------------------------------------------------------------------//
// OnCommand
//
//---------------------------------------------------------------------------//
void CFXCapture::OnCommand(TCommand *pComm)
{
  CEffect::OnCommand(pComm);
}
   

//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXCapture::Run(float fTime)
{
  CEffect::Run(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXCapture::Draw(CDisplayDevice *pDD)
{
  pDD->SetRenderTarget(m_RenderTarget.iTexture);
  m_pVMR->Draw(pDD);
  
  /*
  pDD->SetRenderTarget(m_RenderTarget.iTexture);
  
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  // Frame actual
  m_iFrame = m_pAvi->TimeToFrame(fmodf(m_fTime, GetLength()) + m_fStart*m_fLength);
  if (m_iFrame != m_iCachedFrame)
  {
    CTextura *pTextura = g_pGestorMateriales->GetTextura(m_iTextura);
    if (!m_pAvi->GetFrame(m_iFrame, pTextura))
      return;
    m_iCachedFrame = m_iFrame;
  }
  
  // Quad con textura
  g_pGestorMateriales->SetTextura(m_iTextura, 0);

  // RenderStates especificos
  pDD->ApplyBasicShader();
  pD3D->SetRenderState(D3DRS_CLIPPING,         FALSE);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR,    m_Color);
  pD3D->SetRenderState(D3DRS_ZWRITEENABLE,     D3DZB_FALSE);  
  pD3D->SetRenderState(D3DRS_ZENABLE,          D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_BLENDOP,          D3DBLENDOP_ADD);

  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE  );
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR  );

  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR    );

  pD3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

  pD3D->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  pD3D->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

  pD3D->SetVertexShader(NULL);
  pD3D->SetPixelShader (NULL);

  switch (m_iBlend)
  {
    case BL_NORMAL:
	    pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
      pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
      break;
    case BL_INVERT:
      pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
      pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
      pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
      break;
  }
  */
  //DrawQuad(pDD, true);
}
