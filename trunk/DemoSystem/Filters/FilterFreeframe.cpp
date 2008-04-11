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
#include "FilterFreeframe.h"


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterFreeframe::GetVarCtrls()
{
  return m_pVars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterFreeframe::SetVar(int iVar, void *pData)
{
  SetParameterStruct SetParam;
  SetParam.ParameterNumber   = iVar;
  memcpy(&SetParam.NewParameterValue, pData, 4);
  m_pPlugMain(FF_SETPARAMETER, (unsigned)&SetParam, m_uInstance);
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterFreeframe::GetVar(int iVar)
{
  plugMainUnion retval = m_pPlugMain(FF_GETPARAMETER, iVar, m_uInstance);
  return (&retval.fvalue);
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterFreeframe::Init(COptions &aOptions)
{
  m_Ok = false;

  const string &sPlugin = aOptions.Option("plugin");
  m_hLibrary = LoadLibrary(sPlugin.c_str());
  if (m_hLibrary)
  {
    m_iWidth  = strtoint(aOptions.Option("width",  "320"));
    m_iHeight = strtoint(aOptions.Option("height", "240"));
    m_pPlugMain = (FncPlugMain)GetProcAddress(m_hLibrary, "plugMain");
    plugMainUnion retval = m_pPlugMain ? m_pPlugMain(FF_INITIALISE, 0, 1) : retval;
    if (m_pPlugMain && (retval.ivalue == FF_SUCCESS))
    {
      m_iTextura = g_pGestorMateriales->AddTextura(m_iWidth, m_iHeight, HARD_TEX_FORMAT_A8R8G8B8, false, true, false);
      m_iTextura2= g_pGestorMateriales->AddTextura(m_iWidth, m_iHeight, HARD_TEX_FORMAT_A8R8G8B8, false, false, false, CTextura::SYSTEMMEM);

      VideoInfoStruct VideoInfo;
      VideoInfo.BitDepth    = 2;
      VideoInfo.FrameWidth  = m_iWidth;
      VideoInfo.FrameHeight = m_iHeight;
      VideoInfo.Orientation = FF_ORIENTATION_TL;
      retval = m_pPlugMain(FF_INSTANTIATE, (unsigned)&VideoInfo, 0);
      m_uInstance = retval.ivalue;
      retval = m_pPlugMain(FF_GETNUMPARAMETERS, 0, m_uInstance);
      int iNumParameters = retval.ivalue;
      // Parametros
      m_pVars = NEW_ARRAY(TCtrlVar, iNumParameters+1);
      for (int i = 0; i < iNumParameters; i++)
      {
        m_pVars[i].iType = TCtrlVar::SLIDER;
        m_pVars[i].iNum  = i;
        retval = m_pPlugMain(FF_GETPARAMETERNAME, i, m_uInstance);
        memset (m_pVars[i].pszName, 0, 16);
        strncpy(m_pVars[i].pszName, retval.svalue, 15);
        m_pVars[i].bLinkable   = true;
        m_pVars[i].iNumOptions = 0;
        // Parametros por defecto
        //retval = m_pPlugMain(FF_GETPARAMETERDEFAULT, i, m_uInstance);
        //SetVar(i, &retval.fvalue);
      }
      m_pVars[iNumParameters].iType = TCtrlVar::INVALID; // Last
      m_Ok = true;
    }
    if (!m_Ok)
    {
      if (m_pPlugMain == NULL)
        GLOG(("ERR CFilterFreeframe: Can't find plugMain function in plugin %s\n", sPlugin.c_str()));
      else if (retval.ivalue == -1)
        GLOG(("ERR CFilterFreeframe: Freeframe plugin %s version not compatible\n", sPlugin.c_str()));
      else
        GLOG(("ERR CFilterFreeframe: Can't load plugin %s. Return code = %d\n", sPlugin.c_str(), retval.ivalue));
      FreeLibrary(m_hLibrary);
    }
  }
  else
    GLOG(("ERR CFilterFreeframe: Can't load plugin %s\n", sPlugin.c_str()));

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterFreeframe::End()
{
  if (IsOk())
  {
    FreeLibrary(m_hLibrary);
    g_pGestorMateriales->RemoveTextura(m_iTextura);
    g_pGestorMateriales->RemoveTextura(m_iTextura2);
    DISPOSE_ARRAY(m_pVars);
    CFilter::End();
  }
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CFilterFreeframe::SetTime(float fTime)
{
  CFilter::SetTime(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
bool CFilterFreeframe::Apply(CDisplayDevice *pDD, int iTexSrc, int iTexTgt)
{
  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  pDD->ApplyBasicShader();
  pDD->EndScene();

  // Copiar la textura
  CTextura *pTexSrc  = g_pGestorMateriales->GetTextura(iTexSrc);
  CTextura *pTexDst  = g_pGestorMateriales->GetTextura(m_iTextura);
  CTextura *pTexDst2 = g_pGestorMateriales->GetTextura(m_iTextura2);
  CTextura *pTexTgt  = g_pGestorMateriales->GetTextura(iTexTgt);
  RECT RectSrc = {0,0, pTexSrc->GetWidth(),pTexSrc->GetHeight()};
  RECT RectDst = {0,0, pTexDst->GetWidth(),pTexDst->GetHeight()};

  // Stretch o copy directamente si el tamaño es el mismo
  if (m_iWidth != pTexSrc->GetWidth() || m_iHeight != pTexSrc->GetHeight())
  {
    pD3D->StretchRect(pTexSrc->GetSurfaceD3D(), &RectSrc, pTexDst->GetSurfaceD3D(), &RectDst, D3DTEXF_POINT);
    pD3D->GetRenderTargetData(pTexDst->GetSurfaceD3D(), pTexDst2->GetSurfaceD3D());
  }
  else
    pD3D->GetRenderTargetData(pTexSrc->GetSurfaceD3D(), pTexDst2->GetSurfaceD3D());

  // Llamar el process del filtro
  TSurfaceDesc Desc;
  if (pTexDst2->Lock(NULL, Desc))
  {
    m_pPlugMain(FF_PROCESSFRAME, (unsigned)Desc.pBits, m_uInstance);
    pTexDst2->Unlock();
  }
  // Stretch o copy directamente si el tamaño es el mismo
  if (m_iWidth != pTexSrc->GetWidth() || m_iHeight != pTexSrc->GetHeight())
  {
    RECT RectTgt = {0,0, pTexTgt->GetWidth(),pTexTgt->GetHeight()};
    pD3D->UpdateSurface(pTexDst2->GetSurfaceD3D(), NULL, pTexDst->GetSurfaceD3D(), NULL);
    pD3D->StretchRect  (pTexDst->GetSurfaceD3D(), &RectDst, pTexTgt->GetSurfaceD3D(), &RectTgt, D3DTEXF_POINT);
  }
  else
  {
    POINT p = {0,0};
    RECT RectTgt = {0,0, pTexTgt->GetWidth(),pTexTgt->GetHeight()};
    pD3D->UpdateSurface(pTexDst2->GetSurfaceD3D(), &RectDst, pTexTgt->GetSurfaceD3D(), &p);
  }

  pDD->BeginScene();

  return true;
}
