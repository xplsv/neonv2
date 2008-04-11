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

#ifndef _MGHOTKEY_H
#define _MGHOTKEY_H

#include "MGControl.h"

class CMGHotKey : public CMGControl
{
  public:

  public:
    
                              CMGHotKey     (CMGControl *pParent, int iX, int iY, int iSize, boost::function<void()> fOnChange);
    virtual                  ~CMGHotKey     ();

    virtual void              Set           (char  c) { m_Char = c; }
    virtual void              Get           (char &c) { c = m_Char; }
    virtual void              OnMouseDown   (int iX, int iY, int iButton);
    virtual void              OnChar        (char c);

  protected:              

    virtual void              DerivDraw     ();

  private:

    boost::function<void()>   m_OnChange;
    char                      m_Char;
};

#endif
