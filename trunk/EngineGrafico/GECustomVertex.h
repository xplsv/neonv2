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

#ifndef GE_CUSTOM_VERTEX_H
#define GE_CUSTOM_VERTEX_H

//-----------------------------------------------------------------------------------------
enum
{
  HCV_XYZ_F,
  HCV_XYZ_P,
  HCV_XYZ_S,
  HCV_XYZ_T,
  HCV_MAX_CUSTOM_VERTEX,
};

//---------------------------------------------------------------------------//
// Utils
//---------------------------------------------------------------------------//
#define HCV_SET_XYZ(_pV,_fX,_fY,_fZ)          { (_pV)->Pos.x = _fX; (_pV)->Pos.y = _fY; (_pV)->Pos.z = _fZ;                     }
#define HCV_SET_XYZW(_pV,_fX,_fY,_fZ,_fW)     { (_pV)->Pos.x = _fX; (_pV)->Pos.y = _fY; (_pV)->Pos.z = _fZ; (_pV)->Pos.w = _fW; }
#define HCV_SET_NORMAL(_pV,_fX,_fY,_fZ)       { (_pV)->Normal.x = _fX; (_pV)->Normal.y = _fY; (_pV)->Normal.z = _fZ;            }
#define HCV_SET_TANGENT(_pV,_fX,_fY,_fZ,_fW)  { (_pV)->Tangent.x  = _fX; (_pV)->Tangent.y  = _fY; (_pV)->Tangent.z  = _fZ;      }
#define HCV_SET_BINORMAL(_pV,_fX,_fY,_fZ,_fW) { (_pV)->Binormal.x = _fX; (_pV)->Binormal.y = _fY; (_pV)->Binormal.z = _fZ;      }
#define HCV_SET_COLOR(_pV,_Diffuse)           { (_pV)->Diffuse = _Diffuse;                         }
#define HCV_SET_ARGB(_pV,_uA,_uR,_uG,_uB)     { (_pV)->Diffuse = HARD_COLOR_ARGB(_uA,_uR,_uG,_uB); }
#define HCV_SET_UV0(_pV,_fU,_fV)              { (_pV)->UV0.x = _fU; (_pV)->UV0.y = _fV;            }
#define HCV_SET_UV1(_pV,_fU,_fV)              { (_pV)->UV1.x = _fU; (_pV)->UV1.y = _fV;            }
#define HCV_SET_UV2(_pV,_fU,_fV)              { (_pV)->UV2.x = _fU; (_pV)->UV2.y = _fV;            }
#define HCV_SET_UV3(_pV,_fU,_fV)              { (_pV)->UV3.x = _fU; (_pV)->UV3.y = _fV;            } 
#define HCV_SET_PSIZE(_pV,_fSize)             { (_pV)->PSize = _fSize;                             } 

#define HCV_DECL(Name) struct TVertex_##Name## {
#define HCV_DECL_END() };


//---------------------------------------------------------------------------//
// Vertice para el fixed pipeline
//---------------------------------------------------------------------------//
struct TVertex_HCV_XYZ_F
{
  D3DVECTOR Pos;            // Vertice
  D3DVECTOR Normal;         // Normal
  unsigned  Diffuse;        // Diffuse
  D3DVECTOR UV0;            // Coordenadas de textura para la Stage 0
  D3DVECTOR UV1;            // Coordenadas de textura para la Stage 1
};

//---------------------------------------------------------------------------//
// Vertice para el programable pipeline
//---------------------------------------------------------------------------//
struct TVertex_HCV_XYZ_P
{
  D3DVECTOR Pos;            // Vertice
  D3DVECTOR Normal;         // Normal
  D3DVECTOR Tangent;        // Tangente
  D3DVECTOR Binormal;       // Binormal
  unsigned  Diffuse;        // Diffuse
  D3DVECTOR UV0;            // Coordenadas de textura para la Stage 0
  D3DVECTOR UV1;            // Coordenadas de textura para la Stage 1
  D3DVECTOR UV2;            // Coordenadas de textura para la Stage 2
  D3DVECTOR UV3;            // Coordenadas de textura para la Stage 3
};

//---------------------------------------------------------------------------//
// Vertice para el filtershader
//---------------------------------------------------------------------------//
struct TVertex_HCV_XYZ_S
{
  D3DVECTOR Pos;            // Vertice
  float     PSize;          // Point Size (para Point Sprites)
  unsigned  Diffuse;        // Diffuse
  D3DVECTOR UV0;            // Coordenadas de textura para la Stage 0
  D3DVECTOR UV1;            // Coordenadas de textura para la Stage 1
  D3DVECTOR UV2;            // Coordenadas de textura para la Stage 2
  D3DVECTOR UV3;            // Coordenadas de textura para la Stage 3
};

//---------------------------------------------------------------------------//
// Vertice transformado
//---------------------------------------------------------------------------//
struct TVertex_HCV_XYZ_T
{
  TVector4  Pos;            // Vertice (transformado)
  float     PSize;          // Point Size (para Point Sprites)
  unsigned  Diffuse;        // Diffuse
  D3DVECTOR UV0;            // Coordenadas de textura para la Stage 0
  D3DVECTOR UV1;            // Coordenadas de textura para la Stage 1
};

#endif
