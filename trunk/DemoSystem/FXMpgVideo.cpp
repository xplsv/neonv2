//---------------------------------------------------------------------------//
// File: FXMpgVideo.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "AviDecoder.h"
#include "FXMpgVideo.h"
#include "GEGestorMateriales.h"
#include "GETemp.h"
#include "CtrlVar.h"
#include "Command.h"
#include "FFMpegVideo.h"
#include "Xml.h"
#include <math.h>

#ifdef USE_MPG_VIDEO

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::SLIDER,    0, "Red",    true,  0, {NULL}},
  {TCtrlVar::SLIDER,    1, "Green",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,    2, "Blue",   true,  0, {NULL}},
  {TCtrlVar::SLIDER,    3, "Start",  false, 0, {NULL}},
  {TCtrlVar::SLIDER,    4, "End",    false, 0, {NULL}},
  {TCtrlVar::SLIDER,    5, "Crop L", false, 0, {NULL}},
  {TCtrlVar::SLIDER,    6, "Crop R", false, 0, {NULL}},
  {TCtrlVar::SLIDER,    7, "Crop T", false, 0, {NULL}},
  {TCtrlVar::SLIDER,    8, "Crop B", false, 0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXMpgVideo::GetVars(int iScope) const
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: HARD_COLOR_SET_R(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 1: HARD_COLOR_SET_G(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 2: HARD_COLOR_SET_B(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 3: m_fStart = *(float *)pData; break;
    case 4: m_fEnd   = *(float *)pData; break;
    case 5: m_fCropL = *(float *)pData; break;
    case 6: m_fCropR = *(float *)pData; break;
    case 7: m_fCropT = *(float *)pData; break;
    case 8: m_fCropB = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXMpgVideo::GetVar(int iScope, int iObj, int iVar)
{
  static float c;
  switch (iVar)
  {
    case 0: c = HARD_COLOR_GET_R(m_Color) * 1.f/255.f; return (&c);
    case 1: c = HARD_COLOR_GET_G(m_Color) * 1.f/255.f; return (&c);
    case 2: c = HARD_COLOR_GET_B(m_Color) * 1.f/255.f; return (&c);
    case 3: return (&m_fStart);
    case 4: return (&m_fEnd);
    case 5: return (&m_fCropL);
    case 6: return (&m_fCropR);
    case 7: return (&m_fCropT);
    case 8: return (&m_fCropB);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXMpgVideo::Init(TiXmlElement *pElem)
{
  CEffect::Init(pElem);

  const char *pszFile = SafeString(pElem, "fxfile");
  if (pszFile)
  {
    m_fLength   = 0;
    m_fStart    = 0.f;
    m_fEnd      = 1.f;
    m_pVideo = NEW CFFMpegVideo;
    if (m_pVideo->Init(pszFile))
    {
      m_iTextura  = g_pGestorMateriales->AddTextura(m_pVideo->GetWidth(), m_pVideo->GetHeight(), HARD_TEX_FORMAT_R5G6B5);
      m_pVideo->SetTextura(g_pGestorMateriales->GetTextura(m_iTextura));
      m_fLength = m_pVideo->Length();
      m_fStart  = 0.f;
      m_fEnd    = 1.f;
      m_fCropL  = 0.f;
      m_fCropR  = 1.f;
      m_fCropT  = 0.f;
      m_fCropB  = 1.f;
      m_bAllowRevPlayback = false;
      m_Color   = HARD_COLOR_ARGB(255,255,255,255);
      OnReset();
    }
    else
      m_bOk = false;
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::End()
{
  if (IsOk())
  {
    DISPOSE(m_pVideo);
    g_pGestorMateriales->RemoveTextura(m_iTextura);
    CEffect::End();
  }
}


//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::Attach()
{
  CEffect::Attach();
}


//---------------------------------------------------------------------------//
// Unattach
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::Unattach()
{
  CEffect::Unattach();
}


//---------------------------------------------------------------------------//
// OnReset
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::OnReset()
{
  CEffect::OnReset();
}


//---------------------------------------------------------------------------//
// OnCommand
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::OnCommand(TCommand *pComm)
{
  CEffect::OnCommand(pComm);
}
   

//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::Run(float fTime)
{
  CEffect::Run(fTime);
  // Para la reproduccion hacia atras
  while (m_fTime < 0.f)
    m_fTime+= m_fLength;
  while (m_fTime >= m_fLength)
    m_fTime-= m_fLength;
  m_pVideo->Run(m_fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXMpgVideo::Draw(CDisplayDevice *pDD)
{
  pDD->SetRenderTarget(m_RenderTarget.iTexture);

  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  CTextura *pTextura = g_pGestorMateriales->GetTextura(m_iTextura);
  if (!m_pVideo->GetFrame(pTextura))
    return;

  // Quad con textura
  g_pGestorMateriales->SetTextura(m_iTextura, 0);

  pDD->ApplyBasicShader();
  pD3D->SetRenderState(D3DRS_CLIPPING,         FALSE);
  pD3D->SetRenderState(D3DRS_ZWRITEENABLE,     D3DZB_FALSE);  
  pD3D->SetRenderState(D3DRS_ZENABLE,          D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_BLENDOP,          D3DBLENDOP_ADD);

  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR,    m_Color);
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

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  DrawQuadUV(pDD, TVector2(m_fCropL, m_fCropT), TVector2(m_fCropR, m_fCropB), false);
}

#endif