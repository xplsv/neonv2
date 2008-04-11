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
#include "MGPageControl.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGPage::CMGPage(CMGPageControl *pPageControl, const string &sCaption) : CMGControl(pPageControl, 0,0,0,0)
{
  m_Align        = MGALIGN_CLIENT;
  m_Caption      = sCaption;
  m_AddRectAlign = false;
  pPageControl->AddPage(this);
  OnResize();
}


//---------------------------------------------------------------------------//
// Destruct
//
//---------------------------------------------------------------------------//
CMGPage::~CMGPage()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGPageControl::CMGPageControl(CMGControl *pParent, int iX, int iY, int iW, int iH, int Align, boost::function<void()> fOnSelectPage) : CMGControl(pParent, iX, iY, iW, iH)
{
  m_ActivePage   = NULL;
  m_Align        = Align;
  m_OnSelectPage = fOnSelectPage;
  SetRect (m_Border, 2,23,2,2);
  OnResize();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGPageControl::~CMGPageControl()
{
  CMGControl::~CMGControl();
}


//---------------------------------------------------------------------------//
// Addson
//
//---------------------------------------------------------------------------//
void CMGPageControl::AddPage(CMGPage *pPage)
{
  m_Pages.Inserta(pPage);
  g_pMGApp->RepaintItem(this);
  if (m_ActivePage == NULL)
    SetActivePage(pPage);
  else
  {
    pPage->SetEnabled(false);
    pPage->SetVisible(false);
  }
}


//---------------------------------------------------------------------------//
// SetActivePage
//
//---------------------------------------------------------------------------//
void CMGPageControl::SetActivePage(int Page)
{
  CListaIter<CMGPage *> Iter(m_Pages);
  int i = 0;
  while (!Iter.EsFinal() && i < Page)
  {
    i++;
    Iter++;
  }
  if (!Iter.EsFinal())
    SetActivePage(Iter);
}


//---------------------------------------------------------------------------//
// SetActivePage
//
//---------------------------------------------------------------------------//
void CMGPageControl::SetActivePage(CMGPage *pPage)
{
  if (pPage != m_ActivePage)
  {
    if (m_ActivePage)
    {
      m_ActivePage->SetEnabled(false);
      m_ActivePage->SetVisible(false);
    }
    m_ActivePage = pPage;
    if (m_ActivePage)
    {
      m_ActivePage->SetEnabled(true);
      m_ActivePage->SetVisible(true);
    }
    m_ActivePage->Repaint();
    // Active page
    CListaIter<CMGPage *> Iter(m_Pages);
    int i = 0;
    m_iActivePage = -1;
    while (!Iter.EsFinal() && m_iActivePage == -1)
    {
      CMGPage *lPage = Iter;
      if (lPage == pPage)
        m_iActivePage = i;
      i++;
      Iter++;
    }
    // Notify
    if (m_OnSelectPage)
      m_OnSelectPage();
    Repaint();
  }
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGPageControl::DerivDraw()
{
  g_pMGDD->FillRect(TRect(Left(),   Top(),    Width(),   Height()),    g_pMGDD->Skin()->Color(COL_BASE_BG));
  g_pMGDD->Rect    (TRect(Left()+1, Top()+1,  Width()-2, Height()-2),  g_pMGDD->Skin()->Color(COL_BASE_BORDER));
  int iPages = m_Pages.Cuenta();
  if (iPages > 0)
  {
    int iW = (Width()-4) / iPages;
    int iX = Left()+2;
    int iY = Top ()+2;
    int i  = 0;
    CListaIter<CMGPage *> Iter(m_Pages);
    while (!Iter.EsFinal())
    {
      if (i == (iPages-1))
        iW = (Left()+Width()-2)-iX;
      CMGPage *pPage = Iter;
      Iter++;
      // El no activo lo pintamos de otro color y le hacemos una ralla
      if (m_ActivePage != pPage)
      {
        g_pMGDD->FillRect(TRect(iX, iY, iW, 20), g_pMGDD->Skin()->Color(COL_TAB_BG));
        g_pMGDD->HLine   (iX, Top()+22, iW, g_pMGDD->Skin()->Color(COL_BASE_BORDER));
      }
      if (pPage->GetCaption() != "")
      {
        g_pMGDD->TextDraw(FONT_NORMAL, iX,iY, iW,20, CMGFont::HCENTER, CMGFont::VCENTER, g_pMGDD->Skin()->Color(COL_FONT_LABEL), pPage->GetCaption().c_str());
      }
      if (i > 0)
        g_pMGDD->VLine(iX, iY, 20, g_pMGDD->Skin()->Color(COL_BASE_BORDER));
      iX+=iW;
      i++;
    }
  }
}


//---------------------------------------------------------------------------//
// OnMouseDown
//
//---------------------------------------------------------------------------//
void CMGPageControl::OnMouseDown(int iX, int iY, int iButton)
{
  if (iButton == 1)
  {
    iX-=Left();
    iY-=Top ();
    if (iY < 24 && iY > 1 && iX > 1 && iX < (Width()-2))
    {
      int iPages = m_Pages.Cuenta();
      if (iPages > 0 )
      {
        int iW = (Width()-4) / iPages;
        SetActivePage((iX-3) / iW);
      }
    }
    SetFocus(true);
  }
}
