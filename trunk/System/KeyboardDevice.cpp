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
#include "KeyboardDevice.h"


//---------------------------------------------------------------------------//
// InitSubsystem
//
//---------------------------------------------------------------------------//
void CKeyboardDevice::InitSubsystem()
{
}


//---------------------------------------------------------------------------//
// ShutSubsystem
//
//---------------------------------------------------------------------------//
void CKeyboardDevice::ShutSubsystem()
{
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CKeyboardDevice::Init()
{
  GetKeyboardState(m_KeyboardState);

  return true;
}


//---------------------------------------------------------------------------//
// Update
//
//---------------------------------------------------------------------------//
void CKeyboardDevice::Update()
{
  GetKeyboardState(m_KeyboardState);
}


//---------------------------------------------------------------------------//
// KeyPressed
//
//---------------------------------------------------------------------------//
bool CKeyboardDevice::KeyPressed(int iKey)
{
  return (m_KeyboardState[iKey] & 0x80) != 0;
}
