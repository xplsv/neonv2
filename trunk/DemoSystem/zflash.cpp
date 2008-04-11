//---------------------------------------------------------------------------//
// File: ZFlash.cpp
// Based on code by zZz/KCN aka Anatoliy Samara. feb2003
//---------------------------------------------------------------------------//

#include "Base.h"
#include "zflash.h"
#include "zflash_flash.h"

#include <stdio.h>
#include "comutil.h"

#define SAFE_RELEASE(p)     { if(p) { (p)->Release(); (p)=NULL; } }

CComModule _Module;
static char *flash_clsid_c="{D27CDB6E-AE6D-11CF-96B8-444553540000}";

wchar_t *ConvertStringtoBSTR(char const *)
{
  return NULL;
}

bool IsFileExist1(LPSTR lpszFilename)
{
	HANDLE hFile = CreateFile(lpszFilename, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return TRUE;
	}
  return FALSE;
}

//---------------------------------------------------------------------------//
// Init
// 
//---------------------------------------------------------------------------//
bool CFlash::Init(int version_needed)
{
  bool bRes  = false;
	iflash     = NULL;
	unk        = NULL;
	pcw        = NULL;
	viewobject = NULL;
	IShockwaveFlash *try_flash;

	CoCreateInstance(GetClsid(), NULL, CLSCTX_INPROC_SERVER, GetIid(), (void **)&try_flash);
	if (!try_flash)
  {
    GLOG(("ERR: Can't create IShockwaveFlash instance\n"));
	}
	else
  {
		long version;
		try_flash->FlashVersion(&version);
		bRes = (version >= version_needed);
		try_flash->Release();
	}
  // Si no se ha podido cargar, preguntamos para instalar el activex
	if (!bRes)
  {
		char temp[1024];
		sprintf(temp, "Flash Player v%d.%02d is not registered on this system.\nFlash Player ActiveX is required to run this programm.\n\nRegister it now?",version_needed>>16,version_needed&0xffff);
		int res = MessageBox(NULL,temp,"Requirements",MB_ICONQUESTION|MB_YESNO);
		if (res == IDYES)
    {
			HMODULE hLib = LoadLibrary("flash.ocx");
			if (!hLib)
      {
        GLOG(("ERR: File Flash.ocx not found\n"));
      }
			else
			{
				void (__stdcall *lpDllEntryPoint)(void);
				(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, _T("DllRegisterServer"));
				if (lpDllEntryPoint != NULL)
					(*lpDllEntryPoint)();				
				else
        {
          GLOG(("ERR: Can't create IShockwaveFlash instance\n"));
        }
				FreeLibrary(hLib);
        bRes = true;
			}
		}
	}
	return bRes;
}


//---------------------------------------------------------------------------//
// Release Interfaces
// 
//---------------------------------------------------------------------------//
void CFlash::ReleaseInterfaces()
{
  SAFE_RELEASE(unk);
  SAFE_RELEASE(iflash);
  SAFE_RELEASE(viewobject);
}


//---------------------------------------------------------------------------//
// End
// 
//---------------------------------------------------------------------------//
void CFlash::End()
{
	ReleaseInterfaces();
}


//---------------------------------------------------------------------------//
// GetBackgroundColor
// 
//---------------------------------------------------------------------------//
long CFlash::GetBackgroundColor()
{
	long bg = -1;
	if (iflash)
		iflash->get_BackgroundColor(&bg);
	return bg;
}


//---------------------------------------------------------------------------//
// SetBackgroundColor
// 
//---------------------------------------------------------------------------//
bool CFlash::SetBackgroundColor(long bg)
{
	bool Result = false;
	if (iflash)
		Result = iflash->put_BackgroundColor(bg) == NO_ERROR;
	return Result;
}


//---------------------------------------------------------------------------//
// Stop
// 
//---------------------------------------------------------------------------//
void CFlash::Stop()
{
	if (iflash)
		iflash->StopPlay();
}


//---------------------------------------------------------------------------//
// Play
// 
//---------------------------------------------------------------------------//
void CFlash::Play()
{
	if (iflash)
		iflash->Play();
}


//---------------------------------------------------------------------------//
// SetLoop
// 
//---------------------------------------------------------------------------//
void CFlash::SetLoop(bool bLoop)
{
	if (iflash)
		iflash->put_Loop(bLoop);
}


//---------------------------------------------------------------------------//
// GotoFrame
// 
//---------------------------------------------------------------------------//
void CFlash::GotoFrame(long frame)
{
	if (iflash)
		iflash->GotoFrame(frame);
}


//---------------------------------------------------------------------------//
// GetLoop
// 
//---------------------------------------------------------------------------//
bool CFlash::GetLoop()
{
	short bLoop;
	//bool bLoop;
	if (iflash)
		iflash->get_Loop(&bLoop);
	return bLoop != 0;
}

