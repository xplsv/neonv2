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

#ifndef XML_H
#define XML_H

#include "TinyXml.h"

const char *SafeString      (TiXmlElement *pElem, const char *pChild, const char *pDefault = NULL);
int         SafeInteger     (TiXmlElement *pElem, const char *pChild, int         iDefault = 0);
float       SafeFloat       (TiXmlElement *pElem, const char *pChild, float       fDefault = 0.f);
bool        SafeBool        (TiXmlElement *pElem, const char *pChild, bool        bDefault = false);
const char *SafeStringAttr  (TiXmlElement *pElem, const char *pAttr,  const char *pDefault = NULL);
int         SafeIntegerAttr (TiXmlElement *pElem, const char *pAttr,  int         iDefault = 0);
float       SafeFloatAttr   (TiXmlElement *pElem, const char *pAttr,  float       fDefault = 0.f);
bool        SafeBoolAttr    (TiXmlElement *pElem, const char *pAttr,  bool        bDefault = false);
int         GetNumSameChilds(TiXmlNode    *pNode, const char *pChild);

class CXmlLoader
{

};

#endif
