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
#include "PathLinear.h"
#include <d3dx9.h>

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CPathLinear::Init(int iType, int iNumKeys)
{
  BorraVars();
  CPath::Init(iType, iNumKeys);
  m_pTimes = NEW_ARRAY(float, m_iMaxKeys);
  switch (iType)
  {
    case PATH_1D: m_pKeys1 = NEW_ARRAY(float,    m_iMaxKeys); break;
    case PATH_2D: m_pKeys2 = NEW_ARRAY(TVector2, m_iMaxKeys); break;
    case PATH_3D: m_pKeys3 = NEW_ARRAY(TVector3, m_iMaxKeys); break;
    case PATH_4D: m_pKeys4 = NEW_ARRAY(TVector4, m_iMaxKeys); break;
  }
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CPathLinear::End()
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
void CPathLinear::BorraVars()
{
  m_iKey     = 0;
  m_pKeys1   = NULL;
  m_pKeys2   = NULL;
  m_pKeys3   = NULL;
  m_pKeys4   = NULL;
  m_pTimes   = NULL;
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CPathLinear::LiberaVars()
{
  if (IsOk())
  {
    DISPOSE_ARRAY(m_pTimes);
    DISPOSE_ARRAY(m_pKeys1);
    DISPOSE_ARRAY(m_pKeys2);
    DISPOSE_ARRAY(m_pKeys3);
    DISPOSE_ARRAY(m_pKeys4);
  }
}



//---------------------------------------------------------------------------//
// AddKey
//
//---------------------------------------------------------------------------//
void CPathLinear::AddKey(float fTime)
{
  ASSERT(m_pTimes);
  ASSERT(m_iNumKeys < m_iMaxKeys);

  if (m_iNumKeys < m_iMaxKeys)
  {
    m_fTimeIni = min(fTime, m_fTimeIni);
    m_fTimeEnd = max(fTime, m_fTimeEnd);
    m_fTime    = m_fTimeIni;
    m_pTimes[m_iNumKeys] = fTime;
    m_iNumKeys++;
  }
}


//---------------------------------------------------------------------------//
// AddKey
//
//---------------------------------------------------------------------------//
void CPathLinear::AddKey(float fTime, float Key)
{
  ASSERT(m_pTimes && m_pKeys1);
  ASSERT(m_iNumKeys < m_iMaxKeys);

  if (m_iNumKeys < m_iMaxKeys)
  {
    m_fTimeIni = min(fTime, m_fTimeIni);
    m_fTimeEnd = max(fTime, m_fTimeEnd);
    m_fTime    = m_fTimeIni;
    m_pTimes[m_iNumKeys] = fTime;
    m_pKeys1[m_iNumKeys] = Key;
    m_iNumKeys++;
  }
}


//---------------------------------------------------------------------------//
// AddKey
//
//---------------------------------------------------------------------------//
void CPathLinear::AddKey(float fTime, TVector2 const &Key)
{
  ASSERT(m_pTimes && m_pKeys2);
  ASSERT(m_iNumKeys < m_iMaxKeys);

  if (m_iNumKeys < m_iMaxKeys)
  {
    m_fTimeIni = min(fTime, m_fTimeIni);
    m_fTimeEnd = max(fTime, m_fTimeEnd);
    m_fTime    = m_fTimeIni;
    m_pTimes[m_iNumKeys] = fTime;
    m_pKeys2[m_iNumKeys] = Key;
    m_iNumKeys++;
  }
}


//---------------------------------------------------------------------------//
// AddKey
//
//---------------------------------------------------------------------------//
void CPathLinear::AddKey(float fTime, TVector3 const &Key)
{
  ASSERT(m_pTimes && m_pKeys3);
  ASSERT(m_iNumKeys < m_iMaxKeys);

  if (m_iNumKeys < m_iMaxKeys)
  {
    m_fTimeIni = min(fTime, m_fTimeIni);
    m_fTimeEnd = max(fTime, m_fTimeEnd);
    m_fTime    = m_fTimeIni;
    m_pTimes[m_iNumKeys] = fTime;
    m_pKeys3[m_iNumKeys] = Key;
    m_iNumKeys++;
  }
}


//---------------------------------------------------------------------------//
// AddKey
//
//---------------------------------------------------------------------------//
void CPathLinear::AddKey(float fTime, TVector4 const &Key)
{
  ASSERT(m_pTimes && m_pKeys4);
  ASSERT(m_iNumKeys < m_iMaxKeys);

  if (m_iNumKeys < m_iMaxKeys)
  {
    m_fTimeIni = min(fTime, m_fTimeIni);
    m_fTimeEnd = max(fTime, m_fTimeEnd);
    m_fTime    = m_fTimeIni;
    m_pTimes[m_iNumKeys] = fTime;
    D3DXQuaternionNormalize((D3DXQUATERNION *)&m_pKeys4[m_iNumKeys], (D3DXQUATERNION *)&Key);
    //m_pKeys4[m_iNumKeys] = Key;
    m_iNumKeys++;
  }
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CPathLinear::SetTime(float fTime)
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
void CPathLinear::Run(float fRunTime)
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
void CPathLinear::GetKeyValue(float &Value)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    int iNextKey = NextKey(m_iKey);
    LinearInterp(m_pTimes[m_iKey], m_pTimes[iNextKey], m_fTime,
                 m_pKeys1[m_iKey], m_pKeys1[iNextKey], Value);
  }
  else Value = m_pKeys1[0];
}


//---------------------------------------------------------------------------//
//
//
//---------------------------------------------------------------------------//
void CPathLinear::GetKeyValue(TVector2 &Value)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    int iNextKey = NextKey(m_iKey);
    LinearInterp(m_pTimes[m_iKey], m_pTimes[iNextKey], m_fTime,
                 m_pKeys2[m_iKey], m_pKeys2[iNextKey], Value);
  }
  else Value = m_pKeys2[0];
}


