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
#include "VJLoader.h"
#include "GEDisplayDevice.h"
#include "GETemp.h"


//----------------------------------------------------------------------------
// Init
//
//----------------------------------------------------------------------------
bool CVJLoader::Init(int iMax)
{
  m_iMax = iMax;
  m_iPos = 0;

  return (true);
}


//----------------------------------------------------------------------------
// End
//
//----------------------------------------------------------------------------
void CVJLoader::End()
{
}


//----------------------------------------------------------------------------
// Next
//
//----------------------------------------------------------------------------
void CVJLoader::Next()
{
  m_iPos++;
  if (m_iPos > m_iMax)
    m_iPos = m_iMax;
}


//----------------------------------------------------------------------------
// Draw
//
//----------------------------------------------------------------------------
void CVJLoader::Draw(CDisplayDevice *pDD)
{
  pDD->BeginFrame();
  pDD->BeginScene();
  pDD->Clear(true, true, 0, 1.f);
  TVector2 vIni = TVector2((float)pDD->Width() - 126.f, (float)pDD->Height()-12.f);
  TVector2 vEnd = TVector2((float)pDD->Width() -   6.f, (float)pDD->Height()- 6.f);
  float fPos = ((float)m_iPos * (vEnd.x-vIni.x-4.f) / (float)m_iMax) + vIni.x + 2.f;
  DrawQuadFlat(pDD, TVector2(vIni.x+0,vIni.y+0), TVector2(vEnd.x-0,vEnd.y-0), HARD_COLOR_ARGB(255,255,168,  0), 0.f);
  DrawQuadFlat(pDD, TVector2(vIni.x+1,vIni.y+1), TVector2(vEnd.x-1,vEnd.y-1), HARD_COLOR_ARGB(255,  0,  0,  0), 0.f);
  DrawQuadFlat(pDD, TVector2(vIni.x+2,vIni.y+2), TVector2(fPos,    vEnd.y-2), HARD_COLOR_ARGB(255,255,168,  0), 0.f);
  pDD->EndScene();
  pDD->EndFrame();
  pDD->PageFlip();
}
