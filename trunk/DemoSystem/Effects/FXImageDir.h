 //---------------------------------------------------------------------------//
// File: FXImagen.h
//
//---------------------------------------------------------------------------//

#ifndef FXIMAGEN_H
#define FXIMAGEN_H

#include "Effect.h"

class CDisplayDevice;

struct TImage
{
  int      iIDTex;
  int      iStretchMode;
  bool     bWaveXform;
  TVector2 vPosIni;
  TVector2 vPos;
  TVector2 vSpeed;
  TVector2 vXform;
};

class CFXImagen : public CEffect
{
  public:
    
    virtual             ~CFXImagen       () { End(); }

    virtual bool         Init            (const string &sFile);
    virtual void         End             ();

    virtual void         SetTime         (float fTime);
    virtual void         Draw            (CDisplayDevice *pDD, int iTextureTgt);

    virtual void         GetVarScopes    (vector<string> &Scopes) const;
    virtual void         GetVarObjects   (vector<string> &Objects, int iScope) const;
    virtual TCtrlVar    *GetVarCtrls     (int iScope);
    virtual void         SetVar          (int iScope, int iObj, int iVar, void *pData);
    virtual void        *GetVar          (int iScope, int iObj, int iVar);

  protected:

    int                  m_iNumImagenes;
    int                  m_iImagen;
    TImage              *m_pImagenes;
    vector<string>       m_NombreImagenes;
};

#endif
