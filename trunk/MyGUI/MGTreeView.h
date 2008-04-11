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

#ifndef _MGTREEVIEW_H
#define _MGTREEVIEW_H

#include "MGControl.h"

class CMGTreeView;
class CMGTreeItem;
class CMGScrollBar;

class CMGTreeItem
{
  public:
                           CMGTreeItem   (CMGTreeItem *pParent, const string &sText, void *pData, bool Folder);
                          ~CMGTreeItem   ();
                          
            void           Expand        (bool All = false);
            void           Collapse      (bool All = false);
                          
            bool           IsFolder      () const { return m_Folder;   }
            bool           IsExpanded    () const { return m_Expanded; }
            const string  &GetText       ()       { return m_Text;     }
            void          *GetData       ()       { return m_Data;     }
                          
     CMGTreeItem          *GetItemAt     (int iX, int iY);
     CMGTreeItem          *GetParent     () const { return m_Parent; }
                          
            void           SetPos        (int iX, int iY);
            void           SetSelected   (bool Selected) { m_Selected = Selected; }
                          
            int            Left          () const { return m_Rect.x; }
            int            Top           () const { return m_Rect.y; }
            int            Width         () const { return m_Rect.w; }
            int            Height        () const { return m_Rect.h; }
                          
            void           Draw          (int iYOffset);
                          
            int            GetTotalHeight() const { return m_TotalHeight; }
            int            GetSonsHeight () const { return m_SonsHeight;  }
            int            GetLastHeight () const { return m_LastHeight;  }
                          
            int            CalcHeight    ();

  private:

    bool                   m_Selected;
    bool                   m_Expanded;
    bool                   m_Folder;
    int                    m_TotalHeight;
    int                    m_SonsHeight;
    int                    m_LastHeight;
    void                  *m_Data;
    string                 m_Text;
    CMGTreeItem           *m_Parent;
    CLista<CMGTreeItem *>  m_Sons;
    TRect                  m_Rect;
};

class CMGTreeView : public CMGControl
{
  public:

                      CMGTreeView   (CMGControl *pParent, int iX, int iY, int iW, int iH, int Align = MGALIGN_NONE);
                     ~CMGTreeView   ();
 
            void      Update        ();
            void      Clear         ();
            void      SetRootItem   (CMGTreeItem *pItem) { m_Root = pItem; }
    CMGTreeItem      *GetRoot       () { return m_Root;     }
    CMGTreeItem      *GetSelected   () { return m_Selected; }

    virtual void      OnMouseMove   (int iX, int iY);
    virtual void      OnMouseDown   (int iX, int iY, int iButton);
    virtual void      OnMouseUp     (int iX, int iY, int iButton);

  protected:

    virtual void      OnChangeScroll();
    virtual void      DerivDraw     ();
    CMGTreeItem      *GetItemAt     (int iX, int iY);

  private:

    void              SelectItem    (CMGTreeItem *pItem);

  private:

    CMGTreeItem      *m_Root;
    CMGTreeItem      *m_Selected;
    CMGScrollBar     *m_ScrollBar;
};

#endif
