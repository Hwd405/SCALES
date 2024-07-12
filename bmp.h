#pragma once
#include <fstream>
#include <vector>
#include <Windows.h>

/********************************************
*                   BMP                     *
********************************************/

struct vRGB {
	vRGB() :R(0), G(0), B(0) {}
	vRGB(BYTE r, BYTE g, BYTE b) :R(r), G(g), B(b) {}
	BYTE R, G, B;
};

unsigned short bgr(vRGB colour);
char* readBMP(char* filename);
void getBitmapPixel(HWND hwnd, std::vector<std::vector<vRGB>>& pixel, wchar_t* filename);
