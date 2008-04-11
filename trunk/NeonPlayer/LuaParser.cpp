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

#ifndef DEMOEVENTS_H
#define DEMOEVENTS_H

enum
{
  // For FX
  EV_FX_ATTACH,
  EV_FX_UNATTACH,
  EV_FX_RESET,
  EV_FX_SETSPEED,
  EV_FX_SETALPHA,
  EV_FX_SETBLEND,
  EV_FX_SETTIME,
  EV_FX_SETFILTER,
  EV_FX_SETVAR,
  EV_FX_SETVARFADE,
  // For FF
  EV_FF_SETVAR,
  EV_FF_SETVARFADE,
};

struct TEvent
{
  int iEvent;
  // Target id: FX/Filter
  union
  {
    int iFX;
    int iFF;
  };
  // Event parameter
  union
  {
    float  fSetSpeed;
    float  fSetAlpha;
    float  fSetBlend;
    float  fSetTime;
    char   pSetFilter[32];
    char   pSetVar[32];
  };
  // SetVar
  union
  {
    float fSetVarFloat;
    int   iSetVarInt;
    bool  bSetVarBool;
  };
  // SetVarFade
  float fSetVarFadeDuration;
  union
  {
    float fSetVarFloatTo;
    int   iSetVarIntTo;
    bool  bSetVarBoolTo;
  };
};

#endif
