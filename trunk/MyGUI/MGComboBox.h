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

#ifndef _MGCOMBOBOX_H
#define _MGCOMBOBOX_H

#include "MGControl.h"

class CMGScrollBar;

class CMGComboBox : public CMGControl
{
  public:
    
                            CMGComboBox   (CMGControl *pParent, int iX, int iY, int iSize, int iMaxHeight, bool bNotifyAlways, boost::function<void()> fOnChange);
    virtual                ~CMGComboBox   ();

    virtual void            Clear         ();
    virtual void            Add           (const string &sText);
    virtual int             IndexOf       (const string &sText);
    virtual void            Select        (int iIndex);
    virtual int             GetSelected   () const { return m_Selected; }
    virtual const string   &GetText       () const { return m_Text;     }
    virtual int             GetNumItems   () const { return m_Strings.Cuenta(); }
    virtual void            OnMouseMove   (int iX, int iY);
    virtual void            OnMouseDown   (int iX, int iY, int iButton);
    virtual void            OnMouseUp     (int iX, int iY, int iButton);
    virtual void            OnChange      () { }

            void            OnChangeScroll();

  protected:

    virtual void            DerivDraw     ();
    
  private:

            void            SetUnfolded   (bool bUnfolded);

  private:
    boost::function<void()> m_OnChange;  
    CLista<string>          m_Strings;
    string                  m_Text;
    int                     m_Selected;
    bool                    m_Unfolded;
    bool                    m_ButtonPressed;
    bool                    m_NotifyAlways;
    unsigned                m_MaxHeight;
    unsigned                m_ItemHeight;
    unsigned                m_UnfoldedHeight;
    CMGScrollBar           *m_ScrollBar;
};

#endif

