//---------------------------------------------------------------------------//
// File: Fps.h
//
//---------------------------------------------------------------------------//

#ifndef FPS_H
#define FPS_H

class CFPSTimer
{
  public:

    inline void     Init     ();
    inline void     Frame    ();

    inline float    GetFPS   () { return m_fFPS; }

  protected:

    LARGE_INTEGER   m_TimerFrecuency;
    float           m_fFPS;
    unsigned        m_uFrames;
    unsigned        m_uCurrentFPS;
    float           m_fTime;
};


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
inline void CFPSTimer::Init()
{
  QueryPerformanceFrequency(&m_TimerFrecuency);

  m_fFPS    = 0;
  m_uFrames = 0;

  LARGE_INTEGER pc;
  QueryPerformanceCounter(&pc);
  m_fTime = (float)pc.QuadPart / (float)m_TimerFrecuency.QuadPart;
}


//---------------------------------------------------------------------------//
// Frame
//
//---------------------------------------------------------------------------//
inline void CFPSTimer::Frame()
{
  m_uFrames++;
  LARGE_INTEGER pc;
  QueryPerformanceCounter(&pc);
  float fTime = (float)pc.QuadPart*1000.f / (float)m_TimerFrecuency.QuadPart;
  if ((fTime - m_fTime) >= 1000)
  {
    m_fFPS    = m_uFrames*1000.f / (fTime - m_fTime);
    m_uFrames = 0;
    m_fTime   = fTime;
  }
}

#endif
