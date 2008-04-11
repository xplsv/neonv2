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

#ifndef _MGSHOWFFT_H
#define _MGSHOWFFT_H

#include "MGControl.h"

class CMGLabel;

class CMGShowFFT : public CMGControl
{
  public:
     
                    CMGShowFFT    (CMGControl *pParent, int iX, int iY, int iW, int iH, int iBarras);
    virtual        ~CMGShowFFT    ();

    void            Update        (const float *pFFT);

  protected:

    virtual void    DerivDraw     ();

  private:
  
    bool            m_Paint;
    int             m_Barras;
    float           m_FFT[256];
};

#endif
