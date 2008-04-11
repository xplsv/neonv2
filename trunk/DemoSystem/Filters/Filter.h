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

#ifndef _FILTER_H
#define _FILTER_H

class  CDisplayDevice;
class  COptions;
class  CFilter;
struct TCtrlVar;

class CFilter
{
  public:
    static  CFilter      *Create          (const string &sFile);
                          CFilter         () { m_Ok = false; }
    virtual              ~CFilter         () { End(); }

    virtual bool          Init            (COptions &aOptions) = 0;
    virtual void          End             ()       { m_Ok = false; }
            bool          IsOk            () const { return m_Ok; }

    virtual void          SetTime         (float fTime);
    virtual bool          Apply           (CDisplayDevice *pDD, int iTexSrc, int iTexTgt) = 0;

    virtual TCtrlVar     *GetVarCtrls     ();
    virtual void          SetVar          (int iVar, void *pData) { }
    virtual void         *GetVar          (int iVar) { return NULL; }

  protected:           

    bool                  m_Ok;
    float                 m_fTime;
    float                 m_fTimeAnt;
    float                 m_fTimeFrame;
};

#endif
