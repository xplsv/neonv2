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

#ifndef _LINKBEHAVIOUR_COMBOBOX_H
#define _LINKBEHAVIOUR_COMBOBOX_H

#include "LinkBehaviour.h"

class CLinkDevice;

class CLinkBehaviour_ComboBox : public CLinkBehaviour
{
  public:
    enum
    {
      NEXT,
      PREV,
      RAND,
      SET,
    };
  public:
                            CLinkBehaviour_ComboBox(int eAction, int iValue, int iNumItems, int iSetValue) { m_Action = eAction; m_Value = iValue; m_NumItems = iNumItems; m_SetValue = iSetValue; }

    virtual int             Type                   () { return COMBO_BOX; }
    virtual int             Action                 () { return m_Action;  }
    virtual float           Value                  () { return (float)m_Value; }
    virtual bool            Evaluate               (CLinkDevice *pDevice);

    virtual void            Load                   (CNodeFile::CNode *pNode);
    virtual void            Save                   (CNodeFile        *pFile);
    virtual CLinkBehaviour *Clone                  ();

    virtual int             GetSetValue            () { return m_SetValue; }
  
  protected:

    int                     m_Action;
    int                     m_NumItems;
    int                     m_SetValue;
    int                     m_Value;
};

#endif
