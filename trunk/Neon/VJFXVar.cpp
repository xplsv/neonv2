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
#include "VJFXVar_CheckBox.h"
#include "VJFXVar_EditBox.h"
#include "VJFXVar_ComboBox.h"
#include "VJFXVar_Slider.h"
#include "LinkBehaviour.h"
#include "MGLabel.h"
#include "MGButton.h"
#include "CtrlVar.h"

//---------------------------------------------------------------------------//
// Create
//
//---------------------------------------------------------------------------//  
CVJFXVar *CVJFXVar::Create(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject)
{
  switch (pCtrlVar->iType)
  {
    case TCtrlVar::CHECK_BOX: return NEW CVJFXVar_CheckBox(pParent, pCtrlVar, ypos, iID, iVar, iScope, iObject);
    case TCtrlVar::EDIT_BOX:  return NEW CVJFXVar_EditBox (pParent, pCtrlVar, ypos, iID, iVar, iScope, iObject);
    case TCtrlVar::COMBO_BOX: return NEW CVJFXVar_ComboBox(pParent, pCtrlVar, ypos, iID, iVar, iScope, iObject);
    case TCtrlVar::SLIDER:    return NEW CVJFXVar_Slider  (pParent, pCtrlVar, ypos, iID, iVar, iScope, iObject);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Create
//
//---------------------------------------------------------------------------//  
CVJFXVar *CVJFXVar::Create(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx)
{
  switch (pCtrlVar->iType)
  {
  case TCtrlVar::CHECK_BOX: return NEW CVJFXVar_CheckBox(pParent, pCtrlVar, ypos, iID, iVar, iFilterIdx);
  case TCtrlVar::EDIT_BOX:  return NEW CVJFXVar_EditBox (pParent, pCtrlVar, ypos, iID, iVar, iFilterIdx);
  case TCtrlVar::COMBO_BOX: return NEW CVJFXVar_ComboBox(pParent, pCtrlVar, ypos, iID, iVar, iFilterIdx);
  case TCtrlVar::SLIDER:    return NEW CVJFXVar_Slider  (pParent, pCtrlVar, ypos, iID, iVar, iFilterIdx);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar::CVJFXVar(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject)
{
  m_ID       = iID;
  m_Var      = iVar;
  m_Scope    = iScope;
  m_Object   = iObject;
  m_IsFilter = false;
  m_Link     = NULL;
  m_Label    = NEW CMGLabel(pParent, 4, ypos+2, MGALIGN_NONE, FONT_NORMAL, pCtrlVar->pszName);
  if (pCtrlVar)
    m_BtnLink = NEW CMGButton(pParent, 270, ypos, CMGButton::SMALL, "B", boost::bind(&CVJFXVar::OnBtnLinkClick, this));
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar::CVJFXVar(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx)
{
  m_ID       = iID;
  m_Var      = iVar;
  m_FilterIdx= iFilterIdx;
  m_IsFilter = true;
  m_Label    = NEW CMGLabel(pParent, 4, ypos+2, MGALIGN_NONE, FONT_NORMAL, pCtrlVar->pszName);
  if (pCtrlVar)
    m_BtnLink = NEW CMGButton(pParent, 270, ypos, CMGButton::SMALL, "B", boost::bind(&CVJFXVar::OnBtnLinkClick, this));
}


//---------------------------------------------------------------------------//
// OnBtnLinkClick
//
//---------------------------------------------------------------------------//  
void CVJFXVar::OnBtnLinkClick()
{
  switch (m_Type)
  {
    case CHECK_BOX: g_pAppMain->OpenVarLinker(&m_Link, m_BtnLink->Left(), m_BtnLink->Top(), CLinkBehaviour::CHECK_BOX, false, m_Control, m_BtnLink, bind(&CVJFXVar::OnCloseVarLinker,this,_1)); break;
    case COMBO_BOX: g_pAppMain->OpenVarLinker(&m_Link, m_BtnLink->Left(), m_BtnLink->Top(), CLinkBehaviour::COMBO_BOX, false, m_Control, m_BtnLink, bind(&CVJFXVar::OnCloseVarLinker,this,_1)); break;
    case SLIDER:    g_pAppMain->OpenVarLinker(&m_Link, m_BtnLink->Left(), m_BtnLink->Top(), CLinkBehaviour::TRACK_BAR, false, m_Control, m_BtnLink, bind(&CVJFXVar::OnCloseVarLinker,this,_1)); break;
  }
}


//---------------------------------------------------------------------------//
// OnCloseVarLinker
// m_Link is updated on the VarLinker directly
//---------------------------------------------------------------------------//  
void CVJFXVar::OnCloseVarLinker(CMGWindow::EModalResult iModalResult)
{
  if (iModalResult == CMGWindow::MR_OK)
  {
  }
}
