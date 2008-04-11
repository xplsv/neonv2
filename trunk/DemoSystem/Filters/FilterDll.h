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

#ifndef FILTERDLL_H
#define FILTERDLL_H

#include "Filter.h"

struct TFilterTgt
{
  IDirect3DTexture9 *pTexSrc;
  IDirect3DTexture9 *pTexTgt;
};

typedef int       (__cdecl   *FilterDLL_Init)        (int iVersion, void *pFilterInstance, IDirect3DDevice9 *pD3DDevice, vector< pair<string,string> > const &vOptions, unsigned *pID);
typedef int       (__cdecl   *FilterDLL_End)         (unsigned uID);
typedef void      (__cdecl   *FilterDLL_SetTime)     (unsigned uID, float fTime);
typedef bool      (__cdecl   *FilterDLL_Apply)       (unsigned uID, TFilterTgt *pFilterTgt);
typedef TCtrlVar *(__cdecl   *FilterDLL_GetVarCtrls) (unsigned uID);
typedef void      (__cdecl   *FilterDLL_SetVar)      (unsigned uID, int iVar, void *pData);
typedef void     *(__cdecl   *FilterDLL_GetVar)      (unsigned uID, int iVar);

struct TCtrlVar;

class CFilterDll : public CFilter
{
  public:
    virtual              ~CFilterDll     () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          SetTime        (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, int iTexSrc, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:

    bool                  LoadFunctions  ();
    
  private:

    HINSTANCE             m_hLibrary;
    unsigned              m_uID;
    FilterDLL_Init        m_pFncInit;
    FilterDLL_End         m_pFncEnd;
    FilterDLL_SetTime     m_pFncSetTime;
    FilterDLL_Apply       m_pFncApply;
    FilterDLL_GetVarCtrls m_pFncGetVarCtrls;
    FilterDLL_SetVar      m_pFncSetVar;
    FilterDLL_GetVar      m_pFncGetVar;
};

#endif
