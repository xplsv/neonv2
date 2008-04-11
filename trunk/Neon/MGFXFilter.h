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

#ifndef _MGFXFILTER_H
#define _MGFXFILTER_H

#include "MGControl.h"

class CMGEffect;
class CMGFXFilters;
class CMGPanelVars;

class CMGFXFilter  : public CMGControl
{
  public:
     
                      CMGFXFilter   (CMGControl *pParent, CMGFXFilters *pFXFilters, CMGEffect *pEffect, int x, int y, int iSlot);
    virtual          ~CMGFXFilter   ();

    virtual void      OnMouseMove   (int iX, int iY);
    virtual void      OnMouseDown   (int iX, int iY, int iButton);
    virtual void      OnMouseUp     (int iX, int iY, int iButton);
    virtual void      OnDragOver    (CMGControl *pSource, int iX, int iY, bool &Accept);
    virtual void      OnDragDrop    (CMGControl *pSource, int iX, int iY);

    virtual bool      IsLoaded      () const { return m_FilterName != ""; }
    virtual bool      Load          (const string &sFilterName);
    virtual void      Unload        ();
    virtual void      Show          ();

  public:

    const string     &GetFilterName () const { return m_FilterName; }
    void              Update        ();
    void              Load          (CNodeFile::CNode *pNode);
    void              Save          (CNodeFile        *pFile);
    void              Swap          (CMGFXFilter      *pOther);

  protected:

    virtual void      DerivDraw     ();

  protected:

    int               m_FilterSlot;
    int               m_FilterIdx;
    string            m_FilterName;
    CMGEffect        *m_Effect;
    CMGFXFilters     *m_FXFilters;
    CMGPanelVars     *m_VJFilterVars;
};

#endif
