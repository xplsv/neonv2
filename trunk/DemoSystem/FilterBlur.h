 //---------------------------------------------------------------------------//
// File: FilterBlur.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERBLUR_H
#define FILTERBLUR_H

#include "Filter.h"

class CFilterBlur : public CFilter
{
  public:
    virtual              ~CFilterBlur    () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          Run            (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:
    int                   m_iType;
    bool                  m_bAnimacion;
    int                   m_iState;
    int                   m_iSteps;
    float                 m_fRadio;
    float                 m_fIntens;
    float                 m_fIntensAct;
    float                 m_fAnimSpeed;
    TVector2              m_fRangoAnimacion;
    TVector2              m_PosCentro;
    TVector2              m_Pos;
};

#endif
