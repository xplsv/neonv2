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

#include <NeonPch.h>
#include "MGAppMain.h"
#include "AppOptions.h"
#include "Launcher.h"

void PeticionEndApp() { }

//---------------------------------------------------------------------------//
// WinMain
//
//---------------------------------------------------------------------------//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  CoInitialize(NULL);
  InitFloatToStr();
  srand ((unsigned)time(NULL));

  IAppOptions::GetInstance()->Load("neonv2.cfg");
  // Load launcher
  CLauncher Launcher;
  if (Launcher.Show((unsigned)hInstance))
  {
      IAppOptions::GetInstance()->Save();
      string lLogFile = "neonv2.log";
      GESTION_MEMORIA_INIT();
      GLOGSTART(LOGLISTBOX, (long)lLogFile.c_str());
      CMGAppMain AppMain;
      if (AppMain.Init((unsigned)hInstance))
      {
        while (!AppMain.Update())
        {
        }
        AppMain.End();
      }
      else
        MessageBox(NULL, "ERR: Can't load Neon v2. See neonv2.log for more details", "Fatal Error", MB_OK | MB_ICONERROR);

      GLOGEND();
      GESTION_MEMORIA_END();
  }

  IAppOptions::EndInstance();

  return 0;
}
