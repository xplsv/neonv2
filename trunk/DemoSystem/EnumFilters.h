//---------------------------------------------------------------------------//
// File: EnumFilters.h
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

ITEM("Blur",    CFilterBlur)
ITEM("Glow",    CFilterGlow)
ITEM("Dist",    CFilterDist)
ITEM("Shader",  CFilterShader)
ITEM("Spriter", CFilterSpriter)
ITEM("DLL",     CFilterDll)

#endif

/*
#include "FilterShader.h"
#include "FilterGlow.h"
#include "FilterBlur.h"
#include "FilterPixelate.h"
#include "FilterScanner.h"
#include "FilterRGB.h"
#include "FilterDist.h"
#include "FilterHeat.h"
#include "FilterSpriter.h"
//#include "FilterFreeframe.h"
#include "FilterDll.h"

#else

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
//ITEM("FilterFreeframe",CFilterFreeframe)
ITEM("FilterDll",      CFilterDll)

#endif

*/
