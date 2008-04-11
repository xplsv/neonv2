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
#include "MGBeatManager.h"
#include "MGLabel.h"
#include "MGButton.h"
#include "MGCheckBox.h"
#include "MGTrackBar.h"
#include "MGShowFFT.h"
#include "Link.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGBeatManager::CMGBeatManager(CMGControl *pParent) : CMGControl(pParent, 0,0,0,0)
{
  m_Align = MGALIGN_CLIENT;

  m_LabelBpm = NEW CMGLabel(this, 10,10, MGALIGN_NONE, FONT_BPM, "");
  NEW CMGButton(this,  5,63, CMGButton::SMALL, "+", bind(&CMGBeatManager::OnBtnUpBpm,    this));
  NEW CMGButton(this, 25,63, CMGButton::SMALL, "-", bind(&CMGBeatManager::OnBtnDnBpm,    this));
  NEW CMGButton(this, 45,63, CMGButton::SMALL, "<", bind(&CMGBeatManager::OnBtnRewBpm,   this));
  NEW CMGButton(this, 65,63, CMGButton::SMALL, ">", bind(&CMGBeatManager::OnBtnFwdBpm,   this));
  NEW CMGButton(this, 85,63, CMGButton::SMALL, "R", bind(&CMGBeatManager::OnBtnResetBpm, this));
  
  m_BtnLinkGain = NEW CMGButton(this,279,63, CMGButton::SMALL, "B", bind(&CMGBeatManager::OnBtnLinkGain, this));

  m_ShowFFT = NEW CMGShowFFT (this, 149, 2, 130,60, 15);
  m_Gain    = NEW CMGTrackBar(this, 282, 2, 60, true, bind(&CMGBeatManager::OnChangeGain, this));
  m_Gain->SetPos(0.80f);
  m_UpdateFFT = NEW CMGCheckBox(this, 150, 63, "Show FFT", true, NULL);
  m_UpdateFFT->SetChecked(false);

  m_Timer.Reset();
  m_BeatNum    = -1;
  m_BeatKilled = false;

  SetMasterBpm (130.0);
  OnBtnResetBpm();
  OnResize     ();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGBeatManager::~CMGBeatManager()
{
}


//---------------------------------------------------------------------------//
// SetMasterBpm
//
//---------------------------------------------------------------------------//
void CMGBeatManager::SetMasterBpm(float fBpm)
{
  char pBuffer[32];
  fBpm = Round(fBpm*10.f) / 10.f;
  sprintf_s(pBuffer, 32, "%3.1f", fBpm);
  m_MasterBpm    = fBpm;
  m_MasterBpmInv = 60000.f/fBpm;
  m_LabelBpm->Set(pBuffer);
}


//---------------------------------------------------------------------------//
// IsBeat
//
//---------------------------------------------------------------------------//
bool CMGBeatManager::IsBeat(int iBeat, int iOff)
{
  return (!m_BeatKilled && m_IsBeat && (((m_BeatNum-iOff) % iBeat) == 0));
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//  
void CMGBeatManager::Update()
{
  // Beat
  m_Time = m_Timer.Get();

  // Beat number
  int iBeat = (int)((m_Time - m_TimeBeat) * 1000.f / m_MasterBpmInv);
  m_IsBeat  = (iBeat > m_BeatNum);
  m_BeatNum = iBeat;
  // Show FFT
  if (m_UpdateFFT->Checked())
    m_ShowFFT->Update(g_pAppMain->VJController()->GetFFT());
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CMGBeatManager::Load(CNodeFile::CNode *pNode)
{
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CMGBeatManager::Save(CNodeFile *pFile)
{
}


//---------------------------------------------------------------------------//
// OnBtnUpBpm
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnBtnUpBpm()
{
  SetMasterBpm(m_MasterBpm + 0.1f);
}


//---------------------------------------------------------------------------//
// OnBtnDnBpm
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnBtnDnBpm()
{
  SetMasterBpm(m_MasterBpm - 0.1f);
}


//---------------------------------------------------------------------------//
// OnBtnFwdBpm
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnBtnFwdBpm()
{
}


//---------------------------------------------------------------------------//
// OnBtnRewBpm
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnBtnRewBpm()
{
}


//---------------------------------------------------------------------------//
// OnBtnResetBpm
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnBtnResetBpm()
{
  m_Timer.Reset();
}


//---------------------------------------------------------------------------//
// OnChangeGain
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnChangeGain()
{
  g_pAppMain->VJController()->SetGain(5.f - m_Gain->GetPos() * 5.f);
}


//---------------------------------------------------------------------------//
// OnBtnLinkGain
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnBtnLinkGain()
{
  g_pAppMain->OpenVarLinker(&m_LinkGain, m_BtnLinkGain->Left(), m_BtnLinkGain->Top(), CLinkBehaviour::TRACK_BAR, true, m_Gain, m_BtnLinkGain, bind(&CMGBeatManager::OnCloseLinkGain, this));
}


//---------------------------------------------------------------------------//
// OnCloseLinkGain
//
//---------------------------------------------------------------------------//
void CMGBeatManager::OnCloseLinkGain()
{
}
