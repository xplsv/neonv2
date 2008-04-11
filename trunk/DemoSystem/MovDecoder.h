//---------------------------------------------------------------------------//
// File: MovDecoder.h
//
//---------------------------------------------------------------------------//

#ifndef MOVDECODER_H
#define MOVDECODER_H

#ifdef DEBUG
  #define GOT_DEBUG
  #undef DEBUG
  #define DEBUG 1
#endif
#include <FixMath.h>
#include <Fonts.h>
#include <Gestalt.h>
#include <Navigation.h>
#include <sound.h>
#include <DrawSprocket.h>
#include <Movies.h>

#ifdef GOT_DEBUG
  #undef DEBUG
  #define DEBUG
#endif

class CTextura;

class CMovDecoder
{
  public:
                CMovDecoder  () { m_Movie = NULL; }
               ~CMovDecoder  () { End(); }

    bool        Init         (const char* pcFile, unsigned uBpp);
    void        End          ();

    float       Length       ();
    int         TimeToFrame  (float fTime);
    bool        GetFrame     (int iFrame, CTextura *pTextura);
    int			    GetWidth		 () const { return m_Width;  }
    int			    GetHeight    () const { return m_Height; }

  public:

    static void InitSubsystem();
    static void EndSubsystem ();

  private:

    Rect                 m_MovieRect;
    int                  m_Width;
    int                  m_Height;
    Movie                m_Movie;
    TimeValue            m_MovieDuration;

    // Movie off screen
    GWorldPtr            m_gOffscreen;
    PixMapHandle         m_PixMap;
    unsigned char       *m_BaseAddr;
    unsigned long        m_Stride;
    MovieDrawingCompleteUPP m_MoveDrawComplete;
    Boolean              m_MovieDraw;
};

#endif
