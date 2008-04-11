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

#ifndef N3DFORMAT_H
#define N3DFORMAT_H

//---------------------------------------------------------------------------//
// Cabeceras header v1
//---------------------------------------------------------------------------//
struct TN3DHeaderv1
{
  float    fFirstFrame;
  float    fLastFrame;
  float    fFrameSpeed;
  float    fTicksPerFrame;
  float    fFPS;
  float    fAmbientR;
  float    fAmbientG;
  float    fAmbientB;
  ushort   uNumMateriales;
  ushort   uNumObjetos;
  ushort   uNumCamaras;
  ushort   uNumLuces;
};

struct TN3DMaterialv1
{
  char pNombre[16];
};

struct TObj3DHeaderv1
{
  char     pNombre[16];
  ushort   uFlags;
  ushort   uNumFaces;
  ushort   uNumUV;
  ushort   uNumVertices;
  ushort   uNumFramesVertices;
  ushort   uNumFramesPosiciones;
  ushort   uNumFramesRotaciones;
  ushort   uNumFramesEscalados;
  unsigned uColor;
};

struct TCam3DHeaderv1
{
  ushort uID;
  ushort uNumFramesFov;
  ushort uNumFramesPosSrc;
  ushort uNumFramesPosTgt;
  float  fNearClip;
  float  fFarClip;
};

struct TLuz3DHeaderv1
{
  ushort   uType;
  ushort   uNumFramesPosSrc;
  float    fR, fG, fB;
  float    fIntens;
  float    fRange;
};

#endif
