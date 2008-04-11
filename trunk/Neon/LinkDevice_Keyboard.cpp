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
#include "LinkDevice_Keyboard.h"
#include "KeyboardDevice.h"


//---------------------------------------------------------------------------//
// CLinkDevice_Keyboard
//
//---------------------------------------------------------------------------//  
CLinkDevice_Keyboard::CLinkDevice_Keyboard()
{
  m_Char    = 0;
  m_Pressed = false;
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Keyboard::Set(char c)
{
  m_Char    = c;
  m_Pressed = false;
}


//---------------------------------------------------------------------------//
// Get
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Keyboard::Get(char &c)
{
  c = m_Char;
}


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
bool CLinkDevice_Keyboard::Evaluate()
{
  bool bPressed = g_pAppMain->KeyboardDevice()->KeyPressed(m_Char);
  bool bRes = (bPressed != m_Pressed);
  m_Pressed = bPressed;
  return (bRes);
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Keyboard::Load(CNodeFile::CNode *pNode)
{
  m_Char = pNode->AsInt("char");
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Keyboard::Save(CNodeFile *pFile)
{
  pFile->WriteNode("char", "", m_Char);
}


//---------------------------------------------------------------------------//
// Clone
//
//---------------------------------------------------------------------------//  
CLinkDevice *CLinkDevice_Keyboard::Clone()
{
  return NULL;
}
