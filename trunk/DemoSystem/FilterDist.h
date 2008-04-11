//---------------------------------------------------------------------------//
// File: FilterDist.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERDIST_H
#define FILTERDIST_H

#include "Filter.h"

class CFilterDist : public CFilter
{
  public:
    virtual              ~CFilterDist    () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          Run            (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:
            void          DrawCrystal    (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);
            void          DrawCircCrystal(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);
            void          DrawSin        (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);
            void          DrawPolar      (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);
            void          DrawPlasma     (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);
            void          DrawFish       (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);
            void          DrawMov        (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);
            void          DrawFlip       (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);

  private:
    int                   m_iType;
    float                 m_fSpeed;
    float                 m_fIntens;
    float                 m_fVars[6];
};

#endif
