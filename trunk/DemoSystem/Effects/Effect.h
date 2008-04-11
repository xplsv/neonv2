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

#ifndef EFFECT_H
#define EFFECT_H

#include "GEEngineGrafico.h"

struct TCtrlVar;

class CEffect
{
  public:
    enum
    {
      FXSCOPE_MAIN = -1,
    };

    static  CEffect       *Create          (const string &sClass, const string &sFile);

                           CEffect         ();
    virtual               ~CEffect         () { }

    virtual bool           Init            (const string &sFile) = 0;

            bool           IsOk            () const { return  m_bOk;      }
            float          GetLength       () const { return  m_fLength * (m_fEnd - m_fStart); }
            float          GetPos          () const { return  m_fTime;    }
            
    virtual void           SetTime         (float fTime);
    virtual void           Draw            (CDisplayDevice *pDD, int iTextureTgt) { }

    virtual void           GetVarScopes    (vector<string> &Scopes)              const { Scopes.clear(); }
    virtual void           GetVarObjects   (vector<string> &Objects, int iScope) const { Objects.clear(); }
    virtual TCtrlVar      *GetVarCtrls     (int iScope = FXSCOPE_MAIN)  { return NULL; }
    virtual void           SetVar          (int iScope, int iObj, int iVar, void *pData) { }
    virtual void          *GetVar          (int iScope, int iObj, int iVar) { return NULL; }

  protected:

    bool                   m_bOk;
    bool                   m_bFirstClear;
    bool                   m_bClear;
    bool                   m_bMotionBlur;
    bool                   m_bAllowRevPlayback;
    float                  m_fMotionIntens;
    float                  m_fTime;
    float                  m_fTimeAnt;
    float                  m_fTimeFrame;
    float                  m_fLength;
    float                  m_fStart;
    float                  m_fEnd;
};

#endif
