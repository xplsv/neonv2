//---------------------------------------------------------------------------//
// File: VMRAllocator.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "GEDisplayDevice.h"
#include "VMRAllocator.h"

//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CAllocator::CAllocator()
{
  m_refCount  = 1;
  m_pTexture  = NULL;
  m_pSurfaces = NULL;
  m_iSurfaces = 0;
}

CAllocator::~CAllocator()
{
}

void CAllocator::DeleteSurfaces()
{
  for (int i = 0; i < m_iSurfaces; i++)
    SAFE_RELEASE(m_pSurfaces[i]);
  DISPOSE_ARRAY(m_pSurfaces);
  m_iSurfaces = 0;
}

//IVMRSurfaceAllocator9
HRESULT CAllocator::InitializeDevice( 
                                     /* [in] */ DWORD_PTR dwUserID,
                                     /* [in] */ VMR9AllocationInfo *lpAllocInfo,
                                     /* [out][in] */ DWORD *lpNumBuffers)
{
  if( lpNumBuffers == NULL )
  {
    return E_POINTER;
  }
  
  if( m_lpIVMRSurfAllocNotify == NULL )
  {
    return E_FAIL;
  }
  
  HRESULT hr = S_OK;
  
  lpAllocInfo->dwFlags |= VMR9AllocFlag_TextureSurface;

  DeleteSurfaces();
  m_iSurfaces = *lpNumBuffers;
  m_pSurfaces = NEW_ARRAY(IDirect3DSurface9 *, m_iSurfaces);
  for (int i = 0; i < m_iSurfaces; i++)
    m_pSurfaces[i] = NULL;
  hr = m_lpIVMRSurfAllocNotify->AllocateSurfaceHelper(lpAllocInfo, lpNumBuffers, m_pSurfaces);
  return hr;
}

HRESULT CAllocator::TerminateDevice(DWORD_PTR dwID)
{
  DeleteSurfaces();
  return S_OK;
}

HRESULT CAllocator::GetSurface( 
                               /* [in] */ DWORD_PTR dwUserID,
                               /* [in] */ DWORD SurfaceIndex,
                               /* [in] */ DWORD SurfaceFlags,
                               /* [out] */ IDirect3DSurface9 **lplpSurface)
{
  if( lplpSurface == NULL )
  {
    return E_POINTER;
  }
  
  if ((int)SurfaceIndex >= m_iSurfaces) 
  {
    return E_FAIL;
  }
 
  *lplpSurface = m_pSurfaces[SurfaceIndex];
  return S_OK;
}

HRESULT CAllocator::AdviseNotify(IVMRSurfaceAllocatorNotify9 *lpIVMRSurfAllocNotify)
{
  m_lpIVMRSurfAllocNotify = lpIVMRSurfAllocNotify;
  return S_OK;
}

HRESULT CAllocator::StartPresenting(DWORD_PTR dwUserID)
{
  return S_OK;
}

HRESULT CAllocator::StopPresenting(DWORD_PTR dwUserID)
{
  return S_OK;
}

HRESULT CAllocator::PresentImage(DWORD_PTR dwUserID, VMR9PresentationInfo *lpPresInfo)
{
  // parameter validation
  if (lpPresInfo == NULL)
  {
    return E_POINTER;
  }
  else if (lpPresInfo->lpSurf == NULL)
  {
    return E_POINTER;
  }
  lpPresInfo->lpSurf->GetContainer(IID_IDirect3DTexture9, (void **)&m_pTexture);
  m_bTexturaValidada = true;
 
  return S_OK;
}

bool CAllocator::NeedToHandleDisplayChange()
{
  return false;
}

// IUnknown
HRESULT CAllocator::QueryInterface(REFIID riid, void** ppvObject)
{
  HRESULT hr = E_NOINTERFACE;
  if (ppvObject == NULL)
  {
    hr = E_POINTER;
  } 
  else if (riid == IID_IVMRSurfaceAllocator9)
  {
    *ppvObject = static_cast<IVMRSurfaceAllocator9*>(this);
    AddRef();
    hr = S_OK;
  } 
  else if (riid == IID_IVMRImagePresenter9)
  {
    *ppvObject = static_cast<IVMRImagePresenter9*> (this);
    AddRef();
    hr = S_OK;
  } 
  else if (riid == IID_IUnknown)
  {
    *ppvObject = static_cast<IUnknown*>(static_cast<IVMRSurfaceAllocator9*>(this));
    AddRef();
    hr = S_OK;    
  }
  return hr;
}

ULONG CAllocator::AddRef()
{
  return InterlockedIncrement(& m_refCount);
}

ULONG CAllocator::Release()
{
  ULONG ret = InterlockedDecrement(& m_refCount);
  if (ret == 0)
  {
    delete (this);
  }
  return ret;
}
