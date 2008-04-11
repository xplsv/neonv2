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

#ifndef GESCENELOADER_H
#define GESCENELOADER_H

class CEscena3D;
class CChunker;

template<class _Ty> inline void swap(_Ty& _X, _Ty& _Y)
{
  _Ty _Tmp = _X;
  _X = _Y;
  _Y = _Tmp;
}

class CSceneLoader
{
  public:
    static bool Load            (CEscena3D *pEscena3D, const char *pData, unsigned uDataSize);

  private:
    // N3D Loader
    static bool LoadN3Dv1       (CEscena3D *pEscena3D, const char *pData, unsigned uDataSize);
    static bool LoadN3Dv2       (CEscena3D *pEscena3D, const char *pData, unsigned uDataSize);

    // 3DS Loader 
    static bool Load3DS         (CEscena3D *pEscena3D, const char *pData, unsigned uDataSize);
    static void Load3DSObjects  (CEscena3D *pEscena3D, CChunker *pChunker);
    static void Load3DSKeys     (CEscena3D *pEscena3D, CChunker *pChunker);
};

#endif
