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

#ifndef _LINK_DEVICE_H
#define _LINK_DEVICE_H

class CLinkDevice
{
  public:
    enum
    {
      KEYBOARD,
      BEAT,
      TIMER,
      WAVE,
      MIDI_NOTE,
      MIDI_SLIDER,
      MIDI_CLOCK,
    };
  public:
    virtual int           Type      () = 0;
    virtual bool          Evaluate  () = 0;
    virtual float         Value     () = 0;

    virtual void          Load      (CNodeFile::CNode *pNode) = 0;
    virtual void          Save      (CNodeFile        *pFile) = 0;
    virtual CLinkDevice  *Clone     () = 0;
};

#endif
