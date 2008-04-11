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
#ifndef FICHERO_H
#define FICHERO_H

#include <stdio.h>

class CFichero
{
  public:
    enum
    {
      BEGIN,
      CURRENT,
      END,
    };
    
  public:
                CFichero    () { m_hFile = NULL; }
               ~CFichero    () { Close(); }

    bool        Open        (const char *pFile, const char *pAttribs);
    void        Close       ();
    void        Flush       ();
    int         Read        (void *pData, int iChunkSize, int iChunkNum);
    char       *ReadLn      (char *pData, int iMaxBuffer);
    int         Write       (const void *pData, int iChunkSize, int iChunkNum);
    int         WriteLn     (const char *pData);
    bool        Eof         ();


    // Get
    int         GetPos      ();
    int         GetLength   () { return m_iLength; }

    // Seek
    void        Seek        (int iOffset, int iMode);
    void        SeekToIni   () { Seek(0, BEGIN); }
    void        SeekToEnd   () { Seek(0, END);   }

  private:

    FILE       *m_hFile;
    int         m_iLength;
};

#endif
