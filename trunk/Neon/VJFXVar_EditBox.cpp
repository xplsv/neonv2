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
#include "VJFXVar_EditBox.h"
#include "MGEditBox.h"
#include "CtrlVar.h"
#include "Link.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_EditBox::CVJFXVar_EditBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iScope, int iObject) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iScope,iObject)
{
  m_Type = EDIT_BOX;
  m_EditBox = NEW CMGEditBox(pParent, 90, ypos, 100, boost::bind(&CVJFXVar_EditBox::OnChange, this));
  m_EditBox->SetText((char *)g_pAppMain->VJController()->FXGetVar(iID, iScope, iObject, iVar));
  m_Control = m_EditBox;
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_EditBox::CVJFXVar_EditBox(CMGControl *pParent, TCtrlVar *pCtrlVar, int ypos, int iID, int iVar, int iFilterIdx) : CVJFXVar(pParent,pCtrlVar,ypos,iID,iVar,iFilterIdx)
{
  m_Type = EDIT_BOX;
  m_EditBox = NEW CMGEditBox(pParent, 90, ypos, 100, boost::bind(&CVJFXVar_EditBox::OnChange, this));
  m_EditBox->SetText((char *)g_pAppMain->VJController()->FXGetFilterVar(iID, m_FilterIdx, iVar));
  m_Control = m_EditBox;
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CVJFXVar_EditBox::~CVJFXVar_EditBox()
{
  DISPOSE(m_Link);
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
void CVJFXVar_EditBox::Set(void *pData)
{
  m_EditBox->SetText((char *)pData);
}


//---------------------------------------------------------------------------//
// OnChange
//
//---------------------------------------------------------------------------//  
void CVJFXVar_EditBox::OnChange()
{
  if (!IsFilter())
    g_pAppMain->VJController()->FXSetVar(m_ID, m_Scope, m_Object, m_Var, (void *)m_EditBox->GetText().c_str());
  else
    g_pAppMain->VJController()->FXSetFilterVar(m_ID, m_FilterIdx, m_Var, (void *)m_EditBox->GetText().c_str());
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CVJFXVar_EditBox::Load(CNodeFile::CNode *pNode)
{
  pNode = pNode->FirstNode("var");
  while (pNode && Stricmp(pNode->AttrAsString("name"),m_Label->Get()))
    pNode = pNode->NextNode("var");
  if (pNode)
    m_EditBox->SetText(pNode->AsString("value"));
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CVJFXVar_EditBox::Save(CNodeFile *pFile)
{
  pFile->WriteOpenNode ("var", "name=\"" + m_Label->Get() + "\"");
  pFile->WriteNode     ("value", "", m_EditBox->GetText());
  pFile->WriteCloseNode();
}