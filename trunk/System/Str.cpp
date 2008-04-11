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
#include <math.h>

static bool s_FloatConComas = false;

void InitFloatToStr()
{
  float f = (float)atof("1,5");
  s_FloatConComas = (f > 1.f && f < 2.f);
}

void ExtractDirFile(const string &sStr, string &sDir, string &sFile)
{
  sDir  = "";
  sFile = sStr;
  for (size_t i = sStr.length(); i > 0; i--)
  {
    if (sStr[i-1] == '\\')
    {
      sDir  = sStr.substr(0,i);
      sFile = sStr.substr(i+1,sStr.length()-i-1);
      return;
    }
  }
}

static int    s_iDirStack = 0;
static string s_pDirStack[100];

void PushDir(const string &sDir)
{
  char pDir[MAX_PATH];
  GetCurrentDirectory(MAX_PATH, pDir);
  s_pDirStack[s_iDirStack++] = pDir;
  SetCurrentDirectory(sDir.c_str());
}

void PopDir()
{
  SetCurrentDirectory(s_pDirStack[--s_iDirStack].c_str());
}

int Stricmp(const string &p1, const string &p2)
{
  return _stricmp(p1.c_str(),p2.c_str());
}

float strtofloat(const string &sStr)
{
  string sNew = sStr;
  if (s_FloatConComas)
  {
    for (size_t i = 0; i < sNew.length(); i++)
      if (sNew[i] == '.') sNew[i] = ',';
  }
  else
  {
    for (size_t i = 0; i < sNew.length(); i++)
      if (sNew[i] == ',') sNew[i] = '.';
  }
  return (float)atof(sNew.c_str());
}


bool strtobool(const string &sStr)
{
  if (!Stricmp(sStr, "true" )) return true;
  if (!Stricmp(sStr, "false")) return true;
  return (strtofloat(sStr) > 0);
}


int strtoint(const string &sStr)
{
  return atoi(sStr.c_str());
}
