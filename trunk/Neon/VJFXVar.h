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

#ifndef _VJFXVAR_H
#define _VJFXVAR_H

#include "MGLabel.h"
#include "MGWindow.h"

struct TCtrlVar;
class  CMGControl;
class  CMGButton;
class  CLink;
class  CVJFXVar;

class CVJFXVar
{
  public:
     enum
     {
       CHECK_BOX,
       EDIT_BOX,
       COMBO_BOX,
       SLIDER,
     };
  public:
    static  CVJFXVar *Create           (CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject);
    static  CVJFXVar *Create           (CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx);

                      CVJFXVar         (CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject);
                      CVJFXVar         (CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx);
    virtual          ~CVJFXVar         () {};

    virtual void      Evaluate         () {};

    virtual void      Load             (CNodeFile::CNode *pNode) = 0;
    virtual void      Save             (CNodeFile        *pFile) = 0;
  
  private:

            void      OnBtnLinkClick   ();
            void      OnCloseVarLinker (CMGWindow::EModalResult iModalResult);

  protected:

            bool      IsFilter         () const { return m_IsFilter; }

  protected:

    CMGLabel         *m_Label;
    CMGButton        *m_BtnLink;
    CMGControl       *m_Control;
    CLink            *m_Link;
    int               m_Type;
    int               m_ID;
    int               m_Scope;
    int               m_Object;
    int               m_FilterIdx;
    int               m_Var;
    bool              m_IsFilter;
};

#endif
