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

#include "Base.h"
#include "GESceneLoader.h"

#define N2_N3DFILEV2  (('N'<<8) | ('1'<<0))
#define N2_3DSFILE    0x4D4D

//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//
bool CSceneLoader::Load(CEscena3D *pEscena3D, const char *pData, unsigned uDataSize)
{
  ushort uVersion = *(ushort *)pData;
  switch (uVersion)
  {
    case N2_3DSFILE:   return Load3DS  (pEscena3D, pData, uDataSize);
    case N2_N3DFILEV2: return LoadN3Dv2(pEscena3D, pData, uDataSize);
    default:           return LoadN3Dv1(pEscena3D, pData, uDataSize);
  }
  GLOG(("ERR: File format not supported for 3D file\n"));
  return false;
}
