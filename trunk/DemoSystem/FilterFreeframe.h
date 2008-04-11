//---------------------------------------------------------------------------//
// File: FilterFreeframe.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERFREEFRAME_H
#define FILTERFREEFRAME_H

#include "Filter.h"
#include "GEDisplayDevice.h"
#include <Freeframe.h>

typedef plugMainUnion (__stdcall *FncPlugMain)(DWORD functionCode, DWORD inputValue, DWORD instanceID);

struct TCtrlVar;

class CFilterFreeframe : public CFilter
{
  public:
    virtual          ~CFilterFreeframe() { End(); }

    virtual bool      Init            (TiXmlElement *pNode);
    virtual void      End             ();

    virtual void      Run             (float fTime);
    virtual void      Draw            (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, float fAlphaGlobal);

    virtual void      SetVar          (int iVar, void *pData);
    virtual void     *GetVar          (int iVar);
    virtual TCtrlVar *GetVars         ();

  private:

    HINSTANCE         m_hLibrary;
    FncPlugMain       m_pPlugMain;
    int               m_iWidth;
    int               m_iHeight;
    int               m_iTextura;
    int               m_iTextura2;
    unsigned          m_uInstance;
    TCtrlVar         *m_pVars;
};

#endif
