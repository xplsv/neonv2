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
#include "MGBeatManager.h"
#include "LinkDevice_Beat.h"


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
CLinkDevice_Beat::CLinkDevice_Beat()
{
  m_Beat   = 1;
  m_Off    = 0;
  m_IsBeat = false;
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Beat::Set(int iBeat, int iOff)
{
  m_Beat = iBeat;
  m_Off  = iOff;
}


//---------------------------------------------------------------------------//
// Get
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Beat::Get(int &iBeat, int &iOff)
{
  iBeat = m_Beat;
  iOff  = m_Off;
}


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
bool CLinkDevice_Beat::Evaluate()
{
  GLOG(("Evaluate beat, %d, %d\n", m_Beat, m_Off));
  bool bIsBeat = g_pAppMain->BeatManager()->IsBeat(m_Beat, m_Off);
  bool bRes    = (bIsBeat != m_IsBeat);
  m_IsBeat     = bIsBeat;
  if (m_IsBeat)
    GLOG(("Beat!"));
  return (bRes);
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Beat::Load(CNodeFile::CNode *pNode)
{
  m_Off  = pNode->AsInt("d_off");
  m_Beat = pNode->AsInt("d_beat");
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Beat::Save(CNodeFile *pFile)
{
  pFile->WriteNode("d_off",  "", m_Off);
  pFile->WriteNode("d_beat", "", m_Beat);
}


//---------------------------------------------------------------------------//
// Clone
//
//---------------------------------------------------------------------------//  
CLinkDevice *CLinkDevice_Beat::Clone()
{
  CLinkDevice_Beat *pDevice = NEW CLinkDevice_Beat;
  *pDevice = *this;
  return pDevice;
}
