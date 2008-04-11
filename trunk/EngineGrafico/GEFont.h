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

#ifndef GEFONT_H
#define GEFONT_H

#include "GEVector.h"
#include <d3dx9.h>

class CDisplayDevice;

class CFont
{
  public:
                        CFont () { m_bOk = false; }
                       ~CFont () { End(); }

    bool                Init  (const string &sFontName, int iHeight, bool bBold, bool bItalic);
    void                End   ();
    bool                IsOk  () const { return m_bOk; }

    void                Print (CDisplayDevice *pDD, const string &Text, TVectorI2 const &vPos, unsigned uColor, int iAlign);

  private:

    bool                m_bOk;
    ID3DXFont          *m_pFont;
};

#endif
