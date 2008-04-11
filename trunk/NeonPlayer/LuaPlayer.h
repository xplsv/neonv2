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

#ifndef LuaPlayer_H
#define LuaPlayer_H

#include "Lista.h"
#include "Sound.h"

class  CVJController;
class  CVJLoader;
class  CLuaFile;
class  CLuaPlayer;
extern CLuaPlayer *g_pLuaPlayer;
struct TEvent;

class CLuaPlayer
{
  public:
               CLuaPlayer () { m_Ok = false; }
              ~CLuaPlayer () { End(); }

    bool       Init        (HINSTANCE hInst);
    void       End         ();

    void       DemoInit    (float fDuration, float fStartTime, int iEffects, bool bEnableFPS);
    void       DemoStart   ();
    void       LoadMusic   (const string &sFile);
    int        LoadFX      (const string &sFile);
    void       AddEvent    (TEvent *pEvent);

    bool       Update      ();
    void       Draw        ();

    int        GetVarScope (int iFX, const char *pScope);
    int        GetVarObject(int iFX, int iScope, const char *pObject);
    int        GetVarName  (int iFX, int iScope, const char *pVar);

private:    

    void       LoadVars    (CNodeFile::CNode *pNode, int iFX, int iScope, int iObj);
    void       LoadVars    (CNodeFile::CNode *pNode, int iFX, int iFilter);
    void       ExecEventImm(TEvent *pEvent, float fTime);
    void       ExecEventRun(TEvent *pEvent, float fTime);

  private:    

    bool                 m_Ok;
    float                m_StartTime;
    float                m_Duration;
    CLuaFile            *m_LuaFile;
    CVJController       *m_VJController;
    CVJLoader           *m_VJLoader;
    CLista<TEvent *>     m_Events;
    CLista<TEvent *>     m_EventsSorted;
    CLista<TEvent *>     m_EventsRunning;
    CListaIter<TEvent *> m_EventIter;
};

#endif
