 //---------------------------------------------------------------------------//
// File: FilterGlow.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERGLOW_H
#define FILTERGLOW_H

#include "Filter.h"

class CFilterGlow : public CFilter
{
  public:
    virtual              ~CFilterGlow    () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          Run            (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:
    int                   m_iType;
    float                 m_fIntens;
};

#endif
