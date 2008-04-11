//------------------------------------------------------------------------------
// File: Allocator.h
//
// Desc: DirectShow sample code - interface for the CAllocator class
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef VMRALLOCATOR_H
#define VMRALLOCATOR_H

#include "Base.h"
#include <OBJBASE.H>
#include <d3d9.h>
#include <dshow.h>
#include <vmr9.h>
#include <atlbase.h>

class CAllocator: public IVMRSurfaceAllocator9, IVMRImagePresenter9
{
  public:
             CAllocator();
    virtual ~CAllocator();

    // IVMRSurfaceAllocator9
    virtual HRESULT STDMETHODCALLTYPE InitializeDevice( 
            /* [in] */ DWORD_PTR dwUserID,
            /* [in] */ VMR9AllocationInfo *lpAllocInfo,
            /* [out][in] */ DWORD *lpNumBuffers);
            
    virtual HRESULT STDMETHODCALLTYPE TerminateDevice( 
        /* [in] */ DWORD_PTR dwID);
    
    virtual HRESULT STDMETHODCALLTYPE GetSurface( 
        /* [in] */ DWORD_PTR dwUserID,
        /* [in] */ DWORD SurfaceIndex,
        /* [in] */ DWORD SurfaceFlags,
        /* [out] */ IDirect3DSurface9 **lplpSurface);
    
    virtual HRESULT STDMETHODCALLTYPE AdviseNotify( 
        /* [in] */ IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify);

    // IVMRImagePresenter9
    virtual HRESULT STDMETHODCALLTYPE StartPresenting( 
        /* [in] */ DWORD_PTR dwUserID);
    
    virtual HRESULT STDMETHODCALLTYPE StopPresenting( 
        /* [in] */ DWORD_PTR dwUserID);
    
    virtual HRESULT STDMETHODCALLTYPE PresentImage( 
        /* [in] */ DWORD_PTR dwUserID,
        /* [in] */ VMR9PresentationInfo *lpPresInfo);
    
    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        REFIID riid,
        void** ppvObject);

    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // Mias
    IDirect3DTexture9 *GetTextura() { return m_bTexturaValidada ? m_pTexture : NULL; }
    void    Invalida()              { m_bTexturaValidada = false;                    }
      
  protected:
    void    DeleteSurfaces();
    bool    NeedToHandleDisplayChange();

  private:
    bool                                    m_bTexturaValidada;
    long                                    m_refCount;
    CComPtr<IVMRSurfaceAllocatorNotify9>    m_lpIVMRSurfAllocNotify;
    IDirect3DSurface9                     **m_pSurfaces;
    IDirect3DTexture9                      *m_pTexture;
    int                                     m_iSurfaces;
};

#endif
