//=====================================================================
//
// GFXWin32.cpp - 
//
// Created by skywind on 2020/04/26
// Last Modified: 2020/04/26 20:30:55
//
//=====================================================================
#include "GFXWin32.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Win32);


//---------------------------------------------------------------------
// GdiPlus_Init
//---------------------------------------------------------------------
int GdiPlus_Init(int startup)
{
	static HINSTANCE hDLL = NULL;
	static ULONG token = 0;
	static int inited = 0;
	int retval;

	if (hDLL == NULL) {
		hDLL = LoadLibraryA("gdiplus.dll");
		if (hDLL == NULL) return -1;
	}

	if (startup) {
		struct {
			unsigned int version;
			void *callback;
			BOOL SuppressBackgroundThread;
			BOOL SuppressExternalCodecs;
		}	GStartupInput;
		struct {
			void *hook;
			void *unhook;
		}	GStartupOutput;

		typedef int (WINAPI *GdiPlusStartup_t)(ULONG*, LPVOID, LPVOID);
		static GdiPlusStartup_t GdiPlusStartup_o = NULL;

		if (inited) return 0;

		if (GdiPlusStartup_o == NULL) {
			GdiPlusStartup_o = (GdiPlusStartup_t)GetProcAddress(
				hDLL, "GdiplusStartup");
			if (GdiPlusStartup_o == NULL) {
				return -2;
			}
		}

		GStartupInput.version = 1;
		GStartupInput.callback = NULL;
		GStartupInput.SuppressBackgroundThread = 0;
		GStartupInput.SuppressExternalCodecs = 0;

		retval = GdiPlusStartup_o(&token, &GStartupInput, &GStartupOutput);

		if (retval != 0) {
			return -3;
		}

		inited = 1;
	}
	else {
		typedef int (WINAPI *GdiPlusShutdown_t)(ULONG*);
		static GdiPlusShutdown_t GdiPlusShutdown_o = NULL;

		if (inited == 0) return 0;

		if (GdiPlusShutdown_o == NULL) {
			GdiPlusShutdown_o = (GdiPlusShutdown_t)GetProcAddress(
				hDLL, "GdiplusShutdown");
			if (GdiPlusShutdown_o == NULL) {
				return -4;
			}
		}

		GdiPlusShutdown_o(&token);

		inited = 0;
	}

	return 0;
}



