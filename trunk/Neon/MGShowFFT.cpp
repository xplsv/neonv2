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
#include "MGShowFFT.h"
#include "MGLabel.h"
#include "MGButton.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGShowFFT::CMGShowFFT(CMGControl *pParent, int iX, int iY, int iW, int iH, int iBarras) : CMGControl(pParent, iX,iY,iW,iH)
{
  m_Align  = MGALIGN_NONE;
  m_Paint  = false;
  m_Barras = iBarras;
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGShowFFT::~CMGShowFFT()
{
}


//---------------------------------------------------------------------------//
// Update 
//
//---------------------------------------------------------------------------//
void CMGShowFFT::Update(const float *pFFT)
{
  m_Paint = true;
  memcpy(m_FFT, pFFT, 256 * sizeof(float));
  Repaint();
}


//---------------------------------------------------------------------------//
// DerivDraw
//
//---------------------------------------------------------------------------//
void CMGShowFFT::DerivDraw()
{
  g_pMGDD->FillRect(TRect(Left(), Top(), Width(), Height()), g_pMGDD->Skin()->Color(COL_EDIT_BG));
  g_pMGDD->Rect    (TRect(Left(), Top(), Width(), Height()), g_pMGDD->Skin()->Color(COL_EDIT_BORDER));

  float fBarra = (float)255 / m_Barras;
  float fAncho = (float)(Width() - 3) / m_Barras;
  for (int i = 0; i < m_Barras; i++)
  {
    int fr = Floor(fBarra * (i + 0));
    int to = Floor(fBarra * (i + 1));
    float fLevel = 0.f;
    for (int j = fr; j <= to; j++)
    {
      if (m_FFT[j] > fLevel)
        fLevel = m_FFT[j];
    }
    float r = fLevel;
    float g = 1 - r;
    int  x0 = Floor(fAncho * (i + 0)) + Left() + 2;
    int  x1 = Floor(fAncho * (i + 1) - 1) + Left() + 2;
    int  h  = Floor(fLevel * (Height() - 4));
    int  y0 = Top() + (Height() - 2) - h;

    g_pMGDD->GradRect(TRect(x0, y0, x1 - x0, h), MGColor(r,g,0), MGColor(0,1,0));
  }
}
