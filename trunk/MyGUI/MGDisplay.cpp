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
#include "MGDisplay.h"
#include <gl/gl.h>

CMGDisplay *g_pMGDD = NULL;

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMGDisplay::Init(unsigned hInstance, int iX, int iY, int iW, int iH, int iBpp)
{
  g_pMGDD = this;
  m_uBpp  = iBpp;

	glClearColor    (0.0f, 0.0f, 0.0f, 1.0f);
	glEnable        (GL_DEPTH_TEST);
  glDepthFunc     (GL_GEQUAL);
	glShadeModel    (GL_SMOOTH);
  glDisable       (GL_TEXTURE_2D);
  glHint          (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  OnResize        (iW, iH);
  glClear         (GL_COLOR_BUFFER_BIT);

  m_Skin = NEW CMGSkin;
  if (!m_Skin->Init(hInstance, "Skins\\Default"))
  {
    DISPOSE(m_Skin);
    return false;
  }

  return true;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMGDisplay::End()
{
  DISPOSE(m_Skin);
  //CUR_Quit();
	//SDL_Quit();
}


//---------------------------------------------------------------------------//
// Flip
//
//---------------------------------------------------------------------------//
void CMGDisplay::Flip()
{
  //SDL_GL_SwapBuffers();
}
  

//---------------------------------------------------------------------------//
// OnResize
//
//---------------------------------------------------------------------------//
void CMGDisplay::OnResize(int w, int h)
{
  m_Width   = w;
  m_Height  = h;
  glViewport     (0,0, w,h);
  glMatrixMode   (GL_PROJECTION);
  glLoadIdentity ();
  glOrtho        (0.f, w,h, 0.f, -1.f, 1.f);
  glMatrixMode   (GL_MODELVIEW);
  glLoadIdentity ();
  glClear        (GL_COLOR_BUFFER_BIT);
}


//---------------------------------------------------------------------------//
// LoadImage
//
//---------------------------------------------------------------------------//
TMGSurface *CMGDisplay::LoadImage(const string &sFile)
{
  TMGSurface *pSurface = NEW TMGSurface;
  pSurface->Tga.LoadFromFile(sFile);
	glGenTextures  (1, &pSurface->iTexture);
	glBindTexture  (GL_TEXTURE_2D, pSurface->iTexture);
  glTexImage2D   (GL_TEXTURE_2D, 0, 4, pSurface->Tga.GetImageWidth(), pSurface->Tga.GetImageHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pSurface->Tga.GetPixels());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  pSurface->w     = pSurface->Tga.GetImageWidth();
  pSurface->h     = pSurface->Tga.GetImageHeight();
  pSurface->fInvW = 1.f / pSurface->w;
  pSurface->fInvH = 1.f / pSurface->h;
  return pSurface;
}


//---------------------------------------------------------------------------//
// FreeImage
//
//---------------------------------------------------------------------------//
void CMGDisplay::FreeImage(TMGSurface *pSurface)
{
  if (pSurface)
  {
    if (pSurface->iTexture != -1) glDeleteTextures(1, &pSurface->iTexture);
    DISPOSE(pSurface);
  }
}


//---------------------------------------------------------------------------//
// SetClipRect
//
//---------------------------------------------------------------------------//
void CMGDisplay::SetClipRect(TRect const &Rect)
{
  glScissor(Rect.x, m_Height - (Rect.y+Rect.h), Rect.w, Rect.h);
  glEnable (GL_SCISSOR_TEST);
}


//---------------------------------------------------------------------------//
// SetClipRect
//
//---------------------------------------------------------------------------//
void CMGDisplay::SetClipRect()
{
  glDisable(GL_SCISSOR_TEST);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CMGDisplay::Draw(TMGSurface *pSurface, int x, int y, float fDepth)
{
  Stretch(pSurface, TRect(0,0,pSurface->w,pSurface->h), TRect(x,y,pSurface->w,pSurface->h), fDepth);
}


//---------------------------------------------------------------------------//
// Stretch
//
//---------------------------------------------------------------------------//
void CMGDisplay::Stretch(TMGSurface *pSurface, TRect const &Dst, float fDepth)
{
  Stretch(pSurface, TRect(0,0,pSurface->w,pSurface->h), Dst, fDepth);
}


//---------------------------------------------------------------------------//
// Stretch
//
//---------------------------------------------------------------------------//
void CMGDisplay::Stretch(TMGSurface *pSurface, TRect const &Src, TRect const &Dst, float fDepth)
{
  if (pSurface->iTexture == -1) return;
  //glEnable     (GL_BLEND);
  //glBlendFunc  (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable     (GL_BLEND);
	glAlphaFunc(GL_GREATER,0.1f);						// Set Alpha Testing     (disable blending)
  glEnable(GL_ALPHA_TEST);						// Enable Alpha Testing  (disable blending)

  //glBlendFunc  (GL_SRC_COLOR,GL_ONE_MINUS_SRC_ALPHA);
  glEnable     (GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, pSurface->iTexture);
  glDisable    (GL_DEPTH_TEST);
  glColor3f    (1,1,1);
  glBegin(GL_QUADS);
    float x0 = Src.x*pSurface->fInvW;
    float y0 = Src.y*pSurface->fInvH;
    float x1 =(Src.x+Src.w) * pSurface->fInvW;
    float y1 =(Src.y+Src.h) * pSurface->fInvH;
	  glTexCoord2f(x0, y0);
	  glVertex3f  ((float)Dst.x,       (float)Dst.y, fDepth);
	  glTexCoord2f(x1, y0);
	  glVertex3f  ((float)Dst.x+(float)Dst.w, (float)Dst.y, fDepth);
	  glTexCoord2f(x1, y1);
	  glVertex3f  ((float)Dst.x+(float)Dst.w, (float)Dst.y+(float)Dst.h, fDepth);
	  glTexCoord2f(x0, y1);
	  glVertex3f  ((float)Dst.x,       (float)Dst.y+(float)Dst.h, fDepth);
  glEnd();
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}


//---------------------------------------------------------------------------//
// DrawItem
//
//---------------------------------------------------------------------------//
void CMGDisplay::DrawItem(TRect const &Src, TRect const &Dst, float fDepth)
{
  Stretch(m_Skin->Interface(), Src, Dst, fDepth);
}


//---------------------------------------------------------------------------//
// FillRect
//
//---------------------------------------------------------------------------//
void CMGDisplay::FillRect(TRect const &Src, MGColor const &Color, float fDepth)
{
  glColor3f(Color.r,Color.g,Color.b);
  glBegin(GL_QUADS);
    glVertex3i(Src.x,       Src.y,       (int)fDepth);
	  glVertex3i(Src.x+Src.w, Src.y,       (int)fDepth);
	  glVertex3i(Src.x+Src.w, Src.y+Src.h, (int)fDepth);
	  glVertex3i(Src.x,       Src.y+Src.h, (int)fDepth);
  glEnd();
}


//---------------------------------------------------------------------------//
// GradRect
//
//---------------------------------------------------------------------------//
void CMGDisplay::GradRect(TRect const &Src, MGColor const &Color0, MGColor const &Color1, float fDepth)
{
  glBegin(GL_QUADS);
    glColor3f (Color0.r,Color0.g,Color0.b);
    glVertex3i(Src.x,       Src.y,       (int)fDepth);
    glColor3f (Color0.r,Color0.g,Color0.b);
	  glVertex3i(Src.x+Src.w, Src.y,       (int)fDepth);
    glColor3f (Color1.r,Color1.g,Color1.b);
	  glVertex3i(Src.x+Src.w, Src.y+Src.h, (int)fDepth);
    glColor3f (Color1.r,Color1.g,Color1.b);
	  glVertex3i(Src.x,       Src.y+Src.h, (int)fDepth);
  glEnd();
}


//---------------------------------------------------------------------------//
// Rect
//
//---------------------------------------------------------------------------//
void CMGDisplay::Rect(TRect const &Src, MGColor const &Color, float fDepth)
{
  glColor3f(Color.r,Color.g,Color.b);
  glBegin(GL_LINES);
    float x0 = Src.x+0.5f;
    float x1 = Src.x+Src.w-0.5f;
    float y0 = Src.y+0.5f;
    float y1 = Src.y+Src.h-0.5f;
	  glVertex3f(x0, y0, fDepth);
	  glVertex3f(x1, y0, fDepth);
    glVertex3f(x1, y0, fDepth);
    glVertex3f(x1, y1, fDepth);
    glVertex3f(x1, y1, fDepth);
    glVertex3f(x0, y1, fDepth);
    glVertex3f(x0, y1, fDepth);
    glVertex3f(x0, y0, fDepth);
  glEnd();
}


//---------------------------------------------------------------------------//
// HLine
//
//---------------------------------------------------------------------------//
void CMGDisplay::HLine(int x, int y, int w, MGColor const &Color, float fDepth)
{
  glColor3f(Color.r,Color.g,Color.b);
  glBegin(GL_LINES);
	  glVertex3f(x+0.5f,   y-0.5f, fDepth);
	  glVertex3f(x+w-0.5f, y-0.5f, fDepth);
  glEnd();
}


//---------------------------------------------------------------------------//
// VLine
//
//---------------------------------------------------------------------------//
void CMGDisplay::VLine(int x, int y, int h, MGColor const &Color, float fDepth)
{
  glColor3f(Color.r,Color.g,Color.b);
  glBegin(GL_LINES);
	  glVertex3f(x-0.5f, y+0.5f,   fDepth);
	  glVertex3f(x-0.5f, y+h-0.5f, fDepth);
  glEnd();
}


//---------------------------------------------------------------------------//
// TextDraw
//
//---------------------------------------------------------------------------//
void CMGDisplay::TextDraw(int iFont, int x, int y, int w, int h, int halign, int valign, MGColor const &Color, const string &sText)
{
  g_pMGDD->Skin()->Font(iFont)->Print(x,y,w,h, halign, valign, Color, sText);
}