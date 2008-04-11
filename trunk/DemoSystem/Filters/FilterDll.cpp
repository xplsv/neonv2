//---------------------------------------------------------------------------//
// Neon v2.5
// Copyright (C) 2006,2008 Jordi Ros <shine.3p@gmail.com>
// www.neonv2.com / www.xplsv.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program called license.txt
// If not, see <http://www.gnu.org/licenses/>
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
    GLOG(("ERR: FilterDLL version from file %s is not compatible with current version (%d)\n", sFile.c_str(), FILTER_VERSION));
    FreeLibrary(m_hLibrary);
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
// SetTime
//
//---------------------------------------------------------------------------//
void CFilterDll::SetTime(float fTime)
{
  CFilter::SetTime(fTime);
  m_pFncSetTime(m_uID, fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
bool CFilterDll::Apply(CDisplayDevice *pDD, int iTexSrc, int iTexTgt)
{
  static TFilterTgt FilterTgt;
  FilterTgt.pTexSrc = g_pGestorMateriales->GetTextura(iTexSrc)->GetTexturaD3D();
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
  if (!(m_pFncSetTime     = (FilterDLL_SetTime)    GetProcAddress(m_hLibrary, "Filter_SetTime"    ))) return false;
  if (!(m_pFncApply       = (FilterDLL_Apply)      GetProcAddress(m_hLibrary, "Filter_Apply"      ))) return false;
  if (!(m_pFncGetVarCtrls = (FilterDLL_GetVarCtrls)GetProcAddress(m_hLibrary, "Filter_GetVarCtrls"))) return false;
  if (!(m_pFncSetVar      = (FilterDLL_SetVar)     GetProcAddress(m_hLibrary, "Filter_SetVar"     ))) return false;
  if (!(m_pFncGetVar      = (FilterDLL_GetVar)     GetProcAddress(m_hLibrary, "Filter_GetVar"     ))) return false;
  return true;
}
