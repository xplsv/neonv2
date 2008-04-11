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

#ifndef _MGWINDOW_H
#define _MGWINDOW_H

#include "MGControl.h"

class CMGWindow : public CMGControl
{
  public:
    enum EModalResult
    {
      MR_NONE,
      MR_OK,
      MR_CANCEL,
    };

  public:
                      CMGWindow   (int iW, int iH, const string &sCaption);
    virtual          ~CMGWindow   ();

    virtual void      Show        (int iX, int iY, function<void (EModalResult eModalResult)> fOnModalResult);
    virtual void      Close       (EModalResult eModalResult = MR_OK);

    virtual void      OnMouseMove (int iX, int iY);
    virtual void      OnMouseDown (int iX, int iY, int iButton);
    virtual void      OnMouseUp   (int iX, int iY, int iButton);
    virtual void      OnKeyDown   (int vkey) { }
    virtual void      OnKeyUp     (int vkey) { }

  protected:

    virtual void      DerivDraw   ();
            void      UpdatePos   (int iX, int iY);

  private:

    string            m_Caption;
    bool              m_Moving;
    int               m_AntX;
    int               m_AntY;
    int               m_MoveOffX;
    int               m_MoveOffY;
    function<void (EModalResult eModalResult)> m_OnModalResult;
};

#endif
