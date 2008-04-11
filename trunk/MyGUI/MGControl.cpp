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
#include "MGControl.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
CMGControl::CMGControl(CMGControl *pParent, int iX, int iY, int iW, int iH)
{
  m_Parent  = NULL;
  m_Align   = MGALIGN_NONE;
  m_Enabled = true;
  m_Visible = true;
  m_Focused = false;
  m_AddRectAlign = true;
  m_Transparent  = true;
  m_Color = g_pMGDD->Skin()->Color(COL_BASE_BG);
  SetRect   (m_Border, 0,0,0,0);
  SetPos    (iX, iY);
  SetSize   (iW, iH);
  SetParent (pParent);
  m_FreeRect = m_Rect;
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
CMGControl::~CMGControl()
{
  m_Sons.VaciaConDispose();
}



//---------------------------------------------------------------------------//
// SetParent
//
//---------------------------------------------------------------------------//
void CMGControl::SetParent(CMGControl *pParent)
{
  if (m_Parent) m_Parent->DelSon(this);
  m_Parent = pParent;
  if (m_Parent) m_Parent->AddSon(this);
}


//---------------------------------------------------------------------------//
// AddRectAlign
//
// Se llama cuando un hijo con align ya ha ajustado su tamaño para que los
// sucesivos hijos puedan adaptarse tambien
//---------------------------------------------------------------------------//
void CMGControl::AddRectAlign(CMGControl *pItem)
{
  switch (pItem->GetAlign())
  {
    case MGALIGN_NONE:   break;
    case MGALIGN_LEFT:   m_FreeRect.x+=pItem->Width (); m_FreeRect.w-=pItem->Width (); break;
    case MGALIGN_RIGHT:  m_FreeRect.w-=pItem->Width ();                                break;
    case MGALIGN_TOP:    m_FreeRect.y+=pItem->Height(); m_FreeRect.h-=pItem->Height(); break;
    case MGALIGN_BOTTOM: m_FreeRect.h-=pItem->Height();                                break;
    case MGALIGN_CLIENT: SetRect(m_FreeRect, 0,0,0,0);                                 break;
  }
}


//---------------------------------------------------------------------------//
// AddSon
//
//---------------------------------------------------------------------------//
void CMGControl::AddSon(CMGControl *pSon)
{
  m_Sons.Inserta(pSon);
}


//---------------------------------------------------------------------------//
// DelSon
//
//---------------------------------------------------------------------------//
void CMGControl::DelSon(CMGControl *pSon)
{
  m_Sons.Saca(pSon);
}


//---------------------------------------------------------------------------//
// GetItemAt
//
//---------------------------------------------------------------------------//
CMGControl *CMGControl::GetItemAt(int iX, int iY)
{
  if (IsInside(iX,iY))
  {
    // Mirar si algun hijo esta dentro
    CListaIter<CMGControl *> Iter(m_Sons);
    while (!Iter.EsFinal())
    {
      CMGControl *pItem = Iter;
      CMGControl *pInside = pItem->GetItemAt(iX,iY);
      if (pInside)
        return pInside;
      Iter++;
    }
    return this;
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// IsInside
//
//---------------------------------------------------------------------------//
bool CMGControl::IsInside(int iX, int iY)
{
  return (m_Enabled && m_Visible && (iX >= Left()) && (iX <= (Left() + Width())) && (iY >= Top()) && (iY <= (Top() + Height())));
}


//---------------------------------------------------------------------------//
// UpdateSize
//
//---------------------------------------------------------------------------//
void CMGControl::UpdateSize()
{
  if (m_Align != MGALIGN_NONE)
  {
    TRect Rect;
    if (m_Parent)
      m_Parent->GetFreeRect(Rect);
    else
      SetRect(Rect, 0,0, g_pMGApp->GetDisplay()->GetWidth(), g_pMGApp->GetDisplay()->GetHeight());
    switch (m_Align)
    {
      case MGALIGN_LEFT:   SetPos(Rect.x,                     Rect.y); SetSize(Width(), Rect.h);   break;
      case MGALIGN_RIGHT:  SetPos(Rect.x + Rect.w - Width(),  Rect.y); SetSize(Width(), Rect.h);   break;
      case MGALIGN_TOP:    SetPos(Rect.x,                     Rect.y); SetSize(Rect.w,  Height()); break;
      case MGALIGN_BOTTOM: SetPos(Rect.x, Rect.y + Rect.h - Height()); SetSize(Rect.w,  Height()); break;
      case MGALIGN_CLIENT: SetPos(Rect.x,                     Rect.y); SetSize(Rect.w,  Rect.h);   break;
    }
  }
}


//---------------------------------------------------------------------------//
// UpdateRect
//
//---------------------------------------------------------------------------//
void CMGControl::UpdateRect()
{
  // Update
  if (m_Align != MGALIGN_NONE && m_Parent && m_AddRectAlign)
    m_Parent->AddRectAlign(this);

  // Reset del freerect
  m_FreeRect    = TRect(0,0, m_Rect.w,m_Rect.h);
  m_FreeRect.x += m_Border.x;
  m_FreeRect.y += m_Border.y;
  m_FreeRect.w -= m_Border.w + m_Border.x;
  m_FreeRect.h -= m_Border.h + m_Border.y;
}


//---------------------------------------------------------------------------//
// OnResize
//
//---------------------------------------------------------------------------//
void CMGControl::OnResize()
{
  UpdateSize();
  UpdateRect();
  // Resize sons
  CListaIter<CMGControl *> Iter(m_Sons);
  while (!Iter.EsFinal())
  {
    CMGControl *pItem = Iter;
    pItem->OnResize();
    Iter++;
  }
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
void CMGControl::OnMouseMove(int iX, int iY)
{
  CListaIter<CMGControl *> Iter(m_Sons);
  while (!Iter.EsFinal())
  {
    CMGControl *pItem = Iter;
    if (pItem->IsInside(iX,iY))
      pItem->OnMouseMove(iX,iY);
    Iter++;
  }
}


//---------------------------------------------------------------------------//
// OnDragOver
//
//---------------------------------------------------------------------------//
void CMGControl::OnDragOver(CMGControl *pSource, int iX, int iY, bool &Accept)
{
  Accept = false;
}


//---------------------------------------------------------------------------//
// OnDragDrop
//
//---------------------------------------------------------------------------//
void CMGControl::OnDragDrop(CMGControl *pSource, int iX, int iY)
{
}


//---------------------------------------------------------------------------//
// BeginDrag
//
//---------------------------------------------------------------------------//
void CMGControl::BeginDrag()
{
  g_pMGApp->DragItem(this);
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CMGControl::Run()
{
  if (!m_Visible) return;
  // Run de la derivada
  DerivRun();
  // Run de los hijos
  CListaIter<CMGControl *> Iter(m_Sons);
  while (!Iter.EsFinal())
  {
    CMGControl *pItem = Iter;
    if (pItem->m_Enabled)
      pItem->Run();
    Iter++;
  }
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CMGControl::Draw()
{
  if (!IsVisible()) return;
  if (!m_Transparent)
    g_pMGDD->FillRect(TRect(Left(), Top(), Width(), Height()), m_Color);
  // Draw de la derivada
  DerivDraw();
  // Draw de los hijos
  CListaIter<CMGControl *> Iter(m_Sons);
  while (!Iter.EsFinal())
  {
    CMGControl *pItem = Iter;
    if (pItem->m_Visible)
      pItem->Draw();
    Iter++;
  }
}


//---------------------------------------------------------------------------//
// Repaint
//
//---------------------------------------------------------------------------//
void CMGControl::Repaint()
{
  g_pMGApp->RepaintItem(this);
}


//---------------------------------------------------------------------------//
// Focus
//
//---------------------------------------------------------------------------//
void CMGControl::Focus()
{
  g_pMGApp->FocusItem(this);
  Repaint();
}
