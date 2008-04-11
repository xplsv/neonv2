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

#include "TimeProfile.h"


//---------------------------------------------------------------------------//
// Begin
//
//---------------------------------------------------------------------------//
void CTimeProfile::Begin()
{
  m_Timer.Reset();
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CTimeProfile::End()
{
  m_Time = m_Timer.Get() * 1000.f;
  m_Times.push_back(m_Time);
  if (m_Times.size() > 60)
    m_Times.erase(m_Times.begin(),m_Times.begin()+1);
  m_Average = 0.f;
  for (vector<float>::iterator lIter = m_Times.begin(); lIter != m_Times.end(); ++lIter)
    m_Average+= *lIter;
  if (m_Times.size() > 0)
    m_Average = m_Average / m_Times.size();
}
