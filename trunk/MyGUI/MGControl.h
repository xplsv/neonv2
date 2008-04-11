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

#ifndef _MGControl_H
#define _MGControl_H

#include "MGBase.h"
#include "MGDisplay.h"

class CMGControl;

class CMGControl
{
  public:

                          CMGControl     (CMGControl *pParent, int iX, int iY, int iW, int iH);
    virtual              ~CMGControl     ();

            void          Run         ();
            void          Draw        ();
    virtual void          Repaint     ();
    virtual void          Focus       ();

            const string &GetName     () const { return m_Name;   }
            CMGControl   *GetParent   ()       { return m_Parent; }
            CMGControl   *GetTop      ()       { return m_Parent ? m_Parent->GetTop() : this; }
            unsigned      GetAlign    () const { return m_Align;   }
            CMGControl   *GetItemAt   (int iX, int iY);

            int           Left        () const { return m_Rect.x + (m_Parent ? m_Parent->Left() : 0); }
            int           Top         () const { return m_Rect.y + (m_Parent ? m_Parent->Top () : 0); }
            int           Width       () const { return m_Rect.w;  }
            int           Height      () const { return m_Rect.h;  }

            bool          IsInside    (int iX, int iY);
            bool          IsVisible   () const { return m_Visible && (!m_Parent || m_Parent->IsVisible()); }
            bool          IsEnabled   () const { return m_Enabled; }
            bool          IsFocused   () const { return m_Focused; }

            void          SetParent   (CMGControl *pParent);
            void          SetVisible  (bool Visible) { m_Visible = Visible; if (Visible) Repaint(); }
            void          SetEnabled  (bool Enabled) { m_Enabled = Enabled; if (Enabled) Repaint(); }
            void          SetFocus    (bool Focused) { m_Focused = Focused; if (Focused) Focus();   }
            void          SetPos      (int iLeft,  int iTop)    { m_Rect.x = iLeft;  m_Rect.y = iTop;    }
            void          SetSize     (int iWidth, int iHeight) { m_Rect.w = iWidth; m_Rect.h = iHeight; }
            void          SetColor    (MGColor const &color) { m_Color = color; }
            void          SetBorder   (int iBorder) { SetRect(m_Border, iBorder,iBorder,iBorder,iBorder); }
            void          Transparent (bool bTransparent) { m_Transparent = bTransparent; }
            
            void          AddRectAlign(CMGControl *pItem);
            void          GetFreeRect (TRect &Rect) { Rect = m_FreeRect; }

    virtual void          OnResize    ();
    virtual void          OnMouseMove (int iX, int iY);
    virtual void          OnMouseDown (int iX, int iY, int iButton) { }
    virtual void          OnMouseUp   (int iX, int iY, int iButton) { }
    virtual void          OnVKeyDown  (int iVKey) { }
    virtual void          OnVKeyUp    (int iVKey) { }
    virtual void          OnChar      (char c)    { }
    virtual void          OnDragOver  (CMGControl *pSource, int iX, int iY, bool &Accept);
    virtual void          OnDragDrop  (CMGControl *pSource, int iX, int iY);

            void          BeginDrag   ();

  protected:

            void          UpdateSize  ();
            void          UpdateRect  ();
    virtual void          DerivRun    () { }
    virtual void          DerivDraw   () { }

  private:

            void          AddSon      (CMGControl *pSon);
            void          DelSon      (CMGControl *pSon);

  protected:

    string              m_Name;
    unsigned            m_Align;
    TRect               m_Rect;
    TRect               m_FreeRect;
    TRect               m_Border;
    bool                m_AddRectAlign;
    bool                m_Visible;
    bool                m_Enabled;
    bool                m_Focused;
    bool                m_Transparent;
    MGColor             m_Color;
    CMGControl         *m_Parent;
    CLista<CMGControl*> m_Sons;
};

#endif
