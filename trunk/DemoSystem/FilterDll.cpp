//---------------------------------------------------------------------------//
// File: FilterDll.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "FilterDll.h"

#define FILTER_VERSION 3

//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterDll::GetVarCtrls()
{
  return m_pFncGetVarCtrls(m_uID);
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterDll::SetVar(int iVar, void *pData)
{
  m_pFncSetVar(m_uID, iVar, pData);
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterDll::GetVar(int iVar)
{
  return m_pFncGetVar(m_uID, iVar);
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterDll::Init(COptions &aOptions)
{
  m_Ok = false;

  const string &sFile = aOptions.Option("library");
  if (sFile == "")
  {
    GLOG(("ERR: Library file not specified\n"));
    return false;
  }
  m_hLibrary = LoadLibrary(sFile.c_str());
  if (!m_hLibrary)
  {
    GLOG(("ERR: Can't load library %s\n", sFile.c_str()));
    return false;
  }

  if (!LoadFunctions())
  {
    GLOG(("ERR: FilterDLL version from file %s is not compatible with this one (%d)\n", sFile.c_str(), FILTER_VERSION));
    return false;
  }

  // Filter init
  int iErr = m_pFncInit(FILTER_VERSION, this, g_DisplayDevice.GetD3DDevice(), aOptions.Options(), &m_uID);
  if (iErr)
  {
    if (iErr == -1)
      GLOG(("ERR: FilterDLL version from file %s is not compatible with this one (%d)\n", sFile.c_str(), FILTER_VERSION));
    else
      GLOG(("ERR: FilterDLL can't load library %s. Return code = %d\n", sFile.c_str(), iErr));
    FreeLibrary(m_hLibrary);
    return false;
  }

  m_Ok = true;
  return m_Ok;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterDll::End()
{
  if (IsOk())
  {
    m_pFncEnd(m_uID);
    m_Ok = false;
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterDll::Run(float fTime)
{
  CFilter::Run(fTime);
  m_pFncRun(m_uID, fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
bool CFilterDll::Apply(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  
  static TFilterTgt FilterTgt;
  FilterTgt.pTexSrc = g_pGestorMateriales->GetTextura(pRenderTgt->iTexture)->GetTexturaD3D();
  FilterTgt.pTexTgt = g_pGestorMateriales->GetTextura(iTexTgt)->GetTexturaD3D();
  return m_pFncApply(m_uID, &FilterTgt);
}


//---------------------------------------------------------------------------//
// Load Functions
//
//---------------------------------------------------------------------------//
bool CFilterDll::LoadFunctions()
{
  if (!(m_pFncInit        = (FilterDLL_Init)       GetProcAddress(m_hLibrary, "Filter_Init"       ))) return false;
  if (!(m_pFncEnd         = (FilterDLL_End)        GetProcAddress(m_hLibrary, "Filter_End"        ))) return false;
  if (!(m_pFncRun         = (FilterDLL_Run)        GetProcAddress(m_hLibrary, "Filter_Run"        ))) return false;
  if (!(m_pFncApply       = (FilterDLL_Apply)      GetProcAddress(m_hLibrary, "Filter_Apply"      ))) return false;
  if (!(m_pFncGetVarCtrls = (FilterDLL_GetVarCtrls)GetProcAddress(m_hLibrary, "Filter_GetVarCtrls"))) return false;
  if (!(m_pFncSetVar      = (FilterDLL_SetVar)     GetProcAddress(m_hLibrary, "Filter_SetVar"     ))) return false;
  if (!(m_pFncGetVar      = (FilterDLL_GetVar)     GetProcAddress(m_hLibrary, "Filter_GetVar"     ))) return false;
  return true;
}
