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
#include "MGAppMain.h"
#include "MGWindowNewFX.h"
#include "MGButton.h"
#include "MGButtonModalResult.h"
#include "MGTrackBar.h"
#include "MGEditBox.h"
#include "MGSpinBox.h"
#include "MGComboBox.h"


//---------------------------------------------------------------------------//
// Constructor
//
//---------------------------------------------------------------------------//  
CMGWindowNewFX::CMGWindowNewFX() : CMGWindow(300,250, "New Effect")
{
  m_ButtonOk     = NEW CMGButtonModalResult(this, 140,220, CMGButton::NORMAL, "Ok",     MR_OK);
  m_ButtonCancel = NEW CMGButtonModalResult(this, 220,220, CMGButton::NORMAL, "Cancel", MR_CANCEL);
  NEW CMGEditBox(this, 10,30, 200, NULL);
  CMGSpinBox *pSpin = NEW CMGSpinBox(this, 10,60, 200, 0, 200, 1, NULL);
  pSpin->Set(10);
  CMGComboBox *pComboBox = NEW CMGComboBox(this, 10, 90, 200, 20, false, NULL);
  for (int i = 0; i < 100; i++)
  {
    char pBuffer[32];
    sprintf_s(pBuffer, 32, "Item %d", i);
    pComboBox->Add(pBuffer);
  }
  //= NEW CMGCheckBox(m_WindowNewFX, 5,25, "Press me", true);
  //CMGCheckBox *pCB2 = NEW CMGCheckBox(m_WindowNewFX, 5,47, "Press You", false);
  //CMGTrackBar *pTB1 = NEW CMGTrackBar(this, 2,22, 100,16, false);
  //CMGTrackBar *pTB2 = NEW CMGTrackBar(this, 2,42, 16,100, true);
}


//---------------------------------------------------------------------------//
// Destructor
//
//---------------------------------------------------------------------------//  
CMGWindowNewFX::~CMGWindowNewFX()
{
  CMGWindow::~CMGWindow();
}
