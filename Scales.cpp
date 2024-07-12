#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include "globals.h"
#include "rect.h"
#include "bmp.h"
#include "wad.h"

/********************************************
*                   Other                   *
********************************************/

bool fileExists(const char* fileName) {
	std::ifstream infile(fileName);
	return infile.good();
}

static wchar_t* charToWChar(const char* text) {
	const size_t size = strlen(text) + 1;
	wchar_t* wText = new wchar_t[size];
	mbstowcs(wText, text, size);
	return wText;
}

/********************************************
*                 Version                   *
********************************************/

enum GameVersion : int {
	UNKNOWN,
	S1_ENEWS, // Reserved
	S1_D1,
	S1_JUN15,
	S1_D2,
	S1_JUL11, // Reserved
	S1_AUG12, // S1 NTSC-U
	S1_AUG27,
	S1_SEP10, // S1 PAL
	S1_D3,         // Sep26
	S1_D4,         // Oct16
	S1_JD1,
	S1_JD2,        // Taikenban
	S1_FEB26, // S1 NTSC-J
	S1_D5,
	S2_D1,
	S2_PREV1, // Reserved
	S2_D2,
	S2_AUG11,
	S2_D34U,
	S2_D34E,
	S2_D5,
	S2_SEP15,
	S2_SEP30, // S2 PAL
	S2_JD1,
	S2_OCT11, // S2 NTSC-U
	S2_JD2,
	S2_JD3,
	S2_JAN27, // S2 NTSC-J
	S3_EARLY,
	S3_APR25,
	S3_D1,
	S3_EPREV, // Reserved
	S3_D2,
	S3_D3,
	S3_D4,
	S3_D5,
	S3_D6,
	S3_SEP04,
	S3_SEP11,
	S3_SEP14, // S3 NTSC-U Rev 0
	S3_SEP29, // S3 PAL    Rev 0
	S3_OCT24, // S3 PAL    Rev 1
	S3_OCT31  // S3 NTSC-U Rev 1
};

class VersionData {
public:
	GameVersion version;
	unsigned int wadSize;
	unsigned char level0wad;
	unsigned char rows; // for the image as a whole
	unsigned char cols;
	std::vector<RectangleId> rects;
};

VersionData versions[] = {
	{ S1_AUG12, 110260224, 10, 17, 15, { S1_U_2, S1_U_3 }}, /* start 0x68fa2 (old, contains the shadow block), square 0x78fe2 */
	{ S1_AUG27, 187451392, 10, 17, 15, { S1_E_1, S1_E_2, S1_E_3, S1_E_4, S1_E_5, S1_E_6, S1_E_7 } }, /* 0x60fa2 */ // STILL CONTAINS THE MOBY SHADOW
	{ S1_SEP10, 211079168, 22, 17, 15, { S1_E_1, S1_E_2, S1_E_3, S1_E_4, S1_E_5, S1_E_6, S1_E_7 } },
	{ S1_FEB26, 112793600, 23, 17, 15, { S1_E_1, S1_E_2, S1_E_3, S1_E_4, S1_E_5, S1_E_6, S1_E_7 } },
	{ S2_SEP30, 290250752, 14,  6, 63, { S2 } }, /* 0x40f82 for the rest */
	{ S2_OCT11, 176914432, 15,  6, 63, { S2 } },
	{ S2_JAN27, 171622400, 14,  6, 63, { S2 } },
	{ S3_SEP04, 163371008, 78, 24, 15, { S3 } },
	{ S3_SEP11, 168454144, 97, 24, 15, { S3 } },
	{ S3_SEP14, 166295552, 97, 24, 15, { S3 } },
	{ S3_SEP29, 185448448, 97, 24, 15, { S3 } },
	{ S3_OCT24, 185450496, 97, 24, 15, { S3 } },
	{ S3_OCT31, 166428672, 97, 24, 15, { S3 } }
};

VersionData& getVersionData(GameVersion ver) {
	for (auto& data : versions) {
		if (data.version == ver) {
			return data;
		}
	}
	throw "Version not currently supported.";
}

GameVersion getVersionFromWadSize(unsigned int size) {
	for (auto& data : versions) {
		if (data.wadSize == size) {
			return data.version;
		}
	}
	printf("Provided WAD not currently supported.");
	return UNKNOWN;
}

/********************************************
*                  Print                    *
********************************************/

void printHelp() {
	printf("Usage: .\Scales.exe -i [FILE] -c [FILE]\n\n  -i    disc image file (Spyro 1, 2 or 3)\n  -c    colours file (BMP CLUT image)\n");
	return;
}

void printLine() {
	std::cout << "------------------------------------------------------------------------" << std::endl;
	return;
}

/********************************************
*                   Args                    *
********************************************/

char* inputIso;
char* inputClut;

char* getCmdOption(char** begin, char** end, const std::string& option) {
	char** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end) {
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
	return std::find(begin, end, option) != end;
}

