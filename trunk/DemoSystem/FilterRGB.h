//---------------------------------------------------------------------------//
// File: FilterRGB.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERRGB_H
#define FILTERRGB_H

#include "Filter.h"

class CFilterRGB : public CFilter
{
  public:
    virtual          ~CFilterRGB      () { End(); }

    virtual bool      Init            (TiXmlElement *pNode);
    virtual void      End             ();

    virtual void      Run             (float fTime);
    virtual void      Draw            (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal);

    virtual void      SetVar          (int iVar, void *pData);
    virtual void     *GetVar          (int iVar);
    virtual TCtrlVar *GetVars         ();

  private:

    float   m_fIntensX;
    float   m_fIntensY;
    float   m_fRandom;
};

#endif
