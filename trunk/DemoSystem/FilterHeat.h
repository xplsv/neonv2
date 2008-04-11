 //---------------------------------------------------------------------------//
// File: FilterHeat.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERHEAT_H
#define FILTERHEAT_H

#include "Filter.h"

class CFilterHeat : public CFilter
{
  public:
    virtual          ~CFilterHeat     () { End(); }

    virtual bool      Init            (TiXmlElement *pNode);
    virtual void      End             ();

    virtual void      Run             (float fTime);
    virtual void      Draw            (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal);

    virtual void      SetVar          (int iVar, void *pData);
    virtual void     *GetVar          (int iVar);
    virtual TCtrlVar *GetVars         ();

  private:

  private:

    int               m_iTexAnterior;
    int               m_iTexHeat[2];
    bool              m_bFrame;
};

#endif
