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

#ifndef CTRLVAR_H
#define CTRLVAR_H

struct TCtrlVar
{
  enum
  {
    CHECK_BOX,
    EDIT_BOX,
    COMBO_BOX,
    SLIDER,
    INVALID =-1,
  };
  int    iType;
  int    iNum;
  char   pszName[16];
  bool   bLinkable;
  // For ComboBox
  int    iNumOptions;
  char   pszOptions[16][16];
};

//---------------------------------------------------------------------------//
inline int GetCtrlVarType(const string &sStr)
{
  string pStr[] = {"CHECK_BOX", "EDIT_BOX", "COMBO_BOX", "SLIDER"};
  for (int i = 0; i < 4; i++)
    if (!Stricmp(pStr[i], sStr))
      return i;
  return -1;
}

#endif
