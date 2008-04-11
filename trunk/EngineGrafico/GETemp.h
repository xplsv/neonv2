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

#ifndef GETEMP_H
#define GETEMP_H

#include "GEVector.h"

class  CDisplayDevice;
struct TVertex_HCV_XYZ_T;

void InitPruebasGraficas();
void EndPruebasGraficas ();
void DrawPruebasGraficas(CDisplayDevice *pDD);
void DrawLine           (CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1);
void DrawRect           (CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1);
void DrawQuad           (CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1);
void DrawQuad           (CDisplayDevice *pDD, bool bFlipY = false);
void DrawQuadUV         (CDisplayDevice *pDD, TVector2 const &uv0, TVector2 const &uv1, bool bFlipY = false);
void DrawQuadUV         (CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1, TVector2 const &uv0, TVector2 const &uv1);
void DrawQuadFlat       (CDisplayDevice *pDD, TVector2 const &v0, TVector2 const &v1, unsigned uColor, float fZ);
void PreparaVertices    (TVertex_HCV_XYZ_T *pVertices, TVector2 const &v0, TVector2 const &v1, unsigned uColor = 0xFFFFFFFF);

#endif
