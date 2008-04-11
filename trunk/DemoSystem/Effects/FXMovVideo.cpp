//---------------------------------------------------------------------------//
// File: FXMovVideo.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "MovDecoder.h"
#include "FXMovVideo.h"
#include "CtrlVar.h"
#include "Xml.h"
#include <math.h>

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::CHECK_BOX,  0, "Clear",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,     1, "Red",    true,  0, {NULL}},
  {TCtrlVar::SLIDER,     2, "Green",  true,  0, {NULL}},
  {TCtrlVar::SLIDER,     3, "Blue",   true,  0, {NULL}},
  {TCtrlVar::SLIDER,     4, "Start",  false, 0, {NULL}},
  {TCtrlVar::SLIDER,     5, "End",    false, 0, {NULL}},
  {TCtrlVar::SLIDER,     6, "Crop L", true,  0, {NULL}},
  {TCtrlVar::SLIDER,     7, "Crop R", true,  0, {NULL}},
  {TCtrlVar::SLIDER,     8, "Crop T", true,  0, {NULL}},
  {TCtrlVar::SLIDER,     9, "Crop B", true,  0, {NULL}},
  {TCtrlVar::CHECK_BOX, 10, "Invert", true,  0, {NULL}},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXMovVideo::GetVars(int iScope) const
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXMovVideo::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iVar)
  {
    case 0:  m_bClear = *(bool *)pData; break;
    case 1:  HARD_COLOR_SET_R(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 2:  HARD_COLOR_SET_G(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 3:  HARD_COLOR_SET_B(m_Color, (uchar)(*(float *)pData * 255.f)); break;
    case 4:  m_fStart = *(float *)pData; break;
    case 5:  m_fEnd   = *(float *)pData; break;
    case 6:  m_fCropL = *(float *)pData; break;
    case 7:  m_fCropR = *(float *)pData; break;
    case 8:  m_fCropT = *(float *)pData; break;
    case 9:  m_fCropB = *(float *)pData; break;
    case 10: m_bInvert= *(bool  *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXMovVideo::GetVar(int iScope, int iObj, int iVar)
{
  static float c;
  switch (iVar)
  {
    case 0:  return (&m_bClear);
    case 1:  c = HARD_COLOR_GET_R(m_Color) * 1.f/255.f; return (&c);
    case 2:  c = HARD_COLOR_GET_G(m_Color) * 1.f/255.f; return (&c);
    case 3:  c = HARD_COLOR_GET_B(m_Color) * 1.f/255.f; return (&c);
    case 4:  return (&m_fStart);
    case 5:  return (&m_fEnd);
    case 6:  return (&m_fCropL);
    case 7:  return (&m_fCropR);
    case 8:  return (&m_fCropT);
    case 9:  return (&m_fCropB);
    case 10: return (&m_bInvert);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXMovVideo::Init(TiXmlElement *pElem)
{
  CEffect::Init(pElem);

  const char *pszFile = SafeString(pElem, "fxfile");
  if (pszFile)
  {
    m_Mov = NEW CMovDecoder;
    if (m_Mov->Init(pszFile, m_Bpp))
    {
      // Overload target settings
      if (m_Bpp == 32)
        m_iTextura = g_pGestorMateriales->AddTextura(m_Mov->GetWidth(), m_Mov->GetHeight(), HARD_TEX_FORMAT_A8R8G8B8);
      else
        m_iTextura = g_pGestorMateriales->AddTextura(m_Mov->GetWidth(), m_Mov->GetHeight(), HARD_TEX_FORMAT_A1R5G5B5);
      if (m_iTextura < 0)
      {
        DISPOSE(m_Mov);
        GLOG(("ERR: Can't initialize source texture for Mov file %s\n", pszFile));
        m_bOk = false;
      }
    }
    else
    {
      DISPOSE(m_Mov);
      GLOG(("ERR: Can't initialize Mov file %s\n", pszFile));
      m_bOk = false;
    }
  }
  else
  {
    GLOG(("ERR: FXFile not specified for an FXMovVideo instance\n"));
    m_bOk = false;
  }

  // Save length
  if (IsOk())
  {
    m_fLength = m_Mov->Length();
    m_fStart  = 0.f;
    m_fEnd    = 1.f;
    m_fCropL  = 0.f;
    m_fCropR  = 1.f;
    m_fCropT  = 0.f;
    m_fCropB  = 1.f;
    m_Color   = HARD_COLOR_ARGB(255,255,255,255);
    m_bInvert = false;
    OnReset();
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXMovVideo::End()
{
  if (IsOk())
  {
    g_pGestorMateriales->RemoveTextura(m_iTextura);
    DISPOSE(m_Mov);
    CEffect::End();
  }
}


//---------------------------------------------------------------------------//
// OnReset
//
//---------------------------------------------------------------------------//
void CFXMovVideo::OnReset()
{
  CEffect::OnReset();
  m_iCachedFrame = -1;
  m_iFrame       = 0;
}


//---------------------------------------------------------------------------//
// OnCommand
//
//---------------------------------------------------------------------------//
void CFXMovVideo::OnCommand(TCommand *pComm)
{
  CEffect::OnCommand(pComm);
}
   

//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXMovVideo::Run(float fTime)
{
  CEffect::Run(fTime);
  // Para la reproduccion hacia atras
  while (m_fTime < 0.f)
    m_fTime+= m_fLength;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXMovVideo::Draw(CDisplayDevice *pDD)
{
  pDD->SetRenderTarget(m_RenderTarget.iTexture);
  
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  // Frame actual
  m_iFrame = m_Mov->TimeToFrame(fmodf(m_fTime, GetLength()) + m_fStart*m_fLength);
  if (m_iFrame != m_iCachedFrame)
  {
    CTextura *pTextura = g_pGestorMateriales->GetTextura(m_iTextura);
    if (!m_Mov->GetFrame(m_iFrame, pTextura))
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

  if (m_bInvert)
  {
    pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
    pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
  }
  else
  {
    pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
    pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
		pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
  }

  DrawQuadUV(pDD, TVector2(m_fCropL, 1.f-m_fCropB), TVector2(m_fCropR, 1.f-m_fCropT), true);
}
