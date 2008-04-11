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

#ifndef _MGGRID_H
#define _MGGRID_H

#include "MGControl.h"
#include "MGGrid.h"
#include "MGScrollBar.h"

class CMGScrollBar;

template <class T>
class CMGGrid : public CMGControl
{
  public:

                      CMGGrid        (CMGControl *pParent, int iCellsW, int iCellsH, float Aspect, int Align, bool ScrollBar);
                      CMGGrid        (CMGControl *pParent, int iCellsW, int iCellsH, float Aspect, int iX, int iY, int iW, int iH, bool ScrollBar);
    virtual          ~CMGGrid        ();
 
            void      SetSize        (int cols, int rows);

    virtual int       GetCols        () const { return m_Cols;  }
    virtual int       GetRows        () const { return m_Rows;  }
    virtual T       **GetCells       ()       { return m_Items; }

    virtual void      Clear          ();
    virtual T        *GetCellData    (int col, int row);
    virtual bool      CellAtXY       (int x, int y, int &col, int &row);

    virtual T        *GetSelectedCell() { return m_CellSelected; }
    virtual bool      GetSelectedCell(int &col, int &row) { col = m_CellSelectedCol; row = m_CellSelectedRow; return (m_CellSelected != NULL); }
    virtual void      SelectCell     (int col = -1, int row = -1);

    virtual void      OnResize       ();
    virtual void      OnMouseMove    (int iX, int iY);
    virtual void      OnMouseDown    (int iX, int iY, int iButton);
    virtual void      OnMouseUp      (int iX, int iY, int iButton);
    virtual void      OnDragOver     (CMGControl *pSource, int iX, int iY, bool &Accept);
    virtual void      OnDragDrop     (CMGControl *pSource, int iX, int iY);
    virtual void      OnKeyDown      (int vKey);
    virtual void      OnKeyUp        (int vKey);

  protected:

            void      OnChangeScroll ();

  public:

    boost::function<void()>                                                                      onResize;
    boost::function<void(CMGGrid<T> *pGrid, CMGControl *pSource, int col, int row, bool &Accept)>   onDragOver;
    boost::function<void(CMGGrid<T> *pGrid, CMGControl *pSource, int col, int row)>                 onDragDrop;
    boost::function<void(CMGGrid<T> *pGrid, int iX, int iY)>                                     onMouseMove;
    boost::function<void(CMGGrid<T> *pGrid, int iX, int iY, int iButton)>                        onMouseDown;
    boost::function<void(CMGGrid<T> *pGrid, int iX, int iY, int iButton)>                        onMouseUp;
    boost::function<void(CMGGrid<T> *pGrid, int vKey)>                                           onKeyDown;
    boost::function<void(CMGGrid<T> *pGrid, int vKey)>                                           onKeyUp;
    MGColor                                                                                      m_ColorLines;

  protected:

    virtual void      DerivDraw     ();
            int       GetTotalHeight();

  protected:

