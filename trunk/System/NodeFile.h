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

#ifndef _NODEFILE_H
#define _NODEFILE_H

#include <fstream>
#include <stack>
#include "TinyXml.h"

class CNodeFile
{
  public:
    
    class CNode : public TiXmlNode
    {
      public:
        int            GetNumNodes   (const string &sName = "");
        CNode         *FirstNode     (const string &sName = "");
        CNode         *NextNode      (const string &sName = "");
        string         AsString      (const string &sName, const string &sDefault = "");
        int            AsInt         (const string &sName, int           iDefault = 0);
        float          AsFloat       (const string &sName, float         fDefault = 0.f);
        bool           AsBool        (const string &sName, bool          bDefault = false);
        string         AttrAsString  (const string &sName, const string &sDefault = "");
        int            AttrAsInt     (const string &sName, int           iDefault = 0);
        float          AttrAsFloat   (const string &sName, float         fDefault = 0.f);
        bool           AttrAsBool    (const string &sName, bool          bDefault = false);
    };

  public:

    // Read
    bool               LoadFromFile  (const string &sFile);
    CNode             *FirstNode     (const string &sName);

    // Write
    bool               WriteOpen     (const string &sFile);
    void               WriteClose    ();
    void               WriteOpenNode (const string &sName, const string &sAttribs);
    void               WriteCloseNode();
    void               WriteNode     (const string &sName, const string &sAttribs, const string &sValue);
    void               WriteNode     (const string &sName, const string &sAttribs, int           iValue);
    void               WriteNode     (const string &sName, const string &sAttribs, float         fValue);
    void               WriteNode     (const string &sName, const string &sAttribs, bool          bValue);

  private:

    void               WriteTabs     (int iTabs);

  private:

    // Read
    TiXmlDocument      m_InFile;

    // Write
    CFichero           m_OutFile;
    std::stack<string> m_Nodes;
};

#endif
