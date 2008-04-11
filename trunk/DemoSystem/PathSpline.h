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

#ifndef PATHSPLINE_H
#define PATHSPLINE_H

#include "Path.h"

struct TKeySpline
{
  TVector3 pos;
  TVector3 ds;
  TVector3 dd;

  float    tens;
  float    cont;
  float    bias;
  float    easeFrom;
  float    easeTo;
  float    fTime;
};

class CPathSpline : public CPath
{
  public:
    virtual          ~CPathSpline   () { End(); }

    virtual bool      Init          (int iType, int iNumKeys);
    virtual void      End           ();
    
    virtual void      AddKey        (float fTime)                      { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      AddKey        (float fTime, float           Key) { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      AddKey        (float fTime, TVector2 const &Key) { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      AddKey        (float fTime, TVector3 const &Key);
    virtual void      AddKey        (float fTime, TVector4 const &Key) { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      Initialize    ();

    virtual void      SetTime       (float fTime);
    virtual float     GetTime       () const { return m_fTime; }

    virtual void      Run           (float fRunTime);

    virtual int       GetNumKeys    () const { return m_iNumKeys; }
    virtual int       GetMaxKeys    () const { return m_iMaxKeys; }

    // Get interpolated keys for the computed frame
    virtual void      GetKeyValue   (float    &Value) { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      GetKeyValue   (TVector2 &Value) { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      GetKeyValue   (TVector3 &Value);
    virtual void      GetKeyValue   (TVector4 &Value) { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      GetKeyValues  (TVector2 const *pKeys, TVector2 *pValues, int iNumKeys) { ASSERTM(false, "CPathSpline: Function not supported"); }
    virtual void      GetKeyValues  (TVector3 const *pKeys, TVector3 *pValues, int iNumKeys) { ASSERTM(false, "CPathSpline: Function not supported"); }

  private:

    // GetKeys
    int               CurKey        ()         const { return m_iKey; }
    int               NextKey       (int iKey) const { return ((m_iKey < (m_iMaxKeys-1)) ? (m_iKey+1) : 0); }
    int               PrevKey       (int iKey) const { return ((m_iKey > 0) ? (m_iKey-1) : (m_iMaxKeys-1)); }

    void              CompDeriv     (TKeySpline *pKeyP,  TKeySpline *pKey,  TKeySpline *pKeyN );
    void              CompDerivFirst(TKeySpline *pKey,   TKeySpline *pKeyN, TKeySpline *pKeyNN);
    void              CompDerivLast (TKeySpline *pKeyPP, TKeySpline *pKeyP, TKeySpline *pKey  );
    void              CompDerivTwo  (TKeySpline *pKeyA,  TKeySpline *pKeyB);
    float             Ease          (float fT, float fA, float fB);

    void              BorraVars     ();
    void              LiberaVars    ();
    void              SearchKeys    ();

  private:

    int          m_iKey;
    TKeySpline  *m_pKeys;
};

#endif
