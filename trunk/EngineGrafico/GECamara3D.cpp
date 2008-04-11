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

#include "Base.h"
#include "GECamara3D.h"
#include "GEMotor3D.h"
#include "PathLinear.h"
#include "PathSpline.h"
#include "CtrlVar.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::EDIT_BOX,  0, "FFT Chan",   false, 0, NULL},
  {TCtrlVar::SLIDER,    1, "FFT X-Off",  true,  0, NULL},
  {TCtrlVar::SLIDER,    2, "FFT Y-Off",  true,  0, NULL},
  {TCtrlVar::SLIDER,    3, "FFT Z-Off",  true,  0, NULL},
  {TCtrlVar::SLIDER,    4, "Noise Scale",false, 0, NULL},
  {TCtrlVar::SLIDER,    5, "Noise",      true,  0, NULL},
  {TCtrlVar::CHECK_BOX, 6, "Manual Fov", true,  0, NULL},
  {TCtrlVar::SLIDER,    7, "Fov",        true,  0, NULL},
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CCamara3D::GetVars()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CCamara3D::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: if (pData) m_iFFTChan = atoi((const char *)pData); break;
    case 1: m_fFFTXOff   = *(float *)pData; break;
    case 2: m_fFFTYOff   = *(float *)pData; break;
    case 3: m_fFFTZOff   = *(float *)pData; break;
    case 4: m_fNoiseScale= *(float *)pData; break;
    case 5: m_fNoise     = *(float *)pData; break;
    case 6:
      m_bManualFov = *(bool  *)pData;
      m_fCacheFov = ((m_fFov * 180.f + 30.f) / 90.f); // De 70 a 170
      break;
    case 7:
      m_fFov      = *(float *)pData;
      m_fCacheFov = ((m_fFov * 180.f + 30.f) / 90.f); // De 70 a 170
      break;
  }
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void *CCamara3D::GetVar(int iVar)
{
  static char pBuffer[256];
  switch (iVar)
  {
    case 0: _itoa_s(m_iFFTChan, pBuffer, 256, 10); return pBuffer;
    case 1: return (&m_fFFTXOff);
    case 2: return (&m_fFFTYOff);
    case 3: return (&m_fFFTZOff);
    case 4: return (&m_fNoiseScale);
    case 5: return (&m_fNoise);
    case 6: return (&m_bManualFov);
    case 7: return (&m_fFov);
  }
  return NULL;
}



