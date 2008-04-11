//---------------------------------------------------------------------------//
// File: Sound.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Sound.h"
#include "Bass.h"

static bool s_DataReady = false;

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
CSound::CSound(HWND hWnd)
{
  if (BASS_GetVersion() != MAKELONG(2,1))
  {
    GLOG(("ERR: BASS version 2.1 was not loaded\n"));
    return;
  }

  if (!BASS_Init(1, 44100, 0, (HWND)hWnd, NULL))
  {
    GLOG(("ERR: Can't initialize BASS sound system\n"));
    return;
  }

  m_Playing = false;
  m_hStream = NULL;
  m_hRecord = NULL;
  m_bOk     = true;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
CSound::~CSound()
{
  if (m_hStream) BASS_StreamFree(m_hStream);
  if (m_hRecord) BASS_RecordFree();
  BASS_Free();
}


// buffer the recorded data
BOOL CALLBACK RecordingCallback(HRECORD handle, void *buffer, DWORD length, DWORD user)
{
  s_DataReady = true;
	return TRUE;
}

//---------------------------------------------------------------------------//
// Record
//
//---------------------------------------------------------------------------//
void CSound::Record()
{
	if (BASS_RecordInit(0))
  {
    m_hRecord = BASS_RecordStart(22050, 1, MAKELONG(0,25), &RecordingCallback, 0);
  }
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//
void CSound::Load(const std::string sFile)
{
  m_hStream = BASS_StreamCreateFile(FALSE, sFile.c_str(), 0,0, BASS_MP3_SETPOS);
}


//---------------------------------------------------------------------------//
// Play
//
//---------------------------------------------------------------------------//
void CSound::Play(float fStart)
{
  if (m_hStream)
  {
    BASS_ChannelSetPosition(m_hStream, BASS_ChannelSeconds2Bytes(m_hStream, fStart));
    BASS_ChannelPlay       (m_hStream, FALSE);
  }
}


//---------------------------------------------------------------------------//
// Stop
//
//---------------------------------------------------------------------------//
void CSound::Stop()
{
  if (m_hStream)
  {
    BASS_ChannelStop(m_hStream);
  }
}


//---------------------------------------------------------------------------//
// GetFFT
//
//---------------------------------------------------------------------------//
bool CSound::GetFFT(float *pFFT)
{
  if (s_DataReady)
  {
    if (m_hStream)
      BASS_ChannelGetData(m_hStream, pFFT, BASS_DATA_FFT512);
    else if (m_hRecord)
      BASS_ChannelGetData(m_hRecord, pFFT, BASS_DATA_FFT512);
    s_DataReady = false;
    return true;
  }
  return false;
}
