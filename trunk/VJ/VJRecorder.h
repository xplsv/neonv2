//---------------------------------------------------------------------------//
// File: VJRecorder.h
//
//---------------------------------------------------------------------------//

#ifndef VJRECORDER_H
#define VJRECORDER_H

#pragma pack(1)
struct TVJEvent
{
  uchar iEvent;
  float fTime;
};
#pragma pack()

enum
{
  EV_FFT,
  EV_EFFECT_LOAD,
  EV_EFFECT_UNLOAD,
  EV_EFFECT_RELOAD,
  EV_ATTACH_EFFECT,
  EV_UNATTACH_LAYER,
  EV_EFFECTRESET,
  EV_EFFECTCOMMAND,
  EV_FILTERCOMMAND,
  EV_SETEFFECTSPEED,
  EV_SETEFFECTALPHA,
  EV_SETEFFECTBLEND,
  EV_SETFILTER,
  EV_SETFILTERVAR,
  EV_SETEFFECTVAR,
  EV_SETEFFECTTIME,
};

class CVJServer;

class CVJRecorder
{
  public:
               CVJRecorder          () { m_bOk = false; }
              ~CVJRecorder          () { End(); }

    void       Init                 (CVJServer *pServer);
    void       End                  ();

    bool       IsRecording          () const { return m_pFile != NULL; }

    bool       StartRecording       (const char *pszFile);
    void       StopRecording        ();

    void       RecordFFT            (const float *pFFT);
    void       RecordLoadEffect     (const char *pszFile);
    void       RecordUnloadEffect   (int iIDEffect);
    void       RecordReloadEffect   (const char *pszFile, int iIDEffect);
    void       RecordAttachEffect   (int iID, char iLayer);
    void       RecordUnattachLayer  (char iLayer);
    void       RecordEffectReset    (int iID);
    void       RecordEffectCommand  (int iID, const char *pszCommand);
    void       RecordFilterCommand  (int iID, const char *pszCommand);
    void       RecordSetEffectTime  (int iID, float fTime);
    void       RecordSetEffectSpeed (int iID, float fSpeed);
    void       RecordSetEffectAlpha (int iID, float fAlpha);
    void       RecordSetEffectBlend (int iID, int iBlend);
    void       RecordSetFilter      (int iID, const char *pszFilter);
    void       RecordSetFilterMode  (int iID, int iMode);
    void       RecordSetFilterVar   (int iID, char iVar, void *pValue);
    void       RecordSetEffectVar   (char iScope, char iObj, int iID, char iVar, void *pValue);

  private:    

  private:    

    bool       m_bOk;
    FILE      *m_pFile;
    CVJServer *m_pServer;
    TVJEvent   m_Event;
};

#endif
