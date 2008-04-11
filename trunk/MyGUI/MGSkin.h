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

#ifndef _MGSKIN_H
#define _MGSKIN_H

#include "MGBase.h"

#define ITEM(e,s) e,
enum
{
  #include "MGSkinEnumColors.h"
  MAX_COLORS,
};
#undef ITEM
#define ITEM(e,s,w,h,b,i,a) e,
enum
{
  #include "MGSkinEnumFonts.h"
  MAX_FONTS,
};
#undef ITEM
#define ITEM(e,c) e,
enum
{
  #include "MGSkinEnumCursors.h"
  MAX_CURSORS,
};
#undef ITEM

class CMGSkin
{
  public:
                     ~CMGSkin     () { End(); }

    bool              Init        (unsigned hInstance, const string &sDir);
    void              End         ();

    TMGSurface       *Interface   ()                { return m_Interface;      }
    MGColor           Color       (int Index) const { return m_Colors[Index];  }
    CMGFont          *Font        (int Index) const { return m_Fonts[Index];   }
    HCURSOR           Cursor      (int Index) const { return m_Cursors[Index]; }

  private:
    
    HCURSOR           m_Cursors[MAX_CURSORS];
    MGColor           m_Colors[MAX_COLORS];
    CMGFont          *m_Fonts[MAX_FONTS];
    TMGSurface       *m_Interface;
};

#endif
