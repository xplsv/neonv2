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
#include "PathSpline.h"

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CPathSpline::Init(int iType, int iNumKeys)
{
  CPath::Init(iType, iNumKeys);
  switch (iType)
  {
    case PATH_3D: m_pKeys = NEW_ARRAY(TKeySpline, m_iMaxKeys); break;
    case PATH_1D: GLOG(("Warning: TVector1 keys not supported on PathSpline\n")); break;
    case PATH_2D: GLOG(("Warning: TVector2 keys not supported on PathSpline\n")); break;
    case PATH_4D: GLOG(("Warning: TVector4 keys not supported on PathSpline\n")); break;
  }
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CPathSpline::End()
{
  if (IsOk())
  {
    LiberaVars();
    CPath::End();
  }
}


//---------------------------------------------------------------------------//
// BorraVars
//
//---------------------------------------------------------------------------//
void CPathSpline::BorraVars()
{
  m_iKey  = 0;
  m_pKeys = NULL;
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CPathSpline::LiberaVars()
{
  if (IsOk())
  {
    DISPOSE_ARRAY(m_pKeys);
  }
}


//---------------------------------------------------------------------------//
// AddKey
//
//---------------------------------------------------------------------------//
void CPathSpline::AddKey(float fTime, TVector3 const &Key)
{
  ASSERT(m_pKeys);
  ASSERT(m_iNumKeys < m_iMaxKeys);

  if (m_iNumKeys < m_iMaxKeys)
  {
    m_fTimeIni = min(fTime, m_fTimeIni);
    m_fTimeEnd = max(fTime, m_fTimeEnd);
    m_fTime    = m_fTimeIni;
    m_pKeys[m_iNumKeys].fTime    = fTime;
    m_pKeys[m_iNumKeys].pos      = Key;
    m_pKeys[m_iNumKeys].easeFrom = 0.0f;
    m_pKeys[m_iNumKeys].easeTo   = 0.0f;
    m_pKeys[m_iNumKeys].cont     = 0.2f;
    m_pKeys[m_iNumKeys].bias     = 0.3f;
    m_pKeys[m_iNumKeys].tens     = 0.2f;
    m_pKeys[m_iNumKeys].dd       = TVector3(0.f,0.f,0.f);
    m_pKeys[m_iNumKeys].ds       = TVector3(0.f,0.f,0.f);
    m_iNumKeys++;
  }
}


//---------------------------------------------------------------------------//
// Initialize
//
//---------------------------------------------------------------------------//
void CPathSpline::Initialize()
{
  if (m_iNumKeys <= 1) return;
  if (m_iNumKeys == 2)
  {
    CompDerivTwo (&m_pKeys[0], &m_pKeys[1]);
    return;
  }

  //CompDerivFirst(&m_pKeys[0], &m_pKeys[1], &m_pKeys[2]);
  CompDeriv (&m_pKeys[m_iNumKeys-1], &m_pKeys[0], &m_pKeys[1]);
  for (int i = 1; i < (m_iNumKeys-3); i++)
    CompDeriv (&m_pKeys[i], &m_pKeys[i+1], &m_pKeys[i+2]);
  CompDeriv (&m_pKeys[m_iNumKeys-2], &m_pKeys[m_iNumKeys-1], &m_pKeys[0]);
  //CompDerivLast (&m_pKeys[m_iNumKeys-3], &m_pKeys[m_iNumKeys-2], &m_pKeys[m_iNumKeys-1]);
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CPathSpline::SetTime(float fTime)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    // Rango
    while (fTime > m_fTimeEnd)
      fTime-=(m_fTimeEnd-m_fTimeIni);
    while (fTime < m_fTimeIni)
      fTime+=(m_fTimeEnd-m_fTimeIni);
    m_fTime = fTime;
    SearchKeys();
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CPathSpline::Run(float fRunTime)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    m_fTime+= fRunTime;
    while (m_fTime > m_fTimeEnd)
      m_fTime-=(m_fTimeEnd-m_fTimeIni);
    while (m_fTime < m_fTimeIni)
      m_fTime+=(m_fTimeEnd-m_fTimeIni);
    SearchKeys();
  }
}


//---------------------------------------------------------------------------//
//
//
//---------------------------------------------------------------------------//
void CPathSpline::GetKeyValue(TVector3 &Value)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    TKeySpline *key     = &m_pKeys[NextKey(m_iKey)];
    TKeySpline *prevkey = &m_pKeys[m_iKey];

    float t1 = Ease ((m_fTime - prevkey->fTime) / (key->fTime - prevkey->fTime), prevkey->easeFrom, key->easeTo);
    float t2 = t1*t1;
    float t3 = t2*t1;
    float h[4];
    h[0] = 2*t3-3*t2+1;
    h[1] =-2*t3+3*t2;
    h[2] = t3-2*t2+t1;
    h[3] = t3-t2;

    Value.x = (h[0]*prevkey->pos.x) + (h[1]*key->pos.x) + (h[2]*prevkey->dd.x) + (h[3]*key->ds.x);
    Value.y = (h[0]*prevkey->pos.y) + (h[1]*key->pos.y) + (h[2]*prevkey->dd.y) + (h[3]*key->ds.y);
    Value.z = (h[0]*prevkey->pos.z) + (h[1]*key->pos.z) + (h[2]*prevkey->dd.z) + (h[3]*key->ds.z);
  }
  else
    Value = m_pKeys[0].pos;
}


