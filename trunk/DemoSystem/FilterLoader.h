 //---------------------------------------------------------------------------//
// File: FilterLoader.h
//
//---------------------------------------------------------------------------//

#ifndef FILTERLOADER_H
#define FILTERLOADER_H

class CFilter;

namespace FilterLoader
{
  CFilter *Load(const string &sFile);
}

#endif
