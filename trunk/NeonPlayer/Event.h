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

#ifndef EVENT_H
#define EVENT_H

enum
{
  SET_VAR_INT,
  SET_VAR_FLOAT,
  SET_VAR_BOOL,
};

struct TEvent
{
  int   iEvent;
  float fTime;
  float fDuration;
  // Effect ID
  int   iFX;
  int   iFilterSlot;
  // SetVar
  union
  {
    int  iSetVar;
    char pSetVar[32];
  };
  int  iSetVarScope;
  int  iSetVarObject;
  // Value
  union
  {
    void  *pValue;
    float  fSetVarFloat;
    int    iSetVarInt;
    bool   bSetVarBool;
    char   pSetFilter[32];
  };
  // SetVarType
  int  iSetVarType;
  // Value to
  union
  {
    float fSetVarFloatTo;
    int   iSetVarIntTo;
    bool  bSetVarBoolTo;
  };
  void *GetValuePF(float _fTime)
  {
    static float f;
    f = GetValueF(_fTime);
    return &f;
  }
  float GetValueF(float _fTime)
  {
    if (fDuration > 0)
      return (_fTime > (fTime+fDuration)) ? fSetVarFloatTo : (((_fTime - fTime) * (fSetVarFloatTo - fSetVarFloat) / fDuration) + fSetVarFloat);
    else
      return fSetVarFloat;
  }
};

#endif