    CMGScrollBar     *m_ScrollBar;
    T               **m_Items;
    T                *m_CellSelected;
    int               m_CellSelectedCol;
    int               m_CellSelectedRow;
    float             m_GridAspectRatio;
    float             m_CellW;
    float             m_CellH;
    int               m_Cols;
    int               m_Rows;
    int               m_TotalWidth;
    int               m_TotalHeight;
};


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
template<class T>
CMGGrid<T>::CMGGrid(CMGControl *pParent, int iCellsW, int iCellsH, float Aspect, int Align, bool ScrollBar) : CMGControl(pParent, 0,0,0,0)
{
  m_GridAspectRatio = Aspect;
  m_CellSelected    = NULL;
  m_CellSelectedCol = -1;
  m_CellSelectedRow = -1;
  m_Color           = g_pMGDD->Skin()->Color(COL_GRID_BG);
  m_ColorLines      = g_pMGDD->Skin()->Color(COL_GRID_BORDER);
  m_Items           = NULL;
  m_Align           = Align;
  m_Border          = TRect(1,1,1,1);
  m_ScrollBar       = ScrollBar ? NEW CMGScrollBar(this, true, boost::bind(&CMGGrid<T>::OnChangeScroll, this)) : NULL;
  SelectCell();
  SetSize   (iCellsW, iCellsH);
  OnResize  ();
}


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//
template<class T>
CMGGrid<T>::CMGGrid(CMGControl *pParent, int iCellsW, int iCellsH, float Aspect, int iX, int iY, int iW, int iH, bool ScrollBar) : CMGControl(pParent, 0,0,0,0)
{
  m_Rect            = TRect(iX, iY, iW, iH);
  m_GridAspectRatio = Aspect;
  m_CellSelected    = NULL;
  m_CellSelectedCol = -1;
  m_CellSelectedRow = -1;
  m_Color           = g_pMGDD->Skin()->Color(COL_GRID_BG);
  m_ColorLines      = g_pMGDD->Skin()->Color(COL_GRID_BORDER);
  m_Items           = NULL;
  m_Align           = MGALIGN_NONE;
  m_Border          = TRect(1,1,1,1);
  m_ScrollBar       = ScrollBar ? NEW CMGScrollBar(this, true, boost::bind(&CMGGrid<T>::OnChangeScroll, this)) : NULL;
  SelectCell();
  SetSize   (iCellsW, iCellsH);
  OnResize  ();
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//
template<class T>
CMGGrid<T>::~CMGGrid()
{
  // Delete
  if (m_Items)
  {
    for (int i = 0; i < m_Cols*m_Rows; i++)
      DISPOSE(m_Items[i]);
    DISPOSE_ARRAY(m_Items);
  }
}


//---------------------------------------------------------------------------//
// Clear
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::Clear()
{
  // Delete
  if (m_Items)
  {
    for (int i = 0; i < m_Cols*m_Rows; i++)
      DISPOSE(m_Items[i]);
    DISPOSE_ARRAY(m_Items);
  }
  // Create
  m_Items = NEW_ARRAY(T *, m_Cols*m_Rows);
  for (int j = 0; j < m_Cols; j++)
  {
    for (int i = 0; i < m_Rows; i++)
      m_Items[i+j*m_Rows] = NEW T;
  }
}


//---------------------------------------------------------------------------//
// GetCellXY
//
//---------------------------------------------------------------------------//
template<class T>
bool CMGGrid<T>::CellAtXY(int x, int y, int &col, int &row)
{
  y+= (m_ScrollBar && m_ScrollBar->IsVisible()) ? Round(m_ScrollBar->GetPos() * (m_TotalHeight - Height())) : 0;
  x = Floor((float)x / m_CellW);
  y = Floor((float)y / m_CellH);
  if (x >= 0 && x < m_Cols && y >= 0 && y < m_Rows)
  {
    col = x;
    row = y;
    return true;
  }
  return false;
}


//---------------------------------------------------------------------------//
// GetCellData
//
//---------------------------------------------------------------------------//
template<class T>
T *CMGGrid<T>::GetCellData(int col, int row)
{
  if (col >= 0 && col < m_Cols && row >= 0 && row < m_Rows)
    return m_Items[col+row*m_Cols];
  return NULL;
}


//---------------------------------------------------------------------------//
// SelectCell
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::SelectCell(int col,int row)
{
  m_CellSelected    = GetCellData(col,row);
  m_CellSelectedCol = col;
  m_CellSelectedRow = row;
  Repaint();
}


//---------------------------------------------------------------------------//
// SetSize
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::SetSize(int cols, int rows)
{
  m_Cols = cols;
  m_Rows = rows;
  Clear();
}


//---------------------------------------------------------------------------//
// OnResize
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnResize()
{
  UpdateSize();
  if (m_ScrollBar)
    m_CellW = (float)(Width()-21) / m_Cols; // Para la scrollbar
  else
    m_CellW = (float)(Width()-3) / m_Cols;
  if (m_Align == MGALIGN_TOP)
  {
    m_CellH       = (float)m_CellW * m_GridAspectRatio;
    m_TotalHeight = Ceil(m_CellH * m_Rows) + 2;
    m_Rect.h      = m_TotalHeight;
  }
  else
  {
    m_CellH       = (float)m_CellW * m_GridAspectRatio;
    m_TotalHeight = Ceil(m_CellH * m_Rows) + 2;
  }
  m_TotalWidth = Ceil(m_CellW * m_Cols) + 2;
  UpdateRect();

  // Actualizar scrollbar
  if (m_ScrollBar)
    m_ScrollBar->SetVisible(m_TotalHeight > Height());

  // Resize sons
  CListaIter<CMGControl *> Iter(m_Sons);
  while (!Iter.EsFinal())
  {
    CMGControl *pItem = Iter;
    pItem->OnResize();
    Iter++;
  }
  Repaint();
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::DerivDraw()
{
  int i, j;
  int iHeight = MIN(m_TotalHeight, Height());
  int iOffset = (m_ScrollBar && m_ScrollBar->IsVisible()) ? Round(m_ScrollBar->GetPos() * (m_TotalHeight - Height())) : 0;

  // Rect
  if (!m_Transparent)
    g_pMGDD->FillRect (TRect(Left(),   Top(),   m_TotalWidth,   iHeight),   m_Color);
  g_pMGDD->Rect       (TRect(Left()+1, Top()+1, m_TotalWidth-1, iHeight-1), m_ColorLines);
  g_pMGDD->SetClipRect(TRect(Left()+2, Top()+2, m_TotalWidth-3, iHeight-3));
  
  float x = (float)Left()+2;
  float y = (float)Top ()+2;
  for (i = 1; i < m_Rows; i++)
    g_pMGDD->HLine(Floor(x),Floor(y + i*m_CellH) - iOffset, m_TotalWidth-2, m_ColorLines);
  for (i = 1; i < m_Cols; i++)
    g_pMGDD->VLine(Floor(x + i*m_CellW),Floor(y), iHeight-2, m_ColorLines);

  // Draw Items
  for (j = 0; j < m_Rows; j++)
  {
    for (i = 0; i < m_Cols; i++)
    {
      T *pItem = GetCellData(i,j);
      if (pItem)
      {
        pItem->Draw(Floor(x + i*m_CellW),
                    Floor(y + j*m_CellH) - iOffset,
                    Floor((i+1)*m_CellW) - Floor(i*m_CellW) - 1,
                    Floor((j+1)*m_CellH) - Floor(j*m_CellH) - 1,
                    m_CellSelected == pItem);
      }
    }
  }
  g_pMGDD->SetClipRect();
}


//---------------------------------------------------------------------------//
// OnChangeScroll
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnChangeScroll()
{
  Repaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnMouseMove(int iX, int iY)
{
  if (onMouseMove)
    onMouseMove(this, iX,iY);
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnMouseDown(int iX, int iY, int iButton)
{
  if (onMouseDown)
    onMouseDown(this, iX,iY, iButton);
  Repaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnMouseUp(int iX, int iY, int iButton)
{
  if (onMouseUp)
    onMouseUp(this, iX,iY, iButton);
  Repaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnDragOver(CMGControl *pSource, int iX, int iY, bool &Accept)
{
  Accept = false;
  if (onDragOver)
    onDragOver(this, pSource, iX,iY, Accept);
  Repaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnDragDrop(CMGControl *pSource, int iX, int iY)
{
  if (onDragDrop)
    onDragDrop(this, pSource, iX,iY);
  Repaint();
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnKeyDown(int vKey)
{
  if (onKeyDown)
    onKeyDown(this, vKey);
}


//---------------------------------------------------------------------------//
// OnMouseMove
//
//---------------------------------------------------------------------------//
template<class T>
void CMGGrid<T>::OnKeyUp(int vKey)
{
  if (onKeyUp)
    onKeyUp(this, vKey);
}


#endif
