#pragma once

#include <vector>
#include <string>

struct RGBAPixel;
struct SImage;

// Visual studio padds struct to be 16 byte aligned
// so this disables that allowing for an 18 byte header struct.
#pragma pack(push, 1)
struct TGAHeader
{
    char  idlength;
    char  colourmaptype;
    char  datatypecode;
    short int colourmaporigin;
    short int colourmaplength;
    char  BitsInEachColormapEntry;
    short int x_origin;
    short int y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};
#pragma pack(pop)


class ImageParser
{
public:
	ImageParser();
	~ImageParser();
    bool ReadImage(std::string, SImage* ReturnImage);
	bool ExportImage(std::string Path, SImage* Image); 
    std::vector<RGBAPixel> RGBAPixels;
    TGAHeader tgaHeader;
};

