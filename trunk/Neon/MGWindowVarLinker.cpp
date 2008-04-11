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

#include "NeonPch.h"
#include "MGAppMain.h"
#include "MGWindowVarLinker.h"
#include "MGButton.h"
#include "MGListBox.h"
#include "MGEditBox.h"
#include "MGComboBox.h"
#include "MGGroupBox.h"
#include "MGSpinBox.h"
#include "MGCheckBox.h"
#include "MGTrackBar.h"
#include "MGPanel.h"
#include "MGHotKey.h"
#include "MGLabel.h"
#include "Link.h"
#include "LinkBehaviour.h"
#include "LinkBehaviour_CheckBox.h"
#include "LinkBehaviour_ComboBox.h"
#include "LinkBehaviour_TrackBar.h"
#include "LinkDevice_Keyboard.h"
#include "LinkDevice_Beat.h"
#include "LinkDevice_Wave.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGWindowVarLinker::CMGWindowVarLinker() : CMGWindow(500,232, "Var Linker")
{
  // Accept/Cancel changes
  NEW CMGButton(this, 400,170, CMGButton::NORMAL, "Accept", bind(&CMGWindowVarLinker::OnClickAccept,this));
  NEW CMGButton(this, 400,200, CMGButton::NORMAL, "Cancel", bind(&CMGWindowVarLinker::OnClickCancel,this));

  // Presets
  m_CBPresets     = NEW CMGComboBox(this, 380,21, 116,100, true, bind(&CMGWindowVarLinker::OnChangePreset,this));
  m_BtnSavePreset = NEW CMGButton(this, 380,46, CMGButton::BLEND, "Save", bind(&CMGWindowVarLinker::SavePreset,this));
  m_BtnDelPreset  = NEW CMGButton(this, 457,46, CMGButton::BLEND, "Del",  bind(&CMGWindowVarLinker::OnDeletePreset,this));

  // Create device and action controls
  CreateDeviceCtrls();
  CreateActionCtrls();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGWindowVarLinker::~CMGWindowVarLinker()
{
  // Dispose All
  DISPOSE(m_KD_Panel);
  DISPOSE(m_BD_Panel);
  DISPOSE(m_WD_Panel);
  DISPOSE(m_PanelActionsCheckBox);
  DISPOSE(m_PanelActionsComboBox);
  DISPOSE(m_PanelActionsTrackBarA);
  DISPOSE(m_PanelActionsTrackBarF);
  CMGWindow::~CMGWindow();
}


//---------------------------------------------------------------------------//
// CreateDeviceCtrls
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::CreateDeviceCtrls()
{
  // Devices list
  m_LBDevices = NEW CMGListBox(this, 2,21, 116,209, MGALIGN_NONE, false);
  m_LBDevices->Clear();
  m_LBDevices->m_OnSelectItem = bind(&CMGWindowVarLinker::OnChangeDevice,this);
  // Current device
  m_CurrentDevice = NULL;
  // Keyboard Device
  m_KD_Panel  = NEW CMGGroupBox(NULL, 120,20, 244, 56);
                NEW CMGLabel   (m_KD_Panel,  8,20, MGALIGN_NONE, FONT_NORMAL, "HotKey");
  m_KD_HotKey = NEW CMGHotKey  (m_KD_Panel, 55,18, 70, NULL);
  // Beat Device
  m_BD_Panel  = NEW CMGGroupBox(NULL, 120,20, 244, 56);
                NEW CMGLabel   (m_BD_Panel,  8,20, MGALIGN_NONE, FONT_NORMAL, "Beat");
  m_BD_Beat   = NEW CMGSpinBox (m_BD_Panel, 40,18, 40, 1, 99, 1, NULL);
                NEW CMGLabel   (m_BD_Panel,108,20, MGALIGN_NONE, FONT_NORMAL, "Off");
  m_BD_Off    = NEW CMGSpinBox (m_BD_Panel,140,18, 40, 0, 99, 1, NULL);
  // Wave Device
  m_WD_Panel  = NEW CMGGroupBox(NULL, 120,20, 244, 56);
                NEW CMGLabel   (m_WD_Panel,  8,20, MGALIGN_NONE, FONT_NORMAL, "Chan");
  m_WD_Chan   = NEW CMGComboBox(m_WD_Panel, 40,18, 60, 100, false, NULL);
  m_WD_Chan->Add("Global");
  m_WD_Chan->Add("1");
  m_WD_Chan->Add("2");
  m_WD_Chan->Add("3");
  m_WD_Chan->Add("4");
  m_WD_Chan->Add("5");
                NEW CMGLabel   (m_WD_Panel,108,20, MGALIGN_NONE, FONT_NORMAL, "Gain");
  m_WD_Gain   = NEW CMGSpinBox (m_WD_Panel,140,18, 40, 0, 1000, 10, NULL);
}


//---------------------------------------------------------------------------//
// CreateActionCtrls
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::CreateActionCtrls()
{
  m_CurrentAction = NULL;
  // CheckBox Action
  m_PanelActionsCheckBox = NEW CMGGroupBox(NULL, 120,77, 244, 154);
  m_CHA_Switch           = NEW CMGCheckBox(m_PanelActionsCheckBox,  8, 18, "Switch",               true, bind(&CMGWindowVarLinker::OnChange_CHA_Switch,this));
  m_CHA_DevVal           = NEW CMGCheckBox(m_PanelActionsCheckBox,  8, 48, "Device Value",         true, bind(&CMGWindowVarLinker::OnChange_CHA_DevVal,this));
  m_CHA_InvDev           = NEW CMGCheckBox(m_PanelActionsCheckBox,  8, 78, "Inverse Device Value", true, bind(&CMGWindowVarLinker::OnChange_CHA_InvDev,this));
  // ComboBox Action
  m_PanelActionsComboBox = NEW CMGGroupBox(NULL, 120,77, 244, 154);
  m_CBA_Next             = NEW CMGCheckBox(m_PanelActionsComboBox,  8, 18, "Next",                 true, bind(&CMGWindowVarLinker::OnChange_CBA_Next,this));
  m_CBA_Prev             = NEW CMGCheckBox(m_PanelActionsComboBox,  8, 48, "Previous",             true, bind(&CMGWindowVarLinker::OnChange_CBA_Prev,this));
  m_CBA_Rand             = NEW CMGCheckBox(m_PanelActionsComboBox,  8, 78, "Random",               true, bind(&CMGWindowVarLinker::OnChange_CBA_Rand,this));
  m_CBA_Set              = NEW CMGCheckBox(m_PanelActionsComboBox,  8,108, "Set Item",             true, bind(&CMGWindowVarLinker::OnChange_CBA_Set, this));
  m_CBA_SetValue         = NEW CMGSpinBox (m_PanelActionsComboBox, 95,108, 30, 0,1, 1, NULL);
  // TrackBar Action (Slider)
  m_PanelActionsTrackBarF= NEW CMGGroupBox(NULL, 120,77, 244, 154);
  m_TBA_DevVal           = NEW CMGCheckBox(m_PanelActionsTrackBarF, 8, 48, "Device Value",         true, bind(&CMGWindowVarLinker::OnChange_TBA_DevVal,this));
  m_TBA_InvDev           = NEW CMGCheckBox(m_PanelActionsTrackBarF, 8, 78, "Inverse Device Value", true, bind(&CMGWindowVarLinker::OnChange_TBA_InvDev,this));
  // TrackBar Action (Button)
  m_PanelActionsTrackBarA= NEW CMGGroupBox(NULL, 120,77, 244, 154);
  m_TBA_Set              = NEW CMGCheckBox(m_PanelActionsTrackBarA,  8,  8, "Set Value",            true, bind(&CMGWindowVarLinker::OnChange_TBA_Set,   this));
  m_TBA_Add              = NEW CMGCheckBox(m_PanelActionsTrackBarA,  8, 38, "Add Value",            true, bind(&CMGWindowVarLinker::OnChange_TBA_Add,   this));
  m_TBA_Rand             = NEW CMGCheckBox(m_PanelActionsTrackBarA,  8, 68, "Rand",                 true, bind(&CMGWindowVarLinker::OnChange_TBA_Rand,  this));
  m_TBA_Fade             = NEW CMGCheckBox(m_PanelActionsTrackBarA,  8, 98, "Fade",                 true, bind(&CMGWindowVarLinker::OnChange_TBA_Fade,  this));
  m_TBA_SetValue         = NEW CMGSpinBox (m_PanelActionsTrackBarA, 82,  8, 40,   0, 100,  5, NULL);
  m_TBA_AddValue         = NEW CMGSpinBox (m_PanelActionsTrackBarA, 82, 38, 40,-100, 100,  5, NULL);
  m_TBA_FadeFr           = NEW CMGSpinBox (m_PanelActionsTrackBarA, 65, 98, 40,   0, 100,  5, NULL);
  m_TBA_FadeTo           = NEW CMGSpinBox (m_PanelActionsTrackBarA,135, 98, 40,   0, 100,  5, NULL);
  m_TBA_FadeLength       = NEW CMGSpinBox (m_PanelActionsTrackBarA, 65,128, 50,   0,5000, 50, NULL);
                           NEW CMGLabel   (m_PanelActionsTrackBarA,110,101, MGALIGN_NONE, FONT_NORMAL, "To");
                           NEW CMGLabel   (m_PanelActionsTrackBarA,120,131, MGALIGN_NONE, FONT_NORMAL, "Length (ms)");
                           NEW CMGButton  (m_PanelActionsTrackBarA,180, 98, CMGButton::SMALL, "<>", bind(&CMGWindowVarLinker::OnChange_TBA_SwapFade, this));
}


//---------------------------------------------------------------------------//
// Show
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::Show(CLink **ppToLink, int iX, int iY, int iBehType, bool bOnlyMidiKey, CMGControl *pRControl, CMGButton *pRButton, function<void (EModalResult eModalResult)> fOnModalResult)
{
  m_BehType  = iBehType;
  m_ToLink   = ppToLink;
  m_RControl = pRControl;
  m_RButton  = pRButton;
  m_CBPresets    ->SetVisible(!bOnlyMidiKey);
  m_BtnSavePreset->SetVisible(!bOnlyMidiKey);
  m_BtnDelPreset ->SetVisible(!bOnlyMidiKey);

  // Select devices for this action
  m_LBDevices->Clear();
  switch (m_BehType)
  {
    case CLinkBehaviour::CHECK_BOX:
      m_LBDevices->Add("Keyboard");
      m_LBDevices->Add("Beat");
      //m_LBDevices->Add("MIDI Note");
      //m_LBDevices->Add("MIDI Clock");
      //m_LBDevices->Add("Timer");
      SetAction(m_PanelActionsCheckBox);
      break;
    case CLinkBehaviour::COMBO_BOX:
      m_LBDevices->Add("Keyboard");
      m_LBDevices->Add("Beat");
      //m_LBDevices->Add("MIDI Note");
      //m_LBDevices->Add("MIDI Clock");
      //m_LBDevices->Add("Timer");
      SetAction(m_PanelActionsComboBox);
      break;
    case CLinkBehaviour::TRACK_BAR:
      if (bOnlyMidiKey)
      {
        m_LBDevices->Add("Keyboard");
        //m_LBDevices->Add("MIDI Note");
        //m_LBDevices->Add("MIDI Slider");
      }
      else
      {
        m_LBDevices->Add("Keyboard");
        m_LBDevices->Add("Beat");
        m_LBDevices->Add("Wave FFT");
        //m_LBDevices->Add("MIDI Note");
        //m_LBDevices->Add("MIDI Slider");
        //m_LBDevices->Add("MIDI Clock");
        //m_LBDevices->Add("Timer");
      }
      break;
  }
  m_LBDevices->Select(0);
  m_CBPresets->Select(0);
  LoadLink(*ppToLink);
  LoadPresets();
  // Finally show
  CMGWindow::Show(iX, iY, fOnModalResult);
}


//---------------------------------------------------------------------------//
// Close
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::Close(EModalResult eModalResult)
{
  // Close device
  SetDevice(NULL);
  // Close action
  SetAction(NULL);
  CMGWindow::Close(eModalResult);
}


//---------------------------------------------------------------------------//
// LoadLink
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::LoadLink(CLink *pLink)
{
  if (!pLink) return;
  // Load Device values
  switch (pLink->Device()->Type())
  {
    // Keyboard Device
    case CLinkDevice::KEYBOARD:
      if(m_LBDevices->Select("Keyboard"))
      {
        char c;
        ((CLinkDevice_Keyboard *)(pLink->Device()))->Get(c);
        m_KD_HotKey->Set(c);
      }
      break;
    // Beat Device
    case CLinkDevice::BEAT:
      if (m_LBDevices->Select("Beat"))
      {
        int iBeat, iOff;
        ((CLinkDevice_Beat *)(pLink->Device()))->Get(iBeat, iOff);
        m_BD_Beat->Set(iBeat);
        m_BD_Off ->Set(iOff );
      }
      break;
    // Wave Device
    case CLinkDevice::WAVE:
      if (m_LBDevices->Select("Wave FFT"))
      {
        int   iChan;
        float fGain;
        ((CLinkDevice_Wave *)(pLink->Device()))->Get(iChan, fGain);
        m_WD_Chan->Select(iChan);
        m_WD_Gain->Set   (Floor(fGain * 100));
      }
      break;
  }
  // Load Link values
  switch (pLink->Behaviour()->Type())
  {
    case CLinkBehaviour::CHECK_BOX:
    {
      // CheckBox
      CLinkBehaviour_CheckBox *pLB = (CLinkBehaviour_CheckBox *)pLink->Behaviour();
      switch (pLB->Action())
      {
        case CLinkBehaviour_CheckBox::SWITCH:           m_CHA_Switch->SetChecked(true); break;
        case CLinkBehaviour_CheckBox::DEVICE_VALUE:     m_CHA_DevVal->SetChecked(true); break;
        case CLinkBehaviour_CheckBox::INV_DEVICE_VALUE: m_CHA_InvDev->SetChecked(true); break;
      }
      break;
    }
    case CLinkBehaviour::COMBO_BOX:
    {
      // ComboBox
      CLinkBehaviour_ComboBox *pLB = (CLinkBehaviour_ComboBox *)pLink->Behaviour();
      switch (pLB->Action())
      {
        case CLinkBehaviour_ComboBox::NEXT: m_CBA_Next->SetChecked(true); break;
        case CLinkBehaviour_ComboBox::PREV: m_CBA_Prev->SetChecked(true); break;
        case CLinkBehaviour_ComboBox::RAND: m_CBA_Rand->SetChecked(true); break;
        case CLinkBehaviour_ComboBox::SET:  m_CBA_Set ->SetChecked(true); break;
      }
      m_CBA_SetValue->Set(pLB->GetSetValue());
      break;
    }
    case CLinkBehaviour::TRACK_BAR:
    {
      // TrackBar
      CLinkBehaviour_TrackBar *pLB = (CLinkBehaviour_TrackBar *)pLink->Behaviour();
      switch (pLB->Action())
      {
        case CLinkBehaviour_TrackBar::DEVICE_VALUE:     m_TBA_DevVal->SetChecked(true); break;
        case CLinkBehaviour_TrackBar::INV_DEVICE_VALUE: m_TBA_InvDev->SetChecked(true); break;
        case CLinkBehaviour_TrackBar::SET:              m_TBA_Set   ->SetChecked(true); break;
        case CLinkBehaviour_TrackBar::ADD:              m_TBA_Add   ->SetChecked(true); break;
        case CLinkBehaviour_TrackBar::RAND:             m_TBA_Rand  ->SetChecked(true); break;
        case CLinkBehaviour_TrackBar::FADE:             m_TBA_Fade  ->SetChecked(true); break;
      }
      float fFadeFr, fFadeTo, fFadeLength, fSetValue, fAddValue;
      pLB->GetFadeValues(fFadeFr, fFadeTo, fFadeLength);
      pLB->GetSetValues (fSetValue);
      pLB->GetAddValues (fAddValue);
      m_TBA_SetValue  ->Set((int)(fSetValue   * 100 ));
      m_TBA_AddValue  ->Set((int)(fAddValue   * 100 ));
      m_TBA_FadeFr    ->Set((int)(fFadeFr     * 100 ));
      m_TBA_FadeTo    ->Set((int)(fFadeTo     * 100 ));
      m_TBA_FadeLength->Set((int)(fFadeLength * 1000));
      break;
    }
  }
}


//---------------------------------------------------------------------------//
// CreateLink
//
//---------------------------------------------------------------------------//  
CLink *CMGWindowVarLinker::CreateLink()
{
  // Select device
  CLinkDevice *pDevice = NULL;
  if (m_CurrentDevice == m_KD_Panel)
  {
    // Keyboard Device
    CLinkDevice_Keyboard *pLD = NEW CLinkDevice_Keyboard();
    char c;
    m_KD_HotKey->Get(c);
    pLD->Set(c);
    pDevice = pLD;
  }
  else if (m_CurrentDevice == m_BD_Panel)
  {
    // Beat Device
    CLinkDevice_Beat *pLD = NEW CLinkDevice_Beat();
    pLD->Set(m_BD_Beat->GetValue(), m_BD_Off->GetValue());
    pDevice = pLD;
  }
  else if (m_CurrentDevice == m_WD_Panel)
  {
    // Wave Device
    CLinkDevice_Wave *pLD = NEW CLinkDevice_Wave();
    pLD->Set(m_WD_Chan->GetSelected(), m_WD_Gain->GetValue() * 1.f / 100.f);
    pDevice = pLD;
  }
  // Select action
  CLinkBehaviour *pBehaviour = NULL;
  if (m_CurrentAction == m_PanelActionsCheckBox)
  {
    // CheckBox Action
    int eAction = -1;
    if (m_CHA_Switch->Checked()) eAction = CLinkBehaviour_CheckBox::SWITCH;
    if (m_CHA_DevVal->Checked()) eAction = CLinkBehaviour_CheckBox::DEVICE_VALUE;
    if (m_CHA_InvDev->Checked()) eAction = CLinkBehaviour_CheckBox::INV_DEVICE_VALUE;
    CLinkBehaviour_CheckBox *pLB = NEW CLinkBehaviour_CheckBox(eAction, m_RControl ? ((CMGCheckBox *)m_RControl)->Checked() : false);
    pBehaviour = pLB;
  }
  else if (m_CurrentAction == m_PanelActionsComboBox)
  {
    // ComboBox Action
    int eAction = -1;
    if (m_CBA_Next->Checked()) eAction = CLinkBehaviour_ComboBox::NEXT;
    if (m_CBA_Prev->Checked()) eAction = CLinkBehaviour_ComboBox::PREV;
    if (m_CBA_Rand->Checked()) eAction = CLinkBehaviour_ComboBox::RAND;
    if (m_CBA_Set ->Checked()) eAction = CLinkBehaviour_ComboBox::SET;
    CLinkBehaviour_ComboBox *pLB = NEW CLinkBehaviour_ComboBox(eAction, m_RControl ? ((CMGComboBox *)m_RControl)->GetSelected() : 0, m_RControl ? ((CMGComboBox *)m_RControl)->GetNumItems() : 0, m_CBA_SetValue->GetValue());
    pBehaviour = pLB;
  }
  else if (m_CurrentAction == m_PanelActionsTrackBarA)
  {
    int eAction = -1;
    // TrackBar Action (Button)
    if (m_TBA_Set ->Checked()) eAction = CLinkBehaviour_TrackBar::SET;
    if (m_TBA_Add ->Checked()) eAction = CLinkBehaviour_TrackBar::ADD;
    if (m_TBA_Fade->Checked()) eAction = CLinkBehaviour_TrackBar::FADE;
    if (m_TBA_Rand->Checked()) eAction = CLinkBehaviour_TrackBar::RAND;
    CLinkBehaviour_TrackBar *pLB = NEW CLinkBehaviour_TrackBar(eAction, ((CMGTrackBar *)m_RControl)->GetPos());      
    pLB->SetSetValues (m_TBA_SetValue->GetValue() * 1.f / 100.f);
    pLB->SetAddValues (m_TBA_AddValue->GetValue() * 1.f / 100.f);
    pLB->SetFadeValues(m_TBA_FadeFr->GetValue() * 1.f / 100.f, m_TBA_FadeTo->GetValue() * 1.f / 100.f, m_TBA_FadeLength->GetValue() * 1.f / 1000.f);
    pBehaviour = pLB;
  }
  else if (m_CurrentAction == m_PanelActionsTrackBarF)
  {
    // TrackBar Action (Slider)
    int eAction = -1;
    if (m_TBA_DevVal->Checked()) eAction = CLinkBehaviour_TrackBar::DEVICE_VALUE;
    if (m_TBA_InvDev->Checked()) eAction = CLinkBehaviour_TrackBar::INV_DEVICE_VALUE;
    CLinkBehaviour_TrackBar *pLB = NEW CLinkBehaviour_TrackBar(eAction, ((CMGTrackBar *)m_RControl)->GetPos());
    pBehaviour = pLB;
  }
  return NEW CLink(pDevice, pBehaviour);
}


//---------------------------------------------------------------------------//
// LoadPresets
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::LoadPresets()
{
  TFindFileResult Res;
  PushDir(g_DirPresets);
  FindFileInit(Res, "*" + GetPresetExt());
  m_CBPresets->Clear();
  m_CBPresets->Add("");
  while (FindFileNext(Res))
  {
    if (!Res.bDir)
    {
      m_CBPresets->Add(Res.Filename.substr(0,Res.Filename.size()-4));
    }
  }
  m_CBPresets->Select(0);
  FindFileClose(Res);
  PopDir();
}


//---------------------------------------------------------------------------//
// SavePreset
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::SavePreset()
{
  // Save neon project
  string sFile;
  bool bSave = false;
  switch (m_BehType)
  {
    case CLinkBehaviour::CHECK_BOX: bSave = g_pMGApp->OpenFileSave("Save Link Preset", g_DirPresets.c_str(), "CheckBox Link Preset (*.lph)\0*.lph\0", "*.lph", sFile); break;
    case CLinkBehaviour::COMBO_BOX: bSave = g_pMGApp->OpenFileSave("Save Link Preset", g_DirPresets.c_str(), "ComboBox Link Preset (*.lpc)\0*.lpc\0", "*.lpc", sFile); break;
    case CLinkBehaviour::TRACK_BAR: bSave = g_pMGApp->OpenFileSave("Save Link Preset", g_DirPresets.c_str(), "TrackBar Link Preset (*.lpt)\0*.lpt\0", "*.lpt", sFile); break;
  }
  if (bSave)
  {
    CNodeFile NodeFile;
    NodeFile.WriteOpen(sFile);
      CLink *pLink = CreateLink();
      pLink->Save(&NodeFile, "linkpreset");
      DISPOSE(pLink);
    NodeFile.WriteClose();

    // Update presets
    LoadPresets();
  }
}


//---------------------------------------------------------------------------//
// LoadPreset
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::LoadPreset(const string &sPreset)
{
  CNodeFile NodeFile;
  if (NodeFile.LoadFromFile(g_DirPresets + "\\" + sPreset + GetPresetExt()))
  {
    CNodeFile::CNode *pNode = NodeFile.FirstNode("linkpreset");
    if (pNode)
    {
      CLink *pLink = NEW CLink(NULL, NULL);
      pLink->Load(pNode);
      LoadLink(pLink);
      DISPOSE(pLink);
    }
  }
}


//---------------------------------------------------------------------------//
// OnChangePreset
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChangePreset()
{
  if (m_CBPresets->GetSelected() > 0)
  {
    LoadPreset(m_CBPresets->GetText());
  }
}


//---------------------------------------------------------------------------//
// OnDeletePreset
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnDeletePreset()
{
  if (m_CBPresets->GetSelected() > 0)
  {
    DeleteFile ((g_DirPresets + "\\" + m_CBPresets->GetText() + GetPresetExt()).c_str());
    LoadPresets();
  }
}


//---------------------------------------------------------------------------//
// OnClickAccept
//
//---------------------------------------------------------------------------//  
const string &CMGWindowVarLinker::GetPresetExt() const
{
  static string sExt = "";
  switch (m_BehType)
  {
    case CLinkBehaviour::CHECK_BOX: sExt = ".lph"; break;
    case CLinkBehaviour::COMBO_BOX: sExt = ".lpc"; break;
    case CLinkBehaviour::TRACK_BAR: sExt = ".lpt"; break;
    default: sExt = ""; break;
  }
  return sExt;
}


//---------------------------------------------------------------------------//
// OnClickAccept
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnClickAccept()
{
  // Free previous link
  if (*m_ToLink)
    DISPOSE(*m_ToLink);
  // New One
  *m_ToLink = CreateLink();
  m_RButton->SetLinked(true);
  Close(MR_OK);
}


//---------------------------------------------------------------------------//
// OnClickCancel
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnClickCancel()
{
  Close(MR_CANCEL);
}


//---------------------------------------------------------------------------//
// OnChangeDevice
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChangeDevice()
{
  string selected;
  if (m_LBDevices->GetSelected(selected))
  {
    if (selected == "Keyboard")
    {
      SetDevice(m_KD_Panel);
      if (m_BehType == CLinkBehaviour::TRACK_BAR) SetAction(m_PanelActionsTrackBarA);
    }
    else if (selected == "Beat")
    {
      SetDevice(m_BD_Panel);
      if (m_BehType == CLinkBehaviour::TRACK_BAR) SetAction(m_PanelActionsTrackBarA);
    }
    else if (selected == "Wave FFT")
    {
      SetDevice(m_WD_Panel);
      SetAction(m_PanelActionsTrackBarF);
    }
    else if (selected == "Timer")
    {
      if (m_BehType == CLinkBehaviour::TRACK_BAR) SetAction(m_PanelActionsTrackBarA);
    }
    else if (selected == "MIDI Note")
    {
      if (m_BehType == CLinkBehaviour::TRACK_BAR) SetAction(m_PanelActionsTrackBarA);
    }
    else if (selected == "MIDI Slider")
    {
      if (m_BehType == CLinkBehaviour::TRACK_BAR) SetAction(m_PanelActionsTrackBarF);
    }
    else if (selected == "MIDI Clock")
    {
      if (m_BehType == CLinkBehaviour::TRACK_BAR) SetAction(m_PanelActionsTrackBarA);
    }
  }
  else
    SetDevice(NULL);
}


//---------------------------------------------------------------------------//
// SetDevice
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::SetDevice(CMGGroupBox *pDevice)
{
  if (pDevice == m_CurrentDevice) return;
  if (m_CurrentDevice)
  {
    m_CurrentDevice->SetParent(NULL);
    m_CurrentDevice = NULL;
  }
  m_CurrentDevice = pDevice;
  if (pDevice)
  {
    m_CurrentDevice->SetParent(this);
    ResetDeviceData();
  }
}


//---------------------------------------------------------------------------//
// SetAction
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::SetAction(CMGGroupBox *pAction)
{
  if (pAction == m_CurrentAction) return;
  if (m_CurrentAction)
  {
    m_CurrentAction->SetParent(NULL);
    m_CurrentAction = NULL;
  }
  m_CurrentAction = pAction;
  if (pAction)
  {
    m_CurrentAction->SetParent(this);
    ResetActionData();
  }
}


//---------------------------------------------------------------------------//
// ResetDeviceData
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::ResetDeviceData()
{
  if (m_CurrentDevice == m_KD_Panel)
  {
  }
  else if (m_CurrentDevice == m_BD_Panel)
  {
    m_BD_Beat->Set(1);
    m_BD_Off ->Set(0);
  }
  else if (m_CurrentDevice == m_WD_Panel)
  {
    m_WD_Chan->Select(0);
    m_WD_Gain->Set   (100);
  }
}


//---------------------------------------------------------------------------//
// ResetActionData
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::ResetActionData()
{
  if (m_CurrentAction == m_PanelActionsCheckBox)
  {
    m_CHA_Switch->SetChecked(true);
  }
  else if (m_CurrentAction == m_PanelActionsComboBox)
  {
    m_CBA_Next->SetChecked(true);
    m_CBA_SetValue->Set(0);
  }
  else if (m_CurrentAction == m_PanelActionsTrackBarA)
  {
    m_TBA_Set->SetChecked(true);
    m_TBA_SetValue->Set(100);
    m_TBA_AddValue->Set(10);
    m_TBA_FadeFr->Set(0);
    m_TBA_FadeTo->Set(100);
    m_TBA_FadeLength->Set(250);
  }
  else if (m_CurrentAction == m_PanelActionsTrackBarF)
  {
    m_TBA_DevVal->SetChecked(true);
  }
}


//---------------------------------------------------------------------------//
// OnChange_CHA_Switch
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_CHA_Switch()
{
  if (!m_CHA_Switch->Checked())
  {
    if (!m_CHA_DevVal->Checked() && !m_CHA_InvDev->Checked())
      m_CHA_Switch->SetChecked(true);
  }
  else
  {
    m_CHA_DevVal->SetChecked(false);
    m_CHA_InvDev->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_CHA_DevVal
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_CHA_DevVal()
{
  if (!m_CHA_DevVal->Checked())
  {
    if (!m_CHA_Switch->Checked() && !m_CHA_InvDev->Checked())
      m_CHA_DevVal->SetChecked(true);
  }
  else
  {
    m_CHA_Switch->SetChecked(false);
    m_CHA_InvDev->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_CHA_InvDev
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_CHA_InvDev()
{
  if (!m_CHA_InvDev->Checked())
  {
    if (!m_CHA_DevVal->Checked() && !m_CHA_Switch->Checked())
      m_CHA_InvDev->SetChecked(true);
  }
  else
  {
    m_CHA_DevVal->SetChecked(false);
    m_CHA_Switch->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_CBA_Next
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_CBA_Next()
{
  if (!m_CBA_Next->Checked())
  {
    if (!m_CBA_Prev->Checked() && !m_CBA_Rand->Checked() && !m_CBA_Set->Checked())
      m_CBA_Next->SetChecked(true);
  }
  else
  {
    m_CBA_Prev->SetChecked(false);
    m_CBA_Rand->SetChecked(false);
    m_CBA_Set ->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_CBA_Prev
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_CBA_Prev()
{
  if (!m_CBA_Prev->Checked())
  {
    if (!m_CBA_Next->Checked() && !m_CBA_Rand->Checked() && !m_CBA_Set->Checked())
      m_CBA_Prev->SetChecked(true);
  }
  else
  {
    m_CBA_Next->SetChecked(false);
    m_CBA_Rand->SetChecked(false);
    m_CBA_Set ->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_CBA_Rand
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_CBA_Rand()
{
  if (!m_CBA_Rand->Checked())
  {
    if (!m_CBA_Prev->Checked() && !m_CBA_Next->Checked() && !m_CBA_Set->Checked())
      m_CBA_Rand->SetChecked(true);
  }
  else
  {
    m_CBA_Prev->SetChecked(false);
    m_CBA_Next->SetChecked(false);
    m_CBA_Set ->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_CBA_Set
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_CBA_Set()
{
  if (!m_CBA_Set->Checked())
  {
    if (!m_CBA_Prev->Checked() && !m_CBA_Rand->Checked() && !m_CBA_Next->Checked())
      m_CBA_Set->SetChecked(true);
  }
  else
  {
    m_CBA_Prev->SetChecked(false);
    m_CBA_Rand->SetChecked(false);
    m_CBA_Next->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_TBA_DevVal
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_TBA_DevVal()
{
  if (!m_TBA_DevVal->Checked())
  {
    if (!m_TBA_InvDev->Checked())
      m_TBA_DevVal->SetChecked(true);
  }
  else
  {
    m_TBA_InvDev->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_TBA_InvDev
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_TBA_InvDev()
{
  if (!m_TBA_InvDev->Checked())
  {
    if (!m_TBA_DevVal->Checked())
      m_TBA_InvDev->SetChecked(true);
  }
  else
  {
    m_TBA_DevVal->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_TBA_Set
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_TBA_Set()
{
  if (!m_TBA_Set->Checked())
  {
    if (!m_TBA_Add->Checked() && !m_TBA_Rand->Checked() && !m_TBA_Fade->Checked())
      m_TBA_Set->SetChecked(true);
  }
  else
  {
    m_TBA_Add ->SetChecked(false);
    m_TBA_Rand->SetChecked(false);
    m_TBA_Fade->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_TBA_Add
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_TBA_Add()
{
  if (!m_TBA_Add->Checked())
  {
    if (!m_TBA_Set->Checked() && !m_TBA_Rand->Checked() && !m_TBA_Fade->Checked())
      m_TBA_Add->SetChecked(true);
  }
  else
  {
    m_TBA_Set ->SetChecked(false);
    m_TBA_Rand->SetChecked(false);
    m_TBA_Fade->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_TBA_Rand
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_TBA_Rand()
{
  if (!m_TBA_Rand->Checked())
  {
    if (!m_TBA_Set->Checked() && !m_TBA_Add->Checked() && !m_TBA_Fade->Checked())
      m_TBA_Rand->SetChecked(true);
  }
  else
  {
    m_TBA_Set ->SetChecked(false);
    m_TBA_Add ->SetChecked(false);
    m_TBA_Fade->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_TBA_Fade
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_TBA_Fade()
{
  if (!m_TBA_Fade->Checked())
  {
    if (!m_TBA_Set->Checked() && !m_TBA_Add->Checked() && !m_TBA_Rand->Checked())
      m_TBA_Fade->SetChecked(true);
  }
  else
  {
    m_TBA_Set ->SetChecked(false);
    m_TBA_Add ->SetChecked(false);
    m_TBA_Rand->SetChecked(false);
  }
}


//---------------------------------------------------------------------------//
// OnChange_TBA_SwapFade
//
//---------------------------------------------------------------------------//  
void CMGWindowVarLinker::OnChange_TBA_SwapFade()
{
  int fr = m_TBA_FadeFr->GetValue();
  int to = m_TBA_FadeTo->GetValue();
  m_TBA_FadeFr->Set(to);
  m_TBA_FadeTo->Set(fr);
}
