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
#include "MGGridPanel.h"
#include "MGPageControl.h"
#include "MGGroupBox.h"
#include "MGAppMain.h"
#include "MGEffect.h"
#include "MGTreeView.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGGridPanel::CMGGridPanel(CMGControl *pParent) : CMGControl(pParent, 0,0,0,0)
{
  m_Align = MGALIGN_CLIENT;

  // CCreate grid
  for (int i = 0; i < 10; i++)
    m_GridFX[i] = NULL;
  m_GridFXSelected = NULL;
  CMGGroupBox *pGB = NEW CMGGroupBox (this, 0,0, 720, 88, MGALIGN_NONE, g_pMGDD->Skin()->Color(COL_BASE_BORDER_ACTIVE));
  m_GridMasterFX   = NEW CMGGridFX   (pGB, 10,1, 1.20f, 2,2, 703,83, false);
  m_GridMasterFX->Transparent(true);
  m_PCGrid       = NEW CMGPageControl(this, 0,88, 720,649, MGALIGN_NONE, boost::bind(&CMGGridPanel::OnSelectGridPage, this));
  for (int i = 0; i < 10; i++)
  {
    m_GridMasterFX->GetCellData(i,0)->Init(true);
    char pBuffer[16];
    sprintf_s(pBuffer, 16, "Tab %d", i);
    CMGPage *pPage = NEW CMGPage(m_PCGrid, pBuffer);
    m_GridFX[i] = NEW CMGGridFX(pPage, 10,8, 1.1f, 0,2, 703,631, false);
    // Assign callbacks
    m_GridFX[i]->onMouseDown = boost::bind(&CMGGridPanel::OnGridMouseDown, this, _1,_2,_3,_4);
    m_GridFX[i]->onDragOver  = boost::bind(&CMGGridPanel::OnGridDragOver,  this, _1,_2,_3,_4,_5);
    m_GridFX[i]->onDragDrop  = boost::bind(&CMGGridPanel::OnGridDragDrop,  this, _1,_2,_3,_4);
    m_GridFX[i]->onKeyDown   = boost::bind(&CMGGridPanel::OnGridKeyDown,   this, _1,_2);
    m_GridFX[i]->Transparent(true);
  }
  m_GridFXSelected = m_GridFX[0];
  // Assign callbacks
  m_GridMasterFX->onMouseDown = boost::bind(&CMGGridPanel::OnGridMouseDown, this, _1,_2,_3,_4);
  m_GridMasterFX->onDragOver  = boost::bind(&CMGGridPanel::OnGridDragOver,  this, _1,_2,_3,_4,_5);
  m_GridMasterFX->onDragDrop  = boost::bind(&CMGGridPanel::OnGridDragDrop,  this, _1,_2,_3,_4);
  m_GridMasterFX->onKeyDown   = boost::bind(&CMGGridPanel::OnGridKeyDown,   this, _1,_2);

  OnResize();
}
  

