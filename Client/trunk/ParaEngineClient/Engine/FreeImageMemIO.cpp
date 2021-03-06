//-----------------------------------------------------------------------------
// Class:	GDIEngine
// Authors:	Li, Xizhi
// Emails:	LiXizhi@yeah.net
// Date:	2009.8.27
// Note: in order for GDIEngine to work one needs to call Gdiplus::GdiplusStartup() and Gdiplus::GdiplusShutdown();
// as in the ParaEngineApp.cpp
//-----------------------------------------------------------------------------
#include "ParaEngine.h"
#ifdef USE_FREEIMAGE
#include <FreeImage.h>
#include <gdiplus.h>

#include "FreeImageMemIO.h"

using namespace Gdiplus;


unsigned STDCALL MemIO::_ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	MemIO *memIO = (MemIO*)handle;

	BYTE *tmp = (BYTE *)buffer;

	for (unsigned c = 0; c < count; c++) {
		memcpy(tmp, memIO->_cp, size);

		memIO->_cp = memIO->_cp + size;

		tmp += size;
	}

	return count;
}

unsigned STDCALL MemIO::_WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle) {
	assert( false );
	return size;
}

int STDCALL MemIO::_SeekProc(fi_handle handle, long offset, int origin) {
	assert(origin != SEEK_END);

	MemIO *memIO = (MemIO*)handle;

	if (origin == SEEK_SET) 
		memIO->_cp = memIO->_start + offset;
	else
		memIO->_cp = memIO->_cp + offset;

	return 0;
}

long STDCALL MemIO::_TellProc(fi_handle handle) {
	MemIO *memIO = (MemIO*)handle;

	return memIO->_cp - memIO->_start;
}

// NOT USED: unless you want to save PNG via GDI+ interface, currently I used FreeImage. 
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}
#endif