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

#ifndef _MGLISTBOX_H
#define _MGLISTBOX_H

#include "MGControl.h"

class CMGScrollBar;

class CMGListBox : public CMGControl
{
  public:

                      CMGListBox    (CMGControl *pParent, int iX, int iY, int iW, int iH, int Align, bool bScrollBar);
                     ~CMGListBox    ();
 
            void      Add           (string const &sItem);
            void      Clear         ();
            bool      GetSelected   (string &rSelected);
            void      Select        (int iItem);
            bool      Select        (string const &sText);

    virtual void      OnMouseMove   (int iX, int iY);
    virtual void      OnMouseDown   (int iX, int iY, int iButton);
    virtual void      OnMouseUp     (int iX, int iY, int iButton);
    virtual void      OnDragOver    (CMGControl *pSource, int iX, int iY, bool &Accept);
    virtual void      OnDragDrop    (CMGControl *pSource, int iX, int iY);

  protected:

    virtual void      OnChangeScroll();
    virtual void      DerivDraw     ();
            void      Update        ();
            int       GetItemIndexAt(int iX, int iY);
            int       GetTotalHeight();
 
  public:

    boost::function<void(CMGListBox *pListBox)>                                                     m_OnSelectItem;
    boost::function<void(CMGListBox *pListBox, CMGControl *pSource, int iX, int iY, bool &Accept)>  m_OnDragOver;
    boost::function<void(CMGListBox *pListBox, CMGControl *pSource, int iX, int iY)>                m_OnDragDrop;

  private:

    int                      m_ItemHeight;
    int                      m_Selected;
    vector<string>           m_Items;
    CMGScrollBar            *m_ScrollBar;
};

#endif
