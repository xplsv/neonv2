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

#ifndef _MGBUTTONMODALRESULT_H
#define _MGBUTTONMODALRESULT_H

#include "MGButton.h"
#include "MGWindow.h"

class CMGButtonModalResult : public CMGButton
{
  public:

    CMGButtonModalResult(CMGWindow *pParent, int iX, int iY, int Type, const string &sCaption, CMGWindow::EModalResult iModalResult) : CMGButton(pParent,iX,iY,Type,sCaption,0)
    {
      m_ModalResult = iModalResult;
      m_Window      = pParent;
    }

    virtual void OnMouseUp(int iX, int iY, int iButton)
    {
      if (iButton == 1)
      {
        if (m_Pressed && IsInside(iX, iY))
        {
          m_Window->Close(m_ModalResult);
        }
        m_Pressed = false;
        Repaint();
      }
    }

  protected:

    CMGWindow                *m_Window;
    CMGWindow::EModalResult   m_ModalResult;
};

#endif
