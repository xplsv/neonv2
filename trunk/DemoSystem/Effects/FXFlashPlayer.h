 //---------------------------------------------------------------------------//
// File: FXFlash.h
//
//---------------------------------------------------------------------------//

#ifndef FXFLASH_H
#define FXFLASH_H

#include "Effect.h"

class  CDisplayDevice;
class  CFlash;
class  CTextura;
class  TiXmlElement;
struct TCtrlVar;

class CFXFlash : public CEffect
{
  public:

  public:

    virtual             ~CFXFlash        () { End(); }

    virtual bool         Init            (TiXmlElement *pElem);
    virtual void         End             ();

    virtual void         Attach          ();
    virtual void         Unattach        ();

    virtual void         OnReset         ();
    virtual void         OnCommand       (TCommand *pComm);
    virtual void         Run             (float fTime);
    virtual void         Draw            (CDisplayDevice *pDD);

    virtual void         SetVar          (int iScope, int iObj, int iVar, void *pData);
    virtual void        *GetVar          (int iScope, int iObj, int iVar);
    virtual TCtrlVar    *GetVars         (int iScope) const;

  protected:

    CFlash              *m_pFlash;
    float                m_fQuality;
    int                  m_iWidth;
    int                  m_iHeight;
    int                  m_iTextura;
};

#endif
