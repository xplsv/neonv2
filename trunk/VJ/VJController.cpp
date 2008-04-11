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

#include "VJ.h"
#include "Filters/Filter.h"
#include "Effects/Effect.h"
#include "AppWindow.h"
#include "Sound.h"
#include "GEEngineGrafico.h"
#include "MidiDevice.h"

#include <stdio.h>

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CVJController::Init(int hWnd, int iNumLayers, TModoGrafico const &Modo, TRect const &rWindow, TVectorI2 const &vCustTex1, TVectorI2 const &vCustTex2, bool bCanMoveOutput, bool bShowMouse)
{
  if (!g_AppWindow.Init(Modo, rWindow, bCanMoveOutput, 0, (HWND)hWnd, bShowMouse))
  {
    return false;
  }

  m_fGain  = 1.f;
  m_pSound = NEW CSound((HWND)hWnd);

  vector<string> vDevices;
  CMidiDevice::EnumDevices(vDevices);

  m_Timer.Reset();

  // Layers
  m_iNumLayers = iNumLayers;
  m_Layers = NEW_ARRAY(CVJLayer, m_iNumLayers);

  // Font
  m_pDebugFont = NEW CFont;
  m_pDebugFont->Init("Arial", 24, false, false);

  m_FPSTimer.Init();
  m_bDrawDebug          = false;
  m_bProcessMessages    = (hWnd == 0);
  m_TexturasSnapshot[0] = g_pGestorMateriales->AddTextura(CVJEffect::TVJSnapshot::WIDTH, CVJEffect::TVJSnapshot::HEIGHT, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_TexturasSnapshot[1] = g_pGestorMateriales->AddTextura(CVJEffect::TVJSnapshot::WIDTH, CVJEffect::TVJSnapshot::HEIGHT, HARD_TEX_FORMAT_A8R8G8B8, false, false, false, CTextura::SYSTEMMEM);
  m_TexturasRender[Q_FULL ][0] = g_pGestorMateriales->AddTextura(Modo.iWidth, Modo.iHeight, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_TexturasRender[Q_FULL ][1] = g_pGestorMateriales->AddTextura(Modo.iWidth, Modo.iHeight, HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_TexturasRender[Q_CUST1][0] = g_pGestorMateriales->AddTextura(vCustTex1.x, vCustTex1.y,  HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_TexturasRender[Q_CUST1][1] = g_pGestorMateriales->AddTextura(vCustTex1.x, vCustTex1.y,  HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_TexturasRender[Q_CUST2][0] = g_pGestorMateriales->AddTextura(vCustTex2.x, vCustTex2.y,  HARD_TEX_FORMAT_A8R8G8B8, false, true);
  m_TexturasRender[Q_CUST2][1] = g_pGestorMateriales->AddTextura(vCustTex2.x, vCustTex2.y,  HARD_TEX_FORMAT_A8R8G8B8, false, true);

  Reset();

  // FFT Reset
  for (int i = 0; i < 256; i++)
    m_pFFT[i] = 0.f;
  for (int i = 0; i < FREQ_CHANNELS; i++)
    m_pFFTChannel[i] = 0.f;

  GLOG(("Neon v2.5 Beta v0.4.0\n"));
  GLOG(("Init: %d x %d @ %dHz on device %d\n", Modo.iWidth, Modo.iHeight, Modo.iRefresh, Modo.iDevice));
  GLOG(("CustTex1: %d x %d\n", vCustTex1.x, vCustTex1.y));
  GLOG(("CustTex2: %d x %d\n", vCustTex2.x, vCustTex2.y));

  m_bOk = true;
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CVJController::End()
{
  if (IsOk())
  {
    m_bOk = false;
    m_Effects.VaciaConDispose();
    DISPOSE_ARRAY(m_Layers);
    DISPOSE(m_pSound);
    DISPOSE(m_pDebugFont);
    for (int i = 0; i < Q_QUALITIES; i++)
    {
      g_pGestorMateriales->RemoveTextura(m_TexturasRender[i][0]);
      g_pGestorMateriales->RemoveTextura(m_TexturasRender[i][1]);
    }
    g_pGestorMateriales->RemoveTextura(m_TexturasSnapshot[0]);
    g_pGestorMateriales->RemoveTextura(m_TexturasSnapshot[1]);
    g_AppWindow.End();
  }
}


//---------------------------------------------------------------------------//
// FXAttach
//
//---------------------------------------------------------------------------//
void CVJController::FXAttach(int iIDFX, int iLayer)
{
  ASSERT(iLayer < m_iNumLayers);
  if (iLayer < m_iNumLayers)
  {
    CVJEffect *pVJEffect = GetEffect(iIDFX);
    CVJLayer  *pLayer    = &m_Layers[iLayer];

    // Quitarlo de algun layer q estuviera haciendole fade out y attachearlo al nuevo
    for (int i = 0; i < m_iNumLayers; i++)
    {
      if (m_Layers[i].GetEffectOut() == pVJEffect)
        m_Layers[i].Clear(false,true);
    }
    pLayer->Attach(m_fTime, pVJEffect);
  }
}


//---------------------------------------------------------------------------//
// LayerMove
//
//---------------------------------------------------------------------------//
void CVJController::LayerMove(int iLayerFrom, int iLayerTo)
{
  ASSERT(iLayerFrom < m_iNumLayers && iLayerTo < m_iNumLayers);
  if (iLayerFrom < m_iNumLayers && iLayerTo < m_iNumLayers)
  {
    m_Layers[iLayerFrom].MoveTo(m_fTime, &m_Layers[iLayerTo]);
  }
}


//---------------------------------------------------------------------------//
// FXUnattach
//
//---------------------------------------------------------------------------//
void CVJController::FXUnattach(int iIDFX)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  // Quitarlo de su layer
  for (int i = 0; i < m_iNumLayers; i++)
  {
    if (m_Layers[i].GetEffect() == pVJEffect)
      m_Layers[i].Unattach(m_fTime);
  }
}


//---------------------------------------------------------------------------//
// LayerUnattach
//
//---------------------------------------------------------------------------//
void CVJController::LayerUnattach(int iLayer)
{
  ASSERT(iLayer < m_iNumLayers);
  if (iLayer < m_iNumLayers)
  {
    m_Layers[iLayer].Unattach(m_fTime);
  }
}


//---------------------------------------------------------------------------//
// FXLoad
//
//---------------------------------------------------------------------------//
int CVJController::FXLoad(const string &sClass, const string &sFile, CVJEffect::TVJSnapshot *pSnapshot)
{
  int iID = -1;
  pSnapshot->iTexRender   = m_TexturasSnapshot[0];
  pSnapshot->iTexSnapshot = m_TexturasSnapshot[1];
  CVJEffect *pVJEffect = NEW CVJEffect;
  if (pVJEffect->Load(sClass, sFile, m_iIDEffectCount, pSnapshot, m_TexturasRender))
  {
    iID = m_iIDEffectCount++;
    m_Effects.Inserta(pVJEffect);
  }
  return iID;
}


//---------------------------------------------------------------------------//
// FXUnload
//
//---------------------------------------------------------------------------//
void CVJController::FXUnload(int iIDFX)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  if (pVJEffect)
  {
    // Quitarlo de los layers en los que este
    for (int i = 0; i < m_iNumLayers; i++)
    {
      if (m_Layers[i].GetEffectOut() == pVJEffect)
        m_Layers[i].Clear(false,true);
      if (m_Layers[i].GetEffect() == pVJEffect)
        m_Layers[i].Clear(true,false);
    }
    m_Effects.Saca(pVJEffect);
    DISPOSE(pVJEffect);
  }
}


//---------------------------------------------------------------------------//
// FXSetTime
//
//---------------------------------------------------------------------------//
void CVJController::FXSetTime(int iIDFX, float fTime)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  if (pVJEffect)
    pVJEffect->SetTime(fTime);
}


//---------------------------------------------------------------------------//
// FXSetSpeed
//
//---------------------------------------------------------------------------//
void CVJController::FXSetSpeed(int iIDFX, float fSpeed)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->SetSpeed(fSpeed);
}


//---------------------------------------------------------------------------//
// FXSetAlpha
//
//---------------------------------------------------------------------------//
void CVJController::FXSetAlpha(int iIDFX, float fAlpha)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->SetAlpha(fAlpha);
}



//---------------------------------------------------------------------------//
// FXSetBlend
//
//---------------------------------------------------------------------------//
void CVJController::FXSetBlend(int iIDFX, int iBlend)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->SetBlend(iBlend);
}


