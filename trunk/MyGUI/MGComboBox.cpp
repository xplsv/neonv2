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
#include "MGComboBox.h"
#include "MGScrollBar.h"
#include <stdio.h>


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGComboBox::CMGComboBox(CMGControl *pParent, int iX, int iY, int iSize, int iMaxHeight, bool bNotifyAlways, boost::function<void()> fOnChange) : CMGControl(pParent, iX,iY,iSize,20)
{
  m_Selected       = 0;
  m_MaxHeight      = iMaxHeight;
  m_OnChange       = fOnChange;
  m_ButtonPressed  = false;
  m_NotifyAlways   = bNotifyAlways;
  OnResize();
  m_ItemHeight     = (g_pMGDD->Skin()->Font(FONT_NORMAL)->TextHeight()+2);
  m_UnfoldedHeight = m_ItemHeight * m_MaxHeight + 1;
  m_ScrollBar = NEW CMGScrollBar(this, true, Width()-16, 20, m_UnfoldedHeight-2, boost::bind(&CMGComboBox::OnChangeScroll, this));
  m_ScrollBar->SetVisible(false);
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGComboBox::~CMGComboBox()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Clear
//
//---------------------------------------------------------------------------//
void CMGComboBox::Clear()
{
  m_Strings.Vacia();
  m_Text     = "";
  m_Selected = -1;
}


//---------------------------------------------------------------------------//
// Add
//
//---------------------------------------------------------------------------//
void CMGComboBox::Add(const string &sText)
{
  m_Strings.Inserta(sText);
  if (m_Strings.Cuenta() == 1)
    m_Selected = 0;
}


//---------------------------------------------------------------------------//
// IndexOf
//
//---------------------------------------------------------------------------//
int CMGComboBox::IndexOf(const string &sText)
{
  CListaIter<string> Iter(m_Strings);
  int i = 0;
  while (!Iter.EsFinal())
  {
    string text = Iter;
    if (text == sText)
      return i;
    i++;
    Iter++;
  }
  return -1;
}


//---------------------------------------------------------------------------//
// Select
//
//---------------------------------------------------------------------------//
void CMGComboBox::Select(int iIndex)
{
  string sNew = "";
  if (iIndex < 0) iIndex = 0;
  if (iIndex >= (int)m_Strings.Cuenta()) iIndex = 0;
  m_Selected = iIndex;
  CListaIter<string> Iter(m_Strings);
  int i = 0;
  while (!Iter.EsFinal())
  {
    if (i == iIndex)
    {
      sNew = Iter;
      break;
    }
    i++;
    Iter++;
  }
  if (m_NotifyAlways || sNew != m_Text)
  {
    m_Text = sNew;
    if (m_OnChange)
      m_OnChange();
  }
  else
    m_Text = sNew;
  SetUnfolded(false);
}


//---------------------------------------------------------------------------//
// SetUnfolded
//
//---------------------------------------------------------------------------//
void CMGComboBox::SetUnfolded(bool bUnfolded)
{
  if (m_Strings.Cuenta() > 0)
  {
    m_Unfolded = bUnfolded;
    if (m_Unfolded)
    {
      if (m_Strings.Cuenta() > m_MaxHeight)
        m_ScrollBar->SetVisible(true);
      unsigned uUnfoldedHeight = m_Strings.Cuenta()*m_ItemHeight + 1;
      uUnfoldedHeight = MIN(uUnfoldedHeight, m_UnfoldedHeight);
      SetSize(Width(), 20 + uUnfoldedHeight);
      g_pMGApp->TopItem(this);
    }
    else
    {
      SetSize(Width(), 20);
      m_ScrollBar->SetVisible(false);
      g_pMGApp->TopItem(false);
    }
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGComboBox::OnMouseDown(int iX, int iY, int iButton)
{
  if (m_Strings.Cuenta() == 0) return;

  if (iButton == 1)
  {
    if (m_Unfolded)
    {
      if (IsInside(iX,iY))
      {
        iY-=Top ();
        iX-=Left();
        if (iY > 20)
        {
          if (m_ScrollBar->IsVisible())
          {
            if (iX < (Width()-16))
            {
              int Offset = (int)(m_ScrollBar->GetPos() * m_UnfoldedHeight);
              Select((iY-21+Offset) / (m_ItemHeight));
            }
            else
              m_ScrollBar->OnMouseDown(iX+Left(),iY+Top(),iButton);
          }
          else
          {
            int Offset = (int)(m_ScrollBar->GetPos() * m_UnfoldedHeight);
            Select((iY-21+Offset) / (m_ItemHeight));
          }
        }
        else
          SetUnfolded(false);
      }
      else
      {
        SetUnfolded(false);
      }
    }
    else
    {
      m_ButtonPressed = true;
      SetUnfolded(true);
    }
    SetFocus(true);
  }
  else if (m_ScrollBar->IsVisible())
  {
    if (iButton == 4)
    {
      m_ScrollBar->SetPos(m_ScrollBar->GetPos()-0.1f);
      iX-=Left();
      iY-=Top ();
      if (iX > Left() && iX < Width() && iY > 20)
      {
        int Offset = (int)(m_ScrollBar->GetPos() * m_UnfoldedHeight);
        int iOldSelected = m_Selected;
        m_Selected = (iY-21+Offset) / m_ItemHeight;
        if (iOldSelected != m_Selected)
          Repaint();
      }
    }
    else if (iButton == 5)
    {
      m_ScrollBar->SetPos(m_ScrollBar->GetPos()+0.1f);
      iX-=Left();
      iY-=Top ();
      if (iX > Left() && iX < Width() && iY > 20)
      {
        int Offset = (int)(m_ScrollBar->GetPos() * m_UnfoldedHeight);
        int iOldSelected = m_Selected;
        m_Selected = (iY-21+Offset) / m_ItemHeight;
        if (iOldSelected != m_Selected)
          Repaint();
      }
    }
  }
}


//---------------------------------------------------------------------------//
// OnMouseUp
//
//---------------------------------------------------------------------------//
void CMGComboBox::OnMouseUp(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    m_ButtonPressed = false;
    if (IsInside(iX,iY))
    {
      iY-=Top ();
      iX-=Left();
      if (iY > 20)
      {
        if (m_ScrollBar->IsVisible())
        {
          if (iX < (Width()-16))
          {
            int Offset = (int)(m_ScrollBar->GetPos() * m_UnfoldedHeight);
            Select((iY-21+Offset) / (m_ItemHeight));
          }
          else
            m_ScrollBar->OnMouseDown(iX+Left(),iY+Top(),iButton);
        }
        else
        {
          int Offset = (int)(m_ScrollBar->GetPos() * m_UnfoldedHeight);
          Select((iY-21+Offset) / (m_ItemHeight));
        }
      }
    }
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGComboBox::OnMouseMove(int iX, int iY)
{
  if (m_Unfolded)
  {
    iX-=Left();
    iY-=Top ();
    if (iX >= 0 && iX < (Width()) && iY > 20)
    {
      int Offset = (int)(m_ScrollBar->GetPos() * m_UnfoldedHeight);
      int iOldSelected = m_Selected;
      m_Selected = (iY-21+Offset) / m_ItemHeight;
      if (iOldSelected != m_Selected)
        Repaint();
    }
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGComboBox::DerivDraw()
{
  float fDepth = m_Unfolded ? 0.1f : 0.f;
  g_pMGDD->Rect       (TRect(Left(),   Top(),   Width(),    20), g_pMGDD->Skin()->Color(COL_EDIT_BORDER), fDepth);
  g_pMGDD->FillRect   (TRect(Left()+1, Top()+1, Width()-2,  18), g_pMGDD->Skin()->Color(COL_EDIT_BG), fDepth);
  g_pMGDD->SetClipRect(TRect(Left()+1, Top()+1, Width()-16, 18));
  g_pMGDD->TextDraw   (FONT_NORMAL, Left()+3, Top(), 0,20, CMGFont::LEFT, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_EDIT), m_Text);
  g_pMGDD->SetClipRect();
  // Button pressed
  if (m_ButtonPressed)
    g_pMGDD->DrawItem(TRect(15,70,15,18), TRect(Left()+Width()-16,Top()+1,15,18), fDepth);
  else
    g_pMGDD->DrawItem(TRect( 0,70,15,18), TRect(Left()+Width()-16,Top()+1,15,18), fDepth);
  if (m_Unfolded)
  {
    unsigned uUnfoldedHeight = m_Strings.Cuenta()*m_ItemHeight + 1;
    uUnfoldedHeight = MIN(uUnfoldedHeight, m_UnfoldedHeight);
    g_pMGDD->Rect       (TRect(Left(),   Top()+19, Width(),   uUnfoldedHeight),   g_pMGDD->Skin()->Color(COL_EDIT_BORDER), fDepth);
    g_pMGDD->FillRect   (TRect(Left()+1, Top()+20, Width()-2, uUnfoldedHeight-2), g_pMGDD->Skin()->Color(COL_EDIT_BG), fDepth);
    g_pMGDD->SetClipRect(TRect(Left()+1, Top()+20, Width()-2, uUnfoldedHeight-2));
    CListaIter<string> Iter(m_Strings);
    int iY = Top()+20 - (int)(m_ScrollBar->GetPos() * uUnfoldedHeight);
    int i = 0;
    while (!Iter.EsFinal())
    {
      //if (iY >= (-m_ItemHeight) && iY < (m_UnfoldedHeight-1))
      {
        string sText = Iter;
        if (i == m_Selected)
        {
          g_pMGDD->FillRect(TRect(Left()+1,iY,Width()-17,m_ItemHeight), g_pMGDD->Skin()->Color(COL_EDIT_BG_ACTIVE), fDepth);
          g_pMGDD->TextDraw(FONT_NORMAL, Left()+3, iY+1, 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT_ACTIVE), sText);
        }
        else
        {
          g_pMGDD->TextDraw(FONT_NORMAL, Left()+3, iY+1, 0,0, CMGFont::LEFT, CMGFont::TOP, g_pMGDD->Skin()->Color(COL_FONT_EDIT), sText);
        }
      }
      iY+=m_ItemHeight;
      Iter++;
      i++;
    }
    g_pMGDD->SetClipRect();
  }
}


//---------------------------------------------------------------------------//
// OnChangeScroll
//
//---------------------------------------------------------------------------//
void CMGComboBox::OnChangeScroll()
{
  Repaint();
}