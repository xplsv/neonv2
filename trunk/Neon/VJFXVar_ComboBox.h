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

#ifndef _VJFXVAR_COMBOBOX_H
#define _VJFXVAR_COMBOBOX_H

#include "VJFXVar.h"

struct TCtrlVar;
class  CMGComboBox;

class CVJFXVar_ComboBox : public CVJFXVar
{
  public:
     
                      CVJFXVar_ComboBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject);
                      CVJFXVar_ComboBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx);
    virtual          ~CVJFXVar_ComboBox();

    virtual void      Set              (void *pData);
    virtual void      Evaluate         ();
    virtual void      Load             (CNodeFile::CNode *pNode);
    virtual void      Save             (CNodeFile        *pFile);
   
  private:

            void      OnChange         ();

  private:

    CMGComboBox      *m_ComboBox;
};

#endif