//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CCamara3D::Init()
{
  End();
  BorraVars();

  m_bOk = true;

  return IsOk();
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CCamara3D::End()
{
  if (IsOk())
  {
    LiberaVars();
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// BorraVars
//
//---------------------------------------------------------------------------//
void CCamara3D::BorraVars()
{
  m_Nombre      = "";
  m_fNearClip   = 1.f;
  m_fFarClip    = 1000.f;
  m_pPathFov    = NULL;
  m_pPathSrc    = NULL;
  m_pPathUp     = NULL;
  m_pPathTgt    = NULL;
  m_iFFTChan    = 0;
  m_fFFTXOff    = 0.5f;
  m_fFFTYOff    = 0.5f;
  m_fFFTZOff    = 0.5f;
  m_fNoiseScale = 0.f;
  m_fNoise      = 0.f;
  m_fFov        = 0.2f;
  m_bManualFov  = false;
  m_fCacheFov   = 0.2f;
  m_vCachePosSrc= TVector3(0.f, 100.f, -100.f);
  m_vCachePosTgt= TVector3(0.f,   0.f,    0.f);
  m_vCachePosUp = TVector3(0.f,   1.f,    0.f);
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CCamara3D::LiberaVars()
{
  DISPOSE(m_pPathSrc);
  DISPOSE(m_pPathTgt);
  DISPOSE(m_pPathUp );
  DISPOSE(m_pPathFov);
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CCamara3D::SetTime(float fTime)
{
  if (m_pPathSrc)
  {
    m_pPathSrc->SetTime(fTime);
    m_pPathSrc->GetKeyValue(m_vCachePosSrc);
  }
  if (m_pPathTgt)
  {
    m_pPathTgt->SetTime(fTime);
    m_pPathTgt->GetKeyValue(m_vCachePosTgt);
  }
  if (m_pPathUp)
  {
    m_pPathUp->SetTime(fTime);
    m_pPathUp->GetKeyValue(m_vCachePosUp);
  }
  if (m_pPathFov && !m_bManualFov)
  {
    m_pPathFov->SetTime(fTime);
    m_pPathFov->GetKeyValue(m_fCacheFov);
  }
  // Apply FFT
  ApplyFFT();
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CCamara3D::Run(float fRunTime)
{
  if (m_pPathSrc)
  {
    m_pPathSrc->Run(fRunTime);
    m_pPathSrc->GetKeyValue(m_vCachePosSrc);
  }
  if (m_pPathTgt)
  {
    m_pPathTgt->Run(fRunTime);
    m_pPathTgt->GetKeyValue(m_vCachePosTgt);
  }
  if (m_pPathUp)
  {
    m_pPathUp->Run(fRunTime);
    m_pPathUp->GetKeyValue(m_vCachePosUp);
  }
  if (m_pPathFov && !m_bManualFov)
  {
    m_pPathFov->Run(fRunTime);
    m_pPathFov->GetKeyValue(m_fCacheFov);
  }
  // Apply FFT
  ApplyFFT();
}


//---------------------------------------------------------------------------//
// ApplyFFT
//
//---------------------------------------------------------------------------//
void CCamara3D::ApplyFFT()
{
  float fWave;
  CMotor3D::GetRegisterData(CMotor3D::V_WAVE + m_iFFTChan, &fWave);
  // Actualizar pos segun el offset FFT especificado y el noise
  float fNoise = m_fNoise * m_fNoiseScale * 1000.f;
  m_vCachePosSrc.x+= (m_fFFTXOff-0.5f) * fWave * 1000.f + (rand() & 65535) / 65535.f * fNoise;
  m_vCachePosSrc.y+= (m_fFFTYOff-0.5f) * fWave * 1000.f + (rand() & 65535) / 65535.f * fNoise;
  m_vCachePosSrc.z+= (m_fFFTZOff-0.5f) * fWave * 1000.f + (rand() & 65535) / 65535.f * fNoise;
}


//---------------------------------------------------------------------------//
// SetPosSrc
//
//---------------------------------------------------------------------------//
void CCamara3D::SetPosSrc(TVector3 *pVertices, unsigned *pTimes, unsigned uFrames)
{
  m_pPathSrc = NEW CPathSpline;
  m_pPathSrc->Init(CPath::PATH_3D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathSrc->AddKey((float)pTimes[i], pVertices[i]);
  m_pPathSrc->Initialize();
  // Cache
  m_vCachePosSrc = pVertices[0];
  DISPOSE_ARRAY(pVertices);  
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetPosTgt
//
//---------------------------------------------------------------------------//
void CCamara3D::SetPosTgt(TVector3 *pVertices, unsigned *pTimes, unsigned uFrames)
{
  m_pPathTgt = NEW CPathSpline;
  m_pPathTgt->Init(CPath::PATH_3D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathTgt->AddKey((float)pTimes[i], pVertices[i]);
  m_pPathTgt->Initialize();
  // Cache
  m_vCachePosTgt = pVertices[0];
  DISPOSE_ARRAY(pVertices);  
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetPosUp
//
//---------------------------------------------------------------------------//
void CCamara3D::SetPosUp(TVector3 *pVertices, unsigned *pTimes, unsigned uFrames)
{
  m_pPathUp = NEW CPathSpline;
  m_pPathUp->Init(CPath::PATH_3D, uFrames);
  for (unsigned  i = 0; i < uFrames; i++)
    m_pPathUp->AddKey((float)pTimes[i], pVertices[i]);
  m_pPathUp->Initialize();
  // Cache
  m_vCachePosUp = pVertices[0];
  DISPOSE_ARRAY(pVertices);  
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetFov
//
//---------------------------------------------------------------------------//
void CCamara3D::SetFov(float *pFov, unsigned *pTimes, unsigned uFrames)
{
  m_pPathFov = NEW CPathLinear;
  m_pPathFov->Init(CPath::PATH_1D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathFov->AddKey((float)pTimes[i], pFov[i]);
  m_pPathFov->Initialize();
  // Cache
  m_fCacheFov = pFov[0];
  DISPOSE_ARRAY(pFov);
  DISPOSE_ARRAY(pTimes);
}
