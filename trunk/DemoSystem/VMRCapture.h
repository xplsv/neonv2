//------------------------------------------------------------------------------
// Fichero: VMRCapture.h
//
//------------------------------------------------------------------------------

#ifndef VMRCAPTURE_H
#define VMRCAPTURE_H

#include "Base.h"
#include "VMRAllocator.h"
#include "Control.h"

class CVMRCapture
{
  public:
                CVMRCapture() { m_bOk = false; }
    virtual    ~CVMRCapture() { End(); }

    bool        Init       ();
    void        End        ();
    bool        IsOk       () const { return m_bOk; }

    void        Draw       (CDisplayDevice *pDD);

  private:

    bool        BindFilter (IBaseFilter **pFilter);
    HRESULT     StartGraph ();
    HRESULT     SetAllocatorPresenter(CComPtr<IBaseFilter> filter);
    void        CloseGraph ();

  private:

    bool                            m_bOk;
    CAllocator                     *m_pAllocator;
    DWORD_PTR                       m_userId;
    // DirectShow interfaces
    CComPtr<IGraphBuilder>          m_graph;
    CComPtr<IBaseFilter>            m_filter;
    CComPtr<IBaseFilter>            m_filterCapture;
    CComPtr<IMediaControl>          m_mediaControl;
    CComPtr<IVMRSurfaceAllocator9>  m_allocator;
    CComPtr<IPin>                   m_pCamOutPin;
};

#endif
