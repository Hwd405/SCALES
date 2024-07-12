#include <fstream>
#include <vector>
#include <Windows.h>
#include "bmp.h"
#include "globals.h"

/********************************************
*                   BMP                     *
********************************************/

int clutFileWidth = 0;
int clutFileHeight = 0;

unsigned short bgr(vRGB colour) {
	byte r = (byte)(colour.R >> 3); // Bottom 3 bits are lost
	byte g = (byte)(colour.G >> 3); // Bottom 3 bits are lost
	byte b = (byte)(colour.B >> 3); // Bottom 3 bits are lost
	return (unsigned short)((b << 10) | (g << 5) | (r));
}

char* readBMP(char* filename) {
	int i;
	std::fstream openBmp(filename, std::ios_base::in | std::ios_base::binary);
	unsigned char info[54];

	// read the 54-byte header
	openBmp.seekg(0);
	openBmp.read((char*)&info, 54);

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	int heightSign = 1;
	if (height < 0) {
		heightSign = -1;
	}

	clutFileWidth = width;
	clutFileHeight = abs(height);

	// allocate 3 bytes per pixel
	int size = 3 * width * abs(height);
	char* data = (char*)malloc(size);

	// read the rest of the data at once
	openBmp.seekg(54);
	openBmp.read(data, size);
	openBmp.close();

	if (heightSign == 0) {
		unsigned char tmp;
		for (i = 0; i < size; i += 3) {
			tmp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = tmp;
		}
	}
	return data; // not currently used
}

void getBitmapPixel(HWND hwnd, std::vector<std::vector<vRGB>>& pixel, wchar_t* filename) {
	BITMAP bi;
	HBITMAP hBmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	pixel.clear();

	::GetObject(hBmp, sizeof(bi), &bi);
	int Width = bi.bmWidth; int Height = bi.bmHeight;

	pixel.resize(Height);

	HDC hDC = GetDC(hwnd);
	HDC hMemDC = ::CreateCompatibleDC(hDC);
	SelectObject(hMemDC, hBmp);

	DWORD pixelData = 0;

	for (int y = 0; y < Height; ++y) {
		pixel[y].reserve(Width);

		for (int x = 0; x < Width; ++x) {
			pixelData = GetPixel(hMemDC, x, y);
			pixel[y].push_back(vRGB(GetRValue(pixelData), GetGValue(pixelData), GetBValue(pixelData)));
		}
	}

	DeleteDC(hMemDC);
	ReleaseDC(hwnd, hDC);
	return;
}
