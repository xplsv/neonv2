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

#ifndef _VJCONTROLLER_H
#define _VJCONTROLLER_H

#include "GEEngineGrafico.h"
#include "Sound.h"
#include "FPS.h"
#include "VJEffect.h"

class  CDisplayDevice;
class  CEffect;
class  CFilter;
class  CFont;
class  CVJEffect;
class  CVJLayer;
struct TCtrlVar;

class CVJController
{
  public:
                        CVJController       () { m_bOk = false; }
                       ~CVJController       () { End(); }

    bool                Init                (int hWnd, int iNumLayers, TModoGrafico const &Modo, TRect const &rWindow, TVectorI2 const &vCustTex1, TVectorI2 const &vCustTex2, bool bCanMoveOutput, bool bShowMouse);
    void                End                 ();
    bool                IsOk                () const { return m_bOk; }

    void                Reset               ();
    void                Update              ();
    void                Draw                ();
    float               GetTime             ();
    void                SetTime             (float fTime);

    // Effects
    void                FXAttach            (int iIDFX, int iLayer);
    void                FXUnattach          (int iIDFX);
    void                LayerMove           (int iLayerFrom, int iLayerTo);
    void                LayerUnattach       (int iLayer);

    int                 FXLoad              (const string &sClass, const string &sFile, CVJEffect::TVJSnapshot *pSnapshot = NULL);
    void                FXUnload            (int iIDFX);
    //void                FXSetVar            (int iIDFX, int iVar, float fVal);
    void                FXSetTime           (int iIDFX, float fTime);
    void                FXSetSpeed          (int iIDFX, float fSpeed);
    void                FXSetAlpha          (int iIDFX, float fAlpha);
    void                FXSetBlend          (int iIDFX, int iBlend);
    void                FXSetQuality        (int iIDFX, int iQuality);
    void                FXSetFade           (int iIDFX, float fLength);
    float               FXGetLength         (int iIDFX);
    float               FXGetPos            (int iIDFX);
    int                 FXAddFilter         (int iIDFX, int iSlot, const string &sFilter);
    void                FXRemoveFilter      (int iIDFX, int iSlot);
    void                FXSwapFilter        (int iIDFX, int iSlotSrc, int iSlotDst);
    int                 FXGetFilterID       (int iIDFX, int iSlot);

    void                FXGetVarScopes      (int iIDFX, vector<string> &Scopes);
    void                FXGetVarObjects     (int iIDFX, vector<string> &Objects, int iScope);
    TCtrlVar           *FXGetVarCtrls       (int iIDFX, int iScope = -1);
    void                FXSetVar            (int iIDFX, int iScope, int iObj, int iVar, void *pData);
    void               *FXGetVar            (int iIDFX, int iScope, int iObj, int iVar);
    TCtrlVar           *FXGetFilterVarCtrls (int iIDFX, int iIDFilter);
    void                FXSetFilterVar      (int iIDFX, int iIDFilter, int iVar, void *pData);    
    void               *FXGetFilterVar      (int iIDFX, int iIDFilter, int iVar);

    // Options
    void                SetGain             (float fGain)    { m_fGain      = fGain;      }
    void                DrawDebug           (bool bEnable)   { m_bDrawDebug = bEnable;    }

    // Gets
    CSound             *GetSound            ()       { return m_pSound; }
    const float        *GetFFT              () const { return m_pFFT; }
    const float        *GetFFTChannel       () const { return m_pFFTChannel; }
    float               GetFFTChannel       (unsigned chan) const { return m_pFFTChannel[chan < FREQ_CHANNELS ? chan : 0]; }

  private:

    CVJEffect          *GetEffect           (int iIDFX);
    void                RunFFT              ();

  private:

    bool                m_bOk;
    bool                m_bDrawDebug;
    bool                m_bProcessMessages;
    float               m_fTime;
    float               m_fTimeCur;
    CFont              *m_pDebugFont;
    CTimer              m_Timer;
    CFPSTimer           m_FPSTimer;
    // Sound/FFT
    CSound             *m_pSound;
    float               m_fGain;
    float               m_pFFT[256];
    float               m_pFFTChannel[FREQ_CHANNELS];
    // Layers/Effects
    int                 m_iIDEffectCount;
    int                 m_iNumLayers;
    CVJLayer           *m_Layers;
    CLista<CVJEffect *> m_Effects;
    // Texturas
    int                 m_TexturasSnapshot[2];
    int                 m_TexturasRender[Q_QUALITIES][2];
};

#endif
