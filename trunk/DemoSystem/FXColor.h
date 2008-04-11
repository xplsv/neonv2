//---------------------------------------------------------------------------//
// File: FXColor.h
//
//---------------------------------------------------------------------------//

#ifndef FXCOLOR_H
#define FXCOLOR_H

#include "Effect.h"

class CDisplayDevice;

class CFXColor : public CEffect
{
  public:
    
    virtual               ~CFXColor        () { End(); }

    virtual bool           Init            (const string &sFile);
    virtual void           End             ();

    virtual void           Draw            (CDisplayDevice *pDD);

    virtual TCtrlVar      *GetVarCtrls     (int iScope);
    virtual void           SetVar          (int iScope, int iObj, int iVar, void *pData);
    virtual void          *GetVar          (int iScope, int iObj, int iVar);

  protected:

    float    m_fRed;
    float    m_fGreen;
    float    m_fBlue;
};

#endif
