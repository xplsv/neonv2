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

#ifndef VJSTRUCTS_H
#define VJSTRUCTS_H

#include "CtrlVar.h"

template<class _Ty> inline void swap(_Ty& _X, _Ty& _Y)
{
  _Ty _Tmp = _X;
  _X = _Y;
  _Y = _Tmp;
}

enum EFreqChannels
{
  FREQ_CHANNELS = 6, // 0 = global, 1-5 = frecuencias predefinidas
};

enum ETimeProfiles
{
  PR_UPDATE_GUI,
  PR_UPDATE_APP,
  PR_UPDATE_OUTPUT,
  PR_DRAW_OUTPUT,
  PR_MAX,
};

enum EQuality
{
  Q_FULL,
  Q_CUST1,
  Q_CUST2,
  Q_QUALITIES,
};

enum
{
  // For FX
  EV_FX_LOAD,
  EV_FX_UNLOAD,
  EV_FX_ATTACH,
  EV_FX_UNATTACH,
  EV_FX_RESET,
  EV_FX_SETSPEED,
  EV_FX_SETALPHA,
  EV_FX_SETBLEND,
  EV_FX_SETTIME,
  EV_FX_SETQUALITY,
  EV_FX_SETFADE,
  EV_FX_ADDFILTER,
  EV_FX_DELFILTER,
  EV_FX_SWAPFILTER,
  EV_FX_SETVAR,
  EV_FX_SETLAYOUTX,
  EV_FX_SETLAYOUTY,
  // For FF
  EV_FF_SETVAR,
};

#endif
