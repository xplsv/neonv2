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

#ifndef _MGLABEL_H
#define _MGLABEL_H

#include "MGControl.h"

class CMGLabel : public CMGControl
{
  public:

                          CMGLabel (CMGControl *pParent, int iX, int iY, unsigned Align, int iFont, const string &sText);
    virtual              ~CMGLabel ();

    virtual void          Set      (const string &sText);
    virtual const string &Get      () const { return m_Text; }

  protected:

    virtual void          DerivDraw();

  private:

    int                   m_Font;
    string                m_Text;
};

#endif
  