//---------------------------------------------------------------------
// core routine for loading png/jpg from memory using gdi+
//---------------------------------------------------------------------
void* GdiPlus_LoadCore(const void *data, long size, int *cx, int *cy, 
	long *pitch, int *pfmt, int *bpp, int *errcode)
{
	typedef HRESULT (WINAPI *CreateStream_t)(HGLOBAL, BOOL, LPSTREAM*);
	typedef int (WINAPI *GdipCreateBitmap_t)(LPSTREAM, LPVOID*); 
	typedef int (WINAPI *GdipGetPixelFormat_t)(LPVOID, int*);
	typedef int (WINAPI *GdipDisposeImage_t)(LPVOID);
	typedef int (WINAPI *GdipGetImageWidth_t)(LPVOID, UINT*);
	typedef int (WINAPI *GdipGetImageHeight_t)(LPVOID, UINT*);
	typedef int (WINAPI *GdipLockBits_t)(LPVOID, LPVOID, UINT, int, LPVOID);
	typedef int (WINAPI *GdipUnlockBits_t)(LPVOID, LPVOID);

	static CreateStream_t CreateStream_o = NULL;
	static GdipCreateBitmap_t GdipCreateBitmap_o = NULL;
	static GdipGetPixelFormat_t GdipGetPixelFormat_o = NULL;
	static GdipDisposeImage_t GdipDisposeImage_o = NULL;
	static GdipGetImageWidth_t GdipGetImageWidth_o = NULL;
	static GdipGetImageHeight_t GdipGetImageHeight_o = NULL;
	static GdipLockBits_t GdipLockBits_o = NULL;
	static GdipUnlockBits_t GdipUnlockBits_o = NULL;

	static HINSTANCE hGdiPlusDLL = NULL;
	LPSTREAM ppstm = NULL;
	HGLOBAL hg = NULL;
	LPVOID pg = NULL;
	LPVOID bitmap = NULL;
	void *bits = NULL;
	int retval = 0;
	int gpixfmt = 0;
	int fmt = 0;
	int nbytes = 0;
	UINT width;
	UINT height;
	long stride;
	int GRECT[4];
	int i;

	struct { 
		unsigned int width;
		unsigned int height;
		int pitch;
		int format;
		void *scan0;
		int *reserved;
	}	GBitmapData;

	GBitmapData.width = 0;
	GBitmapData.height = 0;
	GBitmapData.pitch = 0;
	GBitmapData.scan0 = NULL;

	if (CreateStream_o == NULL) {
		static HINSTANCE hOleDLL = NULL;
		if (hOleDLL == NULL) {
			hOleDLL = LoadLibraryA("ole32.dll");
			if (hOleDLL == NULL) {
				if (errcode) errcode[0] = -20;
				return NULL;
			}
		}
		CreateStream_o = (CreateStream_t)GetProcAddress(
			hOleDLL, "CreateStreamOnHGlobal");
		if (CreateStream_o == NULL) {
			if (errcode) errcode[0] = -21;
			return NULL;
		}
	}

	if (hGdiPlusDLL == NULL) {
		hGdiPlusDLL = LoadLibraryA("gdiplus.dll");
		if (hGdiPlusDLL == NULL) {
			if (errcode) errcode[0] = -22;
			return NULL;
		}
	}

	if (GdipCreateBitmap_o == NULL) {
		GdipCreateBitmap_o = (GdipCreateBitmap_t)GetProcAddress(
			hGdiPlusDLL, "GdipCreateBitmapFromStream");
		if (GdipCreateBitmap_o == NULL) {
			if (errcode) errcode[0] = -23;
			return NULL;
		}
	}

	if (GdipGetPixelFormat_o == NULL) {
		GdipGetPixelFormat_o = (GdipGetPixelFormat_t)GetProcAddress(
			hGdiPlusDLL, "GdipGetImagePixelFormat");
		if (GdipGetPixelFormat_o == NULL) {
			if (errcode) errcode[0] = -24;
			return NULL;
		}
	}

	if (GdipDisposeImage_o == NULL) {
		GdipDisposeImage_o = (GdipDisposeImage_t)GetProcAddress(
			hGdiPlusDLL, "GdipDisposeImage");
		if (GdipDisposeImage_o == NULL) {
			if (errcode) errcode[0] = -25;
			return NULL;
		}
	}

	if (GdipGetImageWidth_o == NULL) {
		GdipGetImageWidth_o = (GdipGetImageWidth_t)GetProcAddress(
			hGdiPlusDLL, "GdipGetImageWidth");
		if (GdipGetImageWidth_o == NULL) {
			if (errcode) errcode[0] = -26;
			return NULL;
		}
	}

	if (GdipGetImageHeight_o == NULL) {
		GdipGetImageHeight_o = (GdipGetImageHeight_t)GetProcAddress(
			hGdiPlusDLL, "GdipGetImageHeight");
		if (GdipGetImageHeight_o == NULL) {
			if (errcode) errcode[0] = -27;
			return NULL;
		}
	}

	if (GdipLockBits_o == NULL) {
		GdipLockBits_o = (GdipLockBits_t)GetProcAddress(
			hGdiPlusDLL, "GdipBitmapLockBits");
		if (GdipLockBits_o == NULL) {
			if (errcode) errcode[0] = -28;
			return NULL;
		}
	}

	if (GdipUnlockBits_o == NULL) {
		GdipUnlockBits_o = (GdipUnlockBits_t)GetProcAddress(
			hGdiPlusDLL, "GdipBitmapUnlockBits");
		if (GdipUnlockBits_o == NULL) {
			if (errcode) errcode[0] = -29;
			return NULL;
		}
	}

	if (errcode) errcode[0] = 0;

	hg = GlobalAlloc(GMEM_MOVEABLE, size);

	if (hg == NULL) {
		if (errcode) errcode[0] = -1;
		return NULL;
	}

	pg = GlobalLock(hg);

	if (pg == NULL) {
		GlobalFree(hg);
		if (errcode) errcode[0] = -2;
		return NULL;
	}

	memcpy(pg, data, size);

	GlobalUnlock(hg);

	if (CreateStream_o(hg, 0, &ppstm) != S_OK) {
		GlobalFree(hg);
		if (errcode) errcode[0] = -3;
		return NULL;
	}

	retval = GdipCreateBitmap_o(ppstm, &bitmap);

	if (retval != 0) {
		retval = -4;
		bitmap = NULL;
		goto finalizing;
	}


	#define GPixelFormat1bppIndexed     196865
	#define GPixelFormat4bppIndexed     197634
	#define GPixelFormat8bppIndexed     198659
	#define GPixelFormat16bppGrayScale  1052676
	#define GPixelFormat16bppRGB555     135173
	#define GPixelFormat16bppRGB565     135174
	#define GPixelFormat16bppARGB1555   397319
	#define GPixelFormat24bppRGB        137224
	#define GPixelFormat32bppRGB        139273
	#define GPixelFormat32bppARGB       2498570
	#define GPixelFormat32bppPARGB      925707
	#define GPixelFormat48bppRGB        1060876
	#define GPixelFormat64bppARGB       3424269
	#define GPixelFormat64bppPARGB      29622286
	#define GPixelFormatMax             15
	
	if (GdipGetPixelFormat_o(bitmap, &gpixfmt) != 0) {
		retval = -5;
		goto finalizing;
	}
	
	if (gpixfmt == GPixelFormat8bppIndexed)
		gpixfmt = GPixelFormat8bppIndexed,
		fmt = 8, 
		nbytes = 1;
	else if (gpixfmt == GPixelFormat16bppRGB555)
		gpixfmt = GPixelFormat16bppRGB555,
		fmt = 555,
		nbytes = 2;
	else if (gpixfmt == GPixelFormat16bppRGB565)
		gpixfmt = GPixelFormat16bppRGB565,
		fmt = 565,
		nbytes = 2;
	else if (gpixfmt == GPixelFormat16bppARGB1555)
		gpixfmt = GPixelFormat16bppARGB1555,
		fmt = 1555,
		nbytes = 2;
	else if (gpixfmt == GPixelFormat24bppRGB)
		gpixfmt = GPixelFormat24bppRGB,
		fmt = 888,
		nbytes = 3;
	else if (gpixfmt == GPixelFormat32bppRGB)
		gpixfmt = GPixelFormat32bppRGB,
		fmt = 888,
		nbytes = 4;
	else if (gpixfmt == GPixelFormat32bppARGB)
		gpixfmt = GPixelFormat32bppARGB,
		fmt = 8888,
		nbytes = 4;
	else if (gpixfmt == GPixelFormat64bppARGB) 
		gpixfmt = GPixelFormat32bppARGB,
		fmt = 8888,
		nbytes = 4;
	else if (gpixfmt == GPixelFormat64bppPARGB)
		gpixfmt = GPixelFormat32bppARGB,
		fmt = 8888,
		nbytes = 4;
	else if (gpixfmt == GPixelFormat32bppPARGB)
		gpixfmt = GPixelFormat32bppARGB,
		fmt = 8888,
		nbytes = 4;
	else 
		gpixfmt = GPixelFormat32bppARGB,
		fmt = 8888,
		nbytes = 4;

#if 1
	gpixfmt = GPixelFormat32bppARGB;
	fmt = 8888;
	nbytes = 4;
#endif

	if (bpp) bpp[0] = nbytes * 8;
	if (pfmt) pfmt[0] = fmt;

	if (GdipGetImageWidth_o(bitmap, &width) != 0) {
		retval = -6;
		goto finalizing;
	}

	if (cx) cx[0] = (int)width;

	if (GdipGetImageHeight_o(bitmap, &height) != 0) {
		retval = -7;
		goto finalizing;
	}

	if (cy) cy[0] = (int)height;

	stride = (nbytes * width + 3) & ~3;
	if (pitch) pitch[0] = stride;

	GRECT[0] = 0;
	GRECT[1] = 0;
	GRECT[2] = (int)width;
	GRECT[3] = (int)height;

	if (GdipLockBits_o(bitmap, GRECT, 1, gpixfmt, &GBitmapData) != 0) {
		GBitmapData.scan0 = NULL;
		retval = -8;
		goto finalizing;
	}

	if (GBitmapData.format != gpixfmt) {
		retval = -9;
		goto finalizing;
	}

	bits = (char*)malloc(stride * height);

	if (bits == NULL) {
		retval = -10;
		goto finalizing;
	}

	for (i = 0; i < (int)height; i++) {
		char *src = (char*)GBitmapData.scan0 + GBitmapData.pitch * i;
		char *dst = (char*)bits + stride * i;
		memcpy(dst, src, stride);
	}

	retval = 0;
	
finalizing:
	if (GBitmapData.scan0 != NULL) {
		GdipUnlockBits_o(bitmap, &GBitmapData);
		GBitmapData.scan0 = NULL;
	}

	if (bitmap) {
		GdipDisposeImage_o(bitmap);
		bitmap = NULL;
	}

	if (ppstm) {
		#ifndef __cplusplus
		ppstm->lpVtbl->Release(ppstm);
		#else
		ppstm->Release();
		#endif
		ppstm = NULL;
	}

	if (hg) {
		GlobalFree(hg);
		hg = NULL;
	}

	if (errcode) errcode[0] = retval;

	return bits;
}