//---------------------------------------------------------------------------//
// FXUnload
//
//---------------------------------------------------------------------------//
void CMGGridPanel::FXUnload(CMGEffect *pEffect)
{
  if (pEffect)
  {
    if (pEffect->IsAttached())
      m_GridMasterFX->GetCellData(pEffect->GetLayerAttached(),0)->SetEffect(NULL);
    DISPOSE(pEffect);
    g_pMGApp->FocusItem(NULL);
  }
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGGridPanel::OnGridMouseDown(CMGGridFX *pGrid, int iX, int iY, int iButton)
{
  int col, row;
  if (pGrid->CellAtXY(iX-pGrid->Left(), iY-pGrid->Top(), col, row))
  {
    CMGGridItemFX *pItem = pGrid->GetCellData(col,row);
    if (pItem->GetEffect())
    {
      if (iButton == 1)
      {
        if (pGrid == m_GridMasterFX)
        {
          m_GridFXSelected->SelectCell();
          g_pAppMain->ShowEffect(pItem->GetEffect());
        }
        else
        {
          m_GridMasterFX->SelectCell();
          g_pAppMain->ShowEffect(pItem->GetEffect());
        }
        pGrid->SelectCell(col, row);
        pGrid->BeginDrag();
      }
      else if (iButton == 2)
      {
        if (pGrid != m_GridMasterFX)
        {
          FXUnload(pItem->GetEffect());
          pItem->SetEffect(NULL);
          Repaint();
        }
        else
        {
          pItem->GetEffect()->Unattach();
          pItem->SetEffect(NULL);
          Repaint();
        }
      }
    }
  }
}


//---------------------------------------------------------------------------//
// OnGridDragOver
//
//---------------------------------------------------------------------------//
void CMGGridPanel::OnGridDragOver(CMGGridFX *pGrid, CMGControl *pSource, int iX, int iY, bool &Accept)
{
  int col, row;
  if (pGrid->CellAtXY(iX-pGrid->Left(),iY-pGrid->Top(), col,row))
  {
    if (pGrid == m_GridMasterFX)
    {
      int scol, srow;
      Accept = true; //!pGrid->GetCellData(col,row)->GetEffect();
      if (Accept)
      {
        pGrid->GetSelectedCell(scol, srow);
        // Master FX
        if (pSource == pGrid)
          Accept = scol != col;
        else
          Accept = pSource == m_GridFXSelected;
      }
    }
    else
    {
      // Loaded FX
      Accept = (pSource == g_pAppMain->m_TreeFX || (pSource == m_GridMasterFX) || pSource == pGrid);
      Accept = Accept && !pGrid->GetCellData(col,row)->GetEffect();
    }
  }
}


//---------------------------------------------------------------------------//
// OnGridDragDrop
//
//---------------------------------------------------------------------------//
void CMGGridPanel::OnGridDragDrop(CMGGridFX *pGrid, CMGControl *pSource, int iX, int iY)
{
  if (pGrid == m_GridMasterFX)
  {
    // Dragged to Master FX Grid
    int col,row;
    if (pGrid->CellAtXY(iX-pGrid->Left(),iY-pGrid->Top(), col,row))
    {
      if (pSource == m_GridFXSelected)
      {
        CMGEffect *pEffect = m_GridFXSelected->GetSelectedCell()->GetEffect();
        // From Selected FX Grid to Master FX Grid
        CMGGridItemFX *pItem = pGrid->GetCellData(col,row);
        if (pItem->GetEffect())
        {
          if (pItem->GetEffect() == pEffect)
            return;
          pItem->GetEffect()->Unattach();
          pItem->SetEffect(NULL);
        }
        pGrid->GetCellData(col,row)->SetEffect(pEffect);
        // And attach!
        if (pEffect->IsAttached())
          pGrid->GetCellData(pEffect->GetLayerAttached(),row)->SetEffect(NULL);
        pEffect->Attach(col);
      }
      else if (pSource == pGrid)
      {
        // From Master FX Grid to Master FX Grid
        CMGEffect *pEffect = pGrid->GetSelectedCell()->GetEffect();
        pGrid->GetSelectedCell()->SetEffect(NULL);
        pGrid->GetCellData(col,row)->SetEffect(pEffect);
        pGrid->SelectCell (col,row);
        pEffect->Move(col);
      }
    }
  }
  else
  {
    // Dragged to Selected FX Grid
    if (pSource == g_pAppMain->m_TreeFX)
    {
      // From Tree FX to Selected FX Grid
      CMGTreeItem *pItem = g_pAppMain->m_TreeFX->GetSelected();
      if (pItem)
      {
        // Compose effect name based on 
        string sBuffer = ".nly";
        CMGTreeItem *pParent = pItem;
        do
        {
          const string &sText = pParent->GetText();
          if (pParent->GetParent())
            sBuffer = "\\" + sText + sBuffer;
          else
            sBuffer = sText + sBuffer;
          pParent = pParent->GetParent();
        } while (pParent);
        // Load effect
        CMGEffect *pEffect = CMGEffect::Create(sBuffer);
        if (pEffect)
        {
          int col, row;
          pGrid->CellAtXY   (iX-pGrid->Left(),iY-pGrid->Top(), col, row); // This is true, as OnDragOver we said that this pos was correct
          pGrid->GetCellData(col,row)->SetEffect(pEffect);
          pGrid->SelectCell (col,row);
        }
      }
    }
    else if (pSource == pGrid)
    {
      // From Selected FX Grid to Selected FX Grid
      CMGEffect *pEffect = pGrid->GetSelectedCell()->GetEffect();
      int col,row;
      pGrid->CellAtXY       (iX-pGrid->Left(),iY-pGrid->Top(), col, row); // This is true, as OnDragOver we said that this pos was correct
      pGrid->GetCellData    (col,row)->SetEffect(pEffect);
      pGrid->GetSelectedCell()->SetEffect(NULL);
      pGrid->SelectCell     (col,row);
    }
  }
  Repaint();
}


//---------------------------------------------------------------------------//
// OnKeyDown
//
//---------------------------------------------------------------------------//
void CMGGridPanel::OnGridKeyDown(CMGGridFX *pGrid, int vkey)
{
}


//---------------------------------------------------------------------------//
// Clear
//---------------------------------------------------------------------------//
void CMGGridPanel::Clear()
{
  for (int i = 0; i < 10; i++)
  {
    for (int col = 0; col < m_GridFX[i]->GetCols(); col++)
    {
      for (int row = 0; row < m_GridFX[i]->GetRows(); row++)
      {
        FXUnload(m_GridFX[i]->GetCellData(col,row)->GetEffect());
        m_GridFX[i]->GetCellData(col,row)->SetEffect(NULL);
      }
    }
    m_GridFX[i]->Clear();
  }
}


//---------------------------------------------------------------------------//
// Update
//---------------------------------------------------------------------------//
void CMGGridPanel::Update()
{
  for (int i = 0; i < 10; i++)
  {
    int iCells = m_GridFX[0]->GetCols() * m_GridFX[0]->GetRows();
    for (int j = 0; j < iCells; j++)
    {
      m_GridFX[i]->GetCells()[j]->Update();
    }
  }
}


//---------------------------------------------------------------------------//
// SetActiveGrid
//---------------------------------------------------------------------------//
void CMGGridPanel::SetActiveGrid(int iGrid)
{
  if (m_GridFXSelected)
    m_GridFXSelected->SelectCell();
  m_GridFXSelected = m_GridFX[iGrid];
  if (m_GridFXSelected)
    m_GridFXSelected->SelectCell();
}


//---------------------------------------------------------------------------//
// OnSelectGridPage
//---------------------------------------------------------------------------//
void CMGGridPanel::OnSelectGridPage()
{
  if (m_GridFXSelected)
    m_GridFXSelected->SelectCell(-1,-1);
  m_GridFXSelected = m_GridFX[m_PCGrid->GetActivePage()];
  if (m_GridFXSelected)
    m_GridFXSelected->SelectCell(-1,-1);
}
