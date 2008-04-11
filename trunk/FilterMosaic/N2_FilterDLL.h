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

#ifndef _FILTERDLL_H
#define _FILTERDLL_H

#define FILTER_VERSION 3
#define EXPORT_DLL extern "C" __declspec(dllexport)


//---------------------------------------------------------------------------//
// Struct for setting vars
struct TCtrlVar
{
  enum
  {
    CHECK_BOX,
    EDIT_BOX,
    COMBO_BOX,
    SLIDER,
    INVALID =-1,
  };
  int    iType;
  int    iNum;
  char   pszName[16];
  bool   bLinkable;
  // For ComboBox
  int    iNumOptions;
  char   pszOptions[16][16];
};


//---------------------------------------------------------------------------//
// Vector stuff
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
// Struct for the filter options passed
struct TFilterTgt
{
  IDirect3DTexture9 *pTexSrc;
  IDirect3DTexture9 *pTexTgt;
};


#endif
