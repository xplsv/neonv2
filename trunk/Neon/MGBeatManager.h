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

#ifndef _MGBEATMANAGER_H
#define _MGBEATMANAGER_H

#include "MGControl.h"

class CMGLabel;
class CMGCheckBox;
class CMGShowFFT;
class CMGTrackBar;
class CMGButton;

class CMGBeatManager : public CMGControl
{
  public:
     
                    CMGBeatManager   (CMGControl *pParent);
    virtual        ~CMGBeatManager   ();

    bool            IsBeat           (int iBeat, int iOff);
    void            SetMasterBpm     (float fBpm);
    float           GetMasterBpm     () const { return m_MasterBpm;    }
    float           GetMasterBpmInv  () const { return m_MasterBpmInv; }

    void            Update           ();

    void            Load             (CNodeFile::CNode *pNode);
    void            Save             (CNodeFile        *pFile);

  protected:

    void            OnBtnUpBpm       ();
    void            OnBtnDnBpm       ();
    void            OnBtnFwdBpm      ();
    void            OnBtnRewBpm      ();
    void            OnBtnResetBpm    ();
    void            OnBtnLinkGain    ();
    void            OnChangeGain     ();
    void            OnChangeUpdateFFT();
    void            OnCloseLinkGain  ();

  private:

    CTimer          m_Timer;
    double          m_Time;
    double          m_TimeBeat;
    int             m_MasterBeat;
    float           m_MasterBpm;
    float           m_MasterBpmInv;
    bool            m_BeatKilled;
    bool            m_IsBeat;
    int             m_BeatNum;
    CMGLabel       *m_LabelBpm;
    CMGCheckBox    *m_UpdateFFT;
    CMGShowFFT     *m_ShowFFT;
    CMGTrackBar    *m_Gain;
    CMGButton      *m_BtnLinkGain;
    CLink          *m_LinkGain;
};

#endif
