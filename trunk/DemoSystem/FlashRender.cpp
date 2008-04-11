// FlashRender.cpp: implementation of the CFlashRender class.
//
//////////////////////////////////////////////////////////////////////

//#include "RenderFlash.h"
#include "FlashRender.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#include "zflash\zflash.h"
#include "zflash.h"

// define constants
#define FLASH_IDTIMER  10004
#define FLASH_TIMER    40
#define FLASH_VERSION  0x70000
#define FLASH_WIDTH    320
#define FLASH_HEIGHT   240
#define FLASH_BITS     32


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CALLBACK FlashTimerProc(HWND hwnd,
    UINT uMsg,
    UINT_PTR idEvent,
    DWORD dwTime
)
{
	//g_Flash.DrawFrame();
	//DrawFlash();
	//GetFlashRawBuffer();
}

CFlashRender::CFlashRender()
{	
	m_iFlashWidth = FLASH_WIDTH;
	m_iFlashHeight = FLASH_HEIGHT;
	m_iFlashBitCount = FLASH_BITS;
	strcpy(m_sFlashFileName, "");
	m_pFlashBuffer = NULL;
	m_pBitmapFlashBuffer = NULL;
	m_Flash = NULL;
}

CFlashRender::~CFlashRender()
{
	if (m_pBitmapFlashBuffer)
		free(m_pBitmapFlashBuffer);

	if(m_Flash)
	{
		m_Flash->Exit();
		delete m_Flash;
		m_Flash = NULL;
	}
}

BYTE* CFlashRender::GetFlashRawBuffer()
{
	return m_pFlashBuffer;
}

void CFlashRender::LoadFlash(char *sFlashFileName)
{
	strcpy(m_sFlashFileName, sFlashFileName);
	m_Flash->LoadMovie(sFlashFileName);
	m_pFlashBuffer = (BYTE*)m_Flash->CreateBuffer(m_iFlashWidth, m_iFlashHeight);//*/
}

BYTE* CFlashRender::GetFlashBitmapBuffer()
{
	if (m_pFlashBuffer == NULL)
		return NULL;

	int size = m_iFlashBitCount / 8 * m_iFlashWidth * m_iFlashHeight;
	int fhsize = sizeof(BITMAPFILEHEADER);
	int bisize = sizeof(BITMAPINFOHEADER);

	if (m_pBitmapFlashBuffer == NULL)
	{
		BITMAPFILEHEADER bmfh;
		BITMAPINFOHEADER info;
		
		memset ( &bmfh, 0, sizeof ( BITMAPFILEHEADER ) );
		memset ( &info, 0, sizeof ( BITMAPINFOHEADER ) );

		// First we fill the file header with data
		bmfh.bfType  = 0x4d42; // 0x4d42 = 'BM'
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;
		bmfh.bfSize  = sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) + size;
		bmfh.bfOffBits   = 0x36;  // (54) size of headers

		info.biSize      = sizeof ( BITMAPINFOHEADER );
		info.biWidth     = m_iFlashWidth;
		info.biHeight      = m_iFlashHeight;
		info.biPlanes        = 1;
		info.biBitCount      = m_iFlashBitCount;
		info.biCompression   = BI_RGB;
		info.biSizeImage     = 0;
		info.biXPelsPerMeter = 0x0ec4;    
		info.biYPelsPerMeter = 0x0ec4;
		info.biClrUsed       = 0;
		info.biClrImportant  = 0;

		m_pBitmapFlashBuffer = (BYTE*)malloc(fhsize + bisize + size);

		memcpy(m_pBitmapFlashBuffer, &bmfh, fhsize);
		memcpy(m_pBitmapFlashBuffer + fhsize, &info, bisize);
	}

	memcpy(m_pBitmapFlashBuffer + fhsize + bisize, m_pFlashBuffer, size);
	
	return m_pBitmapFlashBuffer;
}

void CFlashRender::PlayFlash()
{
	if (m_Flash)
		m_Flash->Play();
}

void CFlashRender::StopFlash()
{
	if (m_Flash)
		m_Flash->Stop();
}

void CFlashRender::SetLoop(bool bLoop)
{
	if (m_Flash)
		m_Flash->SetLoop(bLoop);
}

long CFlashRender::GetBackgroundColor()
{
	if (m_Flash)
		return m_Flash->GetBackgroundColor();
	return 0;
}

bool CFlashRender::SetBackgroundColor(long bgcolor)
{
	if (m_Flash)
		return m_Flash->SetBackgroundColor(bgcolor);
	return 0;
}

long CFlashRender::GetFlashRawSize()
{
	return m_iFlashBitCount / 8 * m_iFlashWidth * m_iFlashHeight;
}

int CFlashRender::GetFlashWidth()
{
	return m_iFlashWidth; 
}

int CFlashRender::GetFlashHeight()
{
	return m_iFlashHeight;
}

int CFlashRender::GetFlashBitCount()
{
	return m_iFlashBitCount;
}

int CFlashRender::GetFlashBitmapSize()
{
	int size = GetFlashRawSize();
	int fhsize = sizeof(BITMAPFILEHEADER);
	int bisize = sizeof(BITMAPINFOHEADER);

	return fhsize + bisize + size;
}

void CFlashRender::DrawFrame()
{
	m_Flash->DrawFrame();
}

void CFlashRender::InitFlash()
{
	if (m_Flash == NULL)
	{
		m_Flash = new CFlash();
		
		m_iFlashID = m_Flash->Init(FLASH_VERSION);

		if (m_iFlashID)
			SetTimer(0, FLASH_IDTIMER, FLASH_TIMER, (TIMERPROC)FlashTimerProc);
		else
			MessageBox(NULL, "Can't init the flash player", "Error", MB_ICONERROR|MB_OK);
	}
	else
		MessageBox(NULL, "Inited the flash player", "Error", MB_ICONERROR|MB_OK);
}