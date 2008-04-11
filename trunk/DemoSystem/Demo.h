//---------------------------------------------------------------------------//
// File: Demo.h
//
//---------------------------------------------------------------------------//

#ifndef DEMO_H
#define DEMO_H

#include "Lista.h"

class  CEffect;
class  CDisplayDevice;
struct TCommand;

class CDemo
{
  public:
                      CDemo           () { m_bOk = false; }
                     ~CDemo           () { End(); }

    bool              Load            (const char *pszFile);
    void              End             ();
    bool              IsOk            () const { return m_bOk; }

    void              Reset           ();
    void              SetTime         (float fTime);
    void              Run             (float fRunTime);
    void              Draw            (CDisplayDevice *pDD);

    CEffect          *GetEffectByName (const char *pszNombre);

  private:

    void              BorraVars       ();
    void              LiberaVars      ();

  private:

    bool              m_bOk;
    float             m_fTime;
    CLLP<CEffect  *>  m_ListaEfectos;
    CLLP<TCommand *>  m_ListaComandos;
};

#endif
