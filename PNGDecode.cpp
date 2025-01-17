#include "PNGDecode.hpp"

PNGDecode::PNGDecode(const char* filename)
{
	unsigned int err = lodepng_decode32_file((uint8_t**)&buffer, &png.width, &png.height, filename);
	if (err != 0)
	{
		buffer = nullptr;
		TRACE_ERROR("PNGDecode: open '%s' error %i", filename, err);
		return;
	}
}

PNGDecode::~PNGDecode()
{
	if (isLoaded())
	{
		free(buffer);
	}
}

void PNGDecode::draw(HDC hdc, int x, int y, int sX, int sY, int sW, int sH)
{
    if (sW < 1 || sW > getWidth()) sW = getWidth();
    if (sH < 1 || sH > getHeight()) sH = getHeight();
    if (sX < 0) sX = 0;
    if (sY < 0) sY = 0;
    if ((sX + sW) > getWidth()) sX = getWidth() - sW;
    if ((sY + sH) > getHeight()) sY = getHeight() - sH;
	tRGBA* bmp_bits;
	HDC dc = CreateCompatibleDC(hdc);
	BITMAPINFO bmp_info;
	memset(&bmp_info, 0, sizeof(bmp_info));
	bmp_info.bmiHeader.biSize = sizeof(bmp_info.bmiHeader);
    bmp_info.bmiHeader.biWidth = sW;
    bmp_info.bmiHeader.biHeight = sH;
	bmp_info.bmiHeader.biPlanes = 1;
	bmp_info.bmiHeader.biBitCount = 32;
	bmp_info.bmiHeader.biCompression = BI_RGB;
	HBITMAP bmp = CreateDIBSection(dc, &bmp_info, DIB_RGB_COLORS, (void**)&bmp_bits, NULL, 0);
	HGDIOBJ old_bmp = SelectObject(dc, bmp);
    BitBlt(dc, 0, 0, sW, sH, hdc, x, y, SRCCOPY);
	tRGBA* pIN;
	tRGBA* pOUT;
	int r, g, b, a, r2, g2, b2, a2, iY, oY, bX, bY;
	for (bY = 0; bY < sH; bY++)
	{
		iY = getWidth() * (bY + sY);
        oY = sW * (sH - bY - 1);
		for (bX = 0; bX < sW; bX++)
		{
			pIN = &buffer[iY + bX + sX];
			pOUT = &bmp_bits[oY + bX];
			a = pIN->alpha;
			if (a >= 0xFF)
			{
				pOUT->blue = pIN->red;
				pOUT->green = pIN->green;
				pOUT->red = pIN->blue;
			}
			else if (a > 0)
			{
				r = pIN->blue; g = pIN->green; b = pIN->red;
				r2 = pOUT->red; g2 = pOUT->green; b2 = pOUT->blue; a2 = (255 - a);
				r2 = (r2 * a2 + r * a) / 255;
				g2 = (g2 * a2 + g * a) / 255;
				b2 = (b2 * a2 + b * a) / 255;
				pOUT->blue = b2; pOUT->green = g2; pOUT->red = r2;
			}
			pOUT->alpha = 0xFF;
		}
	}
    BitBlt(hdc, x, y, sW, sH, dc, 0, 0, SRCCOPY);
	SelectObject(dc, old_bmp);
	DeleteObject(bmp);
	DeleteObject(dc);
}
