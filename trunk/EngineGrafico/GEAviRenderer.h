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

#ifndef _AVIRENDERER_H
#define _AVIRENDERER_H

#undef  CDECL
#define CDECL __cdecl
#include <vfw.h>
#undef  CDECL
#define CDECL

class CAviRenderer
{
  public:

                  CAviRenderer ();
                 ~CAviRenderer ();

    bool          StartRender  (CDisplayDevice *pDD, const char *pszFile, unsigned uFramerate);
    bool          StopRender   ();
    bool          SaveFrame    (CDisplayDevice *pDD);

	  bool          IsValid      () const { return m_bIsWorking; }

  private:

	  HANDLE        MakeDib      (HBITMAP hBitmap, UINT uBits);
	  HBITMAP       CaptureScreen(CDisplayDevice *pDD);

  private:

	  char         *m_pAVIFile;

	  AVISTREAMINFO m_aviStreamInfo;
	  PAVIFILE      m_pAviFile;
	  PAVISTREAM    m_pAviStream;
	  PAVISTREAM    m_pAviStreamCompressed;
	  PAVISTREAM    m_pAviText;

	  AVICOMPRESSOPTIONS      m_aviOptions;
	  AVICOMPRESSOPTIONS FAR *m_pAviOptions[1];

	  unsigned      m_uWidth;
    unsigned      m_uHeight;
	  unsigned      m_uFrameCount;
	  bool          m_bIsWorking;
};


#endif
