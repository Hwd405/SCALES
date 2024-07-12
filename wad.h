#pragma once
#include <fstream>

/********************************************
*                   WAD                     *
********************************************/

typedef struct {
	unsigned int offset;
	unsigned int length;
} DataHeader;

class Wad {
public:
	DataHeader data[256];
	unsigned int size;
	Wad(const char* wadPath) {

		// Read WAD header, confirm WAD format
		std::ifstream openWad(wadPath, std::ios::binary);
		openWad.unsetf(std::ios::skipws);
		if (!openWad) {
			throw "WAD.WAD file invalid.";
		}
		std::streampos fileSize;
		openWad.seekg(0, std::ios::end);
		fileSize = openWad.tellg();
		openWad.seekg(0, std::ios::beg);
		if (fileSize < 0x800) {
			throw "WAD.WAD file invalid.";
		}
		this->size = fileSize;

		openWad.read((char*)&(this->data[0]), 0x800);
		openWad.close();
		return;
	}
	bool isValid() {
		for (int i = 0; i < 256; i++) {
			if (this->data[i].offset == 0 && this->data[i].length == 0) {
				continue; // break? not sure if a 0/0 wad can be in the middle of valid wads in <s3
			}
			if (this->data[i].offset + this->data[i].length > this->size) {
				return false;
			}
		}
		return true;
	}
};
