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


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CVJLayer::End()
{
  Clear();
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CVJLayer::SetTime(float fTime)
{
  // Actual
  if (m_VJEffect)
    m_VJEffect->SetTime(fTime);
  // Out
  if (m_VJEffectOut)
  {
    m_VJEffectOut->SetTime(fTime);
    if (m_VJEffectOut->GetFadeAlpha() <= 0)
      Clear(false, true);
  }
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CVJLayer::Draw(CDisplayDevice *pDD)
{
  if (m_VJEffect)
  {
    m_VJEffect->Render(pDD);
    m_VJEffect->Draw  (pDD);
  }
  if (m_VJEffectOut)
  {
    m_VJEffectOut->Render(pDD);
    m_VJEffectOut->Draw  (pDD);
  }
}


//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//
void CVJLayer::Attach(float fTime, CVJEffect *pVJEffect)
{
  if (m_VJEffect)
    Unattach(fTime);
  m_VJEffect = pVJEffect;
  if (m_VJEffect)
  {
    m_VJEffect->Attach(fTime);
    m_VJEffect->FadeIn(fTime);
  }
}


//---------------------------------------------------------------------------//
// Unattach
//
//---------------------------------------------------------------------------//
void CVJLayer::Unattach(float fTime)
{
  if (m_VJEffect)
  {
    m_VJEffectOut = m_VJEffect;
    m_VJEffectOut->FadeOut(fTime);
    m_VJEffect = NULL;
  }
}


//---------------------------------------------------------------------------//
// MoveTo
//
//---------------------------------------------------------------------------//
void CVJLayer::MoveTo(float fTime, CVJLayer *pVJLayer)
{
  ASSERT(pVJLayer);
  if (pVJLayer)
  {
    pVJLayer->Unattach(fTime);
    pVJLayer->m_VJEffect = m_VJEffect;
    m_VJEffect = NULL;
  }
}


//---------------------------------------------------------------------------//
// Clear
//
//---------------------------------------------------------------------------//
void CVJLayer::Clear(bool bActive, bool bOut)
{
  if (bActive)
  {
    if (m_VJEffect)
      m_VJEffect->Unattach();
    m_VJEffect = NULL;
  }
  if (bOut)
  {
    if (m_VJEffectOut)
      m_VJEffectOut->Unattach();
    m_VJEffectOut = NULL;
  }
}
