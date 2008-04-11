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
#include "MGCheckBox.h"
#include "MGButton.h"
#include "VJFXVar_Checkbox.h"
#include "CtrlVar.h"
#include "Link.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_CheckBox::CVJFXVar_CheckBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iScope,iObject)
{
  m_Type     = CHECK_BOX;
  m_CheckBox = NEW CMGCheckBox(pParent, 90, ypos, "", true, boost::bind(&CVJFXVar_CheckBox::OnChange, this));
  m_CheckBox->SetChecked(*(bool *)g_pAppMain->VJController()->FXGetVar(iID, iScope, iObject, iVar));
  m_Control  = m_CheckBox;
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_CheckBox::CVJFXVar_CheckBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iFilterIdx)
{
  m_Type     = CHECK_BOX;
  m_CheckBox = NEW CMGCheckBox(pParent, 90, ypos, "", true, boost::bind(&CVJFXVar_CheckBox::OnChange, this));
  m_CheckBox->SetChecked(*(bool *)g_pAppMain->VJController()->FXGetFilterVar(iID, iFilterIdx, iVar));
  m_Control  = m_CheckBox;
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_CheckBox::~CVJFXVar_CheckBox()
{
  DISPOSE(m_Link);
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
void CVJFXVar_CheckBox::Set(void *pData)
{
  m_CheckBox->SetChecked(*(bool *)pData);
}


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
void CVJFXVar_CheckBox::Evaluate()
{
  if (m_Link && m_Link->Evaluate())
  {
    bool bChecked = m_Link->ValueCheckBox();
    if (!IsFilter())
      g_pAppMain->VJController()->FXSetVar(m_ID, m_Scope, m_Object, m_Var, &bChecked);
    else
      g_pAppMain->VJController()->FXSetFilterVar(m_ID, m_FilterIdx, m_Var, &bChecked);
  }
}


//---------------------------------------------------------------------------//
// OnChange
//
//---------------------------------------------------------------------------//  
void CVJFXVar_CheckBox::OnChange()
{
  bool bChecked = m_CheckBox->Checked();
  if (!IsFilter())
    g_pAppMain->VJController()->FXSetVar(m_ID, m_Scope, m_Object, m_Var, &bChecked);
  else
    g_pAppMain->VJController()->FXSetFilterVar(m_ID, m_FilterIdx, m_Var, &bChecked);
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CVJFXVar_CheckBox::Load(CNodeFile::CNode *pNode)
{
  pNode = pNode->FirstNode("var");
  while (pNode && Stricmp(pNode->AttrAsString("name"),m_Label->Get()))
    pNode = pNode->NextNode("var");
  if (pNode)
  {
    m_CheckBox->SetChecked(pNode->AsBool("value"));
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
void CVJFXVar_CheckBox::Save(CNodeFile *pFile)
{
  pFile->WriteOpenNode ("var", "name=\"" + m_Label->Get() + "\"");
  pFile->WriteNode     ("value", "", m_CheckBox->Checked());
  if (m_Link)
    m_Link->Save(pFile, "link");
  pFile->WriteCloseNode();
}
