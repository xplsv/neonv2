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


//---------------------------------------------------------------------------//
// GetNumNodes
//
//---------------------------------------------------------------------------//  
int CNodeFile::CNode::GetNumNodes(const string &sName)
{
  int iNum = 0;
  TiXmlNode *pIter = NULL;
  if (sName == "")
  {
    while (pIter = IterateChildren(pIter))
      iNum++;
  }
  else
  {
    while (pIter = IterateChildren(sName.c_str(), pIter))
      iNum++;
  }
  return iNum;
}


//---------------------------------------------------------------------------//
// FirstNode
//
//---------------------------------------------------------------------------//  
CNodeFile::CNode *CNodeFile::CNode::FirstNode(const string &sName)
{
    if (sName == "")
      return (CNodeFile::CNode *)FirstChild();
    else
      return (CNodeFile::CNode *)FirstChild(sName.c_str());
}


//---------------------------------------------------------------------------//
// NextNode
//
//---------------------------------------------------------------------------//  
CNodeFile::CNode *CNodeFile::CNode::NextNode(const string &sName)
{
  if (sName == "")
    return (CNodeFile::CNode *)NextSibling();
  else
    return (CNodeFile::CNode *)NextSibling(sName.c_str());
}


//---------------------------------------------------------------------------//
// AsString
//
//---------------------------------------------------------------------------//  
string CNodeFile::CNode::AsString(const string &sName, const string &sDefault)
{
  CNode *pNode = FirstNode(sName);
  if (pNode)
  {
    pNode = pNode->FirstNode();
    if (pNode && pNode->Value())
      return pNode->Value();
  }
  return sDefault;
}


//---------------------------------------------------------------------------//
// AsInt
//
//---------------------------------------------------------------------------//  
int CNodeFile::CNode::AsInt(const string &sName, int iDefault)
{
  CNode *pNode = FirstNode(sName);
  if (pNode)
  {
    pNode = pNode->FirstNode();
    if (pNode && pNode->Value())
      return atoi(pNode->Value());
  }
  return iDefault;
}


//---------------------------------------------------------------------------//
// AsFloat
//
//---------------------------------------------------------------------------//  
float CNodeFile::CNode::AsFloat(const string &sName, float fDefault)
{
  CNode *pNode = FirstNode(sName);
  if (pNode)
  {
    pNode = pNode->FirstNode();
    if (pNode && pNode->Value())
      return strtofloat(pNode->Value());
  }
  return fDefault;
}


//---------------------------------------------------------------------------//
// AsBool
//
//---------------------------------------------------------------------------//  
bool CNodeFile::CNode::AsBool(const string &sName, bool bDefault)
{
  CNode *pNode = FirstNode(sName);
  if (pNode)
  {
    pNode = pNode->FirstNode();
    if (pNode && pNode->Value())
      return (atoi(pNode->Value()) == 0 ? false : true);
  }
  return bDefault;
}



//---------------------------------------------------------------------------//
// AttrAsString
//
//---------------------------------------------------------------------------//  
string CNodeFile::CNode::AttrAsString(const string &sName, const string &sDefault)
{
  const char *pData = ToElement()->Attribute(sName.c_str());
  return (pData ? pData : sDefault);
}


//---------------------------------------------------------------------------//
// AttrAsInt
//
//---------------------------------------------------------------------------//  
int CNodeFile::CNode::AttrAsInt(const string &sName, int iDefault)
{
  const char *pData = ToElement()->Attribute(sName.c_str());
  return (pData ? atoi(pData) : iDefault);
}


//---------------------------------------------------------------------------//
// AttrAsFloat
//
//---------------------------------------------------------------------------//  
float CNodeFile::CNode::AttrAsFloat(const string &sName, float fDefault)
{
  const char *pData = ToElement()->Attribute(sName.c_str());
  return (pData ? (float)strtofloat(pData) : fDefault);
}


//---------------------------------------------------------------------------//
// AttrAsBool
//
//---------------------------------------------------------------------------//  
bool CNodeFile::CNode::AttrAsBool(const string &sName, bool bDefault)
{
  const char *pData = ToElement()->Attribute(sName.c_str());
  return (pData ? (atoi(pData) == 0 ? false : true) : bDefault);
}


//---------------------------------------------------------------------------//
// LoadFromFile
//
//---------------------------------------------------------------------------//  
bool CNodeFile::LoadFromFile(const string &sFile)
{
  return m_InFile.LoadFile(sFile.c_str());
}


//---------------------------------------------------------------------------//
// FirstNode
//
//---------------------------------------------------------------------------//  
CNodeFile::CNode *CNodeFile::FirstNode(const string &sName)
{
  if (sName == "")
    return (CNode *)m_InFile.FirstChild();
  else
    return (CNode *)m_InFile.FirstChild(sName.c_str());
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
bool CNodeFile::WriteOpen(const string &sFile)
{
  while (m_Nodes.size() > 0)
    m_Nodes.pop();
  return m_OutFile.Open(sFile.c_str(), "wb");
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteClose()
{
  while (m_Nodes.size() > 0)
    m_Nodes.pop();
  m_OutFile.Close();
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteOpenNode(const string &sName, const string &sAttribs)
{
  WriteTabs(m_Nodes.size());
  string sLine = "<" + sName + " " + sAttribs + ">\r\n";
  m_OutFile.WriteLn(sLine.c_str());
  m_Nodes.push(sName);
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteCloseNode()
{
  string sName = m_Nodes.top();
  m_Nodes.pop();
  WriteTabs(m_Nodes.size());
  string sLine = "</" + sName + ">\r\n";
  m_OutFile.WriteLn(sLine.c_str());
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteNode(const string &sName, const string &sAttribs, const string &sValue)
{
  WriteTabs(m_Nodes.size());
  char pBuffer[512];
  sprintf_s(pBuffer, 512, "<%s %s>%s</%s>\r\n", sName.c_str(), sAttribs.c_str(), sValue.c_str(), sName.c_str());
  m_OutFile.WriteLn(pBuffer);
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteNode(const string &sName, const string &sAttribs, int iValue)
{
  WriteTabs(m_Nodes.size());
  char pBuffer[512];
  sprintf_s(pBuffer, 512, "<%s %s>%d</%s>\r\n", sName.c_str(), sAttribs.c_str(), iValue, sName.c_str());
  m_OutFile.WriteLn(pBuffer);
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteNode(const string &sName, const string &sAttribs, float fValue)
{
  WriteTabs(m_Nodes.size());
  char pBuffer[512];
  sprintf_s(pBuffer, 512, "<%s %s>%f</%s>\r\n", sName.c_str(), sAttribs.c_str(), fValue, sName.c_str());
  m_OutFile.WriteLn(pBuffer);
}


//---------------------------------------------------------------------------//
// WriteOpen
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteNode(const string &sName, const string &sAttribs, bool bValue)
{
  WriteTabs(m_Nodes.size());
  char pBuffer[512];
  sprintf_s(pBuffer, 512, "<%s %s>%d</%s>\r\n", sName.c_str(), sAttribs.c_str(), bValue ? 1 : 0, sName.c_str());
  m_OutFile.WriteLn(pBuffer);
}


//---------------------------------------------------------------------------//
// WriteTabs
//
//---------------------------------------------------------------------------//  
void CNodeFile::WriteTabs(int iTabs)
{
  int iSpaces = iTabs*2;
  for (int i = 0; i < iSpaces; i++)
      m_OutFile.Write(" ", 1, 1);
}
