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

#ifndef _MGAPPLICATION_H
#define _MGAPPLICATION_H

class IMGAppWindow;
class CMGDisplay;
class CMGWindow;
class CMGControl;

class CMGApplication
{
  public:

                      CMGApplication  () { }
    virtual          ~CMGApplication  () { End(); }

    bool              Init            (unsigned hInstance, int w, int h);
    void              End             ();

    void              Update          ();
    void              MoveMainWindow  (int iOffX, int iOffY);

    void              OnResize        (int w, int h);
    void              OnMouseMove     (int x, int y);
    void              OnMouseDown     (int x, int y, int button);
    void              OnMouseUp       (int x, int y, int button);
    void              OnVKeyDown      (int vkey);
    void              OnVKeyUp        (int vkey);
    void              OnChar          (char c);

    void              Repaint         ();
    void              MustRepaint     () { m_Repaint = true; }
    void              MustFlip        () { m_Flip    = true; }
    void              Flip            ();
    void              SetRootItem     (CMGControl *pItem);
    void              FocusItem       (CMGControl *pItem);
    void              TopItem         (CMGControl *pItem);
    void              RepaintItem     (CMGControl *pItem);
    void              DragItem        (CMGControl *pItem);
    void              PopupWindow     (CMGWindow *pWindow, int iX = -1, int iY = -1);
    CMGControl       *GetItemAt       (int iX, int iY);
    bool              IsControlVisible(CMGControl *pItem);

    bool              OpenFileOpen    (const char *pTitle, const char *pInitialDir, const char *pWildcard, string &rFile);
    bool              OpenFileSave    (const char *pTitle, const char *pInitialDir, const char *pWildcard, const char *pDefExt, string &rFile);

    void              Close           ()       { m_bWantClose = true; }
    bool              WantClose       () const { return m_bWantClose; }
    CMGDisplay       *GetDisplay      () const { return m_pDisplay;   }

    unsigned          GetHInst        () const { return m_hInst;      }
    unsigned          GetHWnd         () const { return m_hWnd;       }
    unsigned          GetHDC          () const { return m_hDC;        }

  private:

    unsigned          m_hInst;
    unsigned          m_hWnd;
    unsigned          m_hDC;
    bool              m_bOk;
    bool              m_MovingWindow;
    int               m_WinX;
    int               m_WinY;
    int               m_PosMouseX;
    int               m_PosMouseY;
    bool              m_Repaint;
    bool              m_Flip;
    bool              m_bWantClose;
    bool              m_DragAccepted;
    IMGAppWindow     *m_pAppWindow;
    CMGDisplay       *m_pDisplay;
    CMGControl       *m_pItemRoot;
    CMGControl       *m_pItemFocused;
    CMGControl       *m_pItemTop;
    CMGControl       *m_pItemDragged;
    CMGWindow        *m_pItemPopup;
    CLista<CMGControl *> m_RepaintItems;
};

#endif
