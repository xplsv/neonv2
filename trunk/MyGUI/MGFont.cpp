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

#include <MGPch.h>
#include "MGFont.h"
#include <GL/GL.h>

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMGFont::Init(const string &sFont, int Width, int Height, bool bBold, bool bItalic, bool bAntialias)
{
  m_DisplayList = glGenLists(96);
  HFONT	Font    = CreateFont(Height,
                             Width,
                             0,
                             0,
                             bBold ? FW_BOLD : FW_NORMAL,
                             bItalic,
                             FALSE,
                             FALSE,
                             ANSI_CHARSET,
                             OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS,
                             DEFAULT_QUALITY,
                             FF_DONTCARE | DEFAULT_PITCH,
                             sFont.c_str());

  HDC hDC = (HDC)g_pMGApp->GetHDC();
  HFONT oldfont = (HFONT)SelectObject(hDC, Font);
  wglUseFontBitmaps(hDC, 32, 96, m_DisplayList);
  ABC abc[256];
  GetTextMetrics  (hDC, &m_TextMet);
  GetCharABCWidths(hDC, 0, 255, abc);
  for (int i = 0; i < 256; i++)
    m_CharWidths[i] = abc[i].abcA + abc[i].abcB + abc[i].abcC;
  SelectObject(hDC, oldfont);
  DeleteObject(Font);

  return true;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMGFont::End()
{
  glDeleteLists(m_DisplayList, 96);
}


//---------------------------------------------------------------------------//
// Print
//
//---------------------------------------------------------------------------//
void CMGFont::Print(int x, int y, int w, int h, int halign, int valign, MGColor const &Color, const string &sText)
{
  // HAlign
  if (halign != LEFT)
  {
    int length = TextLength(sText);
    if (halign == HCENTER)
      x+=(w-length)>>1;
    else
      x=w - length;
  }
  // VAlign
  if (valign != TOP)
  {
    if (valign == VCENTER)
      y+=(h-m_TextMet.tmHeight)>>1;
    else
      y=h - m_TextMet.tmHeight;
  }

  glEnable     (GL_BLEND);
  glBlendFunc  (GL_SRC_COLOR,GL_ONE_MINUS_SRC_ALPHA);
  glDisable    (GL_DEPTH_TEST);
  glColor4f    (Color.r,Color.g,Color.b,1);
  glRasterPos2i(x,y+m_TextMet.tmAscent);
  glPushAttrib (GL_LIST_BIT);
	glListBase   (m_DisplayList - 32);
	glCallLists  (sText.length(), GL_UNSIGNED_BYTE, sText.c_str());
	glPopAttrib  ();
  glDisable    (GL_BLEND);
}


//---------------------------------------------------------------------------//
// TextLength
//
//---------------------------------------------------------------------------//
int CMGFont::TextLength(const string &sText, int iLen)
{
  if (iLen < 0) iLen = sText.length();
  int length = 0;
  for (int i = 0; i < iLen; i++)
		length+=m_CharWidths[sText[i]];
  return length;
}
