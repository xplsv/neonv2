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

#include "DemoSystemPch.h"
#include "Effect.h"
#include "Effects/EnumEffects.h"

//---------------------------------------------------------------------------//
// Crea un efecto de la clase especificada
//
//---------------------------------------------------------------------------//
CEffect *CEffect::Create(const string &sClass, const string &sFile)
{
  CEffect *pEffect = NULL;

#define ITEM(nombre,xxx,clase) \
    if (!pEffect && !Stricmp(nombre,sClass)) \
    {                                        \
      pEffect = NEW clase;                   \
      if (!pEffect->Init(sFile))             \
        DISPOSE(pEffect);                    \
    }
#include "Effects/EnumEffects.h"
#undef ITEM

  if (!pEffect)
    GLOG(("ERR: Can't create FX from file %s (class %s)\n", sFile.c_str(), sClass.c_str()));

  return pEffect;
}


//---------------------------------------------------------------------------//
// CEffect
//
//---------------------------------------------------------------------------//
CEffect::CEffect()
{
  m_fTimeAnt          = 0.f;
  m_fTime             = 0.f;
  m_fTimeFrame        = 0.f;
  m_bClear            = true;
  m_bFirstClear       = true;
  m_fStart            = 0.f;
  m_fEnd              = 1.f;
  m_fLength           = 0.f;
  m_bAllowRevPlayback = true;
  m_bOk               = false;
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CEffect::SetTime(float fTime)
{
  m_fTimeFrame = m_fTime - m_fTimeAnt;
  m_fTimeAnt   = m_fTime;
  m_fTime      = fTime;
}
