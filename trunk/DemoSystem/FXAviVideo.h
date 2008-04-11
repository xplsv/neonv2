 //---------------------------------------------------------------------------//
// File: FXAviVideo.h
//
//---------------------------------------------------------------------------//

#ifndef FXAVIVIDEO_H
#define FXAVIVIDEO_H

#include "Effect.h"

class  CDisplayDevice;
class  CAviDecoder;
struct TCtrlVar;

class CFXAviVideo : public CEffect
{
  public:

                         CFXAviVideo     (unsigned uBpp) { m_Bpp = uBpp; }
    virtual             ~CFXAviVideo     () { End(); }

    virtual bool         Init            (const string &sFile);
    virtual void         End             ();

    virtual void         SetTime         (float fTime);
    virtual void         Draw            (CDisplayDevice *pDD);

    virtual TCtrlVar    *GetVarCtrls     (int iScope) const;
    virtual void         SetVar          (int iScope, int iObj, int iVar, void *pData);
    virtual void        *GetVar          (int iScope, int iObj, int iVar);

  protected:

    CAviDecoder         *m_pAvi;
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