//---------------------------------------------------------------------------//
// SetQuality
// 
//---------------------------------------------------------------------------//
void CFlash::SetQuality(int iQuality)
{
	//bool bLoop;
	if (iflash)
		iflash->put_Quality(iQuality);
}


//---------------------------------------------------------------------------//
// LoadMovie
// 
//---------------------------------------------------------------------------//
void CFlash::LoadMovie(char *movie_name)
{
	ReleaseInterfaces();

	AtlAxWinInit();
	if (pcw == NULL)
		pcw = new CAxWindow();
	if (pcw->IsWindow())
		pcw->DestroyWindow();

	RECT rc = {0, 0, 320,240};
	AdjustWindowRect(&rc, 0, FALSE);
	pcw->Create(NULL, &rc, flash_clsid_c, 0);
	pcw->QueryControl(&unk);
	if (!unk)
  {
    GLOG(("ERR: Can't query for ActiveX control\n"));
  }

	HRESULT hr = unk->QueryInterface(GetIid(), (void **)&iflash);
	if (!iflash)
  {
    GLOG(("ERR: Unable to query for IShockwaveFlash interface\n"));
  }

	//TCHAR sWMode[MAX_PATH] = "Transparent";
	//BSTR bsWMode = _bstr_t("Transparent");
	_bstr_t bsWMode     = "Transparent";
	_bstr_t bsWModeName = "wmode";
	hr = iflash->put_WMode(_bstr_t(bsWMode));
	//hr = iflash->get_WMode((unsigned short**)&s);
	//hr = iflash->put_WMode(bsWMode);
	//hr = iflash->get_WMode((unsigned short**)&s);

	//hr = iflash->SetVariable(bsWModeName, bsWMode);

	//_bstr_t bsBgColor = "0xFE03AA";
	//hr = iflash->put_BGColor(bsBgColor);

	char buf[256];
	if (movie_name[0] == '.')
  {
		GetCurrentDirectory(256,buf);
		char last_ch = buf[strlen(buf)-1];
		if (last_ch != '\\' && last_ch != '/')
			strcat(buf, "/");
		strcat(buf, movie_name);
	}
	else
  {
		if (!IsFileExist1(movie_name) || (strlen(movie_name) > 3 && movie_name[1] != ':' && movie_name[2] != '\\'))
		{
			char szDir[_MAX_PATH];
			GetCurrentDirectory( _MAX_PATH, szDir);
			strcat(szDir, "\\");
			strcat(szDir, movie_name);
			strcpy(buf,szDir);
		}
		else
			strcpy(buf,movie_name);
	}

	//iflash->put_Playing(FALSE);
  wchar_t wBuf[MAX_PATH];
  for (unsigned i = 0; i <= strlen(buf); i++)
    wBuf[i] = buf[i];
	if (iflash->LoadMovie(0,wBuf) != NOERROR)
  {
    GLOG(("ERR: Can't load flash movie %s\n", buf));
  }
	//iflash->put_Quality(5);
	iflash->QueryInterface(IID_IViewObjectEx,(void **)&viewobject);

  long lTotalFrames;
  iflash->get_TotalFrames(&lTotalFrames);
  //iflash->Zoom(1);
  if (lTotalFrames == 1)
    m_fLength = 0.f;
  else
    m_fLength = (float)lTotalFrames / 24.f;

	//iflash->SetZoomRect(0,0,400,400);
  //iflash->Zoom(1);
  //iflash->put_WMode(_bstr_t("Window"));
	//iflash->put_BackgroundColor(0xFFFFFF);

	//_bstr_t bsWMode = "transparent";
	hr = iflash->put_WMode(_bstr_t("Transparent"));
	//hr = iflash->get_WMode((unsigned short**)&s);
	//hr = iflash->put_WMode(bsWMode);

  iflash->Rewind();
  SetLoop(true);

	// reset last frame
	lastframe = -1;
}


//---------------------------------------------------------------------------//
// DrawFrame
// 
//---------------------------------------------------------------------------//
void CFlash::DrawFrame(HDC hdc)
{
	if (iflash == NULL) return;

	long  frame = lastframe;
	short bPlaying;

	HRESULT hr = iflash->CurrentFrame(&frame);
	
	iflash->get_Playing(&bPlaying);
	//iflash->get_TotalFrames(&framenum);
	//iflash->get_FrameNum   (&fr);

  if (!bPlaying)
  {
    iflash->Rewind();
    iflash->Play();
  }
	
  //lastframe = frame;
  RECTL rectl = {0,0,320,240};
	viewobject->Draw(DVASPECT_TRANSPARENT, 1, NULL, NULL, NULL, hdc, &rectl, NULL, NULL, NULL); 
}
