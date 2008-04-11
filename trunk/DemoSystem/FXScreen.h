 //---------------------------------------------------------------------------//
// File: FXScreen.h
//
//---------------------------------------------------------------------------//

#ifndef FXSCREEN_H
#define FXSCREEN_H

#include "Effect.h"

class CDisplayDevice;

class CFXScreen : public CEffect
{
  public:
    
    virtual            ~CFXScreen       () { End(); }

    virtual bool        Init            (const string &sFile);
    virtual void        End             ();

    virtual void        Draw            (CDisplayDevice *pDD);
};

#endif
