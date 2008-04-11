// FlashRender.h: interface for the CFlashRender class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLASHRENDER_H__B2AB3F2A_90D1_430B_BBDF_CA3105722D9C__INCLUDED_)
#define AFX_FLASHRENDER_H__B2AB3F2A_90D1_430B_BBDF_CA3105722D9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zflash.h"

class /*AFX_EXT_CLASS*/ CFlashRender  
{
private:	
	BYTE *m_pFlashBuffer;
	BYTE *m_pBitmapFlashBuffer;
	char m_sFlashFileName[MAX_PATH];
	int m_iFlashWidth;
	int m_iFlashHeight;
	int m_iFlashBitCount;
	int m_iFlashID;
	CFlash *m_Flash;//*/

public:
	void InitFlash();
	void DrawFrame();
	int GetFlashBitmapSize();
	int GetFlashBitCount();
	int GetFlashHeight();
	int GetFlashWidth();
	long GetFlashRawSize();
	long GetBackgroundColor();
	bool SetBackgroundColor(long bgcolor);
	void SetLoop(bool bLoop);
	void StopFlash();
	void PlayFlash();
	BYTE* GetFlashBitmapBuffer();
	void LoadFlash(char* sFlashFileName);
	BYTE* GetFlashRawBuffer();
	CFlashRender();
	virtual ~CFlashRender();

	/*CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xd27cdb6e, 0xae6d, 0x11cf, { 0x96, 0xb8, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };
		return clsid;
	}

	IID const& GetIid()
	{
		static IID const iid
			= { 0xD27CDB6C, 0xAE6D, 0x11CF, { 0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } };
		return iid;
	}//*/
};

#endif // !defined(AFX_FLASHRENDER_H__B2AB3F2A_90D1_430B_BBDF_CA3105722D9C__INCLUDED_)
