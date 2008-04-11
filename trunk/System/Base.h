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

#ifndef  BASE_H
#define  BASE_H

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;
using std::pair;
using std::map;

#ifdef _DEBUG
  #pragma inline_depth(0) // POLTERGESIT
  #pragma inline_depth(0) // Por alguna razon, con el VC++ 6.0 si solo se pone una vez no hace ni puto caso
#else
  #pragma inline_depth(32)
#endif

//----------------------------------------------------------------------------
// Tipos destinados a trabajo lógico
//----------------------------------------------------------------------------
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;

//----------------------------------------------------------------------------
// Tipos destinados a uso como almacenamiento de memoria
//----------------------------------------------------------------------------
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned int   dword;

//----------------------------------------------------------------------------
// NULL
//----------------------------------------------------------------------------
#ifndef NULL
#define NULL 0
#endif

#define sbyte   char
#define sword   short
#define sdword  int
#define sqword  __int64
#define qword   unsigned __int64;

#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

//------------------------------------------------------------------------------
// Codigos de error
//------------------------------------------------------------------------------
enum
{
  RET_OK = 0,
  RET_FAIL,
  ERR_START
};

//------------------------------------------------------------------------------
// Macros de asignacion de memoria
//------------------------------------------------------------------------------
#ifndef USE_MFC
  //inline void *__cdecl operator new(size_t, void *p) { return p; }
  //inline void  __cdecl operator delete(void *, void *p) {}
#else
  #undef delete
  #undef new
#endif

#ifdef GESTORMEMORIA
  void *__cdecl operator new(size_t iTamanyo, const char *pszFichero, int iLinea);
  void  __cdecl operator delete(void *pvMem);
  void  __cdecl operator delete(void *pvMem, const char *pszFichero, int iLinea);
  void  GestionMemoriaInit();
  void  GestionMemoriaEnd();
  void  dispose(void *p);
  #define GESTION_MEMORIA_INIT()  GestionMemoriaInit()
  #define GESTION_MEMORIA_END()   GestionMemoriaEnd()
  #define NEW new(__FILE__, __LINE__)
  #define NEW_ARRAY(x,c) new(__FILE__,__LINE__) x[sizeof(x)*c]
  #define DISPOSE(p)        do { if (p)  { delete   (p); (p)  = NULL; } } while(0);
  #define DISPOSE_ARRAY(pa) do { if (pa) { delete [](pa);(pa) = NULL; } } while(0);
#else
  #define GESTION_MEMORIA_INIT() (void(0))
  #define GESTION_MEMORIA_END()  (void(0))
  void *__cdecl operator new(size_t iTamanyo);
  void  __cdecl operator delete(void *pvMem);
  #define NEW new
  #define NEW_ARRAY(x,c) new(x[sizeof(x)*c])
  #define DISPOSE(p)        do { if (p)  { delete   (p); (p)  = NULL; } } while(0);
  #define DISPOSE_ARRAY(pa) do { if (pa) { delete [](pa);(pa) = NULL; } } while(0);
#endif

#define DISPOSE_ARRAY_PUNTEROS(pa, cant) { if (pa) { for (uint i=0; i<cant; i++) DISPOSE(pa[i]); DISPOSE_ARRAY(pa); } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//----------------------------------------------------------------------------
// Assert
//----------------------------------------------------------------------------
void  NotifyAssertFailure(const char *pszText, const char *pszFile, unsigned Line);
void  NotifyAssertFailure(const char *pszText,const char *pszMensaje,const char *pszFile, unsigned Line);
  
#ifndef _DEBUG
  #ifndef ASSERT
    #define ASSERT(exp)
  #endif
  #ifndef ASSERTM
    #define ASSERTM(exp,msg)
  #endif
  #define NASSERT
#else
  #ifndef ASSERT
    #define ASSERT(exp) (void)( (exp) || (NotifyAssertFailure(#exp, __FILE__, __LINE__), 0) )
  #endif
  #ifndef ASSERTM
    #define ASSERTM(exp,msg) (void)( (exp) || (NotifyAssertFailure(#exp, msg, __FILE__, __LINE__), 0) )
  #endif
#endif

typedef void (*TOnAssertFailure)(void);
void OnAssertFailure(TOnAssertFailure);

//------------------------------------------------------------------------------
// Macros para Log en un fichero
//------------------------------------------------------------------------------
#define ENABLE_LOG
#ifdef ENABLE_LOG
  enum
  {
    LOGOUTPUT,
    LOGLISTBOX,
  };
  void _glogStart(int type, unsigned lParam);
  void _glogEnd();
  void _log(char *fmt, ...);
  #define GLOGSTART(type, lParam) _glogStart(type, lParam)
  #define GLOGEND()               _glogEnd()
  #define GLOG(msg)               _log msg
#else
  #define GLOGSTART(type, lParam) (void(0))
  #define GLOGEND()               (void(0))
  #define GLOG(msg)               (void(0))
#endif

struct TRect
{ 
  TRect() { };
  TRect(int _x, int _y, int _w, int _h) { x = _x; y = _y; w = _w; h = _h; }
  int x, y, w, h;
};
__inline void SetRect(TRect  &Rect, int   x, int   y, int   w, int   h) { Rect = TRect (x,y,w,h); }

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "Fichero.h"
#include "Lista.h"
#include "Str.h"
#include "FindFiles.h"
#include "MMath.h"
#include "NodeFile.h"
#include "Timer.h"

#endif
