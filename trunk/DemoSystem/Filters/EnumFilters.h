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

#ifndef ITEM

#include "FilterBlur.h"
#include "FilterGlow.h"
#include "FilterDist.h"
#include "FilterShader.h"
#include "FilterSpriter.h"
#include "FilterDLL.h"

#else

ITEM("Blur",      CFilterBlur)
ITEM("Glow",      CFilterGlow)
ITEM("Dist",      CFilterDist)
ITEM("Shader",    CFilterShader)
ITEM("Spriter",   CFilterSpriter)
ITEM("DLL",       CFilterDll)

#endif

/*
#include "FilterFreeframe.h"
#include "FilterShader.h"
#include "FilterGlow.h"
#include "FilterBlur.h"
#include "FilterPixelate.h"
#include "FilterScanner.h"
#include "FilterRGB.h"
#include "FilterDist.h"
#include "FilterHeat.h"
#include "FilterSpriter.h"
#include "FilterDll.h"

#else

ITEM("Freeframe",      CFilterFreeframe)
ITEM("FilterBasic",    CFilter)
ITEM("FilterShader",   CFilterShader)
ITEM("FilterGlow",     CFilterGlow)
ITEM("FilterBlur",     CFilterBlur)
ITEM("FilterPixelate", CFilterPixelate)
ITEM("FilterScanner",  CFilterScanner)
ITEM("FilterRGB",      CFilterRGB)
ITEM("FilterDist",     CFilterDist)
ITEM("FilterHeat",     CFilterHeat)
ITEM("FilterSpriter",  CFilterSpriter)
ITEM("FilterDll",      CFilterDll)

#endif

*/
