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

#ifndef _LINKBEHAVIOUR_TRACKBAR_H
#define _LINKBEHAVIOUR_TRACKBAR_H

#include "LinkBehaviour.h"

class CLinkDevice;

class CLinkBehaviour_TrackBar : public CLinkBehaviour
{
  public:
    enum
    {
      DEVICE_VALUE,
      INV_DEVICE_VALUE,
      SET,
      ADD,
      RAND,
      FADE,
    };
  public:
                            CLinkBehaviour_TrackBar(int eAction, float fValue) { m_Action = eAction; m_Value = fValue; }

    virtual int             Type                   () { return TRACK_BAR; }
    virtual int             Action                 () { return m_Action;  }
    virtual float           Value                  () { return m_Value;   }
    virtual bool            Evaluate               (CLinkDevice *pDevice);

    virtual void            Load                   (CNodeFile::CNode *pNode);
    virtual void            Save                   (CNodeFile        *pFile);
    virtual CLinkBehaviour *Clone                  ();
  
    virtual void            SetFadeValues          (float fFadeFr, float fFadeTo, float fFadeLength) { m_FadeFr = fFadeFr; m_FadeTo = fFadeTo; m_FadeLength = fFadeLength; }
    virtual void            SetSetValues           (float fSetValue) { m_SetValue = fSetValue; }
    virtual void            SetAddValues           (float fAddValue) { m_AddValue = fAddValue; }

    virtual void            GetFadeValues          (float &fFadeFr, float &fFadeTo, float &fFadeLength) { fFadeFr = m_FadeFr ; fFadeTo = m_FadeTo ; fFadeLength = m_FadeLength ; }
    virtual void            GetSetValues           (float &fSetValue) { fSetValue = m_SetValue; }
    virtual void            GetAddValues           (float &fAddValue) { fAddValue = m_AddValue; }

  protected:

    int                     m_Action;
    float                   m_FadeIni;
    float                   m_FadeFr;
    float                   m_FadeTo;
    float                   m_FadeLength;
    bool                    m_FadeDone;
    float                   m_SetValue;
    float                   m_AddValue;
    float                   m_Value;
};

#endif
