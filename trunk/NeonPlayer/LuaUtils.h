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

#ifndef _LUAUTILS_H
#define _LUAUTILS_H

extern "C"
{
	#include "lua.h"
}

static __inline int lua_toint(lua_State *L, int idx)
{
  return ((int)lua_tonumber(L,idx));
}

static __inline unsigned lua_tounsigned(lua_State *L, int idx)
{
  return ((unsigned)lua_tonumber(L,idx));
}

static __inline float lua_tofloat(lua_State *L, int idx)
{
  return ((float)lua_tonumber(L,idx));
}

static __inline double lua_todouble(lua_State *L, int idx)
{
  return (lua_tonumber(L,idx));
}

static __inline bool lua_tobool(lua_State *L, int idx)
{
  return (lua_toboolean(L,idx) != 0);
}

#endif
