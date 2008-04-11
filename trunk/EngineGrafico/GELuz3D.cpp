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
#include "GEDisplayDevice.h"
#include "GELuz3D.h"
#include "PathLinear.h"
#include "PathSpline.h"
#include "CtrlVar.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::SLIDER, 0, "Intens", true, 0, NULL},
  {TCtrlVar::SLIDER, 1, "Range",  true, 0, NULL},
  {TCtrlVar::SLIDER, 2, "Red",    true, 0, NULL},
  {TCtrlVar::SLIDER, 3, "Green",  true, 0, NULL},
  {TCtrlVar::SLIDER, 4, "Blue",   true, 0, NULL},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CLuz3D::GetVars()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CLuz3D::SetVar(int iVar, void *pData)
{
  switch (iVar)
  {
    case 0: m_fChangeIntens = *(float *)pData; break;
    case 1: m_fChangeRange  = *(float *)pData; break;
    case 2: m_fR            = *(float *)pData; break;
    case 3: m_fG            = *(float *)pData; break;
    case 4: m_fB            = *(float *)pData; break;
  }
  Update();
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void *CLuz3D::GetVar(int iVar)
{
  switch (iVar)
  {
    case 0: return (&m_fChangeIntens);
    case 1: return (&m_fChangeRange);
    case 2: return (&m_fR);
    case 3: return (&m_fG);
    case 4: return (&m_fB);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CLuz3D::Init()
{
  ZeroMemory(&m_Light, sizeof(m_Light));

  m_fChangeIntens = 1.f;
  m_fChangeRange  = 1.f;
  m_fR            = 1.f;
  m_fG            = 1.f;
  m_fB            = 1.f;
  m_fRange        = 1000.f;

  m_Light.Type         = D3DLIGHT_POINT;
  m_Light.Position.x   = 0.f;
  m_Light.Position.y   = 0.f;
  m_Light.Position.z   = 0.f;
  m_Light.Attenuation0 = 1.f;
  m_Light.Attenuation1 = 0.f;
  m_Light.Attenuation2 = 0.f;
  m_Light.Falloff      = 0.f;

  m_bOk = true;
  
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CLuz3D::End()
{
  if (IsOk())
  {
    DISPOSE(m_pPathSrc);
    DISPOSE(m_pPathTgt);
    DISPOSE(m_pPathFalloff);
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CLuz3D::SetTime(float fTime)
{
  if (m_pPathSrc && m_pPathSrc->GetNumKeys() > 1)
  {
    m_pPathSrc->SetTime(fTime);
    m_pPathSrc->GetKeyValue(*((TVector3 *)&m_Light.Position));
  }
  if (m_pPathTgt && m_pPathTgt->GetNumKeys() > 1)
  {
    TVector3 Target;
    m_pPathTgt->SetTime(fTime);
    m_pPathTgt->GetKeyValue(Target);
    m_Light.Direction.x = Target.x - m_Light.Position.x;
    m_Light.Direction.y = Target.y - m_Light.Position.y;
    m_Light.Direction.z = Target.z - m_Light.Position.z;
  }
  if (m_pPathFalloff && m_pPathFalloff ->GetNumKeys() > 1)
  {
    m_pPathFalloff->SetTime(fTime);
    m_pPathFalloff->GetKeyValue(*((float *)&m_Light.Falloff));
  }
}


//---------------------------------------------------------------------------//
// SetFrame
//
//---------------------------------------------------------------------------//
void CLuz3D::Update()
{
  m_Light.Diffuse.r = m_fR     * m_fChangeIntens;
  m_Light.Diffuse.g = m_fG     * m_fChangeIntens;
  m_Light.Diffuse.b = m_fB     * m_fChangeIntens;
  m_Light.Range     = m_fRange * m_fChangeRange;
}



//---------------------------------------------------------------------------//
// SetPosSrc
//
//---------------------------------------------------------------------------//
void CLuz3D::SetPosSrc(TVector3 *pVertices, unsigned *pTimes, unsigned uFrames)
{
  m_pPathSrc = NEW CPathSpline;
  m_pPathSrc->Init(CPath::PATH_3D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathSrc->AddKey((float)pTimes[i], pVertices[i]);
  m_pPathSrc->Initialize();
  // Cache
  m_Light.Position.x = pVertices[0].x;
  m_Light.Position.y = pVertices[0].y;
  m_Light.Position.z = pVertices[0].z;
  DISPOSE_ARRAY(pVertices);  
  DISPOSE_ARRAY(pTimes);
}


//---------------------------------------------------------------------------//
// SetPosTgt
//
//---------------------------------------------------------------------------//
void CLuz3D::SetPosTgt(TVector3 *pVertices, unsigned *pTimes, unsigned uFrames)
{
  m_pPathSrc = NEW CPathSpline;
  m_pPathSrc->Init(CPath::PATH_3D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathSrc->AddKey((float)pTimes[i], pVertices[i]);
  m_pPathSrc->Initialize();
  // Cache
  m_Light.Direction.x = pVertices[0].x - m_Light.Position.x;
  m_Light.Direction.y = pVertices[0].y - m_Light.Position.y;
  m_Light.Direction.z = pVertices[0].z - m_Light.Position.z;
  DISPOSE_ARRAY(pVertices);  
  DISPOSE_ARRAY(pTimes);
}

//---------------------------------------------------------------------------//
// SetFalloff
//
//---------------------------------------------------------------------------//
void CLuz3D::SetFalloff(float *pFalloff, unsigned *pTimes, unsigned uFrames)
{
  m_pPathFalloff = NEW CPathLinear;
  m_pPathFalloff->Init(CPath::PATH_1D, uFrames);
  for (unsigned i = 0; i < uFrames; i++)
    m_pPathFalloff->AddKey((float)pTimes[i], pFalloff[i]);
  m_pPathFalloff->Initialize();
  // Cache
  m_Light.Falloff = pFalloff[0];
  DISPOSE_ARRAY(pFalloff);
  DISPOSE_ARRAY(pTimes);
}