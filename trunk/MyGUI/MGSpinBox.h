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

#ifndef _MGSPINBOX_H
#define _MGSPINBOX_H

#include "MGControl.h"

class CMGSpinBox : public CMGControl
{
  public:
    
                            CMGSpinBox    (CMGControl *pParent, int iX, int iY, int iSize, int iMin, int iMax, int iStep, boost::function0<void> fOnChange);
    virtual                ~CMGSpinBox    ();

    virtual int             GetValue      () const { return m_Value; }
    virtual void            Set           (int iValue);
    virtual void            SetMinMax     (int iMin, int iMax);
    virtual void            OnMouseMove   (int iX, int iY);
    virtual void            OnMouseDown   (int iX, int iY, int iButton);
    virtual void            OnMouseUp     (int iX, int iY, int iButton);
    virtual void            OnChange      () { }

  protected:              

    virtual void            DerivDraw     ();
    
  private:
    enum
    {
      NONE,
      UP,
      DOWN,
    };
    boost::function0<void>  m_OnChange;
    int                     m_MouseY;
    int                     m_CurrentValue;
    int                     m_Value;
    int                     m_Max;
    int                     m_Min;
    int                     m_Step;
    int                     m_ButtonPressed;
};

#endif
