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
#include "VJController.h"
#include "LinkDevice_Wave.h"


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
CLinkDevice_Wave::CLinkDevice_Wave()
{
  m_Chan  = 0;
  m_Gain  = 1.f;
  m_Value = 0.f;
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Wave::Set(int iChan, float fGain)
{
  m_Chan = iChan;
  m_Gain = fGain;
}


//---------------------------------------------------------------------------//
// Get
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Wave::Get(int &iChan, float &fGain)
{
  iChan = m_Chan;
  fGain = m_Gain;
}


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
bool CLinkDevice_Wave::Evaluate()
{
  float fValue = g_pAppMain->VJController()->GetFFTChannel(m_Chan) * m_Gain;
  bool bRes    = (fValue != m_Value);
  m_Value      = fValue;
  return (bRes);
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Wave::Load(CNodeFile::CNode *pNode)
{
  m_Chan = pNode->AsInt  ("d_chan");
  m_Gain = pNode->AsFloat("d_gain");
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CLinkDevice_Wave::Save(CNodeFile *pFile)
{
  pFile->WriteNode("d_chan", "", m_Chan);
  pFile->WriteNode("d_gain", "", m_Gain);
}


//---------------------------------------------------------------------------//
// Clone
//
//---------------------------------------------------------------------------//  
CLinkDevice *CLinkDevice_Wave::Clone()
{
  return NULL;
}
