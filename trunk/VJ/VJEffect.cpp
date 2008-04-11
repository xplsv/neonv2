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
#include "Options.h"
#include "AppOptions.h"
#include "Xml.h"


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//
bool CVJEffect::Load(const string &sClass, const string &sFile, int iID, TVJSnapshot *pSnapshot, void const *iTexturasRender)
{
  End();
  m_Effect = CEffect::Create(sClass, sFile);
  if (m_Effect)
  {
    for (int i = 0; i < MAX_FILTERS; i++)
    {
      m_Filters[i].iID     = -1;
      m_Filters[i].pFilter = NULL;
    }
    m_ID             = iID;
    m_Ok             = true;
    m_Quality        = Q_CUST1;
    m_Blend          = BL_ALPHABLEND;
    m_Alpha          = 1;
    m_FadeAlpha      = 0;
    m_FadeLength     = 2.40f;
    m_ResetOnAttach  = true;
    m_FadeIn         = true;
    for (int i = 0; i < Q_QUALITIES; i++)
    {
      m_TexturasRender[i][0] = ((int *)iTexturasRender)[i*2+0];
      m_TexturasRender[i][1] = ((int *)iTexturasRender)[i*2+1];
    }
    if (pSnapshot)
      CreaSnapshot(pSnapshot);
  }

  return m_Ok;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CVJEffect::End()
{
  if (IsOk())
  {
    for (int i = 0; i < MAX_FILTERS; i++)
      DISPOSE(m_Filters[i].pFilter);
    DISPOSE(m_Effect);
    m_Ok = false;
  }
}


//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//
void CVJEffect::Attach(float fTime)
{
  //m_Effect->Attach();
  if (m_ResetOnAttach)
  {
    m_LastRealTime = fTime;
    m_Time         = 0.f;
  }
}


//---------------------------------------------------------------------------//
// Unattach
//
//---------------------------------------------------------------------------//
void CVJEffect::Unattach()
{
  //m_Effect->Unattach();
}


//---------------------------------------------------------------------------//
// FadeIn
//
//---------------------------------------------------------------------------//
void CVJEffect::FadeIn(float fTime)
{
  m_FadeTime = fTime;
  m_FadeIn   = true;
}


//---------------------------------------------------------------------------//
// FadeOut
//
//---------------------------------------------------------------------------//
void CVJEffect::FadeOut(float fTime)
{
  m_FadeTime = fTime;
  m_FadeIn   = false;
}


//---------------------------------------------------------------------------//
// AddFilter
//
//---------------------------------------------------------------------------//
int CVJEffect::AddFilter(int iSlot, const string &sFilter)
{
  ASSERT(iSlot >= 0 && iSlot < MAX_FILTERS);
  PushDir("Filters");
  CFilter *pFilter = CFilter::Create(sFilter);
  PopDir();
  if (pFilter)
  {
    DISPOSE(m_Filters[iSlot].pFilter);
    m_Filters[iSlot].iID     = m_CurrentFilterID++;
    m_Filters[iSlot].pFilter = pFilter;
    return m_Filters[iSlot].iID;
  }
  return -1;
}


//---------------------------------------------------------------------------//
// RemoveFilter
//
//---------------------------------------------------------------------------//
void CVJEffect::RemoveFilter(int iSlot)
{
  ASSERT(iSlot >= 0 && iSlot < MAX_FILTERS);
  DISPOSE(m_Filters[iSlot].pFilter);
}


//---------------------------------------------------------------------------//
// SwapFilter
//
//---------------------------------------------------------------------------//
void CVJEffect::SwapFilter(int iSlotSrc, int iSlotDst)
{
  ASSERT(iSlotSrc >= 0 && iSlotSrc < MAX_FILTERS && iSlotDst >= 0 && iSlotDst < MAX_FILTERS);
  swap(m_Filters[iSlotSrc].iID,     m_Filters[iSlotDst].iID);
  swap(m_Filters[iSlotSrc].pFilter, m_Filters[iSlotDst].pFilter);
}


//---------------------------------------------------------------------------//
// GetFilterByID
//
//---------------------------------------------------------------------------//
CFilter *CVJEffect::GetFilterByID(int iIDFilter)
{
  int iSlot = GetFilterSlot(iIDFilter);
  ASSERT(iSlot >= 0);
  return m_Filters[iSlot].pFilter;
}


//---------------------------------------------------------------------------//
// GetFilterSlot
//
//---------------------------------------------------------------------------//
int CVJEffect::GetFilterSlot(int iIDFilter)
{
  for (int i = 0; i < MAX_FILTERS; i++)
  {
    if (m_Filters[i].iID == iIDFilter)
      return i;
  }
  return -1;
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CVJEffect::SetTime(float fTime)
{
  float fRunTime = (fTime - m_LastRealTime) * m_Speed;
  m_Time         = m_Time + fRunTime;
  m_LastRealTime = fTime;

  if (m_FadeIn)
    m_FadeAlpha = (fTime - m_FadeTime) / m_FadeLength;
  else
    m_FadeAlpha = 1 - (fTime - m_FadeTime) / m_FadeLength;
  m_FadeAlpha = MIN(1, MAX(0, m_FadeAlpha));
  m_Effect->SetTime(m_Time);
  for (int i = 0; i < MAX_FILTERS; i++)
  {
    if (m_Filters[i].pFilter)
      m_Filters[i].pFilter->SetTime(m_Time);
  }
}


//---------------------------------------------------------------------------//
// Render
// Updates current render of effect
//---------------------------------------------------------------------------//
void CVJEffect::Render(CDisplayDevice *pDD)
{
  if (IsVisible())
  {
    int iQuality = MAX(MIN(m_Quality,Q_QUALITIES-1),0);
    m_Effect->Draw(pDD, m_TexturasRender[iQuality][0]);
  }
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CVJEffect::Draw(CDisplayDevice *pDD)
{
  if (IsVisible())
  {
    int iQuality = MAX(MIN(m_Quality,Q_QUALITIES-1),0);
    float fRand;
    float fSinTime = sinf(m_Time);
    float fAlpha   = m_Alpha*m_FadeAlpha;
    CMotor3D::SetRegisterData(CMotor3D::V_TIME,    &m_Time  );
    CMotor3D::SetRegisterData(CMotor3D::V_ALPHA,   &fAlpha  );
    CMotor3D::SetRegisterData(CMotor3D::V_SINTIME, &fSinTime);
    fRand = (float)(rand() & 255) / 255.f;
    CMotor3D::SetRegisterData(CMotor3D::V_RAND1, &fRand);
    fRand = (float)(rand() & 255) / 255.f;
    CMotor3D::SetRegisterData(CMotor3D::V_RAND2, &fRand);
    fRand = (float)(rand() & 255) / 255.f;
    CMotor3D::SetRegisterData(CMotor3D::V_RAND3, &fRand);

    // Save copy of the render target and apply all filters to it
    // We save a copy as filters can change its behaviour during the loop
    int iTexTgt0 = m_TexturasRender[iQuality][0];
    int iTexTgt1 = m_TexturasRender[iQuality][1];
    for (int i = 0; i < MAX_FILTERS; i++)
    {
      if (m_Filters[i].pFilter && m_Filters[i].pFilter->Apply(&g_DisplayDevice, iTexTgt0, iTexTgt1))
      {
        int i = iTexTgt0;
        iTexTgt0 = iTexTgt1;
        iTexTgt1 = i;
      }
    }

    //-------------------------------------------------------
    // Draw final result on screen
    pDD->ApplyBasicShader();
    D3DDEVICE *pD3D = pDD->GetD3DDevice();

    unsigned uAlpha = (unsigned)(fAlpha * 255.f);
    unsigned aColor = HARD_COLOR_ARGB(uAlpha,255,255,255);
    pD3D->SetRenderState      (D3DRS_CULLMODE,        D3DCULL_NONE);
    pD3D->SetRenderState      (D3DRS_ZENABLE,         D3DZB_FALSE);
    pD3D->SetRenderState      (D3DRS_TEXTUREFACTOR,   aColor);
    pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    // Only apply alpha modulate when texture is A8R8G8B8
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    pDD->SetRenderTarget(-1);
    pDD->SetBlendMode(m_Blend, fAlpha);
    pDD->SetBilinearFiltering(0, false);
    pDD->SetBilinearFiltering(1, false);
    pDD->SetBilinearFiltering(2, false);
    pDD->SetBilinearFiltering(3, false);
    g_pGestorMateriales->SetTextura(iTexTgt0, 0);
    DrawQuad(pDD);
    //-------------------------------------------------------
  }
}


//---------------------------------------------------------------------------//
// CreaSnapshot
//
//---------------------------------------------------------------------------//
void CVJEffect::CreaSnapshot(TVJSnapshot *pSnapshot)
{
  bool bDrawing = g_DisplayDevice.IsDrawingScene();
  ASSERT(!bDrawing);
  if (!bDrawing)
    g_DisplayDevice.BeginScene();
  g_DisplayDevice.Clear(true, true, 0, 1.f);

  // Draw effect on texture
  D3DDEVICE *pD3D    = g_DisplayDevice.GetD3DDevice();
  CTextura  *pTexSrc = g_pGestorMateriales->GetTextura(pSnapshot->iTexRender);
  CTextura  *pTexDst = g_pGestorMateriales->GetTextura(pSnapshot->iTexSnapshot);

  //m_Effect->Attach ();
  m_Effect->SetTime(1.f);
  m_Effect->Draw   (&g_DisplayDevice, pSnapshot->iTexRender);
  //m_Effect->Unattach();

  if (!bDrawing)
    g_DisplayDevice.EndScene();

  // Coger el render y guardarlo
  if (SUCCEEDED(pD3D->GetRenderTargetData(pTexSrc->GetSurfaceD3D(), pTexDst->GetSurfaceD3D())))
  {
    TSurfaceDesc Desc;
    if (pTexDst->Lock(NULL, Desc))
    {
      unsigned *pSrc = (unsigned *)Desc.pBits;
      unsigned *pDst = pSnapshot->pData;
      for (int i = TVJSnapshot::WIDTH * TVJSnapshot::HEIGHT; i > 0; i--)
      {
        unsigned c = *pSrc++;
        //*pDst++ = (c&0xFF000000) | ((c<<16)&0xFF0000) | (c&0xFF00) | ((c>>16)&0xFF);
        unsigned a = (c>>24);
        unsigned r = (((c    )&0xFF)*a)>>8;
        unsigned g = (((c>> 8)&0xFF)*a)>>8;
        unsigned b = (((c>>16)&0xFF)*a)>>8;
        *pDst++ = 0xFF000000 | (r<<16) | (g<<8) | b;
      }
      pTexDst->Unlock();
    }
  }

  return;
}
