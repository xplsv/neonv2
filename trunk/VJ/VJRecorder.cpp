//---------------------------------------------------------------------------//
// File: VJRecorder.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "VJRecorder.h"
#include "VJServer.h"
#include <string.h>

#define WRITE_EVENT(ev)      \
          m_Event.iEvent = ev; \
          m_Event.fTime  = m_pServer->GetTime(); \
          fwrite(&m_Event, 1, sizeof(m_Event), m_pFile);
#define WRITE_DATA(d,s) \
          fwrite(d, 1, s, m_pFile);

//----------------------------------------------------------------------------
// Init
//
//----------------------------------------------------------------------------
void CVJRecorder::Init(CVJServer *pServer)
{
  m_pServer = pServer;
  m_pFile   = NULL;
}

//----------------------------------------------------------------------------
// End
//
//----------------------------------------------------------------------------
void CVJRecorder::End()
{
  StopRecording();
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
bool CVJRecorder::StartRecording(const char *pszFile)
{
  m_pServer->Reset();
  m_pFile = fopen(pszFile, "wb+");
  return (m_pFile != NULL);
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::StopRecording()
{
  if (m_pFile)
  {
    fclose(m_pFile);
    m_pFile = NULL;
  }
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordFFT(const float *pFFT)
{
  uchar pcFFT[FREQ_CHANNELS];
  for (int i = 0; i < FREQ_CHANNELS; i++)
    pcFFT[i] = (char)(pFFT[i] * 255.f);
  WRITE_EVENT(EV_FFT);
  WRITE_DATA (pcFFT, FREQ_CHANNELS*sizeof(uchar));
}


//----------------------------------------------------------------------------
// Load
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordLoadEffect(const char *pszFile)
{
  WRITE_EVENT(EV_EFFECT_LOAD);
  WRITE_DATA (pszFile, strlen(pszFile)+1);
}


//----------------------------------------------------------------------------
// Unload
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordUnloadEffect(int iIDEffect)
{
  WRITE_EVENT(EV_EFFECT_UNLOAD);
  WRITE_DATA (&iIDEffect, sizeof(int));
}


//----------------------------------------------------------------------------
// Reload
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordReloadEffect(const char *pszFile, int iIDEffect)
{
  WRITE_EVENT(EV_EFFECT_RELOAD);
  WRITE_DATA (pszFile, strlen(pszFile)+1);
  WRITE_DATA (&iIDEffect, sizeof(int));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordAttachEffect(int iID, char iLayer)
{
  WRITE_EVENT(EV_ATTACH_EFFECT);
  WRITE_DATA (&iID, sizeof(int));
  WRITE_DATA (&iLayer, sizeof(char));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordUnattachLayer(char iLayer)
{
  WRITE_EVENT(EV_UNATTACH_LAYER);
  WRITE_DATA (&iLayer, sizeof(char));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordEffectReset(int iID)
{
  WRITE_EVENT(EV_EFFECTRESET);
  WRITE_DATA (&iID, sizeof(int));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordEffectCommand(int iID, const char *pszCommand)
{
  WRITE_EVENT(EV_EFFECTCOMMAND);
  WRITE_DATA (&iID, sizeof(int));
  WRITE_DATA (pszCommand, strlen(pszCommand)+1);
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordFilterCommand(int iID, const char *pszCommand)
{
  WRITE_EVENT(EV_FILTERCOMMAND);
  WRITE_DATA (&iID, sizeof(int));
  WRITE_DATA (pszCommand, strlen(pszCommand)+1);
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordSetEffectTime(int iID, float fTime)
{
  WRITE_EVENT(EV_SETEFFECTTIME);
  WRITE_DATA (&iID,   sizeof(int));
  WRITE_DATA (&fTime, sizeof(float));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordSetEffectSpeed(int iID, float fSpeed)
{
  WRITE_EVENT(EV_SETEFFECTSPEED);
  WRITE_DATA (&iID,    sizeof(int));
  WRITE_DATA (&fSpeed, sizeof(float));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordSetEffectAlpha(int iID, float fAlpha)
{
  WRITE_EVENT(EV_SETEFFECTALPHA);
  WRITE_DATA (&iID,    sizeof(int));
  WRITE_DATA (&fAlpha, sizeof(float));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordSetEffectBlend(int iID, int iBlend)
{
  WRITE_EVENT(EV_SETEFFECTBLEND);
  WRITE_DATA (&iID,    sizeof(int));
  WRITE_DATA (&iBlend, sizeof(int));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordSetFilter(int iID, const char *pszFilter)
{
  WRITE_EVENT(EV_SETFILTER);
  WRITE_DATA (&iID, sizeof(int));
  WRITE_DATA (pszFilter, strlen(pszFilter)+1);
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordSetFilterVar(int iID, char iVar, void *pValue)
{
  WRITE_EVENT(EV_SETFILTERVAR);
  WRITE_DATA (&iID,   sizeof(int));
  WRITE_DATA (&iVar,  sizeof(char));
  WRITE_DATA (pValue, sizeof(int));
}


//----------------------------------------------------------------------------
// Record
//
//----------------------------------------------------------------------------
void CVJRecorder::RecordSetEffectVar(char iScope, char iObj, int iID, char iVar, void *pValue)
{
  if (!pValue)
  {
    return;
  }
  WRITE_EVENT(EV_SETEFFECTVAR);
  WRITE_DATA (&iScope,sizeof(char));
  WRITE_DATA (&iObj,  sizeof(char));
  WRITE_DATA (&iID,   sizeof(int));
  WRITE_DATA (&iVar,  sizeof(char));
  WRITE_DATA (pValue, sizeof(int));
}
