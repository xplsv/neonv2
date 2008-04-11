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
#include "FindFiles.h"


//---------------------------------------------------------------------------//
// FindFileInit
// 
//---------------------------------------------------------------------------//
void FindFileInit(TFindFileResult &Result, const string &sWildcard)
{
  Result.Handle = FindFirstFile(sWildcard.c_str(), &Result.FindData);
  Result.First  = true;
}


//---------------------------------------------------------------------------//
// FindFileNext
// 
//---------------------------------------------------------------------------//
bool FindFileNext(TFindFileResult &Result)
{
  bool bRes = false;
  if (Result.Handle != INVALID_HANDLE_VALUE)
  {
    if (Result.First)
    {
      Result.First = false;
      bRes = true;
    }
    else
    {
      bRes = FindNextFile(Result.Handle, &Result.FindData) != 0;
    }
    if (bRes)
    {
      Result.bDir = (Result.FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
      Result.Filename = Result.FindData.cFileName;
    }
  }
  return bRes;
}


//---------------------------------------------------------------------------//
// FindFileClose
// 
//---------------------------------------------------------------------------//
void FindFileClose(TFindFileResult &Result)
{
  if (Result.Handle != INVALID_HANDLE_VALUE)
    FindClose(Result.Handle);
}
