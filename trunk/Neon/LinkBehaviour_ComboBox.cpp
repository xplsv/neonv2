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
#include "LinkBehaviour_ComboBox.h"


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
bool CLinkBehaviour_ComboBox::Evaluate(CLinkDevice *pDevice)
{
  if (pDevice->Evaluate())
  {
    bool bActive = pDevice->Value() == 1;
    switch (m_Action)
    {
      case NEXT:
        if (bActive)
        {
          m_Value = (m_Value < (m_NumItems-1)) ? m_Value + 1 : 0;
          return true;
        }
        break;
      case PREV:
        if (bActive)
        {
          m_Value = (m_Value > 0) ? m_Value - 1 : (m_NumItems-1);
          return true;
        }
        break;
      case RAND:
        if (bActive)
        {
          if (m_NumItems > 1)
          {
            int iValue;
            do 
            {
              iValue = rand() % m_NumItems;
            } while(m_Value == iValue);
            m_Value = iValue;
          }
          return true;
        }
        break;
      case SET:
        if (bActive)
        {
          m_Value = m_SetValue;
        }
        break;
    }
  }
  return false;
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CLinkBehaviour_ComboBox::Load(CNodeFile::CNode *pNode)
{
  m_Action   = pNode->AsInt ("b_action"  );
  m_NumItems = pNode->AsBool("b_numitems");
  m_SetValue = pNode->AsBool("b_setvalue");
  m_Value    = pNode->AsBool("b_value"   );
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CLinkBehaviour_ComboBox::Save(CNodeFile *pFile)
{
  pFile->WriteNode("b_action",   "", m_Action  );
  pFile->WriteNode("b_numitems", "", m_NumItems);
  pFile->WriteNode("b_setvalue", "", m_SetValue);
  pFile->WriteNode("b_value",    "", m_Value   );
}


//---------------------------------------------------------------------------//
// Clone
//
//---------------------------------------------------------------------------//  
CLinkBehaviour *CLinkBehaviour_ComboBox::Clone()
{
  return NEW CLinkBehaviour_ComboBox(m_Action, m_Value, m_NumItems, m_SetValue);
}