//---------------------------------------------------------------------------//
//
//
//---------------------------------------------------------------------------//
void CPathLinear::GetKeyValue(TVector3 &Value)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    int iNextKey = NextKey(m_iKey);
    LinearInterp(m_pTimes[m_iKey], m_pTimes[iNextKey], m_fTime,
                 m_pKeys3[m_iKey], m_pKeys3[iNextKey], Value);
  }
  else Value = m_pKeys3[0];
}


//---------------------------------------------------------------------------//
//
//
//---------------------------------------------------------------------------//
void CPathLinear::GetKeyValue(TVector4 &Value)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    int iNextKey = NextKey(m_iKey);
    float fT = (m_fTime - m_pTimes[m_iKey]) / (m_pTimes[iNextKey] - m_pTimes[m_iKey]);
    D3DXQuaternionSlerp((D3DXQUATERNION *)&Value, (D3DXQUATERNION *)&m_pKeys4[m_iKey], (D3DXQUATERNION *)&m_pKeys4[iNextKey], fT);
  }
  else Value = m_pKeys4[0];
}


//---------------------------------------------------------------------------//
// GetKeyValues
//
//---------------------------------------------------------------------------//
void CPathLinear::GetKeyValues(TVector3 const *pKeys, TVector3 *pValues, int iNumKeys)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    int iNextKey = NextKey(CurKey());
    TVector3 const *pKeysA = pKeys + iNumKeys * CurKey();
    TVector3 const *pKeysB = pKeys + iNumKeys * iNextKey;
    float fRA = (m_fTime - m_pTimes[m_iKey]) / (m_pTimes[iNextKey] - m_pTimes[m_iKey]);
    for (int i = 0; i < iNumKeys; i++)
    {
      pValues->x = pKeysA->x + (pKeysB->x - pKeysA->x) * fRA;
      pValues->y = pKeysA->y + (pKeysB->y - pKeysA->y) * fRA;
      pValues->z = pKeysA->z + (pKeysB->z - pKeysA->z) * fRA;
      pKeysA++;
      pKeysB++;
      pValues++;
    }
  }
  else memcpy(pValues, pKeys + m_iNumKeys * CurKey(), iNumKeys*sizeof(TVector3));
}


//---------------------------------------------------------------------------//
// GetKeyValues
//
//---------------------------------------------------------------------------//
void CPathLinear::GetKeyValues(TVector2 const *pKeys, TVector2 *pValues, int iNumKeys)
{
  if (m_iNumKeys > 1 && (m_fTimeEnd > m_fTimeIni))
  {
    int iNextKey = NextKey(CurKey());
    TVector2 const *pKeysA = pKeys + iNumKeys * CurKey();
    TVector2 const *pKeysB = pKeys + iNumKeys * iNextKey;
    float fRA = (m_fTime - m_pTimes[m_iKey]) / (m_pTimes[iNextKey] - m_pTimes[m_iKey]);
    for (int i = 0; i < iNumKeys; i++)
    {
      pValues->x = pKeysA->x + (pKeysB->x - pKeysA->x) * fRA;
      pValues->y = pKeysA->y + (pKeysB->y - pKeysA->y) * fRA;
      pKeysA++;
      pKeysB++;
      pValues++;
    }
  }
  else memcpy(pValues, pKeys + m_iNumKeys * CurKey(), iNumKeys*sizeof(TVector2));
}


//---------------------------------------------------------------------------//
//
//
//---------------------------------------------------------------------------//
void CPathLinear::SearchKeys()
{
  ASSERT (m_fTime >= m_fTimeIni && m_fTime <= m_fTimeEnd);
  m_iKey = 0;
  int  iNextKey = NextKey(m_iKey);
  bool bFound   = false;
  do
  {
    if (m_fTime >= m_pTimes[m_iKey] && m_fTime <= m_pTimes[iNextKey])
      bFound = true;
    else
    {
      m_iKey   = iNextKey;
      iNextKey = NextKey(m_iKey);
    }
  } while (!bFound);
}
