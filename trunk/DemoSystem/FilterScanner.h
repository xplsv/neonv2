//---------------------------------------------------------------------------//
// File: FilterScanner.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERSCANNER_H
#define FILTERSCANNER_H

#include "Filter.h"

class CFilterScanner : public CFilter
{
  public:
    virtual          ~CFilterScanner  () { End(); }

    virtual bool      Init            (TiXmlElement *pNode);
    virtual void      End             ();

    virtual void      Run             (float fTime);
    virtual void      Draw            (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal);

    virtual void      SetVar          (int iVar, void *pData);
    virtual void     *GetVar          (int iVar);
    virtual TCtrlVar *GetVars         ();

  private:

};

#endif
