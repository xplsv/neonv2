//------------------------------------------------------------------------------
// Fichero: VMRVideo.h
//
//------------------------------------------------------------------------------

#ifndef VMRVIDEO_H
#define VMRVIDEO_H

#include "Base.h"
#include "VMRAllocator.h"
#include "Control.h"

class CVMRVideo
{
  public:
                CVMRVideo  () { m_bOk = false; }
    virtual    ~CVMRVideo  () { End(); }

    bool        Init       (const char *pszFile);
    void        End        ();
    bool        IsOk       () const { return m_bOk; }

    void        Run        (float fTime);
    void        Restart    ();

    void        Stop       ();
    void        Play       ();

    void        SetStart   (float fStart);
    void        SetEnd     (float fEnd  );
    void        SetPos     (float fTime );
    void        SetSpeed   (float fSpeed);
    void        SetTextura (CDisplayDevice *pDD, int iStage = 0);

    float       GetLength  () { return m_fDuration; }

  private:

    HRESULT     StartGraph (const char *pszFile);
    HRESULT     SetAllocatorPresenter(CComPtr<IBaseFilter> filter);
    void        RemoveAudio();
    void        CloseGraph ();

  private:

    bool                            m_bOk;
    float                           m_fSpeed;
    float                           m_fDuration;
    REFERENCE_TIME                  m_Start;
    REFERENCE_TIME                  m_End;
    CAllocator                     *m_pAllocator;
    DWORD_PTR                       m_userId;
    // DirectShow interfaces
    CComPtr<IGraphBuilder>          m_graph;
    CComPtr<IBaseFilter>            m_filter;
    CComPtr<IBaseFilter>            m_filterSound;
    CComPtr<IMediaControl>          m_mediaControl;
    CComPtr<IMediaSeeking>          m_mediaSeeking;
    CComPtr<IVMRSurfaceAllocator9>  m_allocator;
};

#endif
