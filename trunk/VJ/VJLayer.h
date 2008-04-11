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

#ifndef _VJLAYER_H
#define _VJLAYER_H

#include "Base.h"

class CDisplayDevice;
class CVJEffect;

class CVJLayer
{
  public:
                      CVJLayer    () { m_VJEffect = NULL; m_VJEffectOut = NULL; }
                     ~CVJLayer    () { End(); }

    void              End         ();
    
    void              SetTime     (float fTime);
    void              Draw        (CDisplayDevice *pDD);

    void              Attach      (float fTime, CVJEffect *pVJEffect);
    void              Unattach    (float fTime);
    void              MoveTo      (float fTime, CVJLayer *pVJLayer);
    void              Clear       (bool bActive = true, bool bOut = true);

    CVJEffect        *GetEffect   () { return m_VJEffect;    }
    CVJEffect        *GetEffectOut() { return m_VJEffectOut; }

  private:
    
    CVJEffect        *m_VJEffect;
    CVJEffect        *m_VJEffectOut;
};

#endif
