//---------------------------------------------------------------------------//
// File: Flash.h
//
//---------------------------------------------------------------------------//

#ifndef FLASH_H
#define FLASH_H

#include "MacromediaFlash.h"

#include <atlbase.h>
extern CComModule _Module;
#include <atlwin.h>

class CFlash
{
  protected:
	  void ReleaseInterfaces();
	  void ReleaseGDI();
  public:
	  int Init(int version_needed);
	  void Exit();
	  void LoadMovie(char *movie_name);

	  void* CreateBuffer(int width,int height);
	  long DrawFrame();// return currentframe

	  // vietdoor's code start here
	  long GetBackgroundColor();
	  bool SetBackgroundColor(long bg);
	  void Stop();
	  void SetLoop(bool bLoop);
	  bool GetLoop();
	  void Play();
	  void GotoFrame(long frame);

	  //CLSID m_clsid;
	  //IID m_iid;

	  CLSID const& GetClsid()
	  {
		  static CLSID const clsid
			  = { 0xd27cdb6e, 0xae6d, 0x11cf, { 0x96, 0xb8, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };
		  return clsid;
	  }

	  IID const& GetIid()
	  {
		  static IID const iid
			  = { 0xD27CDB6C, 0xAE6D, 0x11CF, { 0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };
		  return iid;
	  }

  private:
	  int              m_iInt;
	  long             m_iLastFrame;

	  IShockwaveFlash *m_pSWF;
	  IUnknown        *m_pUnk;

	  CAxWindow       *m_pAXWindow;
	  IViewObjectEx   *m_pViewObj

	  int bm_width, bm_height;
	  HDC hdcCompatible;
	  HBITMAP hBitmap;
	  void *frame_buffer;
	RECTL rectl;//*/

};


#endif