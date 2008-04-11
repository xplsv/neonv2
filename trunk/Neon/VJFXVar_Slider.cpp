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
#include "MGTrackBar.h"
#include "MGButton.h"
#include "VJFXVar_Slider.h"
#include "CtrlVar.h"
#include "Link.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_Slider::CVJFXVar_Slider(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iScope,iObject)
{
  m_Type = SLIDER;
  m_TrackBar = NEW CMGTrackBar(pParent, 90, ypos+2, 175, false, boost::bind(&CVJFXVar_Slider::OnChange, this));
  m_TrackBar->SetPos(*(float *)g_pAppMain->VJController()->FXGetVar(iID, iScope, iObject, iVar));
  m_Control = m_TrackBar;
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_Slider::CVJFXVar_Slider(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iFilterIdx)
{
  m_Type = SLIDER;
  m_TrackBar = NEW CMGTrackBar(pParent, 90, ypos+2, 175, false, boost::bind(&CVJFXVar_Slider::OnChange, this));
  m_TrackBar->SetPos(*(float *)g_pAppMain->VJController()->FXGetFilterVar(iID, m_FilterIdx, iVar));
  m_Control = m_TrackBar;
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_Slider::~CVJFXVar_Slider()
{
  DISPOSE(m_Link);
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
void CVJFXVar_Slider::Set(void *pData)
{
  m_TrackBar->SetPos(*(float *)pData);
}


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
void CVJFXVar_Slider::Evaluate()
{
  if (m_Link && m_Link->Evaluate())
  {
    float fPos = m_Link->ValueTrackBar();
    if (!IsFilter())
      g_pAppMain->VJController()->FXSetVar(m_ID, m_Scope, m_Object, m_Var, &fPos);
    else
      g_pAppMain->VJController()->FXSetFilterVar(m_ID, m_FilterIdx, m_Var, &fPos);
  }
}


//---------------------------------------------------------------------------//
// OnChange
//
//---------------------------------------------------------------------------//  
void CVJFXVar_Slider::OnChange()
{
  float fPos = m_TrackBar->GetPos();
  if (!IsFilter())
    g_pAppMain->VJController()->FXSetVar(m_ID, m_Scope, m_Object, m_Var, &fPos);
  else
    g_pAppMain->VJController()->FXSetFilterVar(m_ID, m_FilterIdx, m_Var, &fPos);
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CVJFXVar_Slider::Load(CNodeFile::CNode *pNode)
{
  pNode = pNode->FirstNode("var");
  while (pNode && Stricmp(pNode->AttrAsString("name"), m_Label->Get()))
    pNode = pNode->NextNode("var");
  if (pNode)
  {
    m_TrackBar->SetPos(pNode->AsInt("value") / 200.f);
    CNodeFile::CNode *pNodeLink = pNode->FirstNode("link");
    if (pNodeLink)
    {                                                                                             
      m_Link = NEW CLink(NULL, NULL);
      m_Link->Load(pNodeLink);
    }
  }
  m_BtnLink->SetLinked(m_Link != NULL);
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CVJFXVar_Slider::Save(CNodeFile *pFile)
{
  pFile->WriteOpenNode ("var", "name=\"" + m_Label->Get() + "\"");
  pFile->WriteNode     ("value", "", Floor(m_TrackBar->GetPos() * 200.f));
  if (m_Link)
    m_Link->Save(pFile, "link");
  pFile->WriteCloseNode();
}