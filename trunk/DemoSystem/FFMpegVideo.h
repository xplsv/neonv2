//------------------------------------------------------------------------------
// Fichero: FFMpegVideo.h
//
//------------------------------------------------------------------------------

#ifndef FFMPEGVIDEO_H
#define FFMPEGVIDEO_H

#ifdef USE_MPE_PLAYER
#include "Base.h"
#include "Control.h"
#include "ffmpeg/avcodec.h"
#include "ffmpeg/avformat.h"

class CTextura;

class CFFMpegVideo
{
  public:
                CFFMpegVideo () { m_bOk = false; }
    virtual    ~CFFMpegVideo () { End(); }

    bool        Init         (const char *pszFile);
    void        End          ();
    bool        IsOk         () const { return m_bOk; }

    void        Run          (float fTime);

    void        SetTextura   (CTextura *pTextura);
    int         TimeToFrame  (float fTime);
    bool        GetFrame     (CTextura *pTextura);
    int			    GetWidth		 () const { return m_iWidth;  }
    int			    GetHeight    () const { return m_iHeight; }
    float       Length       () const { return m_fLength; }

  public:

    static void InitSubsystem();
    static void EndSubsystem ();

  private:

    bool        GetNextFrame ();
    
  private:

    bool             m_bOk;
    float            m_fLength;
    float            m_fStart;
    float            m_fEnd;
    int              m_iWidth;
    int              m_iHeight;
    int              m_iFrame;
    int              m_iCurrentFrame;
    int              m_iCachedFrame;

    int              m_VideoStream;
    AVFormatContext *m_pFormatCtx;
	  AVCodecContext  *m_pCodecCtx;
	  AVFrame         *m_pFrame;
    AVFrame         *m_pFrameRGB;
    AVPacket         packet;
    int              bytesRemaining;
    uint8_t         *rawData;
};

#endif

#endif
