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
#include "LuaFile.h"
#include "LuaPlayer.h"
#include "Event.h"
#include "Effect.h"
#include "Filter.h"
#include "CtrlVar.h"
#include "VJStructs.h"

extern "C"
{
	#include "lauxlib.h"
	#include "lualib.h"
}

extern CLuaPlayer *g_LuaPlayer;

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CLuaFile::Init(const char *pszFile)
{
  End();
  BorraVars();

  m_pLuaState = lua_open();
  if (m_pLuaState)
  {
    m_bOk = true;
	  luaopen_base   (m_pLuaState);
    luaopen_table  (m_pLuaState);
    luaopen_string (m_pLuaState);
    luaopen_math   (m_pLuaState);
    luaopen_os     (m_pLuaState);
    
    // Registrar las funiones y procesar el fichero
    #define ITEM(funcname,func) lua_register(m_pLuaState, funcname, func);
    #include "EnumFuncionesLua.h"
    #undef ITEM
    char pBuffer[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,pBuffer);
    strcat_s(pBuffer, "%s\\demo.lua");
    if (luaL_loadfile(m_pLuaState, pBuffer) == 0)
    {
      if (lua_pcall(m_pLuaState, 0, 0, 0) == 0)
      {
      }
      else
      {
        GLOG(("ERR: Error procesando el fichero LUA '%s'", pszFile));
        End();
      }
    }
    else
    {
      GLOG(("ERR: Can't load LUA file\n"));
      End();
    }
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CLuaFile::End()
{
  if (IsOk())
  {
    lua_close(m_pLuaState);
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// BorraVars
//
//---------------------------------------------------------------------------//
void CLuaFile::BorraVars()
{
  m_pLuaState = NULL;
}


//---------------------------------------------------------------------------//
// ExecFunc
//
//---------------------------------------------------------------------------//
void CLuaFile::ExecFunc(const char *pszFunc, ...)
{
  if (!IsOk()) return;

  // Formatear la funcion
  char pFuncion[1024];
	va_list    valist;
	va_start  (valist, pszFunc);
	vsprintf_s(pFuncion, 1024, pszFunc, valist);
  va_end    (valist);

  int error = luaL_dostring(m_pLuaState, pFuncion);
  if (error != 0)
    GLOG(("WAR: Can't execute Lua function '%s'\n", pFuncion));
}



//---------------------------------------------------------------------------//
// Funciones
//
//---------------------------------------------------------------------------//
int DemoInit(lua_State *pLuaState)
{
  g_LuaPlayer->DemoInit(lua_tofloat(pLuaState,1), lua_tofloat(pLuaState,2), lua_toint(pLuaState,3), lua_tobool(pLuaState,4));
  return 0;
}

int DemoLoadMusic(lua_State *pLuaState)
{
  g_LuaPlayer->LoadMusic(lua_tostring(pLuaState,1));
  return 0;
}

int DemoLoadFX(lua_State *pLuaState)
{
  int iFX = g_LuaPlayer->LoadFX(lua_tostring(pLuaState,1));
  lua_pushnumber(pLuaState,iFX);
  return 1;
}

int DemoAddEvent(lua_State *pLuaState)
{
  TEvent *pEvent    = NEW TEvent;
  pEvent->iEvent    = lua_toint  (pLuaState,1);
  pEvent->fTime     = lua_tofloat(pLuaState,2);
  pEvent->fDuration = 0.f;
  switch (pEvent->iEvent)
  {
    // For FX
    case EV_FX_ATTACH:
      pEvent->iFX         = lua_toint(pLuaState,3);
      pEvent->iSetVarInt  = lua_toint(pLuaState,4);
      break;
    case EV_FX_UNATTACH:
      pEvent->iFX         = lua_toint(pLuaState,3);
      break; 
    case EV_FX_RESET:
      pEvent->iFX         = lua_toint(pLuaState,3);
      break;
    case EV_FX_ADDFILTER:
      pEvent->iFX         = lua_toint(pLuaState,3);
      strcpy_s(pEvent->pSetFilter, 32, lua_tostring(pLuaState,4));
      break;
    case EV_FX_SETSPEED:
    case EV_FX_SETALPHA:
    case EV_FX_SETTIME:
      // (EV_FX_SETXXX, TIME, FX, VALUE, VALUETO, DURATION
      pEvent->iFX          = lua_toint  (pLuaState,3);
      pEvent->fDuration    = lua_tofloat(pLuaState,6);
      pEvent->fSetVarFloat = lua_tofloat(pLuaState,4);
      if (pEvent->fDuration > 0.f) pEvent->fSetVarFloatTo = lua_tofloat(pLuaState,5);
      break;
    case EV_FX_SETBLEND:
      pEvent->iFX          = lua_toint(pLuaState,3);
      pEvent->iSetVarInt   = lua_toint(pLuaState,4);
      break;
    case EV_FX_SETVAR:
      // (EV_FX_SETVAR, TIME, FX, VARTYPE, VARSCOPE, VAROBJECT, VAR, VALUE, VALUETO, DURATION);
      pEvent->iFX          = lua_toint(pLuaState,3);
      pEvent->iSetVarType  = lua_toint(pLuaState,4);
      pEvent->iSetVarScope = g_LuaPlayer->GetVarScope (pEvent->iFX, lua_tostring(pLuaState,5));
      pEvent->iSetVarObject= g_LuaPlayer->GetVarObject(pEvent->iFX, pEvent->iSetVarScope, lua_tostring(pLuaState,6));
      pEvent->iSetVar      = g_LuaPlayer->GetVarName  (pEvent->iFX, pEvent->iSetVarScope, lua_tostring(pLuaState,7));
      pEvent->fDuration    = lua_tofloat (pLuaState,10);
      switch (pEvent->iSetVarType)
      {
        case SET_VAR_INT:   pEvent->iSetVarInt   = lua_toint  (pLuaState,8); break;
        case SET_VAR_FLOAT: pEvent->fSetVarFloat = lua_tofloat(pLuaState,8); if (pEvent->fDuration > 0.f) pEvent->fSetVarFloatTo = lua_tofloat(pLuaState,9);
        case SET_VAR_BOOL:  pEvent->bSetVarBool  = lua_tobool (pLuaState,8); break;
      }
      break;
    // For FF
    case EV_FF_SETVAR:
      // (EV_FF_SETVAR, TIME, FX, VARTYPE, VAR, DURATION, VALUE, VALUETO);
      pEvent->iFX          = lua_toint(pLuaState,3);
      pEvent->iSetVarType  = lua_toint   (pLuaState,4);
      strcpy_s(pEvent->pSetVar, 32, lua_tostring(pLuaState,5));
      pEvent->fDuration    = lua_tofloat (pLuaState,8);
      switch (pEvent->iSetVarType)
      {
        case SET_VAR_INT:   pEvent->iSetVarInt   = lua_toint  (pLuaState,6); break;
        case SET_VAR_FLOAT: pEvent->fSetVarFloat = lua_tofloat(pLuaState,6); if (pEvent->fDuration > 0.f) pEvent->fSetVarFloatTo = lua_tofloat(pLuaState,7); break;
        case SET_VAR_BOOL:  pEvent->bSetVarBool  = lua_tobool (pLuaState,6); break;
      }
      break;
  }
  g_LuaPlayer->AddEvent(pEvent);
  return 0;
}