//---------------------------------------------------------------------------//
//
//
//---------------------------------------------------------------------------//
void CPathSpline::SearchKeys()
{
  ASSERT (m_fTime >= m_fTimeIni && m_fTime <= m_fTimeEnd);
  m_iKey = 0;
  int  iNextKey = NextKey(m_iKey);
  bool bFound   = false;
  do
  {
    if (m_fTime >= m_pKeys[m_iKey].fTime && m_fTime <= m_pKeys[iNextKey].fTime)
      bFound = true;
    else
    {
      m_iKey   = iNextKey;
      iNextKey = NextKey(m_iKey);
    }
  } while (!bFound);
}


//---------------------------------------------------------------------------//
// CompElementDeriv
//
//---------------------------------------------------------------------------//
static void CompElementDeriv(float  pp, float   p, float  pn,
                             float *ds, float *dd, float ksm,
                             float ksp, float kdm, float kdp)
{
  float delm = p - pp;
  float delp = pn - p;

  *ds = ksm * delm + ksp * delp;
  *dd = kdm * delm + kdp * delp;
}


//---------------------------------------------------------------------------//
// CompDeriv
//
//---------------------------------------------------------------------------//
void CPathSpline::CompDeriv(TKeySpline *keyp, TKeySpline *key, TKeySpline *keyn)
{
  float tm, cm, cp, bm, bp, tmcm, tmcp;
  float ksm, ksp, kdm, kdp;
  float dt, fp, fn, c;

  dt = 0.5f * (keyn->fTime - keyp->fTime);
  fp = (key->fTime - keyp->fTime) / dt;
  fn = (keyn->fTime - key->fTime) / dt;
  c = (float)fabs (key->cont);
  fp = fp + c  - c * fp;
  fn = fn + c  - c * fn;
  cm = 1.0f - key->cont;
  tm = 0.5f * (1.0f - key->tens);
  cp = 2.0f - cm;
  bm = 1.0f - key->bias;
  bp = 2.0f - bm;
  tmcm = tm * cm;
  tmcp = tm * cp;
  ksm = tmcm * bp * fp; ksp = tmcp * bm * fp;
  kdm = tmcp * bp * fn; kdp = tmcm * bm * fn;

  CompElementDeriv(keyp->pos.x, key->pos.x, keyn->pos.x, &key->ds.x, &key->dd.x, ksm, ksp, kdm, kdp);
  CompElementDeriv(keyp->pos.y, key->pos.y, keyn->pos.y, &key->ds.y, &key->dd.y, ksm, ksp, kdm, kdp);
  CompElementDeriv(keyp->pos.z, key->pos.z, keyn->pos.z, &key->ds.z, &key->dd.z, ksm, ksp, kdm, kdp);
}


//---------------------------------------------------------------------------//
// CompDerivFirst
//
//---------------------------------------------------------------------------//
void CPathSpline::CompDerivFirst(TKeySpline *key, TKeySpline *keyn, TKeySpline *keynn)
{
  float f20, f10, v20, v10;

  f10 = (float)(keyn->fTime - key->fTime);
  f20 = (float)(keynn->fTime - key->fTime);

  v10 = keyn->pos.x - key->pos.x;
  v20 = keynn->pos.x - key->pos.x;
  key->dd.x = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = keyn->pos.y - key->pos.y;
  v20 = keynn->pos.y - key->pos.y;
  key->dd.y = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = keyn->pos.z - key->pos.z;
  v20 = keynn->pos.z - key->pos.z;
  key->dd.z = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);
}


//---------------------------------------------------------------------------//
// CompDerivLast
//
//---------------------------------------------------------------------------//
void CPathSpline::CompDerivLast(TKeySpline *keypp, TKeySpline *keyp, TKeySpline *key)
{
  float f20, f10, v20, v10;

  f10 = (float)(key->fTime - keyp->fTime);
  f20 = (float)(key->fTime - keypp->fTime);

  v10 = key->pos.x - keyp->pos.x;
  v20 = key->pos.x - keypp->pos.x;
  key->ds.x = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = key->pos.y - keyp->pos.y;
  v20 = key->pos.y - keypp->pos.y;
  key->ds.y = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);

  v10 = key->pos.z - keyp->pos.z;
  v20 = key->pos.z - keypp->pos.z;
  key->ds.z = (v20 * (0.25f - f10 / (f20*2)) + (v10 - v20/2) * 3/2 + v20/2);
}


//---------------------------------------------------------------------------//
// CompDerivTwo
//
//---------------------------------------------------------------------------//
void CPathSpline::CompDerivTwo(TKeySpline *key,  TKeySpline *keyn)
{
  key->ds.x = key->ds.y = key->ds.z = 0.f;

  key->dd.x = (keyn->pos.x - key->pos.x) * (1.0f - key->tens);
  key->dd.y = (keyn->pos.y - key->pos.y) * (1.0f - key->tens);
  key->dd.z = (keyn->pos.z - key->pos.z) * (1.0f - key->tens);

  keyn->dd.x = keyn->dd.y = key->dd.z = 0.f;

  keyn->ds.x = (keyn->pos.x - key->pos.x) * (1.0f - keyn->tens);
  keyn->ds.y = (keyn->pos.y - key->pos.y) * (1.0f - keyn->tens);
  keyn->ds.z = (keyn->pos.z - key->pos.z) * (1.0f - keyn->tens);
}


//---------------------------------------------------------------------------//
// Ease
//
//---------------------------------------------------------------------------//
float CPathSpline::Ease(float t, float a, float b)
{
  float k;
  float s = a+b;

  if (s == 0.0f) return t;
  if (s > 1.0f)
  {
    a = a/s;
    b = b/s;
  }
  k = 1.0f/(2.0f-a-b);
  if (t < a) return ((k/a)*t*t);
  else
  {
    if (t < 1.0f-b) return (k*(2*t-a));
    else
    {
      t = 1.0f-t;
      return (1.0f-(k/b)*t*t);
    }
  }
}
