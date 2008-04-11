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

#ifndef _LINKDEVICE_BEAT_H
#define _LINKDEVICE_BEAT_H

#include "LinkDevice.h"

class CLinkDevice_Beat : public CLinkDevice
{
  public:

                          CLinkDevice_Beat    ();

    virtual void          Set                 (int  iBeat, int  iOff);
    virtual void          Get                 (int &iBeat, int &iOff);

    virtual int           Type                () { return BEAT; }
    virtual float         Value               () { return m_IsBeat; }
    virtual bool          Evaluate            ();

    virtual void          Load                (CNodeFile::CNode *pNode);
    virtual void          Save                (CNodeFile        *pFile);
    virtual CLinkDevice  *Clone               ();

  protected:

    bool                  m_IsBeat;
    int                   m_Beat;
    int                   m_Off;
};

#endif
