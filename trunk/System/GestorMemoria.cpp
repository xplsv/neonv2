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

#include "Base.h"
#include "Lista.h"
#include <malloc.h>
#include <string.h>

#undef new
#undef delete

#ifdef GESTORMEMORIA

struct TNodoLista
{
  uint        size;
  char       *szFile;
  int         Linea;
  TNodoLista *pNext;
  TNodoLista *pAnt;
  uint        firma;  // == 0xE0001234
};

static TNodoLista *s_pMemoriaReservada     = NULL;
static TNodoLista *s_pMemoriaReservadaLast = NULL;


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
void GestionMemoriaInit()
{
  s_pMemoriaReservada = s_pMemoriaReservadaLast = NULL;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void GestionMemoriaEnd()
{
  if (s_pMemoriaReservada)
  {
    GLOG(("ERR: Memory Leaks detected!!!\n"));
    TNodoLista *pNodo = s_pMemoriaReservada;
    int conta = 0;
    int total = 0;
    while(pNodo)
    {
      GLOG(("%s(%d) : Ptr = 0x%X (size: %d)\n", pNodo->szFile, pNodo->Linea, (uint)pNodo + sizeof(TNodoLista), pNodo->size));
      total+= pNodo->size;
      conta++;
      pNodo = pNodo->pNext;
    }
    GLOG(("Total: %d (Memory not freed: %d)\n", conta, total));
  }
}


//---------------------------------------------------------------------------//
// new
//
//---------------------------------------------------------------------------//
void *__cdecl operator new(size_t size, const char *szFile, int Linea)
{
  //TNodoLista *pNodo = (TNodoLista *)malloc(sizeof(TNodoLista)+ size);
  TNodoLista *pNodo = (TNodoLista *)GlobalAlloc(0, sizeof(TNodoLista)+ size);
  //TNodoLista *pNodo = (TNodoLista *)::new char[sizeof(TNodoLista)+ size];
  
  memset(pNodo, 0, sizeof(TNodoLista)+size);
//  pNodo->pMemoria = (void *)((uint)pNodo + sizeof(TNodoLista)); // nos servirá como flag para detectar bloques generados por este new
  pNodo->size = size;
  size_t len = strlen(szFile);
  //pNodo->szFile = (char *)malloc(len+1);
  pNodo->szFile = (char *)GlobalAlloc(0, len+1);
  //pNodo->szFile = ::new char[len+1];
  memcpy(pNodo->szFile, szFile, len);
  pNodo->szFile[len] = 0;
  pNodo->Linea = Linea;
  pNodo->pNext = NULL;
  pNodo->pAnt = s_pMemoriaReservadaLast;
  pNodo->firma = 0xE0001234;

  if(!s_pMemoriaReservada)
  {
    s_pMemoriaReservada = pNodo;
    s_pMemoriaReservadaLast = pNodo;
  }
  else
  {
    s_pMemoriaReservadaLast->pNext = pNodo;
    s_pMemoriaReservadaLast = pNodo;
  }

  return (void *)((uint)pNodo + sizeof(TNodoLista));
}


//---------------------------------------------------------------------------//
// delete
//
//---------------------------------------------------------------------------//
void __cdecl operator delete(void* p, const char* szFile, int Linea)
{ 
  ASSERT(0);
}

void __cdecl operator delete(void *p)
{
  if(!p)
    return;

  TNodoLista *pNodo = (TNodoLista *)((uint)p - sizeof(TNodoLista));
  
  if(pNodo->firma != 0xE0001234)  // no se usó nuestro new para reservar este bloque de memoria
  {
    return;
  }

  if(pNodo->pAnt == NULL)
    s_pMemoriaReservada = pNodo->pNext;
  else
    pNodo->pAnt->pNext = pNodo->pNext;
  if(pNodo->pNext == NULL)
    s_pMemoriaReservadaLast = pNodo->pAnt;
  else  
    pNodo->pNext->pAnt = pNodo->pAnt;

  GlobalFree(pNodo->szFile);
  GlobalFree(pNodo);
  //::delete(pNodo->szFile);
  //::delete(pNodo);
}


#endif
