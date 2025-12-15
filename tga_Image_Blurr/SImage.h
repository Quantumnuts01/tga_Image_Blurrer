#pragma once
#include"RGBAPixel.h"
#include <vector>

struct RGBAPixel;

struct SImage
{
	std::vector<RGBAPixel> rgbaPixels;
	int bitsperpixel;
};