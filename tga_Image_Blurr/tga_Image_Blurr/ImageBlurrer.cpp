#include "ImageBlurrer.h"
#include "RGBAPixel.h"
#include <iostream>
#include <thread>
#include <string>
#include<windows.h> 
ImageBlurrer::ImageBlurrer()
{
}

ImageBlurrer::~ImageBlurrer()
{
}

void ImageBlurrer::BoxBlurrRGBA(std::vector<RGBAPixel>* Image, float BlurrAmmount, int StartHeightIndex, int EndHeightIndex,
    int StartWidthIndex, int EndWidthIndex,
    short Width, short Height)
{
    //std::vector<RGBAPixel> PixelSamples = {};
    // Loop through the image and blurr it
    for (int y = StartHeightIndex; y < EndHeightIndex; y++)
    {
        for (int x = StartWidthIndex; x < EndWidthIndex; x++)
        {

            RGBAPixel col = { 0, 0, 0, 0 };

            int pixelsamples = 0;
                // How many pixels in the x direction from the current pixel should be sampled
            for (int iX = -pow(10 * BlurrAmmount, 2); iX <= pow(10 * BlurrAmmount, 2); iX++)
            {
                // is the pixel out of bounds? if so skip that one
                if (x + iX < 0 || x + iX >= Width)
                    continue;
                RGBAPixel color = { 0, 0, 0, 0 };
                color = OriginalRGBAImage.at((y )*Width + x + iX);
                col.R += color.R;
                col.G += color.G;
                col.B += color.B;
                col.A += color.A;
                pixelsamples++;
            }

            // Set the color sample
            Mutex.lock();

            // Set the new pixel RGBA value to the avrage of the sampled ones
            Render1RGBAImage.at(y * Width + x).R = col.R / pixelsamples;
            Render1RGBAImage.at(y * Width + x).G = col.G / pixelsamples;
            Render1RGBAImage.at(y * Width + x).B = col.B / pixelsamples;
            Render1RGBAImage.at(y * Width + x).A = col.A / pixelsamples;

            // Pixelcounter to count the % of the blurring

            Mutex.unlock();
        }

    }

    for (int y = StartHeightIndex; y < EndHeightIndex; y++)
    {
        for (int x = StartWidthIndex; x < EndWidthIndex; x++)
        {


            RGBAPixel col = { 0, 0, 0, 0 };

            int pixelsamples = 0;
            for (int iY = -pow(10 * BlurrAmmount, 2); iY <= pow(10 * BlurrAmmount, 2); iY++)
            {
                if (y + iY < 0 || y + iY >= Height)
                    continue;
                RGBAPixel color = { 0, 0, 0, 0 };
                color = Render1RGBAImage.at((y + iY) * Width + x);
                col.R += color.R;
                col.G += color.G;
                col.B += color.B;
                col.A += color.A;
                pixelsamples++;
            }
            // Set the color sample
            Mutex.lock();

            // Set the new pixel RGBA value to the avrage of the sampled ones
            Image->at(y* Width + x).R = col.R / pixelsamples;
            Image->at(y* Width + x).G = col.G / pixelsamples;
            Image->at(y* Width + x).B = col.B / pixelsamples;
            Image->at(y* Width + x).A = col.A / pixelsamples;

            Mutex.unlock();
        }
    }
}

