//---------------------------------------------------------------------------//
// File: VMRVideo.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "GEEngineGrafico.h"
#include <dshow.h>
#include "VMRVideo.h"

#define FAIL_RET(x) do { if (FAILED(hr = (x))) return hr; } while(0)

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CVMRVideo::Init(const char *pszFile)
{
  m_userId = 0xBADCC0DE;
  m_fSpeed = 1.f;
  if (SUCCEEDED(StartGraph(pszFile)))
    m_bOk = true;
  return IsOk();
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CVMRVideo::End()
{
  CloseGraph();
}


//---------------------------------------------------------------------------//
// StartGraph
//
//---------------------------------------------------------------------------//
CComPtr<IGraphBuilder>          m_graph;

HRESULT CVMRVideo::StartGraph(const char *pszFile)
{
  HRESULT hr;
  CComPtr<IVMRFilterConfig9> filterConfig;
  WCHAR pTemp[MAX_PATH];
  CHAR pDir[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, pDir);
  //sprintf (pBuffer, "%s\\%s", pDir, pszFile);
  unsigned i;
  for (i = 0; i < strlen(pszFile); i++)
    pTemp[i] = pszFile[i];
  pTemp[i] = 0;

  FAIL_RET(m_graph.CoCreateInstance      (CLSID_FilterGraph));
  FAIL_RET(m_graph->RenderFile(L"D:\\Test.mov", NULL));
/*
  FAIL_RET(m_graph.CoCreateInstance      (CLSID_FilterGraph));
  FAIL_RET(m_filter.CoCreateInstance     (CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC_SERVER));
  FAIL_RET(m_filterSound.CoCreateInstance(CLSID_DSoundRender, NULL, CLSCTX_INPROC_SERVER));
  FAIL_RET(m_filter->QueryInterface      (IID_IVMRFilterConfig9, reinterpret_cast<void**>(&filterConfig)));
  FAIL_RET(filterConfig->SetRenderingMode(VMR9Mode_Renderless));
  FAIL_RET(SetAllocatorPresenter(m_filter));
  FAIL_RET(m_graph->AddFilter(m_filter,      L"Neon Video Renderer"));
  FAIL_RET(m_graph->AddFilter(m_filterSound, L"Neon Sound Renderer"));
  FAIL_RET(m_graph->QueryInterface(IID_IMediaControl, reinterpret_cast<void**>(&m_mediaControl)));
  FAIL_RET(m_graph->QueryInterface(IID_IMediaSeeking, reinterpret_cast<void**>(&m_mediaSeeking)));
  //FAIL_RET(m_graph->QueryInterface(IID_IMediaEventEx, reinterpret_cast<void**>(&m_mediaEvent)));
  FAIL_RET(m_graph->RenderFile(L"D:\\Test.mov", NULL));
  // Quitamos el filtro de sonido 
  RemoveAudio();
  FAIL_RET(m_mediaControl->Run());
  */

  REFERENCE_TIME Duration = 0;
  hr = m_mediaSeeking->GetDuration(&Duration);
  m_fDuration = (Duration / 10000000.f);
  m_mediaSeeking->GetPositions(&m_Start, &m_End);

  return hr;
}


//---------------------------------------------------------------------------//
// SetAllocatorPresenter
//
//---------------------------------------------------------------------------//
HRESULT CVMRVideo::SetAllocatorPresenter(CComPtr<IBaseFilter> filter)
{
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
// RemoveAudio
//
//---------------------------------------------------------------------------//
void CVMRVideo::RemoveAudio()
{
  IEnumFilters *pEnum = NULL;
  ULONG         cFetched;

  IEnumPins    *EnumPins;
	IPin         *OutPin;
	IPin         *InPin;

	m_filterSound->EnumPins(&EnumPins);
	EnumPins->Reset();
	EnumPins->Next(1, &InPin, &cFetched);
	SAFE_RELEASE(EnumPins);

  InPin->ConnectedTo(&OutPin);
  if (OutPin)
  {
    InPin->Disconnect();
    OutPin->Disconnect();
  }
  SAFE_RELEASE(InPin);
  SAFE_RELEASE(OutPin);
  m_filterSound = NULL;
}


//---------------------------------------------------------------------------//
// CloseGraph
//
//---------------------------------------------------------------------------//
void CVMRVideo::CloseGraph()
{
  if(m_mediaControl != NULL) 
  {
    OAFilterState state;
    do
    {
      m_mediaControl->Stop();
      m_mediaControl->GetState(0, &state);
    } while (state != State_Stopped);
  }
  
  m_allocator    = NULL;        
  m_mediaControl = NULL;        
  m_filter       = NULL;        
  m_graph        = NULL;
}


//---------------------------------------------------------------------------//
// Play
//
//---------------------------------------------------------------------------//
void CVMRVideo::Play()
{
  m_mediaControl->Run();
}


//---------------------------------------------------------------------------//
// Stop
//
//---------------------------------------------------------------------------//
void CVMRVideo::Stop()
{
  m_mediaControl->Pause();
}


//---------------------------------------------------------------------------//
// SetStart
//
//---------------------------------------------------------------------------//
void CVMRVideo::SetStart(float fStart)
{
  m_Start = (REFERENCE_TIME)(fStart * 10000000.f);
}


//---------------------------------------------------------------------------//
// SetEnd
//
//---------------------------------------------------------------------------//
void CVMRVideo::SetEnd(float fEnd)
{
  m_End = (REFERENCE_TIME)(fEnd * 10000000.f);
}


//---------------------------------------------------------------------------//
// SetPos
//
//---------------------------------------------------------------------------//
void CVMRVideo::SetPos(float fPos)
{
  REFERENCE_TIME Pos = (REFERENCE_TIME)(fPos * 10000000.f);
  m_mediaSeeking->SetPositions(&Pos, AM_SEEKING_AbsolutePositioning, &m_End, AM_SEEKING_AbsolutePositioning);
}


//---------------------------------------------------------------------------//
// SetSpeed
//
//---------------------------------------------------------------------------//
void CVMRVideo::SetSpeed(float fSpeed)
{
  m_fSpeed = fSpeed;
  m_mediaSeeking->SetRate(fSpeed);
  double dRate;
  m_mediaSeeking->GetRate(&dRate);
}


//---------------------------------------------------------------------------//
// Restart
//
//---------------------------------------------------------------------------//
void CVMRVideo::Restart()
{
  SetPos(0.f);
  m_mediaSeeking->SetRate(m_fSpeed);
  double dRate;
  m_mediaSeeking->GetRate(&dRate);
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CVMRVideo::Run(float fTime)
{
  REFERENCE_TIME Current, Stop;
  m_mediaSeeking->GetPositions(&Current, &Stop);
  if (Current >= Stop || Current < m_Start)
    Restart();
}


//---------------------------------------------------------------------------//
// SetTextura
//
//---------------------------------------------------------------------------//
void CVMRVideo::SetTextura(CDisplayDevice *pDD, int iStage)
{
  if (!m_pAllocator || !m_pAllocator->GetTextura())
    return;

  pDD->GetD3DDevice()->SetTexture(iStage, m_pAllocator->GetTextura());
}
