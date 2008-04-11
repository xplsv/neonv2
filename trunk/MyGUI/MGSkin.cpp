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

#include <MGPch.h>
#include "MGSkin.h"
#include "MGDisplay.h"
#include "Xml.h"

MGColor SafeColor(TiXmlElement *pElem, const std::string &sChild, MGColor const &Default = MGColor(0,0,0))
{
  MGColor uRes = Default;
  const string &sColor = SafeString(pElem, sChild.c_str());
  if (sColor != "")
  {
    unsigned color;
    sscanf_s(sColor.c_str(), "%x", &color);
    uRes.r = (float)((color>>16) & 0xFF) * 1.f/255.f;
    uRes.g = (float)((color>> 8) & 0xFF) * 1.f/255.f;
    uRes.b = (float)((color>> 0) & 0xFF) * 1.f/255.f;
  }
  return uRes;
}

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMGSkin::Init(unsigned hInstance, const string &sDir)
{
  bool bRes = false;

  PushDir(sDir.c_str());

  // Load Interface image
  m_Interface = g_pMGDD->LoadImage("interface.tga");
 
  // Load Interface description
  TiXmlDocument XMLDoc("interface.xml");
  if (XMLDoc.LoadFile() && XMLDoc.FirstChild("interface"))
  {
    bRes = true;

    TiXmlNode *pMain = XMLDoc.FirstChild("interface");
    
    // Load Colors
    TiXmlElement *pColors = pMain->FirstChild("colors")->ToElement();
    #define ITEM(e,s) m_Colors[e] = SafeColor(pColors, s);
    #include "MGSkinEnumColors.h"
    #undef ITEM

    // Load Fonts
    #define ITEM(e,s,w,h,b,i,a) m_Fonts[e] = NEW CMGFont; m_Fonts[e]->Init(s,w,h,b,i,a);
    #include "MGSkinEnumFonts.h"
    #undef ITEM

    // Load Cursors
    #define ITEM(e,c) m_Cursors[e] = ::LoadCursorFromFile(c);
    #include "MGSkinEnumCursors.h"
    #undef ITEM
  }
  else
  {
    GLOG(("ERR: Can't load skin \"%s\"\n", sDir.c_str()));
  }

  PopDir();

  return bRes;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMGSkin::End()
{
  g_pMGDD->FreeImage(m_Interface);
  for (int i = 0; i < MAX_FONTS; i++)
    DISPOSE(m_Fonts[i]);
}
