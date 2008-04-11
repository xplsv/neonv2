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

#ifndef PATH_H
#define PATH_H

#include "GEVector.h"

class CPath
{
  public:
    enum
    {
      PATH_TIME,
      PATH_1D,
      PATH_2D,
      PATH_3D,
      PATH_4D,
    };
                      CPath       () { m_bOk = false; }
    virtual          ~CPath       () { End(); }

    virtual bool      Init        (int iType, int iNumKeys);
    virtual void      End         ();
    virtual bool      IsOk        () const { return m_bOk; }
    
    virtual void      AddKey      (float fTime) = 0;
    virtual void      AddKey      (float fTime, float           Key) = 0;
    virtual void      AddKey      (float fTime, TVector2 const &Key) = 0;
    virtual void      AddKey      (float fTime, TVector3 const &Key) = 0;
    virtual void      AddKey      (float fTime, TVector4 const &Key) = 0;
    virtual void      Initialize  () = 0;

    virtual void      SetTime     (float fTime) = 0;
    virtual float     GetTime     () const { return m_fTime; }

    virtual void      Run         (float fRunTime) = 0;

    virtual int       GetNumKeys  () const { return m_iNumKeys; }
    virtual int       GetMaxKeys  () const { return m_iMaxKeys; }

    // Get interpolated keys for the computed frame
    virtual void      GetKeyValue (float    &Value) = 0;
    virtual void      GetKeyValue (TVector2 &Value) = 0;
    virtual void      GetKeyValue (TVector3 &Value) = 0;
    virtual void      GetKeyValue (TVector4 &Value) = 0;
    virtual void      GetKeyValues(TVector2 const *pKeys, TVector2 *pValues, int iNumKeys) = 0;
    virtual void      GetKeyValues(TVector3 const *pKeys, TVector3 *pValues, int iNumKeys) = 0;

  protected:

    bool        m_bOk;
    int         m_iType;
    int         m_iMaxKeys;
    int         m_iNumKeys;
    float       m_fTime;
    float       m_fTimeIni;
    float       m_fTimeEnd;
};

#endif
