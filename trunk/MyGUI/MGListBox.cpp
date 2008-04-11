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
#include "MGListBox.h"
#include "MGScrollBar.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGListBox::CMGListBox(CMGControl *pParent, int iX, int iY, int iW, int iH, int Align, bool bScrollBar) : CMGControl(pParent,iX,iY,iW,iH)
{
  m_Name       = "ListBox";
  m_Align      = Align;
  m_Border     = TRect(2,2,2,2);
  m_Selected   = -1;
  m_ItemHeight = g_pMGDD->Skin()->Font(FONT_NORMAL)->TextHeight()+2;
  m_ScrollBar  = bScrollBar ? NEW CMGScrollBar(this, true, boost::bind(&CMGListBox::OnChangeScroll, this)) : NULL;
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGListBox::~CMGListBox()
{
}


//---------------------------------------------------------------------------//
// Add
//
//---------------------------------------------------------------------------//
void CMGListBox::Add(string const &sItem)
{
  m_Items.push_back(sItem);
  Update();
}


//---------------------------------------------------------------------------//
// Clear
//
//---------------------------------------------------------------------------//
void CMGListBox::Clear()
{
  m_Selected = -1;
  m_Items.clear();
  Update();
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//
void CMGListBox::Update()
{
  if (m_ScrollBar)
    m_ScrollBar->SetVisible(GetTotalHeight() > Height());
  Repaint();
}


//---------------------------------------------------------------------------//
// GetTotalHeight
//
//---------------------------------------------------------------------------//
int CMGListBox::GetTotalHeight()
{
  return (m_Items.size() * m_ItemHeight);
}


//---------------------------------------------------------------------------//
// GetItemIndexAt
//
//---------------------------------------------------------------------------//
int CMGListBox::GetItemIndexAt(int iX, int iY)
{
  int iIndex = -1;
  iX-=Left();
  iY-=Top ();
  if (iX >= 0 && iX < Width() && iY >= 0 && iY < Height())
  {
    if (GetTotalHeight() >= Height() || iY < GetTotalHeight())
    {
      int Offset = iY - 2 + (m_ScrollBar && m_ScrollBar->IsVisible() ? (int)(m_ScrollBar->GetPos() * (GetTotalHeight() - Height() + 4)) : 0);
      iIndex = int((float)Offset / (float)m_ItemHeight);
    }
  }
  return iIndex;
}


//---------------------------------------------------------------------------//
// Select
//
//---------------------------------------------------------------------------//
void CMGListBox::Select(int iItem)
{
  m_Selected = iItem;
  if (m_OnSelectItem)
    m_OnSelectItem(this);
  Update();
}


//---------------------------------------------------------------------------//
// Select
//
//---------------------------------------------------------------------------//
bool CMGListBox::Select(string const &sText)
{
  for (size_t i = 0; i < m_Items.size(); i++)
  {
    if (sText == m_Items[i])
    {
      Select(i);
      return true;
    }
  }
  Select(0);
  return false;
}


//---------------------------------------------------------------------------//
// GetSelected
//
//---------------------------------------------------------------------------//
bool CMGListBox::GetSelected(string &rSelected)
{
  if (m_Selected != -1)
  {
    rSelected = m_Items[m_Selected];
    return true;
  }
  return false;
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGListBox::OnMouseMove(int iX, int iY)
{
  CMGControl::OnMouseMove(iX,iY);
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGListBox::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    int iItem  = GetItemIndexAt(iX,iY);
    if (iItem != -1)
    {
      Select(iItem);
    }
    SetFocus(true);
  }
  else if (m_ScrollBar && m_ScrollBar->IsVisible())
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
void CMGListBox::OnMouseUp(int iX, int iY, int iButton)
{
}


//---------------------------------------------------------------------------//
// OnDragOver
//
//---------------------------------------------------------------------------//
void CMGListBox::OnDragOver(CMGControl *pSource, int iX, int iY, bool &Accept)
{
  Accept = false;
  if (m_OnDragOver)
    m_OnDragOver(this, pSource, iX, iY, Accept);
}


//---------------------------------------------------------------------------//
// OnDragDrop
//
//---------------------------------------------------------------------------//
void CMGListBox::OnDragDrop(CMGControl *pSource, int iX, int iY)
{
  if (m_OnDragDrop)
    m_OnDragDrop(this, pSource, iX, iY);
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGListBox::DerivDraw()
{
  g_pMGDD->FillRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2), g_pMGDD->Skin()->Color(COL_EDIT_BG));
  g_pMGDD->Rect(TRect(Left(), Top(), Width(), Height()), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));
  g_pMGDD->SetClipRect(TRect(Left()+1, Top()+1, Width()-2, Height()-2));
  // Draw all lines
  int iX = Left();
  int iY = Top ();
  int Offset = iY + 1 - (m_ScrollBar && m_ScrollBar->IsVisible() ? (int)(m_ScrollBar->GetPos() * (GetTotalHeight() - Height() + 4)) : 0);
  for (size_t i = 0; i < m_Items.size(); i++)
  {
    if (Offset > (iY - m_ItemHeight) && Offset < (iY + Height()))
    {
      if (i == m_Selected)
      {
        if (m_ScrollBar)
          g_pMGDD->FillRect(TRect(iX+1,Offset,Width()-18,m_ItemHeight), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE));
        else
          g_pMGDD->FillRect(TRect(iX+1,Offset,Width()-2,m_ItemHeight), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE));
        g_pMGDD->TextDraw(FONT_NORMAL, iX+2, Offset, 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT_ACTIVE), m_Items[i]);
      }
      else
      {
        g_pMGDD->TextDraw(FONT_NORMAL, iX+2, Offset, 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Items[i]);
      }
    }
    Offset+= m_ItemHeight;
  }
  g_pMGDD->SetClipRect();
}


//---------------------------------------------------------------------------//
// OnChangeScroll
//
//---------------------------------------------------------------------------//
void CMGListBox::OnChangeScroll()
{
  Repaint();
}
