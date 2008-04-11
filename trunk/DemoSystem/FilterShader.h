 //---------------------------------------------------------------------------//
// File: FilterShader.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERSHADER_H
#define FILTERSHADER_H

#include "Filter.h"

struct TCustomVar;

class CFilterShader : public CFilter
{
  public:
    virtual              ~CFilterShader  () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          Run            (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:

    void                  Draw           (CDisplayDevice *pDD, TVector2 const &uv0, TVector2 const &uv1);
    bool                  InitCtrlVar    (int iVar, const string &sVar);

  protected:

    bool                  m_bEnabled;
    int                   m_iShader;
    int                   m_pTexturas[3];
    int                   m_iNumVars;
    TCustomVar           *m_pVars;
    TCtrlVar             *m_pCtrlVars;
};

#endif
