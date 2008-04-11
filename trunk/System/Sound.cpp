//---------------------------------------------------------------------------//
// Neon v2.5
// Copyright (C) 2006,2008 Jordi Ros <shine.3p@gmail.com>
// www.neonv2.com / www.xplsv.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program called license.txt
// If not, see <http://www.gnu.org/licenses/>
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
