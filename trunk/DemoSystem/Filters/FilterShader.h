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

#ifndef FILTERSHADER_H
#define FILTERSHADER_H

#include "Filter.h"

struct TCustomVar;

class CFilterShader : public CFilter
{
  public:
    virtual              ~CFilterShader  () { End(); }

    virtual bool          Init           (COptions &aOptions);
    virtual void          End            ();

    virtual void          SetTime        (float fTime);
    virtual bool          Apply          (CDisplayDevice *pDD, int iTexSrc, int iTexTgt);

    virtual TCtrlVar     *GetVarCtrls    ();
    virtual void          SetVar         (int iVar, void *pData);
    virtual void         *GetVar         (int iVar);

  private:

    void                  Draw           (CDisplayDevice *pDD, TVector2 const &uv0, TVector2 const &uv1);
    bool                  InitCtrlVar    (int iVar, const string &sVar);

  protected:

    bool                  m_bEnabled;
    int                   m_iShader;
    int                   m_pTexturas[3];
    int                   m_iNumVars;
    TCustomVar           *m_pVars;
    TCtrlVar             *m_pCtrlVars;
};

#endif
