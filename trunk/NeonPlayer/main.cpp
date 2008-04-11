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
#include "GEEngineGrafico.h"
#include "AppOptions.h"
#include "LuaPlayer.h"
#include "Launcher.h"

bool s_End = false;
void PeticionEndApp()
{
  s_End = true;
}

//---------------------------------------------------------------------------//
// WinMain
//
//---------------------------------------------------------------------------//
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  CoInitialize(NULL);
  InitFloatToStr();
  srand ((unsigned)time(NULL));

  // Load launcher
  CLauncher Launcher;
  if (Launcher.Show((unsigned)hInstance))
  {
      string lLogFile = "neonv2.log";
      GESTION_MEMORIA_INIT();
      GLOGSTART(LOGLISTBOX, (long)lLogFile.c_str());

      CLuaPlayer *pLuaPlayer = NEW CLuaPlayer();
      pLuaPlayer->Init(hInstance);

      while (pLuaPlayer->Update() && !s_End)
      {
        pLuaPlayer->Draw();
      }
      DISPOSE(pLuaPlayer);


      GLOGEND();
      GESTION_MEMORIA_END();
  }

  IAppOptions::EndInstance();

  return 0;
}
