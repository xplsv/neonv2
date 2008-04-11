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

#ifndef _MGEDITBOX_H
#define _MGEDITBOX_H

#include "MGControl.h"

class CMGEditBox : public CMGControl
{
  public:
    
                              CMGEditBox    (CMGControl *pParent, int iX, int iY, int iSize, boost::function<void()> fOnChange);
    virtual                  ~CMGEditBox    ();

    virtual void              SetText       (const string &sText);
    virtual const string      GetText       () const { return m_Text; }
    virtual void              OnChange      () { }
    virtual void              OnMouseDown   (int iX, int iY, int iButton);
    virtual void              OnChar        (char c);

  protected:              

    virtual void              DerivDraw     ();
    virtual int               GetPosFromChar(int iChar);

  private:

    boost::function<void()>   m_OnChange;  
    string                    m_Text;
    int                       m_Pos;
    int                       m_PosSel;
};

#endif
