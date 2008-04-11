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

#include <MGPch.h>
#include "MGTreeView.h"
#include "MGScrollBar.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGTreeItem::CMGTreeItem(CMGTreeItem *pParent, const string &sText, void *pData, bool bFolder)
{
  m_Text     = sText;
  m_Folder   = bFolder;
  m_Parent   = pParent;
  m_Data     = pData;
  m_Expanded = false;
  m_Selected = false;
  m_Rect     = TRect(0,0, g_pMGDD->Skin()->Font(FONT_NORMAL)->TextLength(m_Text)+2, g_pMGDD->Skin()->Font(FONT_NORMAL)->TextHeight()+2);
  if (pParent)
    pParent->m_Sons.Inserta(this);
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGTreeItem::~CMGTreeItem()
{
  m_Sons.VaciaConDispose();
}


//---------------------------------------------------------------------------//
// SetPos
//
//---------------------------------------------------------------------------//
void CMGTreeItem::SetPos(int iX, int iY)
{
  m_Rect.x = iX;
  m_Rect.y = iY;
  if (m_Expanded)
  {
    CListaIter<CMGTreeItem *> Iter(m_Sons);
    bool First = true;
    while (!Iter.EsFinal())
    {
      CMGTreeItem *pItem = Iter;
      Iter++;
      pItem->SetPos(iX+18, iY+Height());
      iY+=pItem->GetTotalHeight();
    }
  }
}


//---------------------------------------------------------------------------//
// GetHeightSons
//
//---------------------------------------------------------------------------//
int CMGTreeItem::CalcHeight()
{
  m_LastHeight = 0;
  m_SonsHeight = 0;
  if (m_Expanded)
  {
    CListaIter<CMGTreeItem *> Iter(m_Sons);
    while (!Iter.EsFinal())
    {
      CMGTreeItem *pItem = Iter;
      Iter++;
      m_LastHeight = m_SonsHeight;
      m_SonsHeight+=pItem->CalcHeight();
    }
  }
  m_TotalHeight = m_SonsHeight + Height() + 2;
  return m_TotalHeight;
}


//---------------------------------------------------------------------------//
// Expand
//
//---------------------------------------------------------------------------//
void CMGTreeItem::Expand(bool All)
{
  m_Expanded = true;
  if (All)
  {
    CListaIter<CMGTreeItem *> Iter(m_Sons);
    while (!Iter.EsFinal())
    {
      CMGTreeItem *pItem = Iter;
      Iter++;
      pItem->Expand(true);
    }
  }
}


//---------------------------------------------------------------------------//
// Collapse
//
//---------------------------------------------------------------------------//
void CMGTreeItem::Collapse(bool All)
{
  m_Expanded = false;
  if (All)
  {
    CListaIter<CMGTreeItem *> Iter(m_Sons);
    while (!Iter.EsFinal())
    {
      CMGTreeItem *pItem = Iter;
      Iter++;
      pItem->Collapse(true);
    }
  }
}


//---------------------------------------------------------------------------//
// GetItemAt
//
//---------------------------------------------------------------------------//
CMGTreeItem *CMGTreeItem::GetItemAt(int iX, int iY)
{
  if (iX >= Left() && iX < (Left() + Width() + 38) && iY >= Top() && iY < (Top() + Height()))
    return this;
  if (m_Expanded)
  {
    CListaIter<CMGTreeItem *> Iter(m_Sons);
    while (!Iter.EsFinal())
    {
      CMGTreeItem *pItem   = Iter;
      CMGTreeItem *pItemAt = pItem->GetItemAt(iX,iY);
      if (pItemAt)
        return pItemAt;
      Iter++;
    }
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CMGTreeItem::Draw(int iYOffset)
{
  int iX = Left();
  int iY = Top () - iYOffset;
  // Icon + Lines
  g_pMGDD->HLine(iX+7,iY+8, 22, g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
  if (m_Folder)
    g_pMGDD->DrawItem(TRect(0, 200,16,16), TRect(iX+18, iY+1,16,16));
  else
    g_pMGDD->DrawItem(TRect(16,200,16,16), TRect(iX+18, iY+1,16,16));
  // Item
  if (m_Selected)
  {
    g_pMGDD->FillRect(TRect(iX+37,iY,m_Rect.w,m_Rect.h), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE));
    g_pMGDD->TextDraw(FONT_NORMAL, iX+38, iY+1, 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT_ACTIVE), m_Text);
  }
  else
  {
    g_pMGDD->TextDraw(FONT_NORMAL, iX+38, iY+1, 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Text);
  }
  
  if (m_Sons.Cuenta() > 0)
  {
    if (m_Expanded)
    {
      // Line + box
      g_pMGDD->DrawItem(TRect(189,30,9,9), TRect(iX+3,iY+4,9,9));
      g_pMGDD->VLine(iX+25,iY+13, GetLastHeight()+12, g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
      // Draw Sons
      CListaIter<CMGTreeItem *> Iter(m_Sons);
      while (!Iter.EsFinal())
      {
        CMGTreeItem *pItem = Iter;
        pItem->Draw(iYOffset);
        Iter++;
      }
    }
    else
    {
      g_pMGDD->DrawItem(TRect(180,30,9,9), TRect(iX+3,iY+4,9,9));
    }
  }
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGTreeView::CMGTreeView(CMGControl *pParent, int iX, int iY, int iW, int iH, int Align) : CMGControl(pParent,iX,iY,iW,iH)
{
  m_Align  = Align;
  m_Border = TRect(2,2,2,2);
  m_Root   = NULL;
  m_Selected = NULL;
  OnResize();
  m_ScrollBar = NEW CMGScrollBar(this, true, boost::bind(&CMGTreeView::OnChangeScroll, this));
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGTreeView::~CMGTreeView()
{
  DISPOSE(m_Root);
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//
void CMGTreeView::Update()
{
  m_Root->CalcHeight();
  m_Root->SetPos    (Left()+4, Top()+4);
  m_ScrollBar->SetVisible(m_Root->GetTotalHeight() > Height());
  Repaint();
}


//---------------------------------------------------------------------------//
// Clear
//
//---------------------------------------------------------------------------//
void CMGTreeView::Clear()
{
  m_Selected = NULL;
  DISPOSE(m_Root);
  SetRootItem(NULL);
  Repaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGTreeView::OnMouseMove(int iX, int iY)
{
  CMGControl::OnMouseMove(iX,iY);
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGTreeView::OnMouseDown(int iX, int iY, int iButton)
{
  CMGControl::OnMouseDown(iX,iY,iButton);

  if (iButton == 1)
  {
    int Offset = m_ScrollBar->IsVisible() ? (int)(m_ScrollBar->GetPos() * (m_Root->GetTotalHeight() - Height())) : 0;
    CMGTreeItem *pItemAt = GetItemAt(iX, iY+Offset);
    if (pItemAt)
    {
      if (pItemAt->IsFolder())
      {
        if (iX < (pItemAt->Left()+12))
        {
          if (pItemAt->IsExpanded())
            pItemAt->Collapse();
          else
            pItemAt->Expand();
        }
        else
        {
          SelectItem(pItemAt);
        }
      }
      else
      {
        if (iX >= (pItemAt->Left()+12))
        {
          SelectItem(pItemAt);
          BeginDrag ();
        }
      }
    }
  }
  else if (m_ScrollBar->IsVisible())
  {
    if (iButton == 4)
      m_ScrollBar->SetPos(m_ScrollBar->GetPos()-0.1f);
    else if (iButton == 5)
      m_ScrollBar->SetPos(m_ScrollBar->GetPos()+0.1f);
  }
  Update();
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGTreeView::OnMouseUp(int iX, int iY, int iButton)
{
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGTreeView::DerivDraw()
{
  int Offset = m_ScrollBar->IsVisible() ? (int)(m_ScrollBar->GetPos() * (m_Root->GetTotalHeight() - Height() + 4)) : 0;
  g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2), g_pMGDD->Skin()->Color(COL_EDIT_BG));
  g_pMGDD->Rect(TRect(Left()+1, Top()+1, Width()-2, Height()-2), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
  g_pMGDD->SetClipRect(TRect(Left()+2, Top()+6, Width()-4, Height()-8));
  m_Root->Draw(Offset);
  g_pMGDD->SetClipRect();
}


//---------------------------------------------------------------------------//
// OnChangeScroll
//
//---------------------------------------------------------------------------//
void CMGTreeView::OnChangeScroll()
{
  Repaint();
}


//---------------------------------------------------------------------------//
// SelectItem
//
//---------------------------------------------------------------------------//
void CMGTreeView::SelectItem(CMGTreeItem *pItem)
{
  if (m_Selected) m_Selected->SetSelected(false);
  m_Selected = pItem;
  if (m_Selected) m_Selected->SetSelected(true);
}


//---------------------------------------------------------------------------//
// GetItemAt
//
//---------------------------------------------------------------------------//
CMGTreeItem *CMGTreeView::GetItemAt(int iX, int iY)
{
  return m_Root->GetItemAt(iX,iY);
}
