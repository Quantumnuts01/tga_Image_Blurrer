#include "ImageParser.h"
#include "SImage.h"
#include <fstream>
#include <iostream>
#include "RGBAPixel.h"
#include "ImageBlurrer.h"

ImageParser::ImageParser()
{
}

ImageParser::~ImageParser()
{
}

bool ImageParser::ReadImage(std::string Path, SImage* ReturnImage)
{

    std::ifstream f;

    // Try to open file path
    f.open(Path, std::ios::in | std::ios::binary);
    if (!f.is_open())
    {
        // Path was invalid return false
        std::cout << "Failed to open tga!\n";
        return false;
    }
    // Path was valid continue

    // Array to get the header
    unsigned char header[18];

    // the read function takes a char pointer so reinterpret cast to make it work
    f.read(reinterpret_cast<char*>(header), 18);
    tgaHeader = *reinterpret_cast<TGAHeader*> (header);

    // Set the pixel vector to the correct size so that all pixels are put in their correct index
    RGBAPixels.resize(tgaHeader.width * tgaHeader.height);

    //for RLE
    int ImageSize = tgaHeader.width * tgaHeader.height;

    bool Is32BitImage = tgaHeader.bitsperpixel == 32;

    if (tgaHeader.datatypecode == 10)
    {
        for (size_t i = 0; i < ImageSize; i++)
        {
            unsigned char RGBAPixel[3];
            unsigned char AlphaPixel[1];
            unsigned char PacketHeader[1];
            f.read(reinterpret_cast<char*>(PacketHeader), 1);
            bool NotRaw = PacketHeader[0] & (1 << 7);
            int NumPixels = (PacketHeader[0] & ~(1 << 7)) + 1;

            if (NotRaw)
            {
                if (Is32BitImage)
                    f.read(reinterpret_cast<char*>(AlphaPixel), 1);

                f.read(reinterpret_cast<char*>(RGBAPixel), 3);
                for (size_t j = 0; j < NumPixels; j++)
                {
                    if (Is32BitImage)
                        RGBAPixels[i].A = AlphaPixel[0];

                    RGBAPixels[i].R = RGBAPixel[2];
                    RGBAPixels[i].G = RGBAPixel[1];
                    RGBAPixels[i].B = RGBAPixel[0];
                    i++;
                }
                i--;
            }
            else
            {
                for (size_t j = 0; j < NumPixels; j++)
                {
                    if (Is32BitImage)
                        f.read(reinterpret_cast<char*>(AlphaPixel), 1);
                    f.read(reinterpret_cast<char*>(RGBAPixel), 3);
                    if (Is32BitImage)
                        RGBAPixels[i].A = AlphaPixel[0];

                    RGBAPixels[i].R = RGBAPixel[2];
                    RGBAPixels[i].G = RGBAPixel[1];
                    RGBAPixels[i].B = RGBAPixel[0];
                    i++;
                }
                i--;
            }

        }
    }
    else
    {
        // Read the image and add the pixels to the pixel vector
        for (size_t y = 0; y < tgaHeader.height; y++)
        {
            for (size_t x = 0; x < tgaHeader.width; x++)
            {
                // if it is an 32 bit image the alpha will be first to read so it needs to be read before the RGB
                if (Is32BitImage)
                {
                    unsigned char AlphaPixel[1];
                    f.read(reinterpret_cast<char*>(AlphaPixel), 1);
                    RGBAPixels[y * tgaHeader.width + x].A = (AlphaPixel[0]);
                }
                unsigned char RGBAPixel[3];
                f.read(reinterpret_cast<char*>(RGBAPixel), 3);
                RGBAPixels[y * tgaHeader.width + x].R = RGBAPixel[2];
                RGBAPixels[y * tgaHeader.width + x].G = RGBAPixel[1];
                RGBAPixels[y * tgaHeader.width + x].B = RGBAPixel[0];
            }

        }
    }


    //Set the return image
    ReturnImage->rgbaPixels = RGBAPixels;
    ReturnImage->bitsperpixel = tgaHeader.bitsperpixel;

    // Return true file read
    return true;
}

bool ImageParser::ExportImage(std::string Path, SImage* Image)
{
    std::ofstream f;

    // try to Write to file / create new file
    f.open(Path, std::ios::out | std::ios::binary);
    if (!f.is_open())
    {
        std::cout << "Failed to set filepath!\n";
        return false;
    }

    const int TgaHeaderSize = 18;

    unsigned char TgaHeader[TgaHeaderSize];

    // creation of new file header
    TgaHeader[0] = tgaHeader.idlength;
    TgaHeader[1] = tgaHeader.colourmaptype;
    TgaHeader[2] = 2;// tgaHeader.datatypecode;

    // Set origin point of the colourmap origin
    TgaHeader[3] = tgaHeader.colourmaporigin;
    TgaHeader[4] = tgaHeader.colourmaporigin >> 8;

    // Set the colourmap length
    TgaHeader[5] = tgaHeader.colourmaplength;
    TgaHeader[6] = tgaHeader.colourmaplength >> 8;

    // Set the number of bits in each color map entry. 24 for the Targa 24, 32 for the Targa 32
    TgaHeader[7] = tgaHeader.BitsInEachColormapEntry;

    // X Origin of Image
    TgaHeader[8] = tgaHeader.x_origin;
    TgaHeader[9] = tgaHeader.x_origin >> 8;

    // Y Origin of Image
    TgaHeader[10] = tgaHeader.y_origin;
    TgaHeader[11] = tgaHeader.y_origin >> 8;

    //Image width
    TgaHeader[12] = tgaHeader.width;
    TgaHeader[13] = tgaHeader.width >> 8;

    //Image Height
    TgaHeader[14] = tgaHeader.height;
    TgaHeader[15] = tgaHeader.height >> 8;

    //Bits per pixel
    TgaHeader[16] = tgaHeader.bitsperpixel;

    // Image Descriptor Byte
    TgaHeader[17] = tgaHeader.imagedescriptor;


    f.write(reinterpret_cast<char*>(TgaHeader), TgaHeaderSize);

    const int Pixels = tgaHeader.height * tgaHeader.width;

    // Write to the new image
    for (size_t y = 0; y < tgaHeader.height; y++)
    {
        for (size_t x = 0; x < tgaHeader.width; x++)
        {
            // if it is an 32 bit image the alpha will be first so it needs to be written before the RGB
            if (tgaHeader.bitsperpixel == 32)
            {
                unsigned char AlphaPixel[1];
                AlphaPixel[0] = Image->rgbaPixels[y * tgaHeader.width + x].A;
                f.write(reinterpret_cast<char*>(AlphaPixel), 1);
            }
            unsigned char Pixel[3];
            Pixel[2] = Image->rgbaPixels[y * tgaHeader.width + x].R;
            Pixel[1] = Image->rgbaPixels[y * tgaHeader.width + x].G;
            Pixel[0] = Image->rgbaPixels[y * tgaHeader.width + x].B;
            f.write(reinterpret_cast<char*>(Pixel), 3);
        }
    }
    f.close();
    return true;
}