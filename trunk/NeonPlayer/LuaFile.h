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

#ifndef _LUAFILE_H
#define _LUAFILE_H

#include "LuaUtils.h"

class CLuaFile
{
  public:
                CLuaFile        () { m_bOk = false; }
               ~CLuaFile        () { End(); }

    bool        Init            (const char *pszFile);
    void        End             ();
    bool        IsOk            () { return m_bOk; }

    void        ExecFunc        (const char *pszFuncion, ...);
    //lua_State  *LuaState        () { return m_pLuaState; }

  private:

    void        BorraVars       ();

  private:

    bool        m_bOk;
    lua_State  *m_pLuaState;
};

// Funciones Lua disponibles
#define ITEM(funcname,funcion) int funcion(lua_State *pLuaState);
#include "EnumFuncionesLua.h"
#undef ITEM

#endif
