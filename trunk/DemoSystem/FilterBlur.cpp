//---------------------------------------------------------------------------//
// File: FilterBlur.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "FilterBlur.h"

enum
{
  RADIAL,
  BOX,
  BOXPS,
  HORZ,
  VERT,
};

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "BlurType",    true, 5, {"Radial", "Box", "Box (PS2.0)", "Horz", "Vert"}},
  {TCtrlVar::SLIDER,    1, "Radio",       true, 0, {NULL}},
  {TCtrlVar::CHECK_BOX, 2, "Anim",        true, 0, {NULL}},
  {TCtrlVar::SLIDER,    3, "AnimHRange",  true, 0, {NULL}},
  {TCtrlVar::SLIDER,    4, "AnimVRange",  true, 0, {NULL}},
  {TCtrlVar::SLIDER,    5, "AnimSpeed",   true, 0, {NULL}},
  {TCtrlVar::SLIDER,    6, "Intensity",   true, 0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterBlur::GetVarCtrls()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterBlur::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_iType             = *(int   *)pData; break;
    case 1: m_fRadio            = *(float *)pData; break;
    case 2: m_bAnimacion        = *(bool  *)pData; break;
    case 3: m_fRangoAnimacion.x = *(float *)pData; break;
    case 4: m_fRangoAnimacion.y = *(float *)pData; break;
    case 5: m_fAnimSpeed        = *(float *)pData; break;
    case 6: m_fIntensAct        = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterBlur::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_iType);
    case 1: return (&m_fRadio);
    case 2: return (&m_bAnimacion);
    case 3: return (&m_fRangoAnimacion.x);
    case 4: return (&m_fRangoAnimacion.y);
    case 5: return (&m_fAnimSpeed);
    case 6: return (&m_fIntensAct);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
// Options: None
//---------------------------------------------------------------------------//
bool CFilterBlur::Init(COptions &aOptions)
{
  m_Ok                = true;
  m_iType             = BOX;
  m_bAnimacion        = true;
  m_iSteps            = 12;
  m_PosCentro.x       = 0.5f;
  m_PosCentro.y       = 0.5f;
  m_fAnimSpeed        = 1.f;
  m_fRangoAnimacion.x = 1.f;
  m_fRangoAnimacion.y = 1.f;
  m_fRadio            = 0.5f;
  m_fIntens           = 0.5f;
  m_fIntensAct        = m_fIntens;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterBlur::End()
{
  CFilter::End();
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterBlur::Run(float fTime)
{
  CFilter::Run(fTime);
  if (m_bAnimacion)
  {
    float fV = m_fTime * m_fAnimSpeed;
    m_Pos.x = (m_fRangoAnimacion.x * ((float)sin(fV*6.2f) * 0.28f + (float)cos(fV*5.4f) * 0.20f)) + m_PosCentro.x;
    m_Pos.y = (m_fRangoAnimacion.y * ((float)cos(fV*4.7f) * 0.17f + (float)sin(fV*6.7f) * 0.26f)) + m_PosCentro.y;
  }
  else
  {
    m_Pos.x = m_PosCentro.x;
    m_Pos.y = m_PosCentro.y;
  }
}


//---------------------------------------------------------------------------//
// Apply
//
//---------------------------------------------------------------------------//
bool CFilterBlur::Apply(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  switch (m_iType)
  {
    case BOX:    CEffects::DoBoxBlur        (pDD, pRenderTgt->iTexture, m_fIntensAct * 32.f); break;
    case BOXPS:  CEffects::DoBlur           (pDD, pRenderTgt->iTexture, m_fIntensAct); break;
    case RADIAL: CEffects::DoRadialBlurSteps(pDD, pRenderTgt->iTexture, m_fRadio*80.f, m_fIntensAct, m_iSteps, m_Pos); break;
    case HORZ:   CEffects::DoHorzBlur       (pDD, pRenderTgt->iTexture, m_fIntensAct); break;
    case VERT:   CEffects::DoVertBlur       (pDD, pRenderTgt->iTexture, m_fIntensAct); break;
  }
  return false;
}
