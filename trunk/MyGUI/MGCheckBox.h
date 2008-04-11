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

#ifndef _MGCHECKBOX_H
#define _MGCHECKBOX_H

#include "MGControl.h"

class CMGCheckBox : public CMGControl
{
  public:

                             CMGCheckBox (CMGControl *pParent, int iX, int iY, const string &sCaption, bool IconLeft, boost::function<void()> fOnChange);
    virtual                 ~CMGCheckBox ();

    virtual void             OnChange    ();
    virtual bool             Checked     () const { return m_Checked; }
    virtual void             SetChecked  (bool Checked) { m_Checked = Checked; OnChange(); }
    virtual const string    &GetCaption  () const { return m_Caption; }

    virtual void             OnMouseDown (int iX, int iY, int iButton);

  protected:              

    virtual void             DerivDraw   ();

  private:

    boost::function<void()>  m_OnChange;  
    bool                     m_Checked;
    bool                     m_IconLeft;
    string                   m_Caption;
    int                      m_iTextW;
    int                      m_iTextH;
};

#endif
