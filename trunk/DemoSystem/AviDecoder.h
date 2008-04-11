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

#ifndef AVIDECODER_H
#define AVIDECODER_H

#undef  CDECL
#define CDECL __cdecl
#include <vfw.h>
#undef  CDECL
#define CDECL

class CTextura;

class CAviDecoder
{
  public:
                CAviDecoder  () { m_Avi = NULL; m_Frame = NULL; }
               ~CAviDecoder  () { End(); }

    bool        Init         (const char* pcFile, unsigned uBpp);
    void        End          ();

    float       Length       ();
    int         TimeToFrame  (float fTime);
    bool        GetFrame     (int iFrame, CTextura *pTextura);
    int			    GetWidth		 () const { return m_iWidth;  }
    int			    GetHeight    () const { return m_iHeight; }

  public:

    static void InitSubsystem();
    static void EndSubsystem ();

  private:

    PAVISTREAM        m_Avi;
    PGETFRAME         m_Frame;
    int               m_iLength;
    int               m_iWidth;
    int               m_iHeight;
    char             *m_pB;
    char             *m_pI;
    unsigned          m_Bpp;
    HGLOBAL           m_pHIn;
    HGLOBAL           m_pHOut;
    BITMAPINFOHEADER *m_pBInfoIn;
    BITMAPINFOHEADER *m_pBInfoOut;
    unsigned          m_LengthBInfoIn;
    HIC               m_hIC;
};

#endif
