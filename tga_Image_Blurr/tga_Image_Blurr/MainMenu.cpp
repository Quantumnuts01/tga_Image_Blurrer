#include "MainMenu.h"
#include "ImageParser.h"
#include "ImageBlurrer.h"
#include "SImage.h"

#include <iostream>
#include <fstream>
#include <string>



MainMenu::MainMenu()
{
    m_imageparser = new ImageParser;
    m_imageBlurrer = new ImageBlurrer;
    image = new SImage;
}

MainMenu::~MainMenu()
{
    if (image)
        delete image;
}

void MainMenu::Menu()
{
    // Infinite for loop since i wanted to try it
    for (;;)
    {
        std::cout << "Input filepath to file:\nPath with filename: ";

        std::string InputString;
        std::cin >> InputString;


        if (!m_imageparser->ReadImage( InputString, image ))
        {
            std::cout << "Failed to open filepath!\n";
            std::cout << "Please try again!\n";
            continue;
        }

        float BlurrAmmount;
        std::cout << "Set blurr ammount:\nblurr ammount: ";

        std::cin >> BlurrAmmount;
        if (BlurrAmmount != 0)
        {
            image->rgbaPixels = m_imageBlurrer->BlurrRGBAImage(image->rgbaPixels, m_imageparser->tgaHeader.width, m_imageparser->tgaHeader.height, BlurrAmmount);
        }
        std::cout << "Input export file name\nName:";
        std::cin >> InputString;

        InputString += ".tga";

        m_imageparser->ExportImage(InputString, image);
        break;
    }
}
