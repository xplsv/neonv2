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
#include "MidiDevice.h"
#include <mmsystem.h>


//---------------------------------------------------------------------------//
// Midi Data Callback
//
//---------------------------------------------------------------------------//
void CALLBACK CMidiDevice::MidiDataCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
  CMidiDevice *pMidiDevice = (CMidiDevice *)dwInstance;
  pMidiDevice->OnMidiData(dwParam1, dwParam2);
}


//---------------------------------------------------------------------------//
// InitSubsystem
//
//---------------------------------------------------------------------------//
void CMidiDevice::InitSubsystem()
{
}


//---------------------------------------------------------------------------//
// ShutSubsystem
//
//---------------------------------------------------------------------------//
void CMidiDevice::ShutSubsystem()
{
}


//---------------------------------------------------------------------------//
// EnumDevices
//
//---------------------------------------------------------------------------//
void CMidiDevice::EnumDevices(vector<string> &vDevices)
{
  vDevices.clear();
  int iNumDevices = midiInGetNumDevs();
  MIDIINCAPS miCaps;
  for (int i = 0; i < iNumDevices; i++)
  {
     midiInGetDevCaps(i, &miCaps, sizeof(miCaps));
     vDevices.push_back(miCaps.szPname);
  }
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMidiDevice::Init(int iDevice)
{
  m_Ok = false;
  if (midiInOpen(&m_MidiIn, iDevice, (DWORD_PTR)MidiDataCallback, (DWORD_PTR)this, CALLBACK_FUNCTION) == MMSYSERR_NOERROR)
  {
    if (midiInStart(m_MidiIn) == MMSYSERR_NOERROR)
      m_Ok = true;
    else
      midiInClose(m_MidiIn);
  }
  return m_Ok;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMidiDevice::End()
{
  if (m_Ok)
  {
    midiInStop (m_MidiIn);
    midiInClose(m_MidiIn);
    m_Ok = false;
  }
}


//---------------------------------------------------------------------------//
// OnMidiData
//
//---------------------------------------------------------------------------//
void CMidiDevice::OnMidiData(DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
  GLOG(("MIDI Data: %d, %d\n", dwParam1, dwParam2));
}
