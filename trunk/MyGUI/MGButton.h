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
#ifndef _MGBUTTON_H
#define _MGBUTTON_H

#include "MGControl.h"

class CMGButton : public CMGControl
{
  public:

    enum
    {
      NORMAL,
      MEDIUM,
      SMALL,
      REFRESH,
      BLEND,
      MAX_BUTTONS,
    };
                      CMGButton   (CMGControl *pParent, int iX, int iY, int Type, const string &sCaption, boost::function<void()> fOnClick);
    virtual          ~CMGButton   ();

    virtual void      OnMouseDown (int iX, int iY, int iButton);
    virtual void      OnMouseUp   (int iX, int iY, int iButton);
            
            bool      IsLinked    () const { return m_Linked; }
            void      SetLinked   (bool Linked) { m_Linked = Linked; Repaint(); }

  protected:

    virtual void      DerivDraw   ();

  protected:

    string            m_Caption;
    bool              m_Pressed;
    bool              m_Linked;
    int               m_Type;
    boost::function<void()> m_OnClick;
};

#endif
