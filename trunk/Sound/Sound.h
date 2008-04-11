//---------------------------------------------------------------------------//
// File: Sound.h
//
//---------------------------------------------------------------------------//

#ifndef SOUND_H
#define SOUND_H

#include "bass.h"

class CSound
{
  public:
                   CSound       (HWND hWnd);
                  ~CSound       ();

           void    Record       ();
           void    Load         (const string sFile);
           void    Play         (float fStart);
           void    Stop         ();

           bool    GetFFT       (float *pFFT);

private:

    bool           m_bOk;
    bool           m_Playing;
    HSTREAM        m_hStream;
    HRECORD        m_hRecord;
};

#endif