//---------------------------------------------------------------------------//
// FXSetQuality
//
//---------------------------------------------------------------------------//
void CVJController::FXSetQuality(int iIDFX, int iQuality)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->SetQuality(iQuality);
}


//---------------------------------------------------------------------------//
// FXSetFade
//
//---------------------------------------------------------------------------//
void CVJController::FXSetFade(int iIDFX, float fLength)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->SetFadeLength(fLength);
}


//---------------------------------------------------------------------------//
// FXGetLength
//
//---------------------------------------------------------------------------//
float CVJController::FXGetLength(int iIDFX)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  return pVJEffect->GetEffect()->GetLength();
}


//---------------------------------------------------------------------------//
// FXGetPos
//
//---------------------------------------------------------------------------//
float CVJController::FXGetPos(int iIDFX)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  float fLen = pVJEffect->GetEffect()->GetLength();
  if (fLen > 0)
    return ((float)fmod(pVJEffect->GetTime(),fLen));
  else
    return 0.f;
}


//---------------------------------------------------------------------------//
// FXAddFilter
//
//---------------------------------------------------------------------------//
int CVJController::FXAddFilter(int iIDFX, int iSlot, const string &sFilter)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  return pVJEffect->AddFilter(iSlot, sFilter);
}


//---------------------------------------------------------------------------//
// FXRemoveFilter
//
//---------------------------------------------------------------------------//
void CVJController::FXRemoveFilter(int iIDFX, int iSlot)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->RemoveFilter(iSlot);
}


