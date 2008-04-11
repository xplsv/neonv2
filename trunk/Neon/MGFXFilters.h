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

#ifndef _MGFXFILTERS_H
#define _MGFXFILTERS_H

#include "MGControl.h"

class CMGEffect;
class CMGPanel;
class CMGListBox;
class CMGFXFilter;

class CMGFXFilters : public CMGControl
{
  public:
     
                            CMGFXFilters  (CMGControl *pParent, CMGEffect *pEffect);
    virtual                ~CMGFXFilters  ();

    CMGListBox             *GetLBFilters  ()      { return m_LBFilters; }
    CMGFXFilter            *GetFXFilter   (int i) { return m_FXFilter[i]; }
    CMGControl             *GetFilterVars ()      { return m_FilterVars; }
    void                    ShowFilterVars(CMGControl *pFXFilter);

    void                    Update        ();
    void                    Load          (CNodeFile::CNode *pNode);
    void                    Save          (CNodeFile        *pFile);
    void                    SwapFilters   (int iSlotSrc, int iSlotDst);

  protected:

    virtual void            OnSelectFilter(CMGListBox *pListBox);

  protected:

    enum
    {
      MAX_FILTERS = 5,
    };
    // Vars
    CMGPanel               *m_PanelSup;
    CMGPanel               *m_PanelInf;
    CMGListBox             *m_LBFilters;

    // All FilterVars
    CMGFXFilter            *m_FXFilter[MAX_FILTERS];

    // Active FilterVars
    CMGControl             *m_FilterVars;
};

#endif

