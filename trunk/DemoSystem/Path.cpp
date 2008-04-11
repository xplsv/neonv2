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

#include "DemoSystemPch.h"
#include "Path.h"

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CPath::Init(int iType, int iNumKeys)
{
  End();

  m_iNumKeys = 0;
  m_fTime    = 0.f;
  m_fTimeIni = 999999.f;
  m_fTimeEnd =-999999.f;
  m_iMaxKeys = iNumKeys;
  m_iType    = iType;
  m_bOk      = true;
  
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CPath::End()
{
  if (IsOk())
  {
    m_bOk = false;
  }
}
