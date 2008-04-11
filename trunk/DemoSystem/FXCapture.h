 //---------------------------------------------------------------------------//
// File: FXCapture.h
//
//---------------------------------------------------------------------------//

#ifndef FXCAPTURE_H
#define FXCAPTURE_H

#include "Effect.h"

class  CDisplayDevice;
class  TiXmlElement;
class  CVMRCapture;
struct TCtrlVar;

class CFXCapture : public CEffect
{
  public:

    virtual             ~CFXCapture     () { End(); }

    virtual bool         Init            (TiXmlElement *pElem);
    virtual void         End             ();

    virtual void         OnReset         ();
    virtual void         OnCommand       (TCommand *pComm);
    virtual void         Run             (float fTime);
    virtual void         Draw            (CDisplayDevice *pDD);

    virtual void         SetVar          (int iScope, int iObj, int iVar, void *pData);
    virtual void        *GetVar          (int iScope, int iObj, int iVar);
    virtual TCtrlVar    *GetVars         (int iScope) const;

  protected:

    CVMRCapture *m_pVMR;
};

#endif
