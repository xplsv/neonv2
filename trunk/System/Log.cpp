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
#include <fstream>


static int            s_iLogType;
static int            s_iLogStrings;
static FILE          *s_hLogOut;
static HWND           s_hListBoxLog;
static std::ofstream  s_Stream;;

//----------------------------------------------------------------------------
// Log que muestra cadenas en el Output del VisualStudio
//----------------------------------------------------------------------------
#ifdef ENABLE_LOG

void _glogStart(int iType, unsigned lParam)
{
  s_iLogType = iType;
  s_hLogOut  = fopen((const char *)lParam, "wt+");
  switch (s_iLogType)
  {
    case LOGLISTBOX:
      s_hListBoxLog = CreateWindow("listbox", "Log", WS_VSCROLL | WS_TABSTOP, 0,480, 600,300, NULL, NULL, NULL, NULL);
      s_iLogStrings = 0;
      ShowWindow(s_hListBoxLog, SW_SHOW);
      break;
    case LOGOUTPUT: 
      GLOG(("---------------------------------------\n"));
      GLOG((" Start Application \n"));
      GLOG(("---------------------------------------\n"));
      break;
  }
}

void _log(char *fmt, ...)
{
  char out[1024];
	va_list body;
	va_start(body, fmt);
	vsprintf_s(out, 1024, fmt, body);
	va_end(body);
  int i, iLen;
#ifdef GESTORMEMORIA
  if (s_iLogType != LOGOUTPUT)
    OutputDebugString(out);
#endif
  switch (s_iLogType)
  {
    case LOGLISTBOX:
      fprintf(s_hLogOut, out);
      fflush(s_hLogOut);

      iLen = strlen(out);
      for (i = 0; i < iLen; i++)
      {
        if (out[i] == '\n' || out[i] == '\r')
          out[i] = 0;
      }
	    SendMessage(s_hListBoxLog, LB_ADDSTRING, 0, (LPARAM)out);
      SendMessage(s_hListBoxLog, LB_SETCURSEL, s_iLogStrings, 0);
      s_iLogStrings++;
      break;
  }
}

void _glogEnd()
{
  switch (s_iLogType)
  {
    case LOGLISTBOX:
      fclose(s_hLogOut); break;
      break;
    case LOGOUTPUT:
      GLOG(("---------------------------------------\n"));
      GLOG((" End Application\n"));
      GLOG(("---------------------------------------\n"));
  }
}

#endif
