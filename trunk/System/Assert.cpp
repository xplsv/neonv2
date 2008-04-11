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
#include <stdio.h>
#include <signal.h>

static TOnAssertFailure s_OnAssertFailure = NULL;

void OnAssertFailure(TOnAssertFailure p)
{
  s_OnAssertFailure = p;
}

//----------------------------------------------------------------------
//  Uso:            Visualiza una ventana de dialogo con informacion sobre el assert 
//  Retorno:        nada
//  Parametros:     Buffer: Cadena a visualizar
//----------------------------------------------------------------------

void NotifyAssertFailureInternal( const char *pszBuffer )
{

  if(s_OnAssertFailure)
    s_OnAssertFailure();

  int nCode = MessageBox( NULL, pszBuffer, "Assert Error", MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL );

  switch( nCode )
  {
    case IDABORT:
      raise( SIGABRT ); /* raise abort signal */
      _exit(3);         /* Por si la excepcion es ignorada */
      break;

    case IDRETRY:
      _asm int 3;
      return;           /* return to user code */

    case IDIGNORE:
      break;

    default:
      raise( SIGABRT ); /* raise abort signal */
      _exit(3);         /* Por si la excepcion es ignorada */

  }
}

//----------------------------------------------------------------------
//  Uso:            Visualiza una ventana de dialogo con informacion sobre el assert 
//  Retorno:        nada
//  Parametros:     pcExpr: Linea de codigo que genero el assert
//                  pcFile: Fichero en en que se encuentra sExpr
//                  sLine: Line dentro de sFile
//----------------------------------------------------------------------

void NotifyAssertFailure( const char *pszExpr, const char *pszFile, unsigned uLine )
{
  char Buffer[ 500 ];
  sprintf_s(Buffer, 500, "Expr: %s\nFile: %s\nLine: %ld", pszExpr, pszFile, uLine );
  NotifyAssertFailureInternal( Buffer );

}

//----------------------------------------------------------------------
//  Uso:            Visualiza una ventana de dialogo con informacion sobre el assert 
//  Retorno:        nada
//  Parametros:     pcExpr: Linea de codigo que genero el assert
//                  pcMsg:  Messaje del añadido al assert 
//                  pcFile: Fichero en en que se encuentra sExpr
//                  sLine: Line dentro de sFile
//----------------------------------------------------------------------

void NotifyAssertFailure( const char *pszExpr,const char *pszMsg ,const char *pszFile, unsigned uLine )
{
  char Buffer[ 500 ];
  sprintf_s(Buffer, 500, "Msg: %s\nExpr: %s\nFile: %s\nLine: %ld", pszMsg,pszExpr, pszFile, uLine );
  NotifyAssertFailureInternal( Buffer );

}

#ifdef _DEBUG

void _trace(char *fmt, ...)
{
char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf_s(out, 1024, fmt, body);
	va_end(body);
	OutputDebugString(out);
}

#endif