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
#include "MGPanel.h"
#include "MGListBox.h"
#include "MGFXFilters.h"
#include "MGFXFilter.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGFXFilters::CMGFXFilters(CMGControl *pParent, CMGEffect *pEffect) : CMGControl(pParent, 0,0,0,0)
{
  m_Align      = MGALIGN_CLIENT;
  m_FilterVars = NULL;

  m_PanelSup = NEW CMGPanel(this, 0,0,0,118, MGALIGN_TOP);
  m_PanelInf = NEW CMGPanel(this, 0,0,0,0, MGALIGN_CLIENT);

  m_LBFilters = NEW CMGListBox(m_PanelSup, 1,0,294,81, MGALIGN_NONE, true);
  m_LBFilters->m_OnSelectItem = boost::bind(&CMGFXFilters::OnSelectFilter,this,_1);
  for (size_t i = 0; i < g_pAppMain->FilterList().size(); i++)
      m_LBFilters->Add(g_pAppMain->FilterList()[i]);

  // Filter slots
  for (int i = 0; i < MAX_FILTERS; i++)
    m_FXFilter[i] = NEW CMGFXFilter(m_PanelSup, this, pEffect, 1+59*i, 82, i);

  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGFXFilters::~CMGFXFilters()
{
  if (m_FilterVars)
  {
    m_FilterVars->SetParent(NULL);
    m_FilterVars = NULL;
  }
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// SwapFilters
//
//---------------------------------------------------------------------------//  
void CMGFXFilters::SwapFilters(int iSlotSrc, int iSlotDst)
{
  swap(m_FXFilter[iSlotSrc], m_FXFilter[iSlotDst]);
}


//---------------------------------------------------------------------------//
// OnSelectFilter
//
//---------------------------------------------------------------------------//  
void CMGFXFilters::OnSelectFilter(CMGListBox *pListBox)
{
  string sItem;
  if (pListBox->GetSelected(sItem))
      pListBox->BeginDrag();
}


//---------------------------------------------------------------------------//
// ShowFXFilter
//
//---------------------------------------------------------------------------//  
void CMGFXFilters::ShowFilterVars(CMGControl *pFXFilter)
{
  if (m_FilterVars)
    m_FilterVars->SetParent(NULL);
  m_FilterVars = pFXFilter;
  if (m_FilterVars)
    m_FilterVars->SetParent(m_PanelInf);
  GetParent()->OnResize();
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//  
void CMGFXFilters::Update()
{
  for (int i = 0; i < MAX_FILTERS; i++)
    m_FXFilter[i]->Update();
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CMGFXFilters::Load(CNodeFile::CNode *pNode)
{
  CNodeFile::CNode *pFilter = pNode->FirstNode("filter");
  while (pFilter)
  {
    int cell = pFilter->AttrAsInt("cell");
    const string &sName = pFilter->AttrAsString("name");
    if (cell >= 0 && cell < MAX_FILTERS && sName != "")
    {
      if (m_FXFilter[cell]->Load(sName.c_str()))
        m_FXFilter[cell]->Load(pFilter);
    }
    pFilter = pFilter->NextNode("filter");
  }
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CMGFXFilters::Save(CNodeFile *pFile)
{
  for (int i = 0; i < MAX_FILTERS; i++)
  {
    if (m_FXFilter[i]->IsLoaded())
    {
      char pBuffer[100];
      sprintf_s(pBuffer, 100, "name=\"%s\" cell=\"%d\"", m_FXFilter[i]->GetFilterName().c_str(), i);
      pFile->WriteOpenNode("filter", pBuffer);

        m_FXFilter[i]->Save(pFile);

      pFile->WriteCloseNode();
    }
  }
}
