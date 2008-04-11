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

#ifndef _MGEFFECT_H
#define _MGEFFECT_H

#include "MGControl.h"
#include "MGGridPanel.h"

class CMGPanel;
class CMGGroupBox;
class CMGPageControl;
class CMGPage;
class CMGGridItemFX;
class CMGLabel;
class CMGButton;
class CMGTrackBar;
class CMGCheckBox;
class CMGSpinBox;
class CMGComboBox;
class CMGPanelVars;
class CMGFXFilters;
class CVJFXVars;

class CMGEffect : public CMGControl
{
  public:

    static CMGEffect      *Create             (const string &sFile);
    static CMGEffect      *Create             (CNodeFile::CNode *pNode);
  
  public:
     
                           CMGEffect          ();
    virtual               ~CMGEffect          ();

            bool           Load               (const string &sFile);
            bool           Load               (CNodeFile::CNode *pNode);
            void           Save               (const string &sFile);
            void           Save               (CNodeFile *pNodeFile);
            void           SaveAs             ();
            void           Unload             ();
            void           Reset              ();

            void           Update             ();

            void           Attach             (int Layer);
            void           Unattach           ();
            void           Move               (int Layer);
            bool           IsAttached         () const { return m_Attached; }
            int            GetLayerAttached   () const { return m_LayerAttached; }

            void           SetBlend           (int iBlend);

            const string  &GetFXName          () const { return m_FXName; }
            int            GetID              () const { return m_ID; }
            TMGSurface    *GetBitmap          ()       { return m_Bitmap; }
            CMGFXFilters  *GetFXFilters       ()       { return m_FXFilters; }

  private:                                 

            void           LoadSnapshot       (CVJEffect::TVJSnapshot *pSnapshot, TMGSurface *pSurface);
            void           CreateControls     ();
            void           LoadVars           (CNodeFile::CNode *pNode);
            void           LoadFilters        (CNodeFile::CNode *pNode);

            void           SaveVars           (CNodeFile *pFile);
            void           SaveFilters        (CNodeFile *pFile);

            void           OnBlendAlpha       ();
            void           OnBlendAdd         ();
            void           OnBlendSub         ();
            void           OnBlendInv         ();
            void           OnBlendInvDest     ();
            void           OnBlendMask        ();
            void           OnBlendMul         ();
            void           OnBlendInvMul      ();
            void           OnBlendColMul      ();
            void           TBSpeedOnChange    ();
            void           BtnLinkSpeedOnClick();
            void           BtnSpeedx1OnClick  ();
            float          SpeedToTab         (float fSpeed);
            float          TabToSpeed         (float fSpeed);
            void           CBAdjustOnChange   ();
            void           SBAdjustOnChange   ();
            void           TBAlphaOnChange    ();
            void           BAlphaOnClick      ();
            void           CBScopeChange      ();
            void           CBObjectChange     ();
            void           CBQualityChange    ();
            void           SBFadeChange       ();
            void           BtnSaveOnClick     ();
                                          
  private:            
                      
    int                    m_ID;
    bool                   m_Attached;
    int                    m_LayerAttached;
    string                 m_FXClass;
    string                 m_FXFile;
    string                 m_FXDir;
    string                 m_FXName;
    string                 m_FileName;
    TMGSurface            *m_Bitmap;
    CMGPanel              *m_PanelTitle;
    CMGGroupBox           *m_GBVars;
    CMGPageControl        *m_PCVars;
    CMGPage               *m_PageVars;
    CMGPage               *m_PageObjects;
    CMGPage               *m_PageFilters;
    CMGPage               *m_PageOptions;
                        
    // Speed            
    CMGLabel              *m_LSpeed;
    CMGTrackBar           *m_TBSpeed;
    CMGButton             *m_BSpeed;
    CMGButton             *m_BSpeedx1;
    CMGCheckBox           *m_CBAdjustToBPM;
    CMGSpinBox            *m_SBAdjustToBPM;
    // Alpha
    CMGLabel              *m_LAlpha;
    CMGTrackBar           *m_TBAlpha;
    CMGButton             *m_BAlpha;
    // Blend
    int                    m_Blend;
    CMGButton             *m_BlendButtons[9];
    // Objects
    CMGComboBox           *m_CBScopes;
    CMGComboBox           *m_CBObjects;
    // Quality
    CMGComboBox           *m_CBQuality;
    CMGSpinBox            *m_SBFade;

    // Filter panel
    CMGFXFilters          *m_FXFilters;

    // Vars
    CMGPanelVars                     *m_VJFXVars;
    vector< vector<CMGPanelVars *> >  m_VJScopeVars;
    CMGPanelVars                     *m_ActiveScopePanel;
    CMGPanel                         *m_ScopePanel;
};

#endif

