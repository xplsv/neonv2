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
#include "MGPanelVars.h"
#include "CtrlVar.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGPanelVars::CMGPanelVars(CMGControl *pParent, int iIDEffect, int iScope, int iObject) : CMGControl(pParent, 0,0,0,0)
{
  m_Align = MGALIGN_CLIENT;

  TCtrlVar *pVars = NULL;
  pVars = g_pAppMain->VJController()->FXGetVarCtrls(iIDEffect, iScope);
  // If vars, create them
  if (pVars)
  {
    int iVar = 0;
    int ypos = 20;
    while (pVars[iVar].iType != TCtrlVar::INVALID)
    {
      CVJFXVar *pVar = CVJFXVar::Create(this, &pVars[iVar], ypos, iIDEffect, iVar, iScope, iObject);
      m_VJVars.push_back(pVar);
      if (pVar)
        ypos+=22;
      iVar++;
    }
  }
}



//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGPanelVars::CMGPanelVars(CMGControl *pParent, int iIDEffect, int iFilterIdx) : CMGControl(pParent, 0,0,0,0)
{
  m_Align = MGALIGN_CLIENT;

  TCtrlVar *pVars = NULL;
  pVars = g_pAppMain->VJController()->FXGetFilterVarCtrls(iIDEffect, iFilterIdx);
  // If vars, create them
  if (pVars)
  {
    int iVar = 0;
    int ypos = 20;
    while (pVars[iVar].iType != TCtrlVar::INVALID)
    {
      CVJFXVar *pVar = CVJFXVar::Create(this, &pVars[iVar], ypos, iIDEffect, iVar, iFilterIdx);
      m_VJVars.push_back(pVar);
      if (pVar)
        ypos+=22;
      iVar++;
    }
  }
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGPanelVars::~CMGPanelVars()
{
  for (size_t i = 0; i < m_VJVars.size(); i++)
    DISPOSE(m_VJVars[i]);
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//  
void CMGPanelVars::Update()
{
  for (size_t i = 0; i < m_VJVars.size(); i++)
    m_VJVars[i]->Evaluate();
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CMGPanelVars::Load(CNodeFile::CNode *pNode)
{
  for (size_t i = 0; i < m_VJVars.size(); i++)
    m_VJVars[i]->Load(pNode);
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CMGPanelVars::Save(CNodeFile *pFile)
{
  for (size_t i = 0; i < m_VJVars.size(); i++)
    m_VJVars[i]->Save(pFile);
}
