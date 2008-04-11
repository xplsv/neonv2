//---------------------------------------------------------------------------//
// File: FilterGlow.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "FilterGlow.h"

enum
{
  MIPMAP,
  PIXELSHADER,
  PSYCHO,
};

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "GlowType",      true, 3, {"MipMap", "Pixel (PS2.0)", "Psycho (PS2.0)"}},
  {TCtrlVar::SLIDER,    1, "GlowIntensity", true, 0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterGlow::GetVarCtrls()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterGlow::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_iType   = *(int   *)pData; break;
    case 1: m_fIntens = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterGlow::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_iType);
    case 1: return (&m_fIntens);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterGlow::Init(COptions &aOptions)
{
  m_Ok      = true;
  m_iType   = 1;
  m_fIntens = 1.0f;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterGlow::End()
{
  CFilter::End();
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterGlow::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Apply
//
//---------------------------------------------------------------------------//
bool CFilterGlow::Apply(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  switch (m_iType)
  {
    case MIPMAP:      CEffects::DoGlowMipmap  (pDD, pRenderTgt->iTexture, m_fIntens); break;
    case PIXELSHADER: CEffects::DoGlowPS      (pDD, pRenderTgt->iTexture, m_fIntens); break;
    case PSYCHO:      CEffects::DoPsychoEffect(pDD, pRenderTgt->iTexture, m_fIntens); break;
  }
  return false;
}
