 //---------------------------------------------------------------------------//
// File: FXEscena3D.h
//
//---------------------------------------------------------------------------//

#ifndef FXESCENA3D_H
#define FXESCENA3D_H

#include "Effect.h"

class CDisplayDevice;
class CEscena3D;

class CFXEscena3D : public CEffect
{
  public:
    virtual             ~CFXEscena3D     () { End(); }

    virtual bool         Init            (const string &sFile);
    virtual void         End             ();

    virtual void         SetTime         (float fTime);
    virtual void         Draw            (CDisplayDevice *pDD);

    virtual void         GetVarScopes    (vector<string> &Scopes) const;
    virtual void         GetVarObjects   (vector<string> &Objects, int iScope) const;
    virtual TCtrlVar    *GetVarCtrls     (int iScope);
    virtual void         SetVar          (int iScope, int iObj, int iVar, void *pData);
    virtual void        *GetVar          (int iScope, int iObj, int iVar);

  protected:

    CEscena3D           *m_pEscena3D;
    float                m_fAlphaBackground;
    float                m_fR;
    float                m_fG;
    float                m_fB;
};

#endif
