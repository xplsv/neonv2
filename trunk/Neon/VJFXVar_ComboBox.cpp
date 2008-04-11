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
#include "MGComboBox.h"
#include "MGButton.h"
#include "VJFXVar_ComboBox.h"
#include "CtrlVar.h"
#include "Link.h"

//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_ComboBox::CVJFXVar_ComboBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iScope,iObject)
{
  m_Type = COMBO_BOX;
  m_ComboBox = NEW CMGComboBox(pParent, 90, ypos, 100, 20, false, boost::bind(&CVJFXVar_ComboBox::OnChange, this));
  for (int i = 0; i < pCtrlVar->iNumOptions; i++)
    m_ComboBox->Add(pCtrlVar->pszOptions[i]);
  m_ComboBox->Select(*(int *)g_pAppMain->VJController()->FXGetVar(iID, iScope, iObject, iVar));
  m_Control  = m_ComboBox;
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_ComboBox::CVJFXVar_ComboBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iFilterIdx)
{
  m_Type = COMBO_BOX;
  m_ComboBox = NEW CMGComboBox(pParent, 90, ypos, 100, 20, false, boost::bind(&CVJFXVar_ComboBox::OnChange, this));
  for (int i = 0; i < pCtrlVar->iNumOptions; i++)
    m_ComboBox->Add(pCtrlVar->pszOptions[i]);
  m_ComboBox->Select(*(int *)g_pAppMain->VJController()->FXGetFilterVar(iID, m_FilterIdx, iVar));
  m_Control  = m_ComboBox;
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_ComboBox::~CVJFXVar_ComboBox()
{
  DISPOSE(m_Link);
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
void CVJFXVar_ComboBox::Set(void *pData)
{
  m_ComboBox->Select(*(int *)pData);
}


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
void CVJFXVar_ComboBox::Evaluate()
{
  if (m_Link && m_Link->Evaluate())
  {
    int iValue = m_Link->ValueComboBox();
    if (!IsFilter())
      g_pAppMain->VJController()->FXSetVar(m_ID, m_Scope, m_Object, m_Var, &iValue);
    else
      g_pAppMain->VJController()->FXSetFilterVar(m_ID, m_FilterIdx, m_Var, &iValue);
  }
}


//---------------------------------------------------------------------------//
// OnChange
//
//---------------------------------------------------------------------------//  
void CVJFXVar_ComboBox::OnChange()
{
  int iSelected = m_ComboBox->GetSelected();
  if (!IsFilter())
    g_pAppMain->VJController()->FXSetVar(m_ID, m_Scope, m_Object, m_Var, &iSelected);
  else
    g_pAppMain->VJController()->FXSetFilterVar(m_ID, m_FilterIdx, m_Var, &iSelected);    
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CVJFXVar_ComboBox::Load(CNodeFile::CNode *pNode)
{
  pNode = pNode->FirstNode("var");
  while (pNode && Stricmp(pNode->AttrAsString("name"),m_Label->Get()))
    pNode = pNode->NextNode("var");
  if (pNode)
  {
    m_ComboBox->Select(pNode->AsInt("value"));
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
void CVJFXVar_ComboBox::Save(CNodeFile *pFile)
{
  pFile->WriteOpenNode ("var", "name=\"" + m_Label->Get() + "\"");
  pFile->WriteNode     ("value", "", m_ComboBox->GetSelected());
  if (m_Link)
    m_Link->Save(pFile, "link");
  pFile->WriteCloseNode();
}