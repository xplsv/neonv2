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
#include "Options.h"
#include <fstream>

//---------------------------------------------------------------------------//
// GetNextLine
//
//---------------------------------------------------------------------------//
const string &GetNextLine(std::ifstream &instream)
{
  static string lRes;
  lRes = "";
  for(;;)
  {
    char c = instream.get();
    if (instream.eof())
      return lRes;
    if (c == '\r' || c == '\n')
    {
      char t = instream.peek();
      if (t == '\r' || t == '\n')
        instream.get();
      return lRes;
    }
    else
      lRes = lRes + c;
  }
}


//---------------------------------------------------------------------------//
// LoadFromFile
//
//---------------------------------------------------------------------------//
bool COptions::LoadFromFile(const string &aFile)
{
  bool lRes = false;
  iStrings.clear();
  try
  {
    std::ifstream instream(aFile.c_str());
    if (instream.good())
    {
      lRes = true;
      // Real file
      while (!instream.eof())
      {
        const string &lStr = GetNextLine(instream);
        if (lStr.length() > 3)
        {
          int lPos = (int)lStr.find('=', 0);
          if (lPos > 0)
          {
            int lID = lPos-1;
            while (lID > 0 && lStr[lID] == ' ')
              lID--;
            if (lID >= 0)
            {
              // Assure
              int lLen  = (int)lStr.length();
              int lText = lPos+1;
              while (lText < lLen && lStr[lText] == ' ')
                lText++;
              int lEnd = lText;
              // End of line (as instream.getline gets wrong results in unicode)
              while (lEnd < lLen && lStr[lEnd+1] != 13)
                lEnd++;
              string lStrID = lStr.substr(0, lID+1);
              // Assure the string is correct and not full of spaces
              if (lText <= lEnd)
                iStrings.push_back(pair<string,string>(lStrID, lStr.substr(lText,lEnd-lText+1)));
              else
                iStrings.push_back(pair<string,string>(lStrID, "#ERR#"));
              //iStrings[lStrID] = lStr.substr(lText,lEnd-lText+1);
              //iStrings[lStrID] = "#ERR#";
            }
          }
        }
      }
    }
  }
  catch (...)
  {
  }
  return lRes;
}


//---------------------------------------------------------------------------//
// Option
//
//---------------------------------------------------------------------------//
const string &COptions::Option(const string &aOption, const string &aDefault) const
{
    for (vector< pair<string,string> >::const_iterator lIter = iStrings.begin(); lIter != iStrings.end(); ++lIter)
    {
        if ((*lIter).first == aOption)
            return (*lIter).second;
    }
    static string lRes;
    lRes = aDefault;
    return lRes;
}
