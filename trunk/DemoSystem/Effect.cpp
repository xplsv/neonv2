//---------------------------------------------------------------------------//
// File: Effect.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "Effect.h"


//---------------------------------------------------------------------------//
// CEffect
//
//---------------------------------------------------------------------------//
CEffect::CEffect()
{
  m_fTimeAnt          = 0.f;
  m_fTime             = 0.f;
  m_fTimeFrame        = 0.f;
  m_bClear            = true;
  m_bFirstClear       = true;
  m_fStart            = 0.f;
  m_fEnd              = 1.f;
  m_fLength           = 0.f;
  m_bAllowRevPlayback = true;
  // RenderTarget
  m_RenderTarget.iTexture  = -1;
  m_RenderTarget.iFormat   = HARD_TEX_FORMAT_A8R8G8B8;
  m_RenderTarget.bUseAlpha = true;
  m_RenderTarget.vSize     = TVectorI2(g_DisplayDevice.GetModoGrafico().iWidth, g_DisplayDevice.GetModoGrafico().iHeight);
  m_bOk                    = false;
}


//---------------------------------------------------------------------------//
// ~CEffect
//
//---------------------------------------------------------------------------//
CEffect::~CEffect()
{
  DestruyeTarget();
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CEffect::SetTime(float fTime)
{
  m_fTimeFrame = m_fTime - m_fTimeAnt;
  m_fTimeAnt   = m_fTime;
  m_fTime      = fTime;
}


//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//
void CEffect::Attach()
{
  CreaTarget();
}


//---------------------------------------------------------------------------//
// Unattach
//
//---------------------------------------------------------------------------//
void CEffect::Unattach()
{
  DestruyeTarget();
}


//---------------------------------------------------------------------------//
// CreaTarget
//
//---------------------------------------------------------------------------//
void CEffect::CreaTarget()
{
  DestruyeTarget();
  m_RenderTarget.iTexture = g_pGestorMateriales->AddTextura(m_RenderTarget.vSize.x, m_RenderTarget.vSize.y, m_RenderTarget.iFormat, false, true);
  m_bFirstClear = true;
}


//---------------------------------------------------------------------------//
// DestruyeTarget
//
//---------------------------------------------------------------------------//
void CEffect::DestruyeTarget()
{
  if (m_RenderTarget.iTexture >= 0)
  {
    g_pGestorMateriales->RemoveTextura(m_RenderTarget.iTexture);
    m_RenderTarget.iTexture = -1;
  }
}
