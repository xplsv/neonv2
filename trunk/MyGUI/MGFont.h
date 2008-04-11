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

#ifndef _MGFONT_H
#define _MGFONT_H

#include "MGBase.h"
#include "MGSurface.h"
#include <GL/gl.h>

class CMGFont
{
  public:
    enum EAlign
    {
      LEFT,
      RIGHT,
      HCENTER,
      TOP,
      BOTTOM,
      VCENTER,
    };
  public:
                 CMGFont   () { m_bOk = false; }
                ~CMGFont   () { End(); }
 
    bool         Init      (const string &sFont, int Width, int Height, bool bBold, bool bItalic, bool bAntialias);
    void         End       ();
    bool         IsOk      () { return m_bOk; }
    void         Print     (int x, int y, int w, int h, int halign, int valign, MGColor const &Color, const string &sText);
    int          TextLength(const string &sText, int iLen = -1);
    int          TextHeight() { return m_TextMet.tmHeight; }

  private:
    bool                 m_bOk; 
    TMGSurface           m_Surface;
    unsigned             m_DisplayList;
    TEXTMETRIC           m_TextMet;
    int                  m_CharWidths[256];
};

#endif
