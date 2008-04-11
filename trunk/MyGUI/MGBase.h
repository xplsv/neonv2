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

#ifndef _MGBASE_H
#define _MGBASE_H

#include "Base.h"
#include "MGApplication.h"
#include "MGSurface.h"
#include "MGFont.h"

#define __PLACEMENT_NEW_INLINE
#include <boost/function.hpp>
#include <boost/bind.hpp>
#undef  __PLACEMENT_NEW_INLINE

using boost::function;
using boost::bind;

extern CMGDisplay     *g_pMGDD;
extern CMGApplication *g_pMGApp;

enum EMouseButton
{
  MGB_LEFT,
  MGB_RIGHT,
  MGB_MID,
};

enum EAlign
{
  MGALIGN_NONE,
  MGALIGN_CLIENT,
  MGALIGN_LEFT,
  MGALIGN_RIGHT,
  MGALIGN_TOP,
  MGALIGN_BOTTOM,
};

#endif
