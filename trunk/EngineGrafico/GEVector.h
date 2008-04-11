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

#ifndef GEVECTOR_H
#define GEVECTOR_H

#include <math.h>

//---------------------------------------------------------------------------//
// Rectangulo
//---------------------------------------------------------------------------//
struct TViewport
{ 
  TViewport() { };
  TViewport(int _x0, int _y0, int _x1, int _y1) { x0 = _x0; y0 = _y0; x1 = _x1; y1 = _y1; }
  int x0, y0, x1, y1;
};

//---------------------------------------------------------------------------//
// Definicion de vectores
//---------------------------------------------------------------------------//
struct TVector2
{
  TVector2() { };
  TVector2(float _x, float _y) { x = _x; y = _y; }
  float x;
  float y;
};

struct TVectorI2
{
  TVectorI2() { };
  TVectorI2(int _x, int _y) { x = _x; y = _y; }
  int x;
  int y;
};

struct TVector3
{
  TVector3() { };
  TVector3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
  float x;
  float y;
  float z;
};

struct TVectorI3
{
  TVectorI3() { };
  TVectorI3(int _x, int _y, int _z) { x = _x; y = _y; z = _z; }
  int x;
  int y;
  int z;
};

struct TVector4
{
  TVector4() { };
  TVector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
  float x;
  float y;
  float z;
  float w;
};

//---------------------------------------------------------------------------//
// Funciones
//---------------------------------------------------------------------------//
__inline void Vector_Unit(TVector4 *r, TVector4 *p)
{
  float d = 1.f / (float) sqrt(p->x*p->x + p->y*p->y + p->z*p->z + p->w*p->w);
  r->x = p->x * d;
  r->y = p->y * d;
  r->z = p->z * d;
  r->w = p->w * d;
}

__inline void Vector_Unit(TVector3 *r, TVector3 *p)
{
  float d = 1.f / (float) sqrt(p->x*p->x + p->y*p->y + p->z*p->z);
  r->x = p->x * d;
  r->y = p->y * d;
  r->z = p->z * d;
}

__inline void Vector_Unit(TVector2 *r, TVector2 *p)
{
  float d = 1.f / (float) sqrt(p->x*p->x + p->y*p->y);
  r->x = p->x * d;
  r->y = p->y * d;
}

__inline void Vector_Cross(TVector3 *r, TVector3 *a, TVector3 *b)
{
  r->x = a->y*b->z - a->z*b->y;
  r->y = a->z*b->x - a->x*b->z;
  r->z = a->x*b->y - a->y*b->x;
}

__inline float Vector_Dot(TVector3 *a, TVector3 *b)
{
  return (a->x*b->x + a->y*b->y + a->z*b->z);
}

__inline void Vector_Normal(TVector3 *r, TVector3 *a, TVector3 *b, TVector3 *o)
{
  // Restar O a A y B
  TVector3 va, vb;
  va.x = o->x - a->x;
  va.y = o->y - a->y;
  va.z = o->z - a->z;
  vb.x = o->x - b->x;
  vb.y = o->y - b->y;
  vb.z = o->z - b->z;

  // Calcular el producto cruzado y normalizar
  Vector_Cross(r, &vb, &va);
  Vector_Unit (r, r);
}

//---------------------------------------------------------------------------//
// Interpolacion lineal
//---------------------------------------------------------------------------//
__inline void LinearInterp(float fA, float fB, float fV, float fVA, float fVB, float &fVV)
{
  float fRA = (fV - fA) / (fB - fA);
  fVV = fVA + (fVB - fVA) * fRA;
}

__inline void LinearInterp(float fA, float fB, float fV, TVector2 const &vVA, TVector2 const &vVB, TVector2 &vVV)
{
  float fRA = (fV - fA) / (fB - fA);
  vVV.x = vVA.x + (vVB.x - vVA.x) * fRA;
  vVV.y = vVA.y + (vVB.y - vVA.y) * fRA;
}

__inline void LinearInterp(float fA, float fB, float fV, TVector3 const &vVA, TVector3 const &vVB, TVector3 &vVV)
{
  float fRA = (fV - fA) / (fB - fA);
  vVV.x = vVA.x + (vVB.x - vVA.x) * fRA;
  vVV.y = vVA.y + (vVB.y - vVA.y) * fRA;
  vVV.z = vVA.z + (vVB.z - vVA.z) * fRA;
}

__inline void LinearInterp(float fA, float fB, float fV, TVector4 const &vVA, TVector4 const &vVB, TVector4 &vVV)
{
  float fRA = (fV - fA) / (fB - fA);
  vVV.x = vVA.x + (vVB.x - vVA.x) * fRA;
  vVV.y = vVA.y + (vVB.y - vVA.y) * fRA;
  vVV.z = vVA.z + (vVB.z - vVA.z) * fRA;
  vVV.w = vVA.w + (vVB.w - vVA.w) * fRA;
}

#endif
