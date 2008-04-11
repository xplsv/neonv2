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
#include "Link.h"
#include "LinkDevice.h"
#include "LinkDevice_Beat.h"
#include "LinkDevice_Keyboard.h"
#include "LinkBehaviour.h"
#include "LinkBehaviour_CheckBox.h"
#include "LinkBehaviour_ComboBox.h"
#include "LinkBehaviour_TrackBar.h"


//---------------------------------------------------------------------------//
// CLink
//
//---------------------------------------------------------------------------//  
CLink::CLink(CLinkDevice *pLinkDevice, CLinkBehaviour *pLinkBehaviour)
{
  m_Device    = pLinkDevice;
  m_Behaviour = pLinkBehaviour;
}


//---------------------------------------------------------------------------//
// ~CLink
//
//---------------------------------------------------------------------------//  
CLink::~CLink()
{
  DISPOSE(m_Device);
  DISPOSE(m_Behaviour);
}


//---------------------------------------------------------------------------//
// Evaluate
// Return true if link value changed since last call
//---------------------------------------------------------------------------//  
bool CLink::Evaluate()
{
  return (m_Behaviour->Evaluate(m_Device));
}


//---------------------------------------------------------------------------//
// ValueCheckBox
//
//---------------------------------------------------------------------------//  
bool CLink::ValueCheckBox()
{
  return (m_Behaviour->Value() > 0.f);
}


//---------------------------------------------------------------------------//
// ValueComboBox
//
//---------------------------------------------------------------------------//  
int CLink::ValueComboBox()
{
  return ((int)m_Behaviour->Value());
}


//---------------------------------------------------------------------------//
// ValueTrackBar
//
//---------------------------------------------------------------------------//  
float CLink::ValueTrackBar()
{
  return (m_Behaviour->Value());
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CLink::Load(CNodeFile::CNode *pNode)
{
  DISPOSE(m_Device);
  DISPOSE(m_Behaviour);
  // Load Device
  switch (pNode->AttrAsInt("dev"))
  {
    case CLinkDevice::KEYBOARD:    m_Device = NEW CLinkDevice_Keyboard(); break;
    case CLinkDevice::BEAT:        m_Device = NEW CLinkDevice_Beat    (); break;
    case CLinkDevice::TIMER:       m_Device = NEW CLinkDevice_Beat    (); break;
    case CLinkDevice::WAVE:        m_Device = NEW CLinkDevice_Beat    (); break;
    case CLinkDevice::MIDI_NOTE:   m_Device = NEW CLinkDevice_Beat    (); break;
    case CLinkDevice::MIDI_SLIDER: m_Device = NEW CLinkDevice_Beat    (); break;
    case CLinkDevice::MIDI_CLOCK:  m_Device = NEW CLinkDevice_Beat    (); break;
  }
  if (m_Device)
    m_Device->Load(pNode);
  // Load Behaviour
  switch (pNode->AttrAsInt("beh"))
  {
    case CLinkBehaviour::CHECK_BOX: m_Behaviour = NEW CLinkBehaviour_CheckBox(0, false);   break;
    case CLinkBehaviour::COMBO_BOX: m_Behaviour = NEW CLinkBehaviour_ComboBox(0, 0, 0, 0); break;
    case CLinkBehaviour::TRACK_BAR: m_Behaviour = NEW CLinkBehaviour_TrackBar(0, 0);       break;
  }
  if (m_Behaviour)
    m_Behaviour->Load(pNode);
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CLink::Save(CNodeFile *pFile, const string &sNodeName)
{
  char pBuffer[50];
  sprintf_s(pBuffer, 50, "dev=\"%d\" beh=\"%d\"", m_Device->Type(), m_Behaviour->Type());
  pFile->WriteOpenNode(sNodeName, pBuffer);
    m_Device   ->Save(pFile);
    m_Behaviour->Save(pFile);
  pFile->WriteCloseNode();
}


//---------------------------------------------------------------------------//
// Clone
//
//---------------------------------------------------------------------------//  
CLink *CLink::Clone()
{
  CLink *pLink = NEW CLink(m_Device->Clone(), m_Behaviour->Clone());
  return (pLink);
}
