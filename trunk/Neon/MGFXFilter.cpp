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
#include "MGPanel.h"
#include "MGListBox.h"
#include "MGFXFilter.h"
#include "MGFXFilters.h"
#include "MGPanelVars.h"
#include "MGEffect.h"
#include "VJFXVar.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGFXFilter::CMGFXFilter(CMGControl *pParent, CMGFXFilters *pFXFilters, CMGEffect *pEffect, int x, int y, int iSlot) : CMGControl(pParent, x,y,58,34)
{
  m_Align        = MGALIGN_NONE;
  m_Effect       = pEffect;
  m_FilterSlot   = iSlot;
  m_FilterIdx    = -1;
  m_FilterName   = "";
  m_FXFilters    = pFXFilters;
  m_VJFilterVars = NULL;

  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGFXFilter::~CMGFXFilter()
{
  CMGControl::~CMGControl();
  DISPOSE(m_VJFilterVars);
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//
bool CMGFXFilter::Load(const string &sFilter)
{
  // Unload previous
  Unload();
  // Try to load
  m_FilterIdx = g_pAppMain->VJController()->FXAddFilter(m_Effect->GetID(), m_FilterSlot, sFilter);
  if (m_FilterIdx >= 0)
  {
    m_FilterName   = sFilter;
    m_VJFilterVars = NEW CMGPanelVars(NULL, m_Effect->GetID(), m_FilterIdx);
    return true;
  }
  return false;
}


//---------------------------------------------------------------------------//
// Unload
//
//---------------------------------------------------------------------------//
void CMGFXFilter::Unload()
{
  if (IsLoaded())
  {
    if (m_FXFilters->GetFilterVars() == m_VJFilterVars)
      m_FXFilters->ShowFilterVars(NULL);
    g_pAppMain->VJController()->FXRemoveFilter(m_Effect->GetID(), m_FilterSlot);
    DISPOSE(m_VJFilterVars);
    m_FilterName = "";
  }
}


//---------------------------------------------------------------------------//
// Swap
//
//---------------------------------------------------------------------------//
void CMGFXFilter::Swap(CMGFXFilter *pOther)
{
  m_FXFilters->SwapFilters(m_FilterSlot, pOther->m_FilterSlot);
  g_pAppMain->VJController()->FXSwapFilter(m_Effect->GetID(), m_FilterSlot, pOther->m_FilterSlot);
  swap(m_FilterSlot,   pOther->m_FilterSlot);
  swap(m_FilterName,   pOther->m_FilterName);
  swap(m_VJFilterVars, pOther->m_VJFilterVars);
  m_FXFilters->Repaint();
}


//---------------------------------------------------------------------------//
// Show
//
//---------------------------------------------------------------------------//
void CMGFXFilter::Show()
{
  if (IsLoaded())
    m_FXFilters->ShowFilterVars(m_VJFilterVars);
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGFXFilter::OnMouseMove(int iX, int iY)
{
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGFXFilter::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    if (IsLoaded())
    {
      Show();
      BeginDrag();
      m_FXFilters->Repaint();
    }
    Focus();
  }
  else if (iButton == 2)
  {
    Focus();
    if (IsLoaded())
    {
      Unload();
      m_FXFilters->Repaint();
    }
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGFXFilter::OnMouseUp(int iX, int iY, int iButton)
{
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGFXFilter::OnDragOver(CMGControl *pSource, int iX, int iY, bool &Accept)
{
  if (pSource == m_Effect->GetFXFilters()->GetLBFilters() ||
      pSource == m_Effect->GetFXFilters()->GetFXFilter(0) ||
      pSource == m_Effect->GetFXFilters()->GetFXFilter(1) ||
      pSource == m_Effect->GetFXFilters()->GetFXFilter(2) ||
      pSource == m_Effect->GetFXFilters()->GetFXFilter(3) ||
      pSource == m_Effect->GetFXFilters()->GetFXFilter(4))
  {
    Accept = pSource != this;
  }
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGFXFilter::OnDragDrop(CMGControl *pSource, int iX, int iY)
{
  if (pSource == m_Effect->GetFXFilters()->GetLBFilters())
  {
    string sFilter;
    if (m_Effect->GetFXFilters()->GetLBFilters()->GetSelected(sFilter))
    {
      if (Load(sFilter))
        Show();
    }
  }
  else
  {
    Swap((CMGFXFilter *)pSource);
    Show();
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGFXFilter::DerivDraw()
{
  if (m_VJFilterVars && m_VJFilterVars->GetParent() != NULL)
    g_pMGDD->Rect(TRect(Left(), Top(), Width(), Height()), g_pMGDD->Skin()->Color(COL_EDIT_BORDER_ACTIVE));
  else
    g_pMGDD->Rect(TRect(Left(), Top(), Width(), Height()), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
  g_pMGDD->SetClipRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2));
  if (IsLoaded())
  {
    if (m_VJFilterVars && m_VJFilterVars->GetParent() != NULL)
      g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE));
    else
      g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2), g_pMGDD->Skin()->Color(COL_EDIT_BG));
    g_pMGDD->TextDraw(FONT_NORMAL, Left()+3, Top(), 0,20, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT_ACTIVE), m_FilterName);
  }
  else
  {
    g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2), g_pMGDD->Skin()->Color(COL_TAB_BG));
  }
  g_pMGDD->SetClipRect();
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//  
void CMGFXFilter::Update()
{
  if (IsLoaded())
    m_VJFilterVars->Update();
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CMGFXFilter::Load(CNodeFile::CNode *pNode)
{
  if (IsLoaded())
    m_VJFilterVars->Load(pNode);
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CMGFXFilter::Save(CNodeFile *pFile)
{
  if (IsLoaded())
    m_VJFilterVars->Save(pFile);
}
