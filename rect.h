#pragma once
#include <vector>

/********************************************
*                   Rect                    *
********************************************/

enum RectangleId : int {
	S1_U_1,
	S1_U_2,
	S1_U_3,
	S1_E_1,
	S1_E_2,
	S1_E_3,
	S1_E_4,
	S1_E_5,
	S1_E_6,
	S1_E_7,
	S2,
	S3
};

class VramRectangle {
public:
	RectangleId id;
	unsigned int startRow; // specifically for the input
	unsigned int startX;
	unsigned int startY;
	unsigned int rowCount; // height
	unsigned int colCount; // width
};

VramRectangle& getVramRectangle(RectangleId id);
std::vector<VramRectangle> getRects(std::vector<RectangleId> rects);
unsigned int getVramOffset(unsigned int x, unsigned int y);
