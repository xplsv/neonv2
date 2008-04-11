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
#include "VJFXVar.h"
#include "MGEffect.h"
#include "MGPanel.h"
#include "MGGroupBox.h"
#include "MGPageControl.h"
#include "MGLabel.h"
#include "MGTrackBar.h"
#include "MGButton.h"
#include "MGCheckBox.h"
#include "MGSpinBox.h"
#include "MGComboBox.h"
#include "MGPanelVars.h"
#include "MGFXFilters.h"
#include "MGBeatManager.h"
#include "CtrlVar.h"
#include "AppOptions.h"


//---------------------------------------------------------------------------//
// CreateFX
//
//---------------------------------------------------------------------------//  
CMGEffect *CMGEffect::Create(const string &sFile)
{
  CMGEffect *pEffect = NEW CMGEffect();
  if (!pEffect->Load(sFile))
    DISPOSE(pEffect);
  return pEffect;
}


//---------------------------------------------------------------------------//
// CreateFX
//
//---------------------------------------------------------------------------//  
CMGEffect *CMGEffect::Create(CNodeFile::CNode *pNode)
{
  CMGEffect *pEffect = NEW CMGEffect();
  if (!pEffect->Load(pNode))
    DISPOSE(pEffect);
  return pEffect;
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGEffect::CMGEffect() : CMGControl(NULL, 0,0,0,0)
{
  m_Attached = false;
  m_Align    = MGALIGN_CLIENT;
  m_Bitmap   = NEW TMGSurface;
  m_Blend    = BL_ALPHABLEND;
  m_ActiveScopePanel = NULL;
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGEffect::~CMGEffect()
{
  g_pAppMain->VJController()->FXUnload(m_ID);
  if (m_ActiveScopePanel)
    m_ActiveScopePanel->SetParent (NULL);
  for (unsigned i = 0; i < m_VJScopeVars.size(); i++)
  {
    for (unsigned j = 0; j < m_VJScopeVars[i].size(); j++)
      DISPOSE(m_VJScopeVars[i][j]);
  }
  DISPOSE(m_Bitmap);
  g_pAppMain->ShowEffect(NULL);
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
bool CMGEffect::Load(const string &sFile)
{
  CNodeFile NodeFile;
  if (!NodeFile.LoadFromFile(sFile))
  {
    GLOG(("ERR: Can't load file %s\n", sFile.c_str()));
    return false;
  }

  // Para asegurarnos que tendra nombre, ya que la v2 no tenia estos 2 campos en el xml
  int iFrom = sFile.rfind('\\', sFile.size()) + 1;
  int iTo   = sFile.rfind('.', sFile.size());
  if (iFrom == -1) iFrom = 0;
  if (iTo   == -1) iTo   = 0;
  m_FXName = sFile.substr(iFrom, iTo - iFrom);
  m_FileName  = sFile;

  // Has nodes?
  CNodeFile::CNode *pNode = NodeFile.FirstNode("layer");
  if (!pNode) return false;

  // Effect file?
  CNodeFile::CNode *pNodeFX = pNode->FirstNode("effect");
  if (!pNodeFX) return false;

  Load(pNodeFX);

  return (m_ID != -1);
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
bool CMGEffect::Load(CNodeFile::CNode *pNode)
{
  // Load FX data
  m_FXClass  = pNode->AttrAsString("fxclass");
  m_FXDir    = pNode->AsString    ("fxdir");
  m_FXFile   = pNode->AsString    ("fxfile");
  m_FXName   = pNode->AsString    ("fxname", m_FXName);
  m_FileName = pNode->AsString    ("filename", m_FileName);

  PushDir("Effects/" + m_FXDir);
  CVJEffect::TVJSnapshot Snapshot;
  m_ID = g_pAppMain->VJController()->FXLoad(m_FXClass, m_FXFile, &Snapshot);
  PopDir ();
  if (m_ID != -1)
  {
    // Load controls and snapshot
    LoadSnapshot  (&Snapshot, m_Bitmap);
    CreateControls();

    // Load vars, filters and its links
    LoadVars   (pNode);
    LoadFilters(pNode);
  }

  return (m_ID != -1);
}


//---------------------------------------------------------------------------//
// LoadImage
//
//---------------------------------------------------------------------------//  
void CMGEffect::LoadSnapshot(CVJEffect::TVJSnapshot *pSnapshot, TMGSurface *pSurface)
{
  glGenTextures  (1, &pSurface->iTexture);
  glBindTexture  (GL_TEXTURE_2D, pSurface->iTexture);
  glTexImage2D   (GL_TEXTURE_2D, 0, 4, CVJEffect::TVJSnapshot::WIDTH, CVJEffect::TVJSnapshot::HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, pSnapshot->pData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  pSurface->w     = CVJEffect::TVJSnapshot::WIDTH;
  pSurface->h     = CVJEffect::TVJSnapshot::HEIGHT;
  pSurface->fInvW = 1.f / pSurface->w;
  pSurface->fInvH = 1.f / pSurface->h;
}


//---------------------------------------------------------------------------//
// CreateControls
//
//---------------------------------------------------------------------------//  
void CMGEffect::CreateControls()
{
  m_PanelTitle  = NEW CMGPanel      (this, 0,0,0,25,  MGALIGN_TOP);
  m_GBVars      = NEW CMGGroupBox   (this, 0,0,0,140, MGALIGN_TOP);
  m_PCVars      = NEW CMGPageControl(this, 0,0,0,0,   MGALIGN_CLIENT);

  m_PageVars    = NEW CMGPage(m_PCVars, "Vars");
  m_PageObjects = NEW CMGPage(m_PCVars, "Objects");
  m_PageFilters = NEW CMGPage(m_PCVars, "Filters");
  m_PageOptions = NEW CMGPage(m_PCVars, "Options");

  // FX Title
  CMGLabel *pLTitle = NEW CMGLabel (m_PanelTitle,   2, 2, MGALIGN_NONE, FONT_FX_TITLE, m_FXName);
                      NEW CMGButton(m_PanelTitle, 260, 2, CMGButton::SMALL, "S", boost::bind(&CMGEffect::BtnSaveOnClick, this));

  // Speed
  m_BSpeedx1      = NEW CMGButton  (m_GBVars, 45,10, CMGButton::SMALL, "x1", boost::bind(&CMGEffect::BtnSpeedx1OnClick, this));
  m_LSpeed        = NEW CMGLabel   (m_GBVars, 5, 12, MGALIGN_NONE, FONT_NORMAL, "Speed");
  m_TBSpeed       = NEW CMGTrackBar(m_GBVars, 70,12, 200, false, boost::bind(&CMGEffect::TBSpeedOnChange, this));
  m_BSpeed        = NEW CMGButton  (m_GBVars, 272,10, CMGButton::SMALL, "B", boost::bind(&CMGEffect::BtnLinkSpeedOnClick, this));
  m_CBAdjustToBPM = NEW CMGCheckBox(m_GBVars, 70, 35, "Adjust Speed to BPM", true, boost::bind(&CMGEffect::CBAdjustOnChange, this));
  m_SBAdjustToBPM = NEW CMGSpinBox (m_GBVars, 207,35, 39, 0, 999, 1, boost::bind(&CMGEffect::SBAdjustOnChange, this));
                    NEW CMGLabel   (m_GBVars,  67, 2, MGALIGN_NONE, FONT_MINI, "-5");
                    NEW CMGLabel   (m_GBVars, 168, 2, MGALIGN_NONE, FONT_MINI, "0");
                    NEW CMGLabel   (m_GBVars, 265, 2, MGALIGN_NONE, FONT_MINI, "5");
  // Alpha
  m_LAlpha  = NEW CMGLabel   (m_GBVars, 5,62, MGALIGN_NONE, FONT_NORMAL, "Alpha");
  m_TBAlpha = NEW CMGTrackBar(m_GBVars, 70,62, 200, false, boost::bind(&CMGEffect::TBAlphaOnChange, this));
  m_BAlpha  = NEW CMGButton  (m_GBVars, 272,60, CMGButton::SMALL, "B", boost::bind(&CMGEffect::BAlphaOnClick, this));

  // Blend
  m_BlendButtons[0] = NEW CMGButton(m_GBVars, 70, 80, CMGButton::BLEND, "Alpha",   boost::bind(&CMGEffect::OnBlendAlpha  , this));
  m_BlendButtons[1] = NEW CMGButton(m_GBVars,110, 80, CMGButton::BLEND, "Add",     boost::bind(&CMGEffect::OnBlendAdd    , this));
  m_BlendButtons[2] = NEW CMGButton(m_GBVars,150, 80, CMGButton::BLEND, "Sub",     boost::bind(&CMGEffect::OnBlendSub    , this));
  m_BlendButtons[3] = NEW CMGButton(m_GBVars,190, 80, CMGButton::BLEND, "Inv",     boost::bind(&CMGEffect::OnBlendInv    , this));
  m_BlendButtons[4] = NEW CMGButton(m_GBVars,230, 80, CMGButton::BLEND, "InvDst",  boost::bind(&CMGEffect::OnBlendInvDest, this));
  m_BlendButtons[5] = NEW CMGButton(m_GBVars, 70,100, CMGButton::BLEND, "Mask",    boost::bind(&CMGEffect::OnBlendMask   , this));
  m_BlendButtons[6] = NEW CMGButton(m_GBVars,110,100, CMGButton::BLEND, "Mul",     boost::bind(&CMGEffect::OnBlendMul    , this));
  m_BlendButtons[7] = NEW CMGButton(m_GBVars,150,100, CMGButton::BLEND, "InvMul",  boost::bind(&CMGEffect::OnBlendInvMul , this));
  m_BlendButtons[8] = NEW CMGButton(m_GBVars,190,100, CMGButton::BLEND, "ColMul",  boost::bind(&CMGEffect::OnBlendColMul , this));

  // Set initial vars
  m_TBSpeed->SetPos(SpeedToTab(1.f));
  m_TBAlpha->SetPos(1.f);
  SetBlend(BL_ALPHABLEND);

  // FX Vars
  m_VJFXVars = NEW CMGPanelVars(m_PageVars, m_ID, -1, -1);

  // Object Vars
  CMGPanel *pPanelObjectVarsSup = NEW CMGPanel(m_PageObjects, 0,0,0,54, MGALIGN_TOP);
  m_CBScopes   = NEW CMGComboBox(pPanelObjectVarsSup, 80,4,  140, 100, true, boost::bind(&CMGEffect::CBScopeChange , this));
  m_CBObjects  = NEW CMGComboBox(pPanelObjectVarsSup, 80,28, 140, 100, true, boost::bind(&CMGEffect::CBObjectChange, this));
  m_ScopePanel = NEW CMGPanel   (m_PageObjects, 0,0,0,0, MGALIGN_CLIENT);
  vector<string> Scopes;
  g_pAppMain->VJController()->FXGetVarScopes(m_ID, Scopes);
  for (unsigned i = 0; i < Scopes.size(); i++)
  {
    m_CBScopes->Add(Scopes[i]);
    vector<string>         Objects;
    vector<CMGPanelVars *> Panels;
    g_pAppMain->VJController()->FXGetVarObjects(m_ID, Objects, i);
    for (unsigned j = 0; j < Objects.size(); j++)
    {
      CMGPanelVars *pPanel = NEW CMGPanelVars(NULL, m_ID, i,j);
      Panels.push_back(pPanel);
    }
    m_VJScopeVars.push_back(Panels);
  }

  // FX Filters
  m_FXFilters = NEW CMGFXFilters(m_PageFilters, this);

  // Set first
  if (Scopes.size() > 0)
    m_CBScopes->Select(0);

  // Options
  NEW CMGLabel(m_PageOptions, 7, 12, MGALIGN_NONE, FONT_NORMAL, "Render");
  char pBuffer[100];
  m_CBQuality = NEW CMGComboBox(m_PageOptions, 80,10, 140,100, true, boost::bind(&CMGEffect::CBQualityChange, this));
  sprintf_s(pBuffer, 100, "%dx%d", IAppOptions::GetInstance()->Get("Width",     0), IAppOptions::GetInstance()->Get("Height",    0));
  m_CBQuality->Add(pBuffer);
  sprintf_s(pBuffer, 100, "%dx%d", IAppOptions::GetInstance()->Get("CustTex1W", 0), IAppOptions::GetInstance()->Get("CustTex1H", 0));
  m_CBQuality->Add(pBuffer);
  sprintf_s(pBuffer, 100, "%dx%d", IAppOptions::GetInstance()->Get("CustTex2W", 0), IAppOptions::GetInstance()->Get("CustTex2H", 0));
  m_CBQuality->Add(pBuffer);
  m_CBQuality->Select(0);

             NEW CMGLabel  (m_PageOptions,  7, 42, MGALIGN_NONE, FONT_NORMAL, "Fade (ms)");
  m_SBFade = NEW CMGSpinBox(m_PageOptions, 80, 40, 50, 0, 5000, 50, boost::bind(&CMGEffect::SBFadeChange, this));
}


//---------------------------------------------------------------------------//
// LoadVars
//
//---------------------------------------------------------------------------//  
void CMGEffect::LoadVars(CNodeFile::CNode *pNode)
{
  // Main vars
  m_CBAdjustToBPM->SetChecked(pNode->AsBool("adjusttobpm"   ));
  m_SBAdjustToBPM->Set       (pNode->AsInt ("adjusttobpmnum"));

  pNode = pNode->FirstNode("vars");
  if (!pNode) return;

  // Common vars
  CNodeFile::CNode *pNodeFXVars = pNode->FirstNode("effect");
  m_TBSpeed  ->SetPos  (pNodeFXVars->FirstNode("speed"  ) ? pNodeFXVars->FirstNode("speed"  )->AsInt("value") * 1.f/200.f : SpeedToTab(1.0f));
  m_TBAlpha  ->SetPos  (pNodeFXVars->FirstNode("alpha"  ) ? pNodeFXVars->FirstNode("alpha"  )->AsInt("value") * 1.f/200.f : 1.f);
               SetBlend(pNodeFXVars->FirstNode("blend"  ) ? pNodeFXVars->FirstNode("blend"  )->AsInt("value") : 0);
  m_CBQuality->Select  (pNodeFXVars->FirstNode("quality") ? pNodeFXVars->FirstNode("quality")->AsInt("value") : 0);
  m_SBFade   ->Set     (pNodeFXVars->FirstNode("fade"   ) ? pNodeFXVars->FirstNode("fade"   )->AsInt("value") : 250);

  // FX Vars
  m_VJFXVars->Load(pNodeFXVars);

  // Object Vars
  CNodeFile::CNode *pNodeObjVars = pNode->FirstNode("objvars");
  CNodeFile::CNode *pNodeObjVar = pNodeObjVars->FirstNode("objvar");
  while (pNodeObjVar)
  {
    long iScope = pNodeObjVar->AttrAsInt("scope", -1);
    int iObj   = pNodeObjVar->AttrAsInt("obj",   -1);
    if (iScope != -1 && iObj != -1 && iScope < (int)m_VJScopeVars.size() && iObj < (int)m_VJScopeVars[iScope].size())
      m_VJScopeVars[iScope][iObj]->Load(pNodeObjVar);
    pNodeObjVar = pNodeObjVar->NextNode("objvar");
  }
}


//---------------------------------------------------------------------------//
// LoadFilters
//
//---------------------------------------------------------------------------//  
void CMGEffect::LoadFilters(CNodeFile::CNode *pNode)
{
  CNodeFile::CNode *pFilters = pNode->FirstNode("filters");
  if (!pFilters) return;

  m_FXFilters->Load(pFilters);
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CMGEffect::Save(const string &sFile)
{
  CNodeFile NodeFile;
  if (!NodeFile.WriteOpen(sFile))
  {
    GLOG(("ERR: Can't open %s for writing\n", m_FileName.c_str()));
    return;
  }
  NodeFile.WriteOpenNode("layer", "");

    Save(&NodeFile);

  NodeFile.WriteCloseNode();
  NodeFile.WriteClose();
}



//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CMGEffect::Save(CNodeFile *pFile)
{
  // <effect fxclass="class">
  pFile->WriteOpenNode("effect", "fxclass=\"" + m_FXClass + "\"");
  pFile->WriteNode("fxdir",    "", m_FXDir);
  pFile->WriteNode("fxfile",   "", m_FXFile);
  pFile->WriteNode("fxname",   "", m_FXName);
  pFile->WriteNode("fileName", "", m_FileName);

  // Main vars
  pFile->WriteNode("adjusttobpm",    "", m_CBAdjustToBPM->Checked ());
  pFile->WriteNode("adjusttobpmnum", "", m_SBAdjustToBPM->GetValue());

  // Vars
  pFile->WriteOpenNode("vars", "");

  // FX Vars
  pFile->WriteOpenNode("effect", "");
    // Speed
    pFile->WriteOpenNode ("speed", "");
    pFile->WriteNode     ("value", "", (int)(m_TBSpeed->GetPos() * 200.f));
    pFile->WriteCloseNode();
    // Alpha
    pFile->WriteOpenNode ("alpha", "");
    pFile->WriteNode     ("value", "", (int)(m_TBAlpha->GetPos() * 200.f));
    pFile->WriteCloseNode();
    // Blend
    pFile->WriteOpenNode ("blend", "");
    pFile->WriteNode     ("value", "", m_Blend);
    pFile->WriteCloseNode();
    // Quality
    pFile->WriteOpenNode ("quality", "");
    pFile->WriteNode     ("value", "", (int)(m_CBQuality->GetSelected()));
    pFile->WriteCloseNode();
    // Quality
    pFile->WriteOpenNode ("fade", "");
    pFile->WriteNode     ("value", "", (m_SBFade->GetValue()));
    pFile->WriteCloseNode();
    // FXVars
    m_VJFXVars->Save(pFile);
  pFile->WriteCloseNode(); // effect

  // Obj Vars
  pFile->WriteOpenNode ("objvars", "");
    for (size_t i = 0; i < m_VJScopeVars.size(); i++)
    {
      for (size_t j = 0; j < m_VJScopeVars[i].size(); j++)
      {
        char pBuffer[64];
        sprintf_s(pBuffer, 64, "scope=\"%d\" obj=\"%d\"", i,j);
        pFile->WriteOpenNode ("objvar", pBuffer);
        m_VJScopeVars[i][j]->Save(pFile);
        pFile->WriteCloseNode(); // objvar
      }
    }
  pFile->WriteCloseNode(); // objvars
  pFile->WriteCloseNode(); // vars

  // filters
  SaveFilters(pFile);

  pFile->WriteCloseNode(); // effect
}


//---------------------------------------------------------------------------//
// SaveFilters
//
//---------------------------------------------------------------------------//  
void CMGEffect::SaveFilters(CNodeFile *pFile)
{
  pFile->WriteOpenNode("filters", "");

    m_FXFilters->Save(pFile);

  pFile->WriteCloseNode();
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//  
void CMGEffect::Update()
{
  // Vars
  m_VJFXVars->Update();
  // ObjVars
  for (size_t i = 0; i < m_VJScopeVars.size(); i++)
  {
    for (size_t j = 0; j < m_VJScopeVars[i].size(); j++)
      m_VJScopeVars[i][j]->Update();
  }
  // FXFilters
  m_FXFilters->Update();
}


//---------------------------------------------------------------------------//
// Attach
//
//---------------------------------------------------------------------------//  
void CMGEffect::Attach(int Layer)
{
  if (!m_Attached)
  {
    g_pAppMain->VJController()->FXAttach(m_ID, Layer);
    m_Attached = true;
  }
  m_LayerAttached = Layer;
}


//---------------------------------------------------------------------------//
// Unattach
//
//---------------------------------------------------------------------------//  
void CMGEffect::Unattach()
{
  g_pAppMain->VJController()->FXUnattach(m_ID);
  m_Attached = false;
}


//---------------------------------------------------------------------------//
// Move
//
//---------------------------------------------------------------------------//  
void CMGEffect::Move(int Layer)
{
  if (m_Attached)
  {
    g_pAppMain->VJController()->LayerMove(m_LayerAttached, Layer);
    m_LayerAttached = Layer;
  }
}


//---------------------------------------------------------------------------//
// TBSpeedOnChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::TBSpeedOnChange()
{
  if (!m_CBAdjustToBPM->Checked())
  {
    if (m_TBSpeed)
      g_pAppMain->VJController()->FXSetSpeed(m_ID, TabToSpeed(m_TBSpeed->GetPos()));
  }
}


//---------------------------------------------------------------------------//
// BtnLinkSpeedOnClick
//
//---------------------------------------------------------------------------//  
void CMGEffect::BtnLinkSpeedOnClick()
{
}


//---------------------------------------------------------------------------//
// BtnSpeedx1OnClick
//
//---------------------------------------------------------------------------//  
void CMGEffect::BtnSpeedx1OnClick()
{
  if (!m_CBAdjustToBPM->Checked())
  {
    if (m_TBSpeed)
      m_TBSpeed->SetPos(SpeedToTab(1.0f));
  }
}


//---------------------------------------------------------------------------//
// SpeedToTab
//
//---------------------------------------------------------------------------//  
float CMGEffect::SpeedToTab(float fSpeed)
{
  return ((fSpeed + 5.f) * 1.f/10.f);
}


//---------------------------------------------------------------------------//
// TabToSpeed
//
//---------------------------------------------------------------------------//  
float CMGEffect::TabToSpeed(float fSpeed)
{
  return ((fSpeed * 10.f) - 5.f);
}


//---------------------------------------------------------------------------//
// CBAdjustOnChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::CBAdjustOnChange()
{
  m_SBAdjustToBPM->Set(Round((g_pAppMain->VJController()->FXGetLength(m_ID) * 1000.f / g_pAppMain->BeatManager()->GetMasterBpmInv())));
}


//---------------------------------------------------------------------------//
// SBAdjustOnChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::SBAdjustOnChange()
{
  if (m_CBAdjustToBPM->Checked())
  {
    if (m_SBAdjustToBPM->GetValue() > 0)
      g_pAppMain->VJController()->FXSetSpeed(m_ID, g_pAppMain->VJController()->FXGetLength(m_ID) / (m_SBAdjustToBPM->GetValue() * g_pAppMain->BeatManager()->GetMasterBpmInv() / 1000.f));
    else
      g_pAppMain->VJController()->FXSetSpeed(m_ID, 0.f);
  }
}


//---------------------------------------------------------------------------//
// TBAlphaOnChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::TBAlphaOnChange()
{
  if (m_TBAlpha)
    g_pAppMain->VJController()->FXSetAlpha(m_ID, m_TBAlpha->GetPos());
}


//---------------------------------------------------------------------------//
// BAlphaOnClick
//
//---------------------------------------------------------------------------//  
void CMGEffect::BAlphaOnClick()
{
  //m_BAlpha->SetLinked(true);
  //m_BAlpha->Repaint();
}


//---------------------------------------------------------------------------//
// CBScopeChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::CBScopeChange()
{
  m_CBObjects->Clear();

  // Add new objects
  vector<string> Objects;
  g_pAppMain->VJController()->FXGetVarObjects(m_ID, Objects, m_CBScopes->GetSelected());
  for (unsigned j = 0; j < Objects.size(); j++)
  {
    m_CBObjects->Add(Objects[j]);
  }
  // Select first
  if (Objects.size() > 0)
    m_CBObjects->Select(0);
  else
  {
    if (m_ActiveScopePanel)
    {
      m_ActiveScopePanel->SetParent (NULL);
      m_ActiveScopePanel = NULL;
    }
  }
}


//---------------------------------------------------------------------------//
// CBObjectChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::CBObjectChange()
{
  if (m_ActiveScopePanel)
  {
    m_ActiveScopePanel->SetParent (NULL);
    m_ActiveScopePanel = NULL;
  }
  if (m_VJScopeVars.size() > 0)
  {
    m_ActiveScopePanel = m_VJScopeVars[m_CBScopes->GetSelected()][m_CBObjects->GetSelected()];
    if (m_ActiveScopePanel)
      m_ActiveScopePanel->SetParent(m_ScopePanel);
  }
  m_PageObjects->OnResize();
}


//---------------------------------------------------------------------------//
// CBQualityChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::CBQualityChange()
{
  g_pAppMain->VJController()->FXSetQuality(m_ID, m_CBQuality->GetSelected());
}


//---------------------------------------------------------------------------//
// SBFadeChange
//
//---------------------------------------------------------------------------//  
void CMGEffect::SBFadeChange()
{
  g_pAppMain->VJController()->FXSetFade(m_ID, m_SBFade->GetValue() * (1/1000.f));
}


//---------------------------------------------------------------------------//
// BtnSaveOnClick
//
//---------------------------------------------------------------------------//
void CMGEffect::BtnSaveOnClick()
{
  Save(m_FileName + " NEW");
}


//---------------------------------------------------------------------------//
// SetBlend
//
//---------------------------------------------------------------------------//
void CMGEffect::SetBlend(int iBlend)
{
  m_BlendButtons[m_Blend]->SetLinked(false);
  g_pAppMain->VJController()->FXSetBlend(m_ID, iBlend);
  m_Blend = iBlend;
  m_BlendButtons[m_Blend]->SetLinked(true);
}


//---------------------------------------------------------------------------//
// OnBlend
//
//---------------------------------------------------------------------------//
void CMGEffect::OnBlendAlpha  () { SetBlend(BL_ALPHABLEND   ); }
void CMGEffect::OnBlendAdd    () { SetBlend(BL_ADDITIVE     ); }
void CMGEffect::OnBlendSub    () { SetBlend(BL_SUBTRACTIVE  ); }
void CMGEffect::OnBlendInv    () { SetBlend(BL_INVERT       ); }
void CMGEffect::OnBlendInvDest() { SetBlend(BL_INVERTDEST   ); }
void CMGEffect::OnBlendMask   () { SetBlend(BL_MASK         ); }
void CMGEffect::OnBlendMul    () { SetBlend(BL_MULTIPLY     ); }
void CMGEffect::OnBlendInvMul () { SetBlend(BL_INVMULTIPLY  ); }
void CMGEffect::OnBlendColMul () { SetBlend(BL_COLORMULTIPLY); }
