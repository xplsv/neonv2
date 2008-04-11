 //---------------------------------------------------------------------------//
// File: FilterDll.h
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
typedef void      (__cdecl   *FilterDLL_Run)         (unsigned uID, float fTime);
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

    virtual void          Run            (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);

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
    FilterDLL_Run         m_pFncRun;
    FilterDLL_Apply       m_pFncApply;
    FilterDLL_GetVarCtrls m_pFncGetVarCtrls;
    FilterDLL_SetVar      m_pFncSetVar;
    FilterDLL_GetVar      m_pFncGetVar;
};

#endif
