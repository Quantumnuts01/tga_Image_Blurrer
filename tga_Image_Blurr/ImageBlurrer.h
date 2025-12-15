#pragma once

#include <mutex>
#include <vector>

struct RGBAPixel;

class ImageBlurrer
{
public:

	ImageBlurrer();
	~ImageBlurrer();

	std::vector<RGBAPixel> BlurrRGBAImage(std::vector<RGBAPixel> Image, short Width, short Height, float BlurrAmmount);

	void BoxBlurrRGBA(std::vector<RGBAPixel>* Image, float BlurrAmmount, int StartHeightIndex, int EndHeightIndex,
		int StartWidthIndex, int EndWidthIndex,
		short Width, short Height);

private:

	std::vector<RGBAPixel> OriginalRGBAImage;
	std::vector<RGBAPixel> Render1RGBAImage;
	std::vector<RGBAPixel> Render2RGBAImage;
	std::mutex Mutex;
	int PixelsWritten = 0;
};

