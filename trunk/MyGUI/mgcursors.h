//---------------------------------------------------------------------------//
// File: MGCursors.h
//
//---------------------------------------------------------------------------//

#ifndef _MGCURSORS_H
#define _MGCURSORS_H

#include "MGBase.h"

enum
{
  CUR_ARROW,
  CUR_DRAG,
  CUR_DRAGNO,
  MAX_CURSORS,
};

void         CUR_Init     ();
void         CUR_Quit     ();

#endif
