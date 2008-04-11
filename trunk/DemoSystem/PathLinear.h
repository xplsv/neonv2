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

#ifndef PATHLINEAR_H
#define PATHLINEAR_H

#include "Path.h"

class CPathLinear : public CPath
{
  public:
    virtual          ~CPathLinear () { End(); }

    virtual bool      Init        (int iType, int iNumKeys);
    virtual void      End         ();
    
    virtual void      AddKey      (float fTime);
    virtual void      AddKey      (float fTime, float           Key);
    virtual void      AddKey      (float fTime, TVector2 const &Key);
    virtual void      AddKey      (float fTime, TVector3 const &Key);
    virtual void      AddKey      (float fTime, TVector4 const &Key);
    virtual void      Initialize  () { }

    virtual void      SetTime     (float fTime);
    virtual float     GetTime     () const { return m_fTime; }

    virtual void      Run         (float fRunTime);

    virtual int       GetNumKeys  () const { return m_iNumKeys; }
    virtual int       GetMaxKeys  () const { return m_iMaxKeys; }

    // Get interpolated keys for the computed frame
    virtual void      GetKeyValue (float    &Value);
    virtual void      GetKeyValue (TVector2 &Value);
    virtual void      GetKeyValue (TVector3 &Value);
    virtual void      GetKeyValue (TVector4 &Value);
    virtual void      GetKeyValues(TVector2 const *pKeys, TVector2 *pValues, int iNumKeys);
    virtual void      GetKeyValues(TVector3 const *pKeys, TVector3 *pValues, int iNumKeys);

  private:

    // GetKeys
    int               CurKey      ()         const { return m_iKey; }
    int               NextKey     (int iKey) const { return ((m_iKey < (m_iMaxKeys-1)) ? (m_iKey+1) : 0); }
    int               PrevKey     (int iKey) const { return ((m_iKey > 0) ? (m_iKey-1) : (m_iMaxKeys-1)); }

    // Get constant keys
    virtual float    *GetKey1     (int iKey) { ASSERT(iKey>=0&&iKey<m_iNumKeys); return &m_pKeys1[iKey]; }
    virtual TVector2 *GetKey2     (int iKey) { ASSERT(iKey>=0&&iKey<m_iNumKeys); return &m_pKeys2[iKey]; }
    virtual TVector3 *GetKey3     (int iKey) { ASSERT(iKey>=0&&iKey<m_iNumKeys); return &m_pKeys3[iKey]; }
    virtual TVector4 *GetKey4     (int iKey) { ASSERT(iKey>=0&&iKey<m_iNumKeys); return &m_pKeys4[iKey]; }

    void              BorraVars   ();
    void              LiberaVars  ();
    void              SearchKeys  ();

  private:

    // Keys
    int         m_iKey;
    float      *m_pKeys1;
    TVector2   *m_pKeys2;
    TVector3   *m_pKeys3;
    TVector4   *m_pKeys4;
    float      *m_pTimes;
};

#endif
