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

#ifndef _LINKDEVICE_WAVE_H
#define _LINKDEVICE_WAVE_H

#include "LinkDevice.h"

class CLinkDevice_Wave : public CLinkDevice
{
  public:

                          CLinkDevice_Wave    ();

    virtual void          Set                 (int  iChan, float  fGain);
    virtual void          Get                 (int &iChan, float &fGain);

    virtual int           Type                () { return WAVE;    }
    virtual float         Value               () { return m_Value; }
    virtual bool          Evaluate            ();

    virtual void          Load                (CNodeFile::CNode *pNode);
    virtual void          Save                (CNodeFile        *pFile);
    virtual CLinkDevice  *Clone               ();

  protected:

    float                 m_Value;
    int                   m_Chan;
    float                 m_Gain;
};

#endif
