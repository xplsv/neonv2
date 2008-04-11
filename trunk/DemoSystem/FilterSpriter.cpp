//---------------------------------------------------------------------------//
// File: FilterSpriter.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "FilterSpriter.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::SLIDER, 0, "Min Size",   true,  0, {NULL}},
  {TCtrlVar::SLIDER, 1, "Max Size",   true,  0, {NULL}},
  {TCtrlVar::SLIDER, 2, "Resolution", true,  0, {NULL}},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterSpriter::GetVarCtrls()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterSpriter::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_fMinSize    = *(float *)pData; break;
    case 1: m_fMaxSize    = *(float *)pData; break;
    case 2: m_fResolution = *(float *)pData; break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterSpriter::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_fMinSize);
    case 1: return (&m_fMaxSize);
    case 2: return (&m_fResolution);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterSpriter::Init(COptions &aOptions)
{
  m_fMinSize    = 0.1f;
  m_fMaxSize    = 0.5f;
  m_fResolution = 0.5f;
  m_iSprite     = -1;
  const string &sFile = aOptions.Option("sprite");
  if (sFile != "")
  {
    m_iSprite = g_pGestorMateriales->AddTextura(sFile);
    if (m_iSprite >= 0)
    {
      m_Ok = true;
    }
    else
      GLOG(("Err: Can't load sprite %s for CFilterSpriter\n", sFile.c_str()));
  }
  else
    GLOG(("ERR: Can't find sprite tag for CFilterSpriter\n"));

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterSpriter::End()
{
  if (IsOk())
  {
    g_pGestorMateriales->RemoveTextura(m_iSprite);
    CFilter::End();
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterSpriter::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Apply
//
//---------------------------------------------------------------------------//
bool CFilterSpriter::Apply(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  CEffects::DoSpriter(pDD, pRenderTgt->iTexture, iTexTgt, m_iSprite, m_fResolution, m_fMinSize*20.f+1.f, m_fMaxSize*75.f+m_fMinSize);

  return true;
}
