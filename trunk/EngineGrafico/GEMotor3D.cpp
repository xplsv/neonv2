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
#include "GEMotor3D.h"

using namespace CMotor3D;

struct TRegister
{
  int    iPos;
  int    iSize;
  string sName;
};

static TRegister s_pRegisterData[V_MAX_REGISTERS];
static float    *s_pRegisters;

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
void CMotor3D::Init()
{
  int iArrayPos = 0;
  int i = 0;
  #define ITEM(varsize,str,e) \
          s_pRegisterData[i].iPos  = iArrayPos; \
          s_pRegisterData[i].iSize = varsize; \
          s_pRegisterData[i].sName = str; \
          iArrayPos+= varsize; i++;
  #include "EnumVariablesMotor3D.h"
  #undef ITEM
  s_pRegisters = NEW_ARRAY(float, iArrayPos);
  memset(s_pRegisters, 0, iArrayPos*sizeof(float));
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMotor3D::End()
{
  DISPOSE_ARRAY(s_pRegisters);
}


//---------------------------------------------------------------------------//
// SetRegisterData
//
//---------------------------------------------------------------------------//
void CMotor3D::SetRegisterData(int iRegister, float const *pValue)
{
  ASSERT(iRegister >= 0 && iRegister < V_MAX_REGISTERS);
  memcpy(&s_pRegisters[s_pRegisterData[iRegister].iPos], pValue, s_pRegisterData[iRegister].iSize*sizeof(float));
}


//---------------------------------------------------------------------------//
// GetRegisterData
//
//---------------------------------------------------------------------------//
void CMotor3D::GetRegisterData(int iRegister, float *pValue)
{
  memcpy(pValue, &s_pRegisters[s_pRegisterData[iRegister].iPos], s_pRegisterData[iRegister].iSize*sizeof(float));
}


//---------------------------------------------------------------------------//
// GetRegisterPtr
//
//---------------------------------------------------------------------------//
float *CMotor3D::GetRegisterPtr(int iRegister)
{
  return (&s_pRegisters[s_pRegisterData[iRegister].iPos]);
}


//---------------------------------------------------------------------------//
// GetRegister
//
//---------------------------------------------------------------------------//
int CMotor3D::GetRegisterByName(const string &sRegister)
{
  for (int i = 0; i < V_MAX_REGISTERS; i++)
  {
    if (!Stricmp(sRegister.c_str(), s_pRegisterData[i].sName.c_str()))
      return i;
  }
  return -1;
}


//---------------------------------------------------------------------------//
// GetRegister
//
//---------------------------------------------------------------------------//
bool CMotor3D::GetRegisterName(int iRegister, string &sRegister)
{
  if (iRegister >= 0 && iRegister < V_MAX_REGISTERS)
  {
    sRegister = s_pRegisterData[iRegister].sName;
    return true;
  }
  return false;
}