//---------------------------------------------------------------------------//
// FXSwapFilter
//
//---------------------------------------------------------------------------//
void CVJController::FXSwapFilter(int iIDFX, int iSlotSrc, int iSlotDst)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->SwapFilter(iSlotSrc, iSlotDst);
}


//---------------------------------------------------------------------------//
// FXGetFilterID
//
//---------------------------------------------------------------------------//
int CVJController::FXGetFilterID(int iIDFX, int iSlot)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  return pVJEffect->GetFilterID(iSlot);
}


//---------------------------------------------------------------------------//
// FXGetVarScopes
//
//---------------------------------------------------------------------------//
void CVJController::FXGetVarScopes(int iIDFX, vector<string> &Scopes)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->GetEffect()->GetVarScopes(Scopes);
}


//---------------------------------------------------------------------------//
// FXGetVarObjects
//
//---------------------------------------------------------------------------//
void CVJController::FXGetVarObjects(int iIDFX, vector<string> &Objects, int iScope)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->GetEffect()->GetVarObjects(Objects, iScope);
}


//---------------------------------------------------------------------------//
// FXGetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CVJController::FXGetVarCtrls(int iIDFX, int iScope)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  return pVJEffect->GetEffect()->GetVarCtrls(iScope);
}


//---------------------------------------------------------------------------//
// FXSetVar
//
//---------------------------------------------------------------------------//
void CVJController::FXSetVar(int iIDFX, int iScope, int iObj, int iVar, void *pData)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->GetEffect()->SetVar(iScope, iObj, iVar, pData);
}


//---------------------------------------------------------------------------//
// FXGetVar
//
//---------------------------------------------------------------------------//
void *CVJController::FXGetVar(int iIDFX, int iScope, int iObj, int iVar)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  return pVJEffect->GetEffect()->GetVar(iScope, iObj, iVar);
}


//---------------------------------------------------------------------------//
// FXGetFilterVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CVJController::FXGetFilterVarCtrls(int iIDFX, int iIDFilter)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  return pVJEffect->GetFilterByID(iIDFilter)->GetVarCtrls();
}


//---------------------------------------------------------------------------//
// FXSetFilterVar
//
//---------------------------------------------------------------------------//
void CVJController::FXSetFilterVar(int iIDFX, int iIDFilter, int iVar, void *pData)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  pVJEffect->GetFilterByID(iIDFilter)->SetVar(iVar, pData);
}


