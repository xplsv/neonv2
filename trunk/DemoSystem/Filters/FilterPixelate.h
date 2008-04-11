//---------------------------------------------------------------------------//
// File: FilterPixelate.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERPIXELATE_H
#define FILTERPIXELATE_H

#include "Filter.h"

class CFilterPixelate : public CFilter
{
  public:
    virtual          ~CFilterPixelate () { End(); }

    virtual bool      Init            (TiXmlElement *pNode);
    virtual void      End             ();

    virtual void      Run             (float fTime);
    virtual void      Draw            (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal);

    virtual void      SetVar          (int iVar, void *pData);
    virtual void     *GetVar          (int iVar);
    virtual TCtrlVar *GetVars         ();

  private:

    int               m_iType;
    bool              m_bWireFrame;
    float             m_fIntens;
};

#endif
