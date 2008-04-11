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
#include "Xml.h"

//---------------------------------------------------------------------------//
// SafeGets
//---------------------------------------------------------------------------//
const char *SafeString(TiXmlElement *pElem, const char *pChild, const char *pDefault)
{
  TiXmlNode *pData = pElem->FirstChild(pChild);
  return ((pData && pData->FirstChild()) ? pData->FirstChild()->Value() : pDefault);
}

int SafeInteger(TiXmlElement *pElem, const char *pChild, int iDefault)
{
  TiXmlNode *pData = pElem->FirstChild(pChild);
  return ((pData && pData->FirstChild()) ? atoi(pData->FirstChild()->Value()) : iDefault);
}

float SafeFloat(TiXmlElement *pElem, const char *pChild, float fDefault)
{
  TiXmlNode *pData = pElem->FirstChild(pChild);
  return ((pData && pData->FirstChild()) ? (float)strtofloat(pData->FirstChild()->Value()) : fDefault);
}

bool SafeBool(TiXmlElement *pElem, const char *pChild, bool bDefault)
{
  TiXmlNode *pData = pElem->FirstChild(pChild);
  return ((pData && pData->FirstChild()) ? (atoi(pData->FirstChild()->Value()) == 0 ? false : true) : bDefault);
}

//---------------------------------------------------------------------------//
// SafeGetAtt
//---------------------------------------------------------------------------//
const char *SafeStringAttr(TiXmlElement *pElem, const char *pAttr, const char *pDefault)
{
  const char *pData = pElem->Attribute(pAttr);
  return (pData ? pData : pDefault);
}

int SafeIntegerAttr(TiXmlElement *pElem, const char *pAttr, int iDefault)
{
  const char *pData = pElem->Attribute(pAttr);
  return (pData ? atoi(pData) : iDefault);
}

float SafeFloatAttr(TiXmlElement *pElem, const char *pAttr, float fDefault)
{
  const char *pData = pElem->Attribute(pAttr);
  return (pData ? (float)strtofloat(pData) : fDefault);
}

bool SafeBoolAttr(TiXmlElement *pElem, const char *pAttr, bool bDefault)
{
  const char *pData = pElem->Attribute(pAttr);
  return (pData ? (atoi(pData) == 0 ? false : true) : bDefault);
}

//---------------------------------------------------------------------------//
// GetNumSameChilds
//---------------------------------------------------------------------------//
int GetNumSameChilds(TiXmlNode *pNode, const char *pChild)
{
  int iNum = 0;
  TiXmlNode *pIter = NULL;
  while (pIter = pNode->IterateChildren(pChild, pIter))
    iNum++;
  return iNum;
}
