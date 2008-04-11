//---------------------------------------------------------------------------//
// File: FXVideo.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "AviDecoder.h"
#include "FXVideo.h"
#include "GEGestorMateriales.h"
#include "GETemp.h"
#include "CtrlVar.h"
#include "Command.h"
#include "VMRVideo.h"
#include "Xml.h"
#include <math.h>

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
TCtrlVar *CFXVideo::GetVars(int iScope) const
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXVideo::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: HARD_COLOR_SET_R(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 1: HARD_COLOR_SET_G(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 2: HARD_COLOR_SET_B(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 3: m_fStart = *(float *)pData; m_pVMR->SetStart(m_fStart * m_fLength); break;
    case 4: m_fEnd   = *(float *)pData; m_pVMR->SetEnd  (m_fEnd * m_fLength);   break;
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
void *CFXVideo::GetVar(int iScope, int iObj, int iVar)
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
CVMRVideo *g_Video;
extern CVMRVideo *g_Video;
bool CFXVideo::Init(TiXmlElement *pElem)
{
  CEffect::Init(pElem);

  const char *pszFile = SafeString(pElem, "fxfile");
  if (pszFile)
  {
    m_fLength   = 0;
    m_fStart    = 0.f;
    m_fEnd      = 1.f;
    m_fOldSpeed = 1.f;
    //m_pVMR      = NEW CVMRVideo;
    m_pVMR = g_Video;
    if (!m_pVMR->Init(pszFile))
    {
      DISPOSE(m_pVMR);
      GLOG(("ERR: Can't load FXVideo file %s\n", pszFile));
      m_bOk = false;
    }
    else
    {
      m_fLength = m_pVMR->GetLength();
      m_fStart  = 0.f;
      m_fEnd    = 1.f;
      m_fCropL  = 0.f;
      m_fCropR  = 1.f;
      m_fCropT  = 0.f;
      m_fCropB  = 1.f;
      m_Color   = HARD_COLOR_ARGB(255,255,255,255);
      OnReset();
    }
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXVideo::End()
{
  if (IsOk())
  {
    DISPOSE(m_pVMR);
    CEffect::End();
  }
}


//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//
void CFXVideo::Attach()
{
  CEffect::Attach();
  m_pVMR->Play();
}


//---------------------------------------------------------------------------//
// Unattach
//
//---------------------------------------------------------------------------//
void CFXVideo::Unattach()
{
  CEffect::Unattach();
  m_pVMR->Stop();
}


//---------------------------------------------------------------------------//
// OnReset
//
//---------------------------------------------------------------------------//
void CFXVideo::OnReset()
{
  CEffect::OnReset();
  m_pVMR->Restart();
}


//---------------------------------------------------------------------------//
// OnCommand
//
//---------------------------------------------------------------------------//
void CFXVideo::OnCommand(TCommand *pComm)
{
  CEffect::OnCommand(pComm);

  if (!Stricmp(pComm->pComm, "SetTime"))
    m_pVMR->SetPos((float)strtofloat(pComm->apArgs[0]));
}
   

//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXVideo::Run(float fTime)
{
  CEffect::Run(fTime);
  if (m_fOldSpeed != m_fSpeed)
  {
    m_pVMR->SetSpeed(m_fSpeed);
    m_fOldSpeed = m_fSpeed;
  }
  m_pVMR->Run(m_fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXVideo::Draw(CDisplayDevice *pDD)
{
  pDD->SetRenderTarget(m_RenderTarget.iTexture);

  D3DDEVICE *pD3D = pDD->GetD3DDevice();

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

  m_pVMR->SetTextura(pDD, 0);

  DrawQuadUV(pDD, TVector2(m_fCropL, m_fCropT), TVector2(m_fCropR, m_fCropB), false);
}
