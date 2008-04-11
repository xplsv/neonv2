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

#ifndef _MIDI_DEVICE_H
#define _MIDI_DEVICE_H

#include "Base.h"
#include <mmsystem.h>

class CMidiDevice
{
  public:

    static void          InitSubsystem   ();
    static void          ShutSubsystem   ();

    static void          EnumDevices     (vector<string> &vDevices);

  public:
                         CMidiDevice     () { m_Ok = false; }
                        ~CMidiDevice     () { End(); }

           bool          Init            (int iDevice);
           void          End             ();

  private:

    static void CALLBACK MidiDataCallback(HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
           void          OnMidiData      (DWORD_PTR dwParam1, DWORD_PTR dwParam2);
     
  private:

    bool                 m_Ok;
    HMIDIIN              m_MidiIn;
}; 

#endif