//---------------------------------------------------------------------
// Load Image from memory
//---------------------------------------------------------------------
GFX::Image* GdiPlus_LoadImage(const void *data, long size, int *errcode)
{
	void *bits = NULL;
	int cx = -1;
	int cy = -1;
	long pitch = -1;
	int pfmt = -1;
	int bpp = -1;
	int errorcode = 0;
	int notfind = 0;
	GFX::PixelFormat fmt = FMT_A8R8G8B8;

	bits = GdiPlus_LoadCore(data, size, &cx, &cy, &pitch, &pfmt, &bpp, &errorcode);

	if (bits == NULL) {
		if (errcode) errcode[0] = errorcode;
		return NULL;
	}

	// printf("pfmt=%d\n", pfmt);

	switch (pfmt) {
	case 8: fmt = FMT_G8; break;
	case 555: fmt = FMT_A1R5G5B5; break;
	case 565: fmt = FMT_R5G6B5; break;
	case 888: fmt = FMT_R8G8B8; break;
	case 8888: fmt = FMT_A8R8G8B8; break;
	default:
		notfind = 1;
		break;
	}

	// printf("fmt=%d\n", (int)fmt);

	if (notfind) {
		if (errcode) errcode[0] = -20;
		free(bits);
		return NULL;
	}

	GFX::Image *img = new GFX::Image(cx, cy, fmt);

	if (img == NULL) {
		if (errcode) errcode[0] = -21;
		free(bits);
		return NULL;
	}

	for (int j = 0; j < img->GetHeight(); j++) {
		void *dst = img->GetLine(j);
		void *src = (char*)bits + pitch * j;
		memcpy(dst, src, (img->GetBpp() / 8) * cx);
	}

	free(bits);

	// printf("image: fmt=%d bpp=%d\n", (int)img->GetFormat(), img->GetBpp());

	return img;
}


//---------------------------------------------------------------------
// load image from file with GdiPlus
//---------------------------------------------------------------------
GFX::Image* GdiPlus_LoadFile(const char *filename)
{
	std::string content;
	if (!LoadFile(filename, content)) 
		return NULL;
	GFX::Image *img = GdiPlus_LoadImage(content.data(), (int)content.size(), NULL);
	return img;
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Win32);
NAMESPACE_END(GFX);



