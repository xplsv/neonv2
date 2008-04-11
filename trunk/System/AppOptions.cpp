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
#include "AppOptions.h"
#include "NodeFile.h"
#include <map>

#define MAX_VALUE_NAME 512
#define MAX_VALUE_SIZE 1024
#define MAX_KEY_LENGTH 2048

class CAppOptions : virtual public IAppOptions
{
  public:

    static  IAppOptions              *GetInstance();

  public:

    virtual void                      Set        (const std::string &aParamName, const int         &aValue);
    virtual int                       Get        (const std::string &aParamName, const int         &aDefaultValue);
    virtual void                      Set        (const std::string &aParamName, const std::string &aValue);
    virtual const std::string        &Get        (const std::string &aParamName, const std::string &aDefaultValue);

    virtual void                      Clear      ();

    virtual bool                      Load       (const string &aName);
    virtual void                      Save       (const string &aName = "");

  public:
    static IAppOptions               *m_AppOptions;
    std::map<std::string,int>         m_IntMap;
    std::map<std::string,std::string> m_StrMap;
    std::string                       m_Name;
};

IAppOptions *CAppOptions::m_AppOptions = NULL;


//---------------------------------------------------------------------------//
// Interface creator
//
//---------------------------------------------------------------------------//
IAppOptions *IAppOptions::GetInstance()
{
  return CAppOptions::GetInstance();
}


//---------------------------------------------------------------------------//
// GetInstance
//
//---------------------------------------------------------------------------//
IAppOptions *CAppOptions::GetInstance()
{
  if (!m_AppOptions)
    m_AppOptions = new CAppOptions();
  return m_AppOptions;
}


//---------------------------------------------------------------------------//
// EndInstance
//
//---------------------------------------------------------------------------//
void IAppOptions::EndInstance()
{
  DISPOSE(CAppOptions::m_AppOptions);
}



//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//
void CAppOptions::Set(const std::string &aParamName, const int &aValue)
{
  m_IntMap[aParamName] = aValue;
}


//---------------------------------------------------------------------------//
// Get
//
//---------------------------------------------------------------------------//
int CAppOptions::Get(const std::string &aParamName, const int &aDefaultValue)
{
  std::map<std::string,int>::const_iterator lIt = m_IntMap.find(aParamName);
  if (lIt == m_IntMap.end())
  {
    Set(aParamName, aDefaultValue);
    return aDefaultValue;
  }
  return lIt->second;
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//
void CAppOptions::Set(const std::string &aParamName, const std::string &aValue)
{
  m_StrMap[aParamName] = aValue;
}


//---------------------------------------------------------------------------//
// Get
//
//---------------------------------------------------------------------------//
const std::string &CAppOptions::Get(const std::string &aParamName, const std::string &aDefaultValue)
{ 
  std::map<std::string,std::string>::const_iterator lIt = m_StrMap.find(aParamName);
  if (lIt == m_StrMap.end())
  {
    Set(aParamName, aDefaultValue);
    return aDefaultValue;
  }
  return lIt->second;
}


//---------------------------------------------------------------------------//
// Get
//
//---------------------------------------------------------------------------//
void CAppOptions::Clear()
{
  m_IntMap.clear();
  m_StrMap.clear();
}


//---------------------------------------------------------------------------//
// Load values from the specified key
//
//---------------------------------------------------------------------------//
bool CAppOptions::Load(const string &aName)
{
  m_Name = aName;
  CNodeFile NodeFile;
  if (NodeFile.LoadFromFile(aName))
  {
    CNodeFile::CNode *pNode = NodeFile.FirstNode("config");
    if (pNode)
    {
      // Strings
      CNodeFile::CNode *pStrings = pNode->FirstNode("string");
      while (pStrings)
      {
        const string &sName  = pStrings->AttrAsString("name",  "");
        if (sName != "") Set(sName, pStrings->AttrAsString("value", ""));
        pStrings = pStrings->NextNode("string");
      }
      // Ints
      CNodeFile::CNode *pInts = pNode->FirstNode("int");
      while (pInts)
      {
        const string &sName = pInts->AttrAsString("name",  "");
        if (sName != "") Set(sName, pInts->AttrAsInt("value", 0));
        pInts = pInts->NextNode("int");
      }
    }
    return true;
  }
  return false;
}


//---------------------------------------------------------------------------//
// Save values to specified place
//
//---------------------------------------------------------------------------//
void CAppOptions::Save(const string &aName)
{
  if (aName != "")
    m_Name = aName;

  CNodeFile NodeFile;
  NodeFile.WriteOpen(m_Name);
  NodeFile.WriteOpenNode("config", "");

  // Save ints
  for (std::map<std::string,int>::const_iterator lIter = m_IntMap.begin(); lIter != m_IntMap.end(); lIter++)
  {
    char pBuffer[1000];
    sprintf_s(pBuffer, 1000, "name=\"%s\" value=\"%d\"", lIter->first.c_str(), lIter->second);
    NodeFile.WriteNode("int", pBuffer, false);
  }
  // Save strings
  for (std::map<std::string,std::string>::const_iterator lIter = m_StrMap.begin(); lIter != m_StrMap.end(); lIter++)
  {
    char pBuffer[1000];
    sprintf_s(pBuffer, 1000, "name=\"%s\" value=\"%s\"", lIter->first.c_str(), lIter->second.c_str());
    NodeFile.WriteNode("string", pBuffer, false);
  }

  NodeFile.WriteCloseNode();
  NodeFile.WriteClose();
}
