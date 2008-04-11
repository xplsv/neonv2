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

#ifndef _MGDISPLAY_H
#define _MGDISPLAY_H

#include "MGBase.h"
#include "MGSkin.h"

class CMGDisplay
{
  public:

                      CMGDisplay  () { }
    virtual          ~CMGDisplay  () { End(); }

    bool              Init        (unsigned hInstance, int iX, int iY, int iW, int iH, int iBpp);
    void              End         ();

    void              Flip        ();
    void              OnResize    (int w, int h);

    unsigned          GetWidth    () const { return m_Width;  }
    unsigned          GetHeight   () const { return m_Height; }

    void              SetClipRect (TRect const &Rect);
    void              SetClipRect ();
    void              Draw        (TMGSurface *pSurface, int x, int y, float fDepth = 0.f);
    void              Stretch     (TMGSurface *pSurface, TRect const &Src, TRect const &Dst, float fDepth = 0.f);
    void              Stretch     (TMGSurface *pSurface, TRect const &Dst, float fDepth = 0.f);
    void              DrawItem    (TRect const &Src, TRect const &Dst, float fDepth = 0.f);
    void              FillRect    (TRect const &Src, MGColor const &Color, float fDepth = 0.f);
    void              GradRect    (TRect const &Src, MGColor const &Color0, MGColor const &Color1, float fDepth = 0.f);
    void              Rect        (TRect const &Src, MGColor const &Color, float fDepth = 0.f);
    void              HLine       (int x, int y, int w, MGColor const &Color, float fDepth = 0.f);
    void              VLine       (int x, int y, int h, MGColor const &Color, float fDepth = 0.f);
    void              TextDraw    (int iFont, int x, int y, int w, int h, int halign, int valign, MGColor const &Color, const string &sText);

    CMGSkin          *Skin        () { return m_Skin; }

    TMGSurface       *LoadImage   (const string &sFile);
    void              FreeImage   (TMGSurface *pSurface);

  protected:

  private:
    
    bool              m_bOk;
    unsigned          m_uBpp;
    unsigned          m_Width;
    unsigned          m_Height;
    CMGSkin          *m_Skin;
};

#endif
