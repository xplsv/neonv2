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
#include "LinkBehaviour_TrackBar.h"


//---------------------------------------------------------------------------//
// Evaluate
//
//---------------------------------------------------------------------------//  
bool CLinkBehaviour_TrackBar::Evaluate(CLinkDevice *pDevice)
{
  if (pDevice->Evaluate())
  {
    bool bActive = pDevice->Value() == 1;
    switch (m_Action)
    {
      case DEVICE_VALUE:
        m_Value = pDevice->Value();
        break;
      case INV_DEVICE_VALUE:
        m_Value = 1.f - pDevice->Value();
        break;
      case SET:
        if (bActive)
          m_Value = m_SetValue;
        break;
      case ADD:
        if (bActive)
        {
          m_Value+= m_AddValue;
          if (m_Value > 1.f) m_Value = 1.f;
          if (m_Value < 0.f) m_Value = 0.f;
        }
        break;
      case RAND:
        m_Value = (rand() & 1023) * 1.f/1023.f;
        break;
      case FADE:
        m_Value    = m_FadeFr;
        m_FadeIni  = g_pAppMain->Timer()->Get();
        m_FadeDone = false;
        break;
    }
    return true;
  }
  // Evaluate fading
  if (m_Action == FADE && !m_FadeDone)
  {
    float fTime = g_pAppMain->Timer()->Get() - m_FadeIni;
    if (fTime > m_FadeLength)
    {
      m_FadeDone = true;
      m_Value    = m_FadeTo;
    }
    else
    {
      m_Value = m_FadeFr + fTime * (m_FadeTo - m_FadeFr) / m_FadeLength;
    }
    return true;
  }
  return false;
}


//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//  
void CLinkBehaviour_TrackBar::Load(CNodeFile::CNode *pNode)
{
  m_FadeIni    = 0.f;
  m_Action     = pNode->AsInt  ("b_action"    );
  m_FadeFr     = pNode->AsFloat("b_fadefr"    );
  m_FadeTo     = pNode->AsFloat("b_fadeto"    );
  m_FadeLength = pNode->AsFloat("b_fadelength");
  m_FadeDone   = pNode->AsBool ("b_fadedone"  );
  m_SetValue   = pNode->AsFloat("b_setvalue"  );
  m_AddValue   = pNode->AsFloat("b_addvalue"  );
  m_Value      = pNode->AsFloat("b_value"     );
}


//---------------------------------------------------------------------------//
// Save
//
//---------------------------------------------------------------------------//  
void CLinkBehaviour_TrackBar::Save(CNodeFile *pFile)
{
  pFile->WriteNode("b_action",     "", m_Action    );
  pFile->WriteNode("b_fadefr",     "", m_FadeFr    );
  pFile->WriteNode("b_fadeto",     "", m_FadeTo    );
  pFile->WriteNode("b_fadelength", "", m_FadeLength);
  pFile->WriteNode("b_fadedone",   "", m_FadeDone  );
  pFile->WriteNode("b_setvalue",   "", m_SetValue  );
  pFile->WriteNode("b_addvalue",   "", m_AddValue  );
  pFile->WriteNode("b_value",      "", m_Value     );
}


//---------------------------------------------------------------------------//
// Clone
//
//---------------------------------------------------------------------------//  
CLinkBehaviour *CLinkBehaviour_TrackBar::Clone()
{
  return NULL;
}
