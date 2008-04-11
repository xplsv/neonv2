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
#include "MGApplication.h"
#include "MGPanel.h"
#include "MGPanelWin.h"
#include "MGGroupBox.h"
#include "MGPageControl.h"
#include "MGLabel.h"
#include "MGButton.h"
#include "MGCheckBox.h"
#include "MGTreeView.h"
#include "MGWindow.h"
#include "MGGridPanel.h"
#include "MGEffect.h"
#include "MGAppMain.h"
#include "MGWindowNewFX.h"
#include "MGWindowVarLinker.h"
#include "MGBeatManager.h"
#include "VJController.h"
#include "AppOptions.h"
#include "KeyboardDevice.h"
#include "MidiDevice.h"
#include "Resource.h"

CMGAppMain *g_pAppMain;
string      g_DirEffects  = "Effects";
string      g_DirFilters  = "Filters";
string      g_DirPresets  = "Presets";
string      g_DirProjects = "Projects";


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMGAppMain::Init(unsigned hInstance)
{
  g_pAppMain = this;
  m_ActiveFX = NULL;

  GLOG(("Loading Neonv2::\n"));
  GLOG(("VJController...\n"));
  m_VJController = NULL;
  m_VJController = NEW CVJController;
  if (!m_VJController->Init(NULL, 10, TModoGrafico(IAppOptions::GetInstance()->Get("Device", 0),
                                                   IAppOptions::GetInstance()->Get("Width", 640),
                                                   IAppOptions::GetInstance()->Get("Height", 480),
                                                   32,
                                                   IAppOptions::GetInstance()->Get("Refresh", 60),
                                                   IAppOptions::GetInstance()->Get("Fullscreen", 60) != 0,
                                                   false),
                                      TRect       (IAppOptions::GetInstance()->Get("RLeft", 0),
                                                   IAppOptions::GetInstance()->Get("RTop", 0),
                                                   IAppOptions::GetInstance()->Get("RWidth", 0),
                                                   IAppOptions::GetInstance()->Get("RHeight", 0)),
                                      TVectorI2   (IAppOptions::GetInstance()->Get("CustTex1W", 0),
                                                   IAppOptions::GetInstance()->Get("CustTex1H", 0)),
                                      TVectorI2   (IAppOptions::GetInstance()->Get("CustTex2W", 0),
                                                   IAppOptions::GetInstance()->Get("CustTex2H", 0)),
                                                   false,
                                                   true))
  {
    DISPOSE(m_VJController);
    return false;
  }

  GLOG(("MGApplication...\n"));
  m_MGApp = NEW CMGApplication;
  if (!m_MGApp->Init(hInstance, 1024, 768))
  {  
    DISPOSE(m_VJController);
    DISPOSE(m_MGApp);
    return false;
  }

  CreaBase      ();
  CreaPanelDatos();
  CreaPanelSup  ();
  CreaWindows   ();

  GLOG(("Devices...\n"));
  CKeyboardDevice::InitSubsystem();
  CMidiDevice    ::InitSubsystem();

  GLOG(("Sound...\n"));
  m_VJController->DrawDebug(true);
  m_VJController->GetSound()->Record();
  m_Timer = NEW CTimer();
  m_KeyboardDevice = NEW CKeyboardDevice();

  RefreshData();

  ShowWindow  ((HWND)m_MGApp->GetHWnd(), SW_SHOWNORMAL);
  UpdateWindow((HWND)m_MGApp->GetHWnd());
  SetFocus    ((HWND)m_MGApp->GetHWnd());

  GLOG(("Init ok, drawing interface...\n"));
  m_MGApp->Repaint();
  m_MGApp->MustRepaint();

  m_Timer->Reset();

  return true;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMGAppMain::End()
{
  m_PanelGrid->Clear();
  DISPOSE(m_Timer);
  DISPOSE(m_WindowNewFX);
  DISPOSE(m_WindowVarLinker);
  DISPOSE(m_VJController);
  DISPOSE(m_MGApp);
  DISPOSE(m_KeyboardDevice);
  
  CKeyboardDevice::ShutSubsystem();
  CMidiDevice    ::ShutSubsystem();
}


//---------------------------------------------------------------------------//
// CloseApplication
//
//---------------------------------------------------------------------------//
void CMGAppMain::CloseApplication()
{
  m_MGApp->Close();
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//
bool CMGAppMain::Update()
{
  UpdateMain  ();
  UpdateOutput();

  return m_MGApp->WantClose();
}


//---------------------------------------------------------------------------//
// UpdateMain
//
//---------------------------------------------------------------------------//
void CMGAppMain::UpdateMain()
{
  m_KeyboardDevice->Update();

  // Update GUI app
  m_MGApp->Update();

  // BeatManager
  m_BeatManager->Update();
  // PanelGrid Update
  m_PanelGrid->Update();
}


//---------------------------------------------------------------------------//
// UpdateOutput
//
//---------------------------------------------------------------------------//
void CMGAppMain::UpdateOutput()
{
  // Run
  if (m_VJController)
    m_VJController->Update();

  // Draw
  if (m_VJController)
    m_VJController->Draw();
}


//---------------------------------------------------------------------------//
// CreaBase
//
//---------------------------------------------------------------------------//
void CMGAppMain::CreaBase()
{
  CMGPanel    *PanelPrincipal = NEW CMGPanel      (NULL,           0,0, 1024,768, MGALIGN_CLIENT);
               m_GBSup        = NEW CMGGroupBox   (PanelPrincipal, 0,0,  10,  31, MGALIGN_TOP);
  CMGPanel    *PanelDatos     = NEW CMGPanel      (PanelPrincipal, 0,0, 304,   0, MGALIGN_LEFT);
               m_PanelGrid    = NEW CMGGridPanel  (PanelPrincipal);
  CMGGroupBox *PanelBeat      = NEW CMGGroupBox   (PanelDatos,     0,0, 150,  88, MGALIGN_TOP);
               m_PCDatos      = NEW CMGPageControl(PanelDatos,     0,0,   0,   0, MGALIGN_CLIENT);
               m_BeatManager  = NEW CMGBeatManager(PanelBeat);
  m_MGApp->SetRootItem(PanelPrincipal);
}


//---------------------------------------------------------------------------//
// CreaPanelDatos
//
//---------------------------------------------------------------------------//
void CMGAppMain::CreaPanelDatos()
{
  CMGPage *pPageOptions = NEW CMGPage(m_PCDatos, "Options");
           m_PageFXList = NEW CMGPage(m_PCDatos, "FX List");
           m_PageEffect = NEW CMGPage(m_PCDatos, "Effect");
  CMGPage *pPageLayer   = NEW CMGPage(m_PCDatos, "Layer");
  
  m_PanelFXSup = NEW CMGPanel   (m_PageFXList, 0,0,0,26, MGALIGN_TOP);
  m_TreeFX     = NEW CMGTreeView(m_PageFXList, 0,0,0,0,  MGALIGN_CLIENT);

  NEW CMGButton(m_PanelFXSup, 1,   1, CMGButton::NORMAL,  "New FX", boost::bind(&CMGAppMain::NewEffect, this));
  NEW CMGButton(m_PanelFXSup, 78,  1, CMGButton::NORMAL,  "New FX", boost::bind(&CMGAppMain::DelEffect, this));
  NEW CMGButton(m_PanelFXSup, 274, 1, CMGButton::REFRESH, "",       boost::bind(&CMGAppMain::LoadFX,    this));
}

  
//---------------------------------------------------------------------------//
// CreaPanelSup
//
//---------------------------------------------------------------------------//
void CMGAppMain::CreaPanelSup()
{
  CMGPanel    *pPanelR = NEW CMGPanel   (m_GBSup, 0,0,22,0, MGALIGN_RIGHT);
  NEW CMGButton(pPanelR, 0, 3, CMGButton::SMALL, "X", boost::bind(&CMGAppMain::CloseApplication, this));
  CMGPanelWin *pPanel  = NEW CMGPanelWin(m_GBSup, 0,0,0,0, MGALIGN_CLIENT);
  NEW CMGLabel (pPanel, 0,0, MGALIGN_RIGHT, FONT_NORMAL, "Neon v2.5 Beta");
  NEW CMGButton(pPanel, 1,      1, CMGButton::NORMAL,  "New Project", boost::bind(&CMGAppMain::NewProject, this));
  NEW CMGButton(pPanel, 1+75+2, 1, CMGButton::NORMAL,  "Load Project", boost::bind(&CMGAppMain::LoadProject, this));
  NEW CMGButton(pPanel, 1+150+4,1, CMGButton::NORMAL,  "Save Project", boost::bind(&CMGAppMain::SaveProject, this));
}


//---------------------------------------------------------------------------//
// CreaWindows
//
//---------------------------------------------------------------------------//
void CMGAppMain::CreaWindows()
{
  m_WindowNewFX     = NEW CMGWindowNewFX    ();
  m_WindowVarLinker = NEW CMGWindowVarLinker();
}


//---------------------------------------------------------------------------//
// LoadFX
//
//---------------------------------------------------------------------------//
void CMGAppMain::LoadFX()
{
  m_TreeFX->Clear();
  PushDir(g_DirEffects);
    CMGTreeItem *pItemLayouts = NEW CMGTreeItem(NULL, g_DirEffects, NULL, true);
    LoadFXDir(pItemLayouts);
  PopDir();

  pItemLayouts->Expand();
  m_TreeFX->SetRootItem(pItemLayouts);
  m_TreeFX->Update();
}


//---------------------------------------------------------------------------//
// LoadFXDir
//
//---------------------------------------------------------------------------//
void CMGAppMain::LoadFXDir(CMGTreeItem *pParent)
{
  TFindFileResult Res;
  FindFileInit(Res, "*.*");
  while (FindFileNext(Res))
  {
    if (Res.bDir)
    {
      if (!(Res.Filename[0] == '.' && (Res.Filename[1] == '.' || Res.Filename[1] == 0)))
      {
        PushDir(Res.Filename);
        CMGTreeItem *pItem = NEW CMGTreeItem(pParent, Res.Filename, NULL, true);
        LoadFXDir(pItem);
        PopDir();
      }
    }
    else
    {
      size_t i = Res.Filename.size();
      if (i > 4)
      {
        string sFile = Res.Filename;
        std::transform(sFile.begin(),sFile.end(),sFile.begin(),(int(*)(int))toupper);
        if (sFile.find(".NLY") == sFile.size()-4)
        {
          CMGTreeItem *pItem = NEW CMGTreeItem(pParent, Res.Filename.substr(0,Res.Filename.size()-4), NULL, false);
        }
      }
    }
  }
  FindFileClose(Res);
}


//---------------------------------------------------------------------------//
// RefreshData
//
//---------------------------------------------------------------------------//
void CMGAppMain::RefreshData()
{
  LoadFX();

  m_FilterList.clear();
  TFindFileResult Res;
  FindFileInit(Res, "Filters/*.nf2");
  while (FindFileNext(Res))
  {
    m_FilterList.push_back(Res.Filename.substr(0, Res.Filename.find(".nf2")));
  }
  FindFileClose(Res);
}


//---------------------------------------------------------------------------//
// ResetProject
//
//---------------------------------------------------------------------------//
void CMGAppMain::ResetProject()
{
  m_PanelGrid->Clear();
  m_PanelGrid->SetActiveGrid(0);
}


//---------------------------------------------------------------------------//
// NewProject
//
//---------------------------------------------------------------------------//
void CMGAppMain::NewProject()
{
  ResetProject();
}


//---------------------------------------------------------------------------//
// LoadProject
//
//---------------------------------------------------------------------------//
void CMGAppMain::LoadProject()
{
  string sFile;
  // Load neon project
  bool bOpen = g_pMGApp->OpenFileOpen("Load Project", g_DirProjects.c_str(), "Project File\0*.npj\0", sFile);
  if (bOpen)
  {
    NewProject();

    CNodeFile NodeFile;
    if (!NodeFile.LoadFromFile(sFile))
    {
      GLOG(("ERR: Can't load project file %s\n", sFile.c_str()));
      return;
    }

    // Has nodes?
    CNodeFile::CNode *pNode = NodeFile.FirstNode("project");
    if (!pNode) return;

    // Layout
    CNodeFile::CNode *pNodeLay = pNode->FirstNode("layout");
    while (pNodeLay)
    {
      // Correct layout?
      int iTab = pNodeLay->AttrAsInt("tab", -1);
      int iCol = pNodeLay->AttrAsInt("col", -1);
      int iRow = pNodeLay->AttrAsInt("row", -1);
      if (iTab != -1 && iCol != -1 && iRow != -1)
      {
        CNodeFile::CNode *pNodeFX = pNodeLay->FirstNode("effect");
        CMGEffect *pEffect = CMGEffect::Create(pNodeFX);
        // If ok, add
        if (pEffect)
          m_PanelGrid->GetGrid(iTab)->GetCellData(iCol,iRow)->SetEffect(pEffect);
      }
      // Next node
      pNodeLay = pNodeLay->NextNode("layout"); 
    }
  }
}


//---------------------------------------------------------------------------//
// SaveProject
//
//---------------------------------------------------------------------------//
void CMGAppMain::SaveProject()
{
  string sFile;
  // Save neon project
  if (g_pMGApp->OpenFileSave("Save Project", g_DirProjects.c_str(), "Project File\0*.npj\0", "*.npj", sFile))
  {
    CNodeFile NodeFile;
    if (!NodeFile.WriteOpen(sFile))
    {
      GLOG(("ERR: Can't open %s for writing\n", sFile.c_str()));
      return;
    }
    NodeFile.WriteOpenNode("project", "");

      // Save effects for all grids
      for (int i = 0; i < 10; i++)
      {
        CMGGridFX *pGrid = m_PanelGrid->GetGrid(i);
        for (int r = 0; r < pGrid->GetRows(); r++)
        {
          for (int c = 0; c < pGrid->GetCols(); c++)
          {
            CMGEffect *pEffect = pGrid->GetCellData(c,r)->GetEffect();
            if (pEffect)
            {
              // Save effect in specified tab, col and row
              char pAttr[MAX_PATH];
              sprintf_s(pAttr, MAX_PATH, "tab=\"%d\" col=\"%d\" row=\"%d\"", i, c, r);
              NodeFile.WriteOpenNode("layout", pAttr);

                pEffect->Save(&NodeFile);

              NodeFile.WriteCloseNode();
            }
          }
        }
      }

    NodeFile.WriteCloseNode();
    NodeFile.WriteClose    ();
  }
}


//---------------------------------------------------------------------------//
// NewEffect
//
//---------------------------------------------------------------------------//
void CMGAppMain::NewEffect()
{
}


//---------------------------------------------------------------------------//
// DelEffect
//
//---------------------------------------------------------------------------//
void CMGAppMain::DelEffect()
{
}


//---------------------------------------------------------------------------//
// ShowEffect
//
//---------------------------------------------------------------------------//
void CMGAppMain::ShowEffect(CMGEffect *pVJEffect)
{
  if (m_ActiveFX)
  {
    m_ActiveFX->SetParent (NULL);
    m_ActiveFX->SetVisible(false);
  }
  m_ActiveFX = pVJEffect;
  if (m_ActiveFX)
  {
    m_ActiveFX->SetParent (m_PageEffect);
    m_ActiveFX->SetVisible(true);
  }
  m_PageEffect->OnResize();
  m_PageEffect->Repaint ();
  if (m_ActiveFX) m_PCDatos->SetActivePage(m_PageEffect);
}


//---------------------------------------------------------------------------//
// OpenVarLinker
//
//---------------------------------------------------------------------------//
void CMGAppMain::OpenVarLinker(CLink **ppToLink, int iX, int iY, int iBehType, bool bOnlyMidiKey, CMGControl *pRControl, CMGButton *pRButton, function<void (CMGWindow::EModalResult)> fOnModalResult)
{
  if (!m_WindowVarLinker->IsVisible())
    m_WindowVarLinker->Show(ppToLink, iX, iY, iBehType, bOnlyMidiKey, pRControl, pRButton, fOnModalResult);
}


//---------------------------------------------------------------------------//
// OnBtnLinkClick
//
//---------------------------------------------------------------------------//
void CMGAppMain::OnBtnLinkClick(CVJFXVar *pVJVar)
{
}