//---------------------------------------------------------------------------//
// FXGetFilterVar
//
//---------------------------------------------------------------------------//
void *CVJController::FXGetFilterVar(int iIDFX, int iIDFilter, int iVar)
{
  CVJEffect *pVJEffect = GetEffect(iIDFX);
  return pVJEffect->GetFilterByID(iIDFilter)->GetVar(iVar);
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CVJController::SetTime(float fTime)
{
  float const *fWave = GetFFTChannel();
  CMotor3D::SetRegisterData(CMotor3D::V_WAVE,  &fWave[0]);
  CMotor3D::SetRegisterData(CMotor3D::V_WAVE1, &fWave[1]);
  CMotor3D::SetRegisterData(CMotor3D::V_WAVE2, &fWave[2]);
  CMotor3D::SetRegisterData(CMotor3D::V_WAVE3, &fWave[3]);
  CMotor3D::SetRegisterData(CMotor3D::V_WAVE4, &fWave[4]);
  CMotor3D::SetRegisterData(CMotor3D::V_WAVE5, &fWave[5]);
  for (int i = 0; i < m_iNumLayers; i++)
    m_Layers[i].SetTime(fTime);
  m_fTime = fTime;
  // Procesado de mensajes si somos la ventana principal
  if (m_bProcessMessages)
    g_AppWindow.ProcessMessages();
  g_AppWindow.UpdateFullScreen();

}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CVJController::Update()
{
  RunFFT ();
  SetTime(GetTime());
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CVJController::Draw()
{
  D3DDEVICE *pD3D = g_DisplayDevice.GetD3DDevice();

  m_FPSTimer.Frame();

  // Inicializar el frame
  g_DisplayDevice.Clear(true, false, 0, 1.0f);
  g_DisplayDevice.BeginFrame();
  g_DisplayDevice.BeginScene();

  // Draw de todos los layers
  for (int i = 0; i < m_iNumLayers; i++)
    m_Layers[i].Draw(&g_DisplayDevice);

  // Draw debug
  if (m_bDrawDebug)
  {
    char pBuffer[256];
    sprintf_s(pBuffer, 256, "%.2ffps - %.2f - Device %d", m_FPSTimer.GetFPS(), m_fTime, g_DisplayDevice.GetModoGrafico().iDevice);
    m_pDebugFont->Print(&g_DisplayDevice, pBuffer, TVectorI2(6, 6), HARD_COLOR_ARGB(192,   0,   0,   0), DT_LEFT);
    m_pDebugFont->Print(&g_DisplayDevice, pBuffer, TVectorI2(5, 5), HARD_COLOR_ARGB(255, 255, 255, 255), DT_LEFT);
  }

  // Terminar el frame y mostrar
  g_DisplayDevice.EndScene();
  g_DisplayDevice.EndFrame();

  g_DisplayDevice.PageFlip();
}


//---------------------------------------------------------------------------//
// GetEffect
//
//---------------------------------------------------------------------------//
CVJEffect *CVJController::GetEffect(int iID)
{
  CListaIter<CVJEffect *> Iter(m_Effects);
  while (!Iter.EsFinal())
  {
    CVJEffect *pVJEffect = Iter;
    if (pVJEffect->GetID() == iID)
      return pVJEffect;
    Iter++;
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Reset
//
//---------------------------------------------------------------------------//
void CVJController::Reset()
{
  for (int i = 0; i < m_iNumLayers; i++)
    m_Layers[i].Clear();
  m_Effects.VaciaConDispose();
  m_Timer.Reset();
  m_fTime          = 0.f;
  m_fTimeCur       = 0.f;
  m_iIDEffectCount = 0;
}


//---------------------------------------------------------------------------//
// GetTime
//
//---------------------------------------------------------------------------//
float CVJController::GetTime()
{
  return m_Timer.Get();
}


//---------------------------------------------------------------------------//
// RunFFT
//
//---------------------------------------------------------------------------//
void CVJController::RunFFT()
{
  float BassFFT2[256];
  if (m_pSound->GetFFT(BassFFT2))
  {
    for (int i = 0; i < 256; i++)
    {
      BassFFT2[i] = BassFFT2[i] * (1.f + i*0.09f) * 1.0f * m_fGain;
      if (BassFFT2[i] > m_pFFT[i])
      {
        m_pFFT[i] = BassFFT2[i];
        if (m_pFFT[i] > 1.0f)
          m_pFFT[i] = 1.0f;
      }
      else
      {
        m_pFFT[i] = m_pFFT[i]-0.04f;
        if (m_pFFT[i] < BassFFT2[i])
          m_pFFT[i] = BassFFT2[i];
      }
    }
    // FreqChannels
    for (int i = 0; i < FREQ_CHANNELS; i++)
    {
      m_pFFTChannel[i] = 0;
      // Rango
      int l = 0;
      int r = 255;
      if (i > 0)
      {
        l = ((i-1) * (255 / 5));
        r = (l + (255 / 5));
      }
      for (int j = l; j < r; j++)
      {
        if (m_pFFT[j] > m_pFFTChannel[i])
          m_pFFTChannel[i] = m_pFFT[j];
      }
    }
  }
}
