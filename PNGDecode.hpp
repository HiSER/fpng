#ifndef __PNGDECODE_HPP
#define __PNGDECODE_HPP

#include "stdint.h"
#include "lib/lodepng.h"
#ifdef _MSC_VER
#include "Windows.h"
#endif

class PNGDecode
{
public:

	explicit PNGDecode(const char* filename);
	~PNGDecode();

	operator bool() { return buffer != nullptr; }

	int getWidth() { return png.width; }
	int getHeight() { return png.height; }

#ifdef _MSC_VER
	void draw(HDC hdc, int x = 0, int y = 0, int sX = 0, int sY = 0, int sW = 0, int sH = 0);
#else
	// TODO: other platforms
#endif

#ifdef _MSC_VER
#include <pshpack1.h>
	typedef struct
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t alpha;
	} tRGBA;
#include <poppack.h>
#else
	typedef struct __attribute__((aligned(1)))
	{
		uint8_t blue;
		uint8_t green;
		uint8_t red;
		uint8_t alpha;
	} tRGBA;
#endif

private:

	tRGBA* buffer;
	struct
	{
		uint32_t width;
		uint32_t height;
	} png;

};

#endif
