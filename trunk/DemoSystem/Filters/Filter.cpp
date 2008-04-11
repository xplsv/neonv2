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

#include "DemoSystemPch.h"
#include "Filters/EnumFilters.h"


//---------------------------------------------------------------------------//
// Carga un filtro desde un fichero NF2 (OptionsFile)
//
//---------------------------------------------------------------------------//
CFilter *CFilter::Create(const string &sFile)
{
  CFilter *pFilter = NULL;
  // Options document
  COptions Options;
  if (Options.LoadFromFile(sFile + ".nf2"))
  {
    const string &sClass = Options.Option("filter");
    if (sClass != "")
    {
      #define ITEM(nombre,clase) if (!pFilter && !Stricmp(nombre,sClass.c_str())) { pFilter = NEW clase; }
      #include "Filters/EnumFilters.h"
      #undef ITEM
      if (!pFilter || !pFilter->Init(Options))
      {
        DISPOSE(pFilter);
        GLOG(("ERR: Can't load Filter '%s'\n", sClass.c_str()));
      }
    }
    else
      GLOG(("ERR: Filter type not specified on file %s.nf2\n", sFile.c_str()));
  }
  else
      GLOG(("ERR: Failed to load Filter from file %s.nf2\n", sFile.c_str()));
  return pFilter;
}


//---------------------------------------------------------------------------//
// El filtro por defecto no tiene variables
//---------------------------------------------------------------------------//
static TCtrlVar s_Vars[] =
{
  {TCtrlVar::INVALID},
};


//---------------------------------------------------------------------------//
// GetVarCtrls
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilter::GetVarCtrls()
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CFilter::SetTime(float fTime)
{
  m_fTimeFrame = m_fTime - m_fTimeAnt;
  m_fTimeAnt   = m_fTime;
  m_fTime      = fTime;
}
