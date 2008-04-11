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

#ifndef _MGAPPMAIN_H
#define _MGAPPMAIN_H

#include "MGWindow.h"

class CVJController;
class CVJFXVar;
class CLink;
class CMGApplication;
class CMGLabel;
class CMGPanel;
class CMGPanelWin;
class CMGGroupBox;
class CMGPageControl;
class CMGPage;
class CMGButton;
class CMGTreeView;
class CMGTreeItem;
class CMGEffect;
class CMGGridPanel;
class CMGWindowNewFX;
class CMGWindowVarLinker;
class CMGBeatManager;
class CTimer;
class CKeyboardDevice;
class CMidiDevice;

class CMGAppMain
{
  public:       
                      CMGAppMain      () { m_bOk = false; }
                     ~CMGAppMain      () { }

    bool              Init            (unsigned hInstance);
    void              End             ();

    bool              Update          ();
    void              UpdateMain      ();
    void              UpdateOutput    ();

    void              ResetProject    ();
    void              NewProject      ();
    void              LoadProject     ();
    void              SaveProject     ();

    void              NewEffect       ();
    void              DelEffect       ();
    void              OpenLayout      ();

    void              OpenVarLinker   (CLink **pLink, int iX, int iY, int iBehType, bool bOnlyMidiKey, CMGControl *pRControl, CMGButton *pRButton, function<void (CMGWindow::EModalResult)> fOnModalResult);

    vector<string>   &FilterList      () { return m_FilterList; }

    void              ShowEffect      (CMGEffect *pEffect);
    CMGEffect        *GetActiveEffect () { return m_ActiveFX; }

    CMGBeatManager   *BeatManager     () { return m_BeatManager; }

    CVJController    *VJController    () { return m_VJController; }
    CTimer           *Timer           () { return m_Timer; }
    CKeyboardDevice  *KeyboardDevice  () { return m_KeyboardDevice; }
    CMidiDevice      *MidiDevice      () { return m_MidiDevice; }

    void              CloseApplication();

    // Link Var Window
    void              OnBtnLinkClick  (CVJFXVar *pVJVar);

  private:

    void              LoadFX          ();
    void              LoadFXDir       (CMGTreeItem *pParent);

    void              CreaBase        ();
    void              CreaPanelDatos  ();
    void              CreaPanelSup    ();
    void              CreaWindows     ();

    void              RefreshData     ();

  private:

    bool              m_bOk;
    CVJController    *m_VJController;
    CMGApplication   *m_MGApp;
    CMGEffect        *m_ActiveFX;
    CTimer           *m_Timer;
    CKeyboardDevice  *m_KeyboardDevice;
    CMidiDevice      *m_MidiDevice;

  public:

    // Base
    CMGGroupBox      *m_GBSup;
    CMGGroupBox      *m_GBDatos;

    // BeatManager
    CMGBeatManager   *m_BeatManager;

    // Datos
    CMGPageControl   *m_PCDatos;

    // FXList
    CMGPanel         *m_PanelFXSup;
    CMGPage          *m_PageFXList;
    CMGPage          *m_PageEffect;
    CMGTreeView      *m_TreeFX;

    // Grid
    CMGGridPanel     *m_PanelGrid;

    // Windows
    CMGWindowNewFX     *m_WindowNewFX;
    CMGWindowVarLinker *m_WindowVarLinker;

    // Filter list
    vector<string>    m_FilterList;
};

extern CMGAppMain    *g_pAppMain;
extern string         g_DirLayouts;
extern string         g_DirEffects;
extern string         g_DirFilters;
extern string         g_DirPresets;
extern string         g_DirProjects;

#endif
