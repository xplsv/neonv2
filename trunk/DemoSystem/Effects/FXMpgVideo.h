//---------------------------------------------------------------------------//
// File: FXMpgVideo.h
//
//---------------------------------------------------------------------------//

#ifndef FXMPGVIDEO_H
#define FXMPGVIDEO_H

#include "Effect.h"

class  CDisplayDevice;
class  CFFMpegVideo;
class  TiXmlElement;
struct TCtrlVar;

class CFXMpgVideo : public CEffect
{
  public:

    virtual             ~CFXMpgVideo    () { End(); }

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

    CFFMpegVideo        *m_pVideo;
    int                  m_iTextura;
    unsigned             m_Color;
    float                m_fCropL;
    float                m_fCropR;
    float                m_fCropT;
    float                m_fCropB;


};

#endif
