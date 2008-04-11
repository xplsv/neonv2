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

#include "Base.h"
#include "GEFont.h"
#include "GEEngineGrafico.h"

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFont::Init(const string &sFontName, int iHeight, bool bBold, bool bItalic)
{
  D3DXFONT_DESCA LogFont =
  {
    iHeight,                 // height
    0,                       // width; 
    0,                       // lfEscapement; 
    0,                       // lfOrientation; 
    bBold ? FW_BOLD : FALSE, // lfWeight; 
    bItalic ? TRUE : FALSE,  // lfItalic; 
    FALSE,                   // lfUnderline; 
    FALSE,                   // lfStrikeOut; 
    DEFAULT_CHARSET,         // lfCharSet; 
    OUT_DEFAULT_PRECIS,      // lfOutPrecision; 
    CLIP_DEFAULT_PRECIS,     // lfClipPrecision; 
    ANTIALIASED_QUALITY,     // lfQuality; 
    DEFAULT_PITCH,           // lfPitchAndFamily; 
    //pszFontName              // lfFaceName[LF_FACESIZE]; 
  };
  strcpy_s(LogFont.FaceName, LF_FACESIZE, sFontName.c_str());

  m_bOk = true;
  if(FAILED(D3DXCreateFontIndirect(g_DisplayDevice.GetD3DDevice(), &LogFont, &m_pFont)))
  {
    GLOG(("ERR: Can't create font %s\n", sFontName.c_str()));
    m_bOk = false;
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFont::End()
{
  if (IsOk())
  {
    HARD_RELEASE(m_pFont);
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// Print
//
//---------------------------------------------------------------------------//
void CFont::Print(CDisplayDevice *pDD, const string &sText, TVectorI2 const &vPos, unsigned uColor, int iAlign)
{
  RECT rect;
  rect.left  = vPos.x;
  rect.top   = vPos.y;
  rect.right = pDD->Width ();
  rect.bottom= pDD->Height();
  m_pFont->DrawText(NULL, sText.c_str(), -1, &rect, iAlign, uColor);
}