std::vector<RGBAPixel> ImageBlurrer::BlurrRGBAImage(std::vector<RGBAPixel> Image, short Width, short Height, float BlurrAmmount)
{
    Render1RGBAImage = Image;
    //unsigned int coresAvilable = 4/*std::thread::hardware_concurrency()*/;
    //OriginalRGBAImage = Image;
    //int Xrow = 0;
    //int Yrow = 0;
    //std::vector<std::thread*> Threads;
    //// more than 4? then use 4 threads
    //for (size_t i = 0; i < coresAvilable; i++)
    //{
    //    //Xrow = sqrt(8);
    //    int startHeightIndex = (Yrow * (Height / sqrt(coresAvilable)));
    //    int EndHeightIndex = ((Height / (sqrt(coresAvilable)) * (1 + Yrow)));
    //    int startWidthIndex = (Xrow * (Width / sqrt(coresAvilable)));
    //    int EndWidthIndex = ((Width / sqrt(coresAvilable)) * (1 + Xrow));
    //
    //    std::thread* Thread1 = new std::thread(&ImageBlurrer::BoxBlurrRGBA, this, &Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex,
    //        Width, Height);
    //    Threads.push_back(Thread1);
    //    Xrow++;
    //    if (Xrow == sqrt(coresAvilable))
    //    {
    //        Xrow = 0;
    //        Yrow++;
    //    }
    //}
    //for (size_t i = 0; i < Threads.size(); i++)
    //{
    //    Threads[i]->join();
    //}
    //for (size_t i = 0; i < Threads.size(); i++)
    //{
    //    delete Threads[i];
    //}

    // Return the blurred image
   // return Image;
    // How many cores is in the system?
        unsigned int coresAvilable = std::thread::hardware_concurrency();
    OriginalRGBAImage = Image;
    
    // more than 4? then use 4 threads
    if (coresAvilable > 4)
    {
        // math to blurr 25% of the image per thread
        int startHeightIndex = 0;
        int EndHeightIndex = Height / 2;
        int startWidthIndex = 0;
        int EndWidthIndex = Width / 2;
    
    
        std::thread Thread0(&ImageBlurrer::BoxBlurrRGBA, this, &Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex,
            Width, Height);
    
    
        startHeightIndex = 0;
        EndHeightIndex =  Height / 2;
        startWidthIndex = Width - (Width / 2) - 1;
        EndWidthIndex = Width - 1;
    
    
        std::thread Thread1(&ImageBlurrer::BoxBlurrRGBA, this, &Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex,
            Width, Height);
    
        startHeightIndex = Height - (Height / 2) - 1;
        EndHeightIndex = Height - 1;
        startWidthIndex = 0;
        EndWidthIndex = Width / 2;
    
    
        std::thread Thread2(&ImageBlurrer::BoxBlurrRGBA, this, &Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex,
            Width, Height);
    
        startHeightIndex = Height - (Height / 2);
        EndHeightIndex = Height - 1;
        startWidthIndex = Width - (Width / 2) - 1;
        EndWidthIndex = Width - 1;
    
    
        std::thread Thread3(&ImageBlurrer::BoxBlurrRGBA, this, &Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex,
            Width, Height);
    
        Thread0.join();
        Thread1.join();
        Thread2.join();
        Thread3.join();
    
    
    
    
    
    
    }
    // more than or 2? then use 2 threads
    else if (coresAvilable >= 2)
    {
        // Blurr half image per thread
        int startHeightIndex = 0;
        int EndHeightIndex = Height;
        int startWidthIndex = 0;
        int EndWidthIndex = Width / 2;
    
    
        std::thread Thread0(&ImageBlurrer::BoxBlurrRGBA, this, &Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex,
            Width, Height);
    
    
        startHeightIndex = 0;
        EndHeightIndex = Height;
        startWidthIndex = Width - (Width / 2);
        EndWidthIndex = Width - 1;
    
    
        std::thread Thread1(&ImageBlurrer::BoxBlurrRGBA, this, &Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex,
            Width, Height);
    
    
        Thread0.join();
        Thread1.join();
    }
    // Less than 2? then use no threads very unlikley but lets call this legacy support
    else
    {
        int startHeightIndex = 0;
        int EndHeightIndex = Height - 1;
        int startWidthIndex = 0;
        int EndWidthIndex = Width - 1;
    
    
        BoxBlurrRGBA(&Image, BlurrAmmount, startHeightIndex, EndHeightIndex, startWidthIndex, EndWidthIndex, Width, Height);
    }
    
    // Return the blurred image
    return Image;
}
