#include <vector>
#include "rect.h"
#include "globals.h"

/********************************************
*                   Rect                    *
********************************************/

VramRectangle rects[] = {
	//{ S1_U_1,  0, 465, 418,  2, 15 }, // probably unused
	{ S1_U_2,  0, 465, 429,  3, 15 },
	{ S1_U_3,  3, 497, 481, 14, 15 },
	{ S1_E_1,  0, 465, 386,  2, 15 },
	{ S1_E_2,  2, 481, 385,  3, 15 },
	{ S1_E_3,  5, 497, 385,  3, 15 },
	{ S1_E_4,  8, 465, 416,  1, 15 }, // this probably needs to be split in two? one for the first two and one for the bottom two
	{ S1_E_5,  9, 481, 417,  3, 15 },
	{ S1_E_6, 12, 497, 417,  2, 15 },
	{ S1_E_7, 14, 465, 429,  3, 15 },
	{     S2,  0, 449, 257,  6, 63 },
	{     S3,  0, 449, 257, 24, 15 }
};

VramRectangle& getVramRectangle(RectangleId id) {
	for (auto& rect : rects) {
		if (rect.id == id) {
			return rect;
		}
	}
	throw "Rectangle not currently supported.";
}

std::vector<VramRectangle> getRects(std::vector<RectangleId> rects) {
	std::vector<VramRectangle> out;
	for (auto& id : rects) {
		auto r = getVramRectangle(id);
		out.push_back(r);
	}
	return out;
}

unsigned int getVramOffset(unsigned int x, unsigned int y) {
	return 0x800 + 0x400 * y + 2 * x;
}
