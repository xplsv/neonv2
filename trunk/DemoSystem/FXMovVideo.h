 //---------------------------------------------------------------------------//
// File: FXMovVideo.h
//
//---------------------------------------------------------------------------//

#ifndef FXMOVVIDEO_H
#define FXMOVVIDEO_H

#include "Effect.h"

class  CDisplayDevice;
class  CMovDecoder;
class  CTextura;
class  TiXmlElement;
struct TCtrlVar;

class CFXMovVideo : public CEffect
{
  public:

                         CFXMovVideo     () { CEffect::CEffect(); }
    virtual             ~CFXMovVideo     () { End(); }

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

    CMovDecoder         *m_Mov;
    int                  m_Bpp;
    int                  m_iFrame;
    int                  m_iCachedFrame;
    int                  m_iBlend;
    int                  m_iTextura;
    unsigned             m_Color;
    float                m_fCropL;
    float                m_fCropR;
    float                m_fCropT;
    float                m_fCropB;
    bool                 m_bInvert;
};

#endif