bool isArgsValid(int argc, char* argv[]) { // and parse

	// Supprted args should eventually be:
	// --version / -v - version name
	// --iso / -i - iso filename
	// --colours / -c colours / CLUT filename
	// maybe optional level number if you just want to patch a specific level?
	// should probably have a --help / an error message indicating to do this too

	if (cmdOptionExists(argv, argv + argc, "-h") || cmdOptionExists(argv, argv + argc, "--help")) {
		printHelp();
		return false;
	}

	if (!cmdOptionExists(argv, argv + argc, "-i")) {
		printf("No disc image file provided.\n");
		printHelp();
		return false;
	}
	inputIso = getCmdOption(argv, argv + argc, "-i");

	if (!cmdOptionExists(argv, argv + argc, "-c")) {
		printf("No colours image file provided.\n");
		printHelp();
		return false;
	}
	inputClut = getCmdOption(argv, argv + argc, "-c");

	return true;
}

/********************************************
*                  Setup                    *
********************************************/

bool isMkpsxisoFilesPresent() {

	// Check that dumpsxiso and mkpsxiso are present
	if (!fileExists("mkpsxiso.exe")) {
		printf("mkpsxiso.exe not present. Please ensure the file is in the working directory.\n");
		return false;
	}
	else if (!fileExists("dumpsxiso.exe")) {
		printf("dumpsxiso.exe not present. Please ensure the file is in the working directory.\n");
		return false;
	}
	return true;

}

bool isWadPresent() {
	if (!fileExists("data/WAD.WAD")) {
		printf("WAD.WAD not present. Please ensure the input file is a valid copy of Spyro 1, 2 or 3.\n");
		return false;
	}
	return true;
}

/////////////////////////////////////////////
/********************************************
*                   Main                    *
********************************************/
/////////////////////////////////////////////

std::string version = "1.0.0";
GameVersion gameVersion = UNKNOWN;
VersionData versionData;

int main(int argc, char* argv[]) {

	const char* date = __DATE__;
	const char* time = __TIME__;

	std::cout << "SCALES: A Spyro Palette Tool" << " (v" << version << " - " << date << " " << time << ")" << std::endl;
	std::cout << "By Hwd405" << std::endl;
	std::cout << "Extra special thanks to Altro, Laura and Pepper" << std::endl;
	printLine();

	printf("A copy of Spyro 1, 2 or 3 (e.g. bin/cue) is required to run this tool.\n\n");
	
	// parse args and validate that the correct files are present
	if (!isArgsValid(argc, argv) || !isMkpsxisoFilesPresent()) {
		getchar();
		exit(1);
	}

	printf("Press ENTER to begin.\n\n");
	getchar();

	// dumpsxiso
	printLine();
	printf("Running dumpsxiso.exe...\n");
	std::string iso = inputIso;
	std::string command = "dumpsxiso -x data -s buildConfig.xml \"" + iso + "\"";
	system(command.c_str());
	printLine();

	// Check WAD and get header
	if (!isWadPresent()) {
		getchar();
		exit(1);
	}
	auto wad = Wad("data/WAD.WAD");
	if (!wad.isValid()) {
		getchar();
		exit(1);
	}
	
	// --version not currently implemented, so just get the version automatically
	// If no version is provided, use the WAD to get the version
	if (gameVersion == UNKNOWN) {
		gameVersion = getVersionFromWadSize(wad.size);
		if (gameVersion == UNKNOWN) {
			getchar();
			exit(1);
		}
	}
	versionData = getVersionData(gameVersion);

	unsigned char levelCount = 35;
	unsigned char wadCountPerLevel = 2; // may be more for some spyro 2 builds??
	if (gameVersion >= S2_D1) {
		levelCount = 29;
		if (gameVersion >= S3_EARLY) {
			levelCount = 37;
		}
	}

	// --validate the bmp dimensions against the version info
	readBMP(inputClut); // I don't think its output is needed
	if (clutFileHeight < versionData.rows || clutFileWidth < versionData.cols) {
		printf("Invalid CLUT file provided.");
		getchar();
		exit(1);
	}

	// load the bmp into "spyroColours" variable
	HWND hwnd{ 0 };
	std::vector<std::vector<vRGB>> spyroColours;
	getBitmapPixel(hwnd, spyroColours, charToWChar(inputClut));

	// open the wad
	std::fstream openWad;
	openWad.open("data/WAD.WAD", std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	// iterate over the WADs
	std::cout << "Updating..." << std::endl;
	unsigned short writeColour;
	for (int i = 0; i < levelCount; i++) {
		DataHeader wadHeader = wad.data[versionData.level0wad + wadCountPerLevel * i];
		if (wadHeader.length == 0 || wadHeader.offset == 0) continue;
		auto rectList = getRects(versionData.rects);
		for (auto& rect : rectList) {
			for (int row = 0; row < rect.rowCount; row++) {
				for (int col = 0; col < rect.colCount; col++) {
					writeColour = bgr(spyroColours[row + rect.startRow][col]);
					openWad.seekp(wadHeader.offset + getVramOffset(col + rect.startX, row + rect.startY));
					openWad.write((char*)&writeColour, sizeof(writeColour));
				}
			}
		}
	}

	// close the wad
	openWad.close();

	// that's a wrap folks
	printLine();
	printf("Running mkpsxiso.exe...\n");
	system("mkpsxiso -q -o SpyroScales.bin -c SpyroScales.cue -y buildConfig.xml");
	printLine();
	printf("\nUpdate complete. Enjoy your new scales! (Press ENTER to exit)");
	getchar();
	return 0;

}
