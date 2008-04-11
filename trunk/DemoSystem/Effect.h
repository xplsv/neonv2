 //---------------------------------------------------------------------------//
// File: Effect.h
//
//---------------------------------------------------------------------------//

#ifndef EFFECT_H
#define EFFECT_H

#include "GEEngineGrafico.h"
#include "RenderTarget.h"

struct TCtrlVar;

class CEffect
{
  public:
    enum
    {
      FXSCOPE_MAIN = -1,
    };
                           CEffect         ();
    virtual               ~CEffect         ();

    virtual bool           Init            (const string &sFile) = 0;

            bool           IsOk            () const { return  m_bOk;      }
            float          GetLength       () const { return  m_fLength * (m_fEnd - m_fStart); }
            float          GetPos          () const { return  m_fTime;    }
            TRenderTgt    *GetRenderTarget ()       { return &m_RenderTarget; }

    virtual void           Attach          ();
    virtual void           Unattach        ();
            
    virtual void           SetTime         (float fTime);
    virtual void           Draw            (CDisplayDevice *pDD) { }

    virtual void           GetVarScopes    (vector<string> &Scopes)              const { Scopes.clear(); }
    virtual void           GetVarObjects   (vector<string> &Objects, int iScope) const { Objects.clear(); }
    virtual TCtrlVar      *GetVarCtrls     (int iScope = FXSCOPE_MAIN)  { return NULL; }
    virtual void           SetVar          (int iScope, int iObj, int iVar, void *pData) { }
    virtual void          *GetVar          (int iScope, int iObj, int iVar) { return NULL; }

  protected:

            void           CreaTarget      ();
            void           DestruyeTarget  ();

  protected:

    bool                   m_bOk;
    bool                   m_bFirstClear;
    bool                   m_bClear;
    bool                   m_bMotionBlur;
    bool                   m_bAllowRevPlayback;
    float                  m_fMotionIntens;
    float                  m_fTime;
    float                  m_fTimeAnt;
    float                  m_fTimeFrame;
    float                  m_fLength;
    float                  m_fStart;
    float                  m_fEnd;
    TRenderTgt             m_RenderTarget;
};

#endif
