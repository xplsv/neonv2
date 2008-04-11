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

#ifndef _MGVARLINKER_H
#define _MGVARLINKER_H

#include "MGControl.h"

class CMGButton;
class CMGListBox;
class CMGComboBox;
class CMGCheckBox;
class CMGHotKey;
class CMGSpinBox;

class CMGWindowVarLinker : public CMGWindow
{
  public:
     
                      CMGWindowVarLinker   ();
    virtual          ~CMGWindowVarLinker   ();

    virtual void      Show                 (CLink **ppToLink, int iX, int iY, int iBehType, bool bOnlyMidiKey, CMGControl *pRControl, CMGButton *pRButton, function<void (EModalResult eModalResult)> fOnModalResult);
    virtual void      Close                (EModalResult eModalResult);

  private:

            void      Reset                ();
            void      LoadLink             (CLink *pLink);
            CLink    *CreateLink           ();

            void      LoadPresets          ();
            void      SavePreset           ();
            void      LoadPreset           (const string &sPreset);

            void      CreateDeviceCtrls    ();
            void      CreateActionCtrls    ();
            void      SetDevice            (CMGGroupBox *pDevice);
            void      SetAction            (CMGGroupBox *pAction);
            void      ResetDeviceData      ();
            void      ResetActionData      ();

            // Callbacks
            void      OnClickAccept        ();
            void      OnClickCancel        ();

            void      OnChangeDevice       ();
            void      OnChangePreset       ();
            void      OnDeletePreset       ();
            const string &GetPresetExt     () const;

            // CheckBox
            void      OnChange_CHA_Switch  ();
            void      OnChange_CHA_DevVal  ();
            void      OnChange_CHA_InvDev  ();
            // ComboBox
            void      OnChange_CBA_Next    ();
            void      OnChange_CBA_Prev    ();
            void      OnChange_CBA_Rand    ();
            void      OnChange_CBA_Set     ();
            // TrackBar
            void      OnChange_TBA_DevVal  ();
            void      OnChange_TBA_InvDev  ();
            void      OnChange_TBA_Set     ();
            void      OnChange_TBA_Add     ();
            void      OnChange_TBA_Rand    ();
            void      OnChange_TBA_Fade    ();
            void      OnChange_TBA_SwapFade();

  private:

    int               m_BehType;
    CLink           **m_ToLink;
    // Related to
    CMGControl       *m_RControl;
    CMGButton        *m_RButton;
    // Controls
    CMGListBox       *m_LBDevices;
    CMGComboBox      *m_CBPresets;
    CMGButton        *m_BtnSavePreset;
    CMGButton        *m_BtnDelPreset;
    // Current Device
    CMGGroupBox      *m_CurrentDevice;
    // Keyboard Device
    CMGGroupBox      *m_KD_Panel;
    CMGHotKey        *m_KD_HotKey;
    // Beat Device
    CMGGroupBox      *m_BD_Panel;
    CMGSpinBox       *m_BD_Beat;
    CMGSpinBox       *m_BD_Off;
    // Wave Device
    CMGGroupBox      *m_WD_Panel;
    CMGComboBox      *m_WD_Chan;
    CMGSpinBox       *m_WD_Gain;
    // Current Action
    CMGGroupBox      *m_CurrentAction;
    // CheckBox Action
    CMGGroupBox      *m_PanelActionsCheckBox;
    CMGCheckBox      *m_CHA_Switch;
    CMGCheckBox      *m_CHA_DevVal;
    CMGCheckBox      *m_CHA_InvDev;
    // ComboBox Action
    CMGGroupBox      *m_PanelActionsComboBox;
    CMGCheckBox      *m_CBA_Next;
    CMGCheckBox      *m_CBA_Prev;
    CMGCheckBox      *m_CBA_Rand;
    CMGCheckBox      *m_CBA_Set;
    CMGSpinBox       *m_CBA_SetValue;
    // TrackBar Action (button + slider)
    CMGGroupBox      *m_PanelActionsTrackBarA;
    CMGGroupBox      *m_PanelActionsTrackBarF;
    CMGCheckBox      *m_TBA_DevVal;
    CMGCheckBox      *m_TBA_InvDev;
    CMGCheckBox      *m_TBA_Set;
    CMGCheckBox      *m_TBA_Add;
    CMGCheckBox      *m_TBA_Rand;
    CMGCheckBox      *m_TBA_Fade;
    CMGSpinBox       *m_TBA_SetValue;
    CMGSpinBox       *m_TBA_AddValue;
    CMGSpinBox       *m_TBA_FadeFr;
    CMGSpinBox       *m_TBA_FadeTo;
    CMGSpinBox       *m_TBA_FadeLength;
};

#endif
