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

#ifndef FILTERBLUR_H
#define FILTERBLUR_H

#include "Filter.h"

class CFilterBlur : public CFilter
{
  public:
    virtual              ~CFilterBlur    () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          SetTime        (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, int iTexSrc, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:
    int                   m_iType;
    bool                  m_bAnimacion;
    int                   m_iState;
    int                   m_iSteps;
    float                 m_fRadio;
    float                 m_fIntens;
    float                 m_fIntensAct;
    float                 m_fAnimSpeed;
    TVector2              m_fRangoAnimacion;
    TVector2              m_PosCentro;
    TVector2              m_Pos;
};

#endif
