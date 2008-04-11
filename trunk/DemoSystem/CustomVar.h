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

#ifndef CUSTOMVAR_H
#define CUSTOMVAR_H

struct TCustomVar
{
  enum
  {
    V_BOOL,
    V_INT,
    V_FLOAT,
    INVALID = -1,
  };
  void     Init    (bool  bV, int iReg) { iType = V_BOOL;  bValue = bV; iRegister = iReg; }
  void     Init    (int   iV, int iReg) { iType = V_INT;   iValue = iV; iRegister = iReg; }
  void     Init    (float fV, int iReg) { iType = V_FLOAT; fValue = fV; iRegister = iReg; }
  
  void     SetValue(void *pData) { memcpy(&pValue, pData, 4); }
  void    *GetValue() { return &pValue; }
  float    GetFloat()
  {
    if (iType == V_BOOL) return (float)bValue;
    if (iType == V_INT)  return (float)iValue;
    return fValue;
  }

  int      iType;
  int      iRegister;
  union
  {
    void  *pValue;
    bool   bValue;
    int    iValue;
    float  fValue;
  };
};

#endif
