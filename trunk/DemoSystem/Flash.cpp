//code by zZz/KCN aka Anatoliy Samara. feb2003

#include "zflash.h"
#include "zflash_flash.h"


#include <stdio.h>

#include <atlbase.h>
CComModule _Module;
#include <atlwin.h>

#include "comutil.h" //_bstr_t

//#include "objbase.h"

//extern void __Error(char *message,const char *file=__FILE__,int line=__LINE__);

/*IShockwaveFlash *iflash = NULL;
IUnknown *unk = NULL;
CAxWindow cw;
IViewObjectEx *viewobject = NULL;//vietdoor 01/02/2005

int bm_width = 0, bm_height = 0;
HDC hdcCompatible = NULL;
HBITMAP hBitmap = NULL;
void *frame_buffer;
RECTL rectl;

// last frame played
static long lastframe = -1;//*/

char *flash_clsid_c="{D27CDB6E-AE6D-11CF-96B8-444553540000}";

namespace ZFLASH{

void __Error(LPTSTR szFormat,...);
BOOL IsFileExist1(LPSTR lpszFilename);
BOOL IsFileExist2(LPSTR lpszFilename);
BOOL IsFileExist3(LPSTR lpszFilename);
//extern void __Error(char *message,const char *file=__FILE__,int line=__LINE__);


int CFlash::Init(int version_needed){

	
	////////////////////////////////////////////
	// Init - Start
	iflash = NULL;
	unk = NULL;
	//pcw = new CAxWindow();
	pcw = NULL;
	viewobject = NULL;//vietdoor 01/02/2005//

	bm_width = 0;
	bm_height = 0;
	hdcCompatible = NULL;
	hBitmap = NULL;
	frame_buffer = NULL;//

	//_Module.Init(NULL, NULL); 
	//AtlAxWinInit();
	// Init - End
	////////////////////////////////////////////
	//*/

	int player_ok = 1;
	IShockwaveFlash *try_flash;

	//OleInitialize(NULL);
	//CoCreateInstance(CLSID_ShockwaveFlash,NULL,CLSCTX_INPROC_SERVER,IID_IShockwaveFlash,(void **)&try_flash);
	//#define _WIN32_DCOM
	//CoInitializeEx(NULL, COINIT_MULTITHREADED);
	//CoCreateInstance(m_clsid, NULL, CLSCTX_INPROC_SERVER, m_iid, (void **)&iflash);

	//CoCreateInstance(GetClsid(), NULL, CLSCTX_INPROC_SERVER, GetIid(), (void **)&iflash);

	CoCreateInstance(GetClsid(), NULL, CLSCTX_INPROC_SERVER, GetIid(), (void **)&try_flash);

	//CoCreateInstance(CLSID_ShockwaveFlash,NULL,CLSCTX_INPROC_SERVER,IID_IShockwaveFlash,(void **)&iflash);
	if (!try_flash){
		player_ok=0;
	}
	else{
		long version;
		try_flash->FlashVersion(&version);
		if (version<version_needed)
			player_ok = 0;
		try_flash->Release();
	}

	if (!player_ok){
		char temp[1024];
		sprintf(temp, "Flash Player v%d.%02d is not registered on this system!\nFlash Player is required to run this programm.\n\nRegister it now?",version_needed>>16,version_needed&0xffff);
		int res = MessageBox(NULL,temp,"Requirements",MB_ICONQUESTION|MB_YESNO);
		if (res == IDYES){
			HMODULE hLib = LoadLibrary("flash.ocx");
			if (!hLib)
				__Error("File flash.ocx not found");
			else
			{
				//MessageBox(NULL,temp,"Requirements",MB_ICONQUESTION|MB_YESNO);
				//void (__stdcall *DllRegisterServer)(void);
				//DllRegisterServer = (void (__stdcall *)(void))GetProcAddress(hlib, "DllRegisterServer");
				//if (MessageBox(NULL,temp,"Requirements",MB_ICONQUESTION|MB_YESNO);)
				//DllRegisterServer();
				//__Error("File swflash.ocx not found");
				//MessageBox(NULL,temp,"Requirements",MB_ICONQUESTION|MB_YESNO);

				//if (FAILED(OleInitialize(NULL))) 
        /*
				if (FAILED(CoInitialize(NULL))) 
				{
					__Error("Initialize OLE failed");
					return 0;
				}
        */

				void (__stdcall *lpDllEntryPoint)(void);
				(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, _T("DllRegisterServer"));
				if (lpDllEntryPoint != NULL)
					(*lpDllEntryPoint)();				
				else
					__Error("DllRegisterServer not found");

				FreeLibrary(hLib);

				//CoUninitialize();
				//OleUninitialize();
			}
		}
		else{
			return 0;
		}
	}
	return 1;
}


void CFlash::ReleaseInterfaces(){
	if (unk) {
		unk->Release();
		unk = NULL;
	}
	if (iflash){
		iflash->Release();
		iflash = NULL;
	}

	// vietdoor's code
	// modified on 01/02/2005
	if (viewobject){
		viewobject->Release();
		viewobject = NULL;
	}
}

void CFlash::ReleaseGDI(){
	if (hBitmap){
		DeleteObject(hBitmap);
	}

	if (hdcCompatible){
		DeleteDC(hdcCompatible);
	}
}

void CFlash::Exit(){
	ReleaseInterfaces();
	ReleaseGDI();
	CoUninitialize();
	//OleUninitialize();
}

// vietdoor's code start here
long CFlash::GetBackgroundColor(){
	long bg = -1;

	if (iflash)
		iflash->get_BackgroundColor(&bg);

	return bg;
}

// vietdoor's code start here
bool CFlash::SetBackgroundColor(long bg){
	bool Result = false;

	if (iflash)
		Result = iflash->put_BackgroundColor(bg) == NO_ERROR;

	return Result;
}

void CFlash::Stop()
{
	if (iflash)
		iflash->StopPlay();
}

void CFlash::Play()
{
	if (iflash)
		iflash->Play();
}

void CFlash::SetLoop(bool bLoop)
{
	if (iflash)
		iflash->put_Loop(bLoop);
}
// vietdoor's code end here
	
void* CFlash::CreateBuffer(int width,int height){
	ReleaseGDI();

	HDC desktop_dc = GetDC(GetDesktopWindow());
	hdcCompatible = CreateCompatibleDC(desktop_dc);	

	BITMAPINFO binfo;
	memset(&binfo,0, sizeof(binfo));
	binfo.bmiHeader.biSize = sizeof(binfo);
	binfo.bmiHeader.biPlanes = 1;
	binfo.bmiHeader.biBitCount = 32;
	binfo.bmiHeader.biCompression = BI_RGB;
	binfo.bmiHeader.biHeight = height;
	binfo.bmiHeader.biWidth = width;
	
	hBitmap = CreateDIBSection(hdcCompatible, &binfo, DIB_RGB_COLORS, &frame_buffer, NULL, 0);
	SelectObject(hdcCompatible, hBitmap);
	bm_width = width;
	bm_height = height;

	//RECT rc = {0, 0, width, height};
	//AdjustWindowRect(&rc,cw.GetWindowLong(GWL_STYLE),FALSE);
	//cw.SetWindowPos(NULL,0,0,rc.right-rc.left,rc.bottom-rc.top,SWP_NOMOVE|SWP_NOREDRAW|SWP_DEFERERASE|
	//                                      SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOOWNERZORDER|SWP_NOSENDCHANGING|
	//									  SWP_NOZORDER);

	rectl.left = 0;
	rectl.right = bm_width;
	rectl.top = 0;
	rectl.bottom = bm_height;

	// vietdoor's code start here
	::SetMapMode(hdcCompatible, MM_TEXT);
	// vietdoor's code end here

	return frame_buffer;
}

void CFlash::LoadMovie(char *movie_name){
	
	ReleaseInterfaces();
	ReleaseGDI();

	AtlAxWinInit();
	if (pcw == NULL)
		pcw = new CAxWindow();
	if (pcw->IsWindow())
		pcw->DestroyWindow();

	RECT rc = {0, 0, 400, 400};
	AdjustWindowRect(&rc, 0, FALSE);
	pcw->Create(NULL, &rc, flash_clsid_c, 0);

	pcw->QueryControl(&unk);
	if (!unk) __Error("NULL IUnknown");

	//HRESULT hr = unk->QueryInterface(m_iid, (void **)&iflash);
	HRESULT hr = unk->QueryInterface(GetIid(), (void **)&iflash);
	if (!iflash) __Error("Unable to query IFlash");

	//BSTR WMode;
	char *s;	
	//hr = iflash->get_WMode(WMode);
	//TCHAR sWMode[MAX_PATH] = "Transparent";
	//BSTR bsWMode = _bstr_t("Transparent");
	_bstr_t bsWMode = "transparent";
	_bstr_t bsWModeName = "wmode";
	//hr = iflash->put_WMode(_bstr_t(sWMode));
	hr = iflash->get_WMode((unsigned short**)&s);
	//hr = iflash->put_WMode(bsWMode);
	hr = iflash->get_WMode((unsigned short**)&s);

	//hr = iflash->SetVariable(bsWModeName, bsWMode);

	//_bstr_t bsBgColor = "0xFE03AA";
	//hr = iflash->put_BGColor(bsBgColor);

	char buf[256];
	if (movie_name[0] == '.'){
		GetCurrentDirectory(256,buf);
		char last_ch = buf[strlen(buf)-1];
		if (last_ch != '\\' && last_ch != '/'){
			strcat(buf, "/");
		}
		strcat(buf, movie_name);
	}
	else{
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

	unsigned short real_name[256];
	MultiByteToWideChar(CP_ACP,0,buf,-1,real_name,256);
	
	//iflash->put_Playing(FALSE);
	if (iflash->LoadMovie(0,real_name) != NOERROR)
		__Error("Can't load movie");// Code's vietdoor
	//iflash->put_Quality(5);

	iflash->QueryInterface(IID_IViewObjectEx,(void **)&viewobject);

	//iflash->SetZoomRect(0,0,256,256);

	//iflash->put_WMode(_bstr_t("Window"));
	//iflash->put_BackgroundColor(0xFFFFFF);
	//iflash->put_BackgroundColor(0x000000);

	//_bstr_t bsWMode = "transparent";
	//hr = iflash->put_WMode(bsWMode);
	//hr = iflash->get_WMode((unsigned short**)&s);
	//hr = iflash->put_WMode(bsWMode);
	//hr = iflash->get_WMode((unsigned short**)&s);
	
	// reset last frame
	lastframe = -1;
}

long CFlash::DrawFrame(){

	if (iflash == NULL)
		return -1;

	//static long lastframe=-1;
	long frame = lastframe, framenum, fr;
	short bPlaying;

	// Code's vietdoor
	HRESULT hr = iflash->CurrentFrame(&frame);
	
	iflash->get_Playing(&bPlaying);
	//printf("bPlaying %d\n", bPlaying);

	iflash->get_TotalFrames(&framenum);

	iflash->get_FrameNum(&fr);

	//if (bPlaying == -1)
	//	iflash->GotoFrame(frame + 1);
		//iflash->Play();		
	
	//printf("DrawFrame frame = %d framenum = %d fr = %d\n", frame, framenum, fr);
	if (frame != lastframe)
	{
		lastframe = frame;
		viewobject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hdcCompatible, &rectl, &rectl, NULL, NULL); 
		return frame;
	}
	else if (frame == framenum)
	{		
		short bLoop;
		iflash->get_Loop(&bLoop);
		if (bLoop)
			iflash->GotoFrame(0);
	}

	return -1;
}

void CFlash::GotoFrame(long frame){
	if (iflash)
		iflash->GotoFrame(frame);
}

bool CFlash::GetLoop()
{
	short bLoop;
	//bool bLoop;
	if (iflash)
	{				
		iflash->get_Loop(&bLoop);
	}
	return bLoop != 0;
}

// Code's vietdoor about error msg
void __Error(LPTSTR szFormat,...)
{
    static TCHAR szBuffer[2048]={0};
    const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    const int LASTCHAR = NUMCHARS - 1;

    // Format the input string
    va_list pArgs;
    va_start(pArgs, szFormat);

    // Use a bounded buffer size to prevent buffer overruns.  Limit count to
    // character size minus one to allow for a NULL terminating character.
    _vsntprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    va_end(pArgs);

    // Ensure that the formatted string is NULL-terminated
    szBuffer[LASTCHAR] = TEXT('\0');

    MessageBox(NULL, szBuffer, NULL, 
               MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
}

BOOL IsFileExist1(LPSTR lpszFilename)
{
	HANDLE hFile = CreateFile(lpszFilename, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return TRUE;
	} else
	{
		return FALSE;
	}
}


BOOL IsFileExist2(LPSTR lpszFilename)
{
	WIN32_FIND_DATA wfd;

	HANDLE hFind = FindFirstFile(lpszFilename, &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return FALSE;
	else
	{
		FindClose(hFind);
		return TRUE;
	}
}

BOOL IsFileExist3(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}

}//namespace