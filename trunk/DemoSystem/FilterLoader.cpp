//---------------------------------------------------------------------------//
// File: Filter.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "FilterLoader.h"
#include "OptionFile.h"
#include "EnumFilters.h"

namespace FilterLoader {

//---------------------------------------------------------------------------//
// Carga un filtro de un fichero de definicion de filtro (nfl)
//
//---------------------------------------------------------------------------//
CFilter *CargaFiltro(const string &sFile)
{
  CFilter *pFilter = NULL;

  // XML Document
  COptionFile File;
  if (File.Load(sFile))
  {
      const string &sClass = File.Get("filterclass");
      if (sClass != "")
      {
          #define ITEM(nombre,clase) if (!pFilter && !Stricmp(nombre,pszFilter)) { pFilter = NEW clase; }
          #include "EnumFilters.h"
          #undef ITEM
          if (!pFilter || !pFilter->Init(File.Options()))
          {
            DISPOSE(pFilter);
            GLOG(("ERR: Can't load FilterClass %s\n", sClass.c_str()));
          }
        }
        else
          GLOG(("ERR: FilterClass not specified on Filter %s\n", sFile.c_str()));
      }
  }

  return pFilter;
}

}