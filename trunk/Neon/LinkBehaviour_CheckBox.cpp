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
#include "LinkDevice.h"
#include "LinkBehaviour_CheckBox.h"


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
bool CLinkBehaviour_CheckBox::Evaluate(CLinkDevice *pDevice)
{
  if (pDevice->Evaluate())
  {
    bool bActive = pDevice->Value() > 0.5;
    switch (m_Action)
    {
      case SWITCH:
        // Switch only when device is activated
        if (bActive)
        {
          m_Value = !m_Value;
          return true;
        }
        break;
      case DEVICE_VALUE:     m_Value =  bActive; return true;
      case INV_DEVICE_VALUE: m_Value = !bActive; return true;
    }
  }
  return false;
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CLinkBehaviour_CheckBox::Load(CNodeFile::CNode *pNode)
{
  m_Action = pNode->AsInt ("b_action");
  m_Value  = pNode->AsBool("b_value" );
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CLinkBehaviour_CheckBox::Save(CNodeFile *pFile)
{
  pFile->WriteNode("b_action", "", m_Action);
  pFile->WriteNode("b_value",  "", m_Value );
}


//---------------------------------------------------------------------------//
// Clone
//
//---------------------------------------------------------------------------//  
CLinkBehaviour *CLinkBehaviour_CheckBox::Clone()
{
  return NEW CLinkBehaviour_CheckBox(m_Action, m_Value);
}
