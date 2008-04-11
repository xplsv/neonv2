//---------------------------------------------------------------------------//
// File: FXFlash.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "FXFlashPlayer.h"
#include "GEGestorMateriales.h"
#include "GETextura.h"
#include "GETemp.h"
#include "CtrlVar.h"
#include "Xml.h"
#include <math.h>

#include "ZFlash.h"

#define FLASH_IDTIMER 10004
#define FLASH_TIMER   40
#define FLASH_VERSION 0x70000
#define FLASH_WIDTH   320
#define FLASH_HEIGHT  240
#define FLASH_BITS    16

void CALLBACK FlashTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  return;
}

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::SLIDER,   0, "Quality",  false, 0, {NULL}},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXFlash::GetVars(int iScope) const
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXFlash::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_fQuality = *(float *)pData; m_pFlash->SetQuality((int)m_fQuality * 5); break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXFlash::GetVar(int iScope, int iObj, int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_fQuality);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXFlash::Init(TiXmlElement *pElem)
{
  CEffect::Init(pElem);

  const char *pszFile = SafeString(pElem, "fxfile");
  if (pszFile)
  {
    m_pFlash   = NEW CFlash();
    if (m_pFlash->Init(FLASH_VERSION))
    {
      m_fQuality = 1.f;
      m_pFlash->LoadMovie((char *)pszFile);
      m_iWidth   = FLASH_WIDTH;
      m_iHeight  = FLASH_HEIGHT;
      m_fLength  = m_pFlash->GetLength();
      m_iTextura = g_pGestorMateriales->AddTextura(m_iWidth, m_iHeight, HARD_TEX_FORMAT_R5G6B5);
      SetTimer(0, FLASH_IDTIMER, FLASH_TIMER, (TIMERPROC)FlashTimerProc);
    }
    else
    {
      GLOG(("ERR: Can't initialize SWF player for movie %s\n", pszFile));
      m_bOk = false;
    }
  }
  else
  {
    GLOG(("ERR: FXFile not specified for an FXFlash instance\n"));
    m_bOk = false;
  }

  // Todo Ok
  if (IsOk())
  {
    m_fStart  = 0.f;
    m_fEnd    = 1.f;
    OnReset();
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXFlash::End()
{
  if (IsOk())
  {
    g_pGestorMateriales->RemoveTextura(m_iTextura);
    DISPOSE(m_pFlash);
    CEffect::End();
  }
}



//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//
void CFXFlash::Attach()
{
  CEffect::Attach();
  m_pFlash->Play();
}


//---------------------------------------------------------------------------//
// Unattach
//
//---------------------------------------------------------------------------//
void CFXFlash::Unattach()
{
  CEffect::Unattach();
  m_pFlash->Stop();
}


//---------------------------------------------------------------------------//
// OnReset
//
//---------------------------------------------------------------------------//
void CFXFlash::OnReset()
{
  CEffect::OnReset();
}


//---------------------------------------------------------------------------//
// OnCommand
//
//---------------------------------------------------------------------------//
void CFXFlash::OnCommand(TCommand *pComm)
{
  CEffect::OnCommand(pComm);
}
   

//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXFlash::Run(float fTime)
{
  CEffect::Run(fTime);
  // Para la reproduccion hacia atras
  while (m_fTime < 0.f)
    m_fTime+= m_fLength;
  //m_pFlash->GotoFrame((unsigned)m_fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXFlash::Draw(CDisplayDevice *pDD)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  
  pDD->SetRenderTarget(m_RenderTarget.iTexture);

  // Pintar directamente sobre la DC de la surface
  CTextura *pTex = g_pGestorMateriales->GetTextura(m_iTextura);
  HDC hdc = NULL;
  HRESULT hr = pTex->GetSurfaceD3D()->GetDC(&hdc);
  m_pFlash->DrawFrame(hdc);
  pTex->GetSurfaceD3D()->ReleaseDC(hdc);

  // Quad con textura
  g_pGestorMateriales->SetTextura(m_iTextura, 0);

  // RenderStates especificos
  pDD->ApplyBasicShader();
  pD3D->SetRenderState(D3DRS_CLIPPING,         FALSE);
  pD3D->SetRenderState(D3DRS_ZWRITEENABLE,     D3DZB_FALSE);  
  pD3D->SetRenderState(D3DRS_ZENABLE,          D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_BLENDOP,          D3DBLENDOP_ADD);

  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE  );

  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE    );

  pD3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

  pD3D->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  pD3D->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

  pD3D->SetVertexShader(NULL);
  pD3D->SetPixelShader (NULL);

  pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
  pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  DrawQuadUV(pDD, TVector2(13.f/FLASH_WIDTH, 0.f), TVector2(300.f/FLASH_WIDTH, 215.f/FLASH_HEIGHT));
}
