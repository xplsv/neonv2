//---------------------------------------------------------------------------//
// File: FilterSpriter.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERSPRITER_H
#define FILTERSPRITER_H

#include "Filter.h"

class CFilterSpriter : public CFilter
{
  public:
    virtual              ~CFilterSpriter () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          Run            (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:

    int                   m_iSprite;
    float                 m_fResolution;
    float                 m_fMinSize;
    float                 m_fMaxSize;
};

#endif
