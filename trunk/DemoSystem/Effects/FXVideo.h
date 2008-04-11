 //---------------------------------------------------------------------------//
// File: FXVideo.h
//
//---------------------------------------------------------------------------//

#ifndef FXVIDEO_H
#define FXVIDEO_H

#include "Effect.h"

class  CDisplayDevice;
class  TiXmlElement;
class  CVMRVideo;
struct TCtrlVar;

class CFXVideo : public CEffect
{
  public:

    virtual             ~CFXVideo     () { End(); }

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

  private:
    
    void                 SetQuality      (float fQuality);

  private:

    CVMRVideo           *m_pVMR;
    float                m_fOldSpeed;
    unsigned             m_Color;
    float                m_fCropL;
    float                m_fCropR;
    float                m_fCropT;
    float                m_fCropB;

};

#endif
