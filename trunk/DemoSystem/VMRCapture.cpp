//---------------------------------------------------------------------------//
// File: VMR.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "GEEngineGrafico.h"
#include <dshow.h>
#include "VMRCapture.h"

#define FAIL_RET(x) do { if (FAILED(hr = (x))) return hr; } while(0)

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CVMRCapture::Init()
{
  m_userId = 0xBADCC0DE;
  StartGraph();
  m_bOk    = true;
  return IsOk();
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CVMRCapture::End()
{
  CloseGraph();
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
bool CVMRCapture::BindFilter(IBaseFilter **pFilter)
{
  // enumerate all video capture devices
  CComPtr<ICreateDevEnum> pCreateDevEnum;
  CComPtr<IEnumMoniker>   pEm;

  pCreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER);
  pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
  
  pEm->Reset();
  ULONG cFetched;
  IMoniker *pM;
  int index = 0;
  HRESULT hr;
  while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= 0)
  {
    IPropertyBag *pBag;
    hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
    if(SUCCEEDED(hr)) 
    {
      VARIANT var;
      var.vt = VT_BSTR;
      hr = pBag->Read(L"FriendlyName", &var, NULL);
      if (hr == NOERROR) 
      {
        if (index == 0)
        {
          pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
        }
        SysFreeString(var.bstrVal);
      }
      pBag->Release();
    }
    pM->Release();
    index++;
  }
  return true;
}


//---------------------------------------------------------------------------//
// StartGraph
//
//---------------------------------------------------------------------------//
HRESULT CVMRCapture::StartGraph()
{
  HRESULT hr;
  CComPtr<IVMRFilterConfig9> filterConfig;

  FAIL_RET(m_graph.CoCreateInstance      (CLSID_FilterGraph));
  FAIL_RET(m_filter.CoCreateInstance     (CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER));
  FAIL_RET(m_filter->QueryInterface      (IID_IVMRFilterConfig9, reinterpret_cast<void**>(&filterConfig)));
  FAIL_RET(filterConfig->SetRenderingMode(VMR9Mode_Renderless));
  //FAIL_RET(filterConfig->SetNumberOfStreams(2));
  FAIL_RET(SetAllocatorPresenter(m_filter));
  FAIL_RET(m_graph->AddFilter(m_filter, L"Video Mixing Renderer 9"));
  BindFilter(&m_filterCapture);    
  FAIL_RET(m_graph->AddFilter(m_filterCapture, L"Video Capture"));
  CComPtr<IEnumPins> pEnum;
  m_filterCapture->EnumPins(&pEnum);
  hr = pEnum->Reset();
  hr = pEnum->Next(1, &m_pCamOutPin, NULL); 

  FAIL_RET(m_graph->QueryInterface(IID_IMediaControl, reinterpret_cast<void**>(&m_mediaControl)));
  //FAIL_RET(g_graph->QueryInterface(IID_IMediaEventEx, reinterpret_cast<void**>(&g_mediaEvent)));
  FAIL_RET(m_graph->Render(m_pCamOutPin));
  FAIL_RET(m_mediaControl->Run());

  return hr;
}


//---------------------------------------------------------------------------//
// SetAllocatorPresenter
//
//---------------------------------------------------------------------------//
HRESULT CVMRCapture::SetAllocatorPresenter(CComPtr<IBaseFilter> filter)
{
  if(filter == NULL)
  {
    return E_FAIL;
  }
  
  HRESULT hr;
  
  CComPtr<IVMRSurfaceAllocatorNotify9> lpIVMRSurfAllocNotify;
  FAIL_RET(filter->QueryInterface(IID_IVMRSurfaceAllocatorNotify9, reinterpret_cast<void**>(&lpIVMRSurfAllocNotify)));
  
  // create our surface allocator
  m_pAllocator = NEW CAllocator;
  m_allocator.Attach(m_pAllocator);
  
  // let the allocator and the notify know about each other
  FAIL_RET(lpIVMRSurfAllocNotify->AdviseSurfaceAllocator(m_userId, m_allocator));
  FAIL_RET(lpIVMRSurfAllocNotify->SetD3DDevice(g_DisplayDevice.GetD3DDevice(), g_DisplayDevice.GetD3D()->GetAdapterMonitor(D3DADAPTER_DEFAULT)));
  FAIL_RET(m_allocator->AdviseNotify(lpIVMRSurfAllocNotify));
  
  return hr;
}


//---------------------------------------------------------------------------//
// CloseGraph
//
//---------------------------------------------------------------------------//
void CVMRCapture::CloseGraph()
{
  if(m_mediaControl != NULL) 
  {
    OAFilterState state;
    do
    {
      m_mediaControl->Stop();
      m_mediaControl->GetState(0, & state );
    } while( state != State_Stopped ) ;
  }
  
  m_pCamOutPin    = NULL;
  m_allocator     = NULL;        
  m_mediaControl  = NULL;        
  m_filter        = NULL;        
  m_filterCapture = NULL;
  m_graph         = NULL;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CVMRCapture::Draw(CDisplayDevice *pDD)
{
  if (!m_pAllocator || !m_pAllocator->GetTextura())
    return;

  D3DDEVICE *pD3D = pDD->GetD3DDevice();

  pDD->ApplyBasicShader();
  pD3D->SetRenderState(D3DRS_CLIPPING,         FALSE);
  pD3D->SetRenderState(D3DRS_ZWRITEENABLE,     D3DZB_FALSE);  
  pD3D->SetRenderState(D3DRS_ZENABLE,          D3DZB_FALSE);
  pD3D->SetRenderState(D3DRS_BLENDOP,          D3DBLENDOP_ADD);

  pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE  );
  pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR  );

  pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR    );

  pD3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  pD3D->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

  pD3D->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  pD3D->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

  pD3D->SetVertexShader(NULL);
  pD3D->SetPixelShader (NULL);

	pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  pD3D->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
  pD3D->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  pDD->GetD3DDevice()->SetTexture(0, m_pAllocator->GetTextura());

  DrawQuad(pDD);
}
