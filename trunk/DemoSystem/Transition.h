 //---------------------------------------------------------------------------//
// File: Transition.h
//
//---------------------------------------------------------------------------//

#ifndef TRANSITION_H
#define TRANSITION_H

#include "Lista.h"

class  CDisplayDevice;
class  CTextura;
class  TiXmlElement;
struct TCtrlVar;

class CTransition
{
  public:
                        CTransition     () { m_bOk = false; }
    virtual            ~CTransition     () { End(); }

    virtual bool        Init            (TiXmlElement *pNode);
    virtual void        End             ();
            bool        IsOk            () const { return m_bOk;     }
            bool        DoingTransition () const { return m_bDoingTransition; }
            const char *GetName         () const { return m_pNombre; }

    virtual void        Run             (float fTime);
    virtual void        Draw            (CDisplayDevice *pDD, TFilterSource *pFilterSource);

    virtual const char *GetHelp         () const { return NULL; }
    virtual const char *GetCommands     () const { return NULL; }

    virtual void        SetVar          (int iVar, void *pData);
    virtual void       *GetVar          (int iVar);
    virtual TCtrlVar   *GetVars         ();

  protected:           

    virtual void        ApplyMode       (CDisplayDevice *pDD, float fAlpha);

  private:

            void        Reset           ();

  protected:           

    bool                m_bOk;
    bool                m_bDoingTransition;
    char               *m_pNombre;
};

void     GetListaTransiciones(int &iNumFiltros, const char **&pListaFiltros);
CFilter *CargaTransicion     (const char *pszFile);

#endif
