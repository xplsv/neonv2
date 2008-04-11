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

#ifndef _APPOPTIONS_H
#define _APPOPTIONS_H

#include <string>

class IAppOptions
{
  public:
    
    static  IAppOptions            *GetInstance();
    static  void                    EndInstance();

  public:

    virtual                        ~IAppOptions() {};

    virtual void                    Set        (const std::string &aParamName, const int         &aValue)        = 0;
    virtual int                     Get        (const std::string &aParamName, const int         &aDefaultValue) = 0;
    virtual void                    Set        (const std::string &aParamName, const std::string &aValue)        = 0;
    virtual const std::string      &Get        (const std::string &aParamName, const std::string &aDefaultValue) = 0;

    virtual void                    Clear      () = 0;

    virtual bool                    Load       (const string &aName) = 0;
    virtual void                    Save       (const string &aName = "") = 0;
};

#endif